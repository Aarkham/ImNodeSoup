#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>


#include "kamada_kawai.hpp"

namespace nodesoup
{



static std::vector<std::vector<vertex_id_t>> floyd_warshall_(const adj_list_t& aAdjList)
{
  // build adjacency matrix (infinity = no edge, 1 = edge)
  unsigned int infinity = std::numeric_limits<unsigned int>::max() / 2;
  std::vector<std::vector<vertex_id_t>> distances(aAdjList.size(), std::vector<vertex_id_t>(aAdjList.size(), infinity));

  for(vertex_id_t v_id=0; v_id<aAdjList.size(); v_id++)
    {
      distances[v_id][v_id]=0;
      for(vertex_id_t adj_id : aAdjList[v_id])
        {
          if(adj_id>v_id)
            {
              distances[v_id][adj_id]=1;
              distances[adj_id][v_id]=1;
            }
        }
    }

  // floyd warshall itself, find length of shortest path for each pair of vertices
  for(vertex_id_t k=0; k<aAdjList.size(); k++)
    {
      for(vertex_id_t i=0; i<aAdjList.size(); i++)
        {
          for(vertex_id_t j=0; j<aAdjList.size(); j++)
            {
              distances[i][j]=std::min(distances[i][j],distances[i][k]+distances[k][j]);
            }
        }
    }

  return distances;
}







KamadaKawai::KamadaKawai(const adj_list_t& aAdjList,double aK,double aEnergyThreshold)
    : m_AdjList(aAdjList)
    , m_EnergyThreshold(aEnergyThreshold)
    , m_K(aK)
    , m_SteadyEnergyCount(0)
    , m_MaxVertexEnergy(0.0)
    , m_VertexId(0)
{

}


void KamadaKawai::Start(const std::vector<ImVec2>& aPositions)
{
  m_Positions.resize(aPositions.size());
  std::copy(aPositions.begin(),aPositions.end(),m_Positions.begin());

  std::vector<std::vector<vertex_id_t>> distances=floyd_warshall_(m_AdjList);

  // find biggest distance
  size_t biggest_distance = 0;
  for(vertex_id_t v_id=0; v_id<m_AdjList.size(); v_id++)
    {
      for(vertex_id_t other_id=0; other_id<m_AdjList.size(); other_id++)
        {
          if(distances[v_id][other_id] > biggest_distance)
            {
              biggest_distance = distances[v_id][other_id];
            }
        }
    }

  // Ideal length for all edges. we don't really care, the layout is going to be scaled.
  // Let's chose 1.0 as the initial positions will be on a 1.0 radius circle, so we're
  // on the same order of magnitude
  double length=1.0/biggest_distance;

  // init springs lengths and strengths matrices
  m_Springs.clear();
  m_Springs.reserve(m_AdjList.size());
  for(vertex_id_t v_id=0; v_id<m_AdjList.size(); v_id++)
    {
      std::vector<Spring> v_springs;
      v_springs.reserve(m_AdjList.size());

      for(vertex_id_t other_id=0; other_id<m_AdjList.size(); other_id++)
        {
          Spring spring;
          if(v_id == other_id)
            {
              spring.m_Length = 0.0;
              spring.m_Strength = 0.0;
            }
          else
            {
              size_t distance=distances[v_id][other_id];
              spring.m_Length=distance*length;
              spring.m_Strength=m_K/(distance*distance);
            }

          v_springs.push_back(spring);
        }
      m_Springs.push_back(v_springs);
    }

  m_SteadyEnergyCount = 0;
  auto res = FindMaxVertexEnergy(m_Positions);
  m_MaxVertexEnergy=std::get<double>(res);
  m_VertexId=std::get<vertex_id_t>(res);
}






#define MAX_VERTEX_ITERS_COUNT 50
#define MAX_STEADY_ENERGY_ITERS_COUNT 50


// Reduce the energy of the next vertex with most energy until all the vertices have
// a energy below energy_threshold
void KamadaKawai::Step(float aWidth,float aHeight,std::vector<ImVec2>& aPositions) const
{
  // assert m_Positions.size()==aPositions.size()

  if(m_MaxVertexEnergy>m_EnergyThreshold  && m_SteadyEnergyCount<MAX_STEADY_ENERGY_ITERS_COUNT)
    {
      // move vertex step by step until its energy goes below threshold
      // (apparently this is equivalent to the newton raphson method)
      unsigned int vertex_count = 0;
      do
        {
          m_Positions[m_VertexId]=ComputeNextVertexPosition(m_VertexId,m_Positions);
          vertex_count++;
        }
      while (ComputeVertexEnergy(m_VertexId,m_Positions)>m_EnergyThreshold  &&  vertex_count<MAX_VERTEX_ITERS_COUNT);

      double max_vertex_energy_prev=m_MaxVertexEnergy;
      auto res = FindMaxVertexEnergy(m_Positions);
      m_MaxVertexEnergy=std::get<double>(res);
      m_VertexId=std::get<vertex_id_t>(res);

      if(std::abs(m_MaxVertexEnergy-max_vertex_energy_prev) < 1e-20)
        {
          m_SteadyEnergyCount++;
        }
      else
        {
          m_SteadyEnergyCount=0;
        }
    }

  CenterAndScale(aWidth,aHeight,aPositions);
}





// Find @p max_energy_v_id with the most potential energy and @return its energy
// https://gist.github.com/terakun/b7eff90c889c1485898ec9256ca9f91d
std::tuple<double,vertex_id_t> KamadaKawai::FindMaxVertexEnergy(const std::vector<ImVec2>& aPositions) const noexcept
{
  double max_energy=-1.0;
  vertex_id_t max_energy_v_id=0;

  for(vertex_id_t v_id=0; v_id<m_AdjList.size(); v_id++)
    {
      double energy=ComputeVertexEnergy(v_id,aPositions);
      if(energy>max_energy)
        {
          max_energy_v_id=v_id;
          max_energy=energy;
        }
    }

  return {max_energy,max_energy_v_id};
}




// @return the potential energies of springs between @p v_id and all other vertices
double KamadaKawai::ComputeVertexEnergy(vertex_id_t aVertexId,const std::vector<ImVec2>& aPositions) const noexcept
{
  double x_energy=0.0;
  double y_energy=0.0;

  for(vertex_id_t other_id=0; other_id<m_AdjList.size(); other_id++)
    {
      if(aVertexId==other_id)
        {
          continue;
        }

      ImVec2 delta=aPositions[aVertexId]-aPositions[other_id];
      double distance=norm(delta);

      // delta * k * (1 - l / distance)
      Spring spring=m_Springs[aVertexId][other_id];
      x_energy += delta.x*spring.m_Strength * (1.0-spring.m_Length/distance);
      y_energy += delta.y*spring.m_Strength * (1.0-spring.m_Length/distance);
    }

  return sqrt(x_energy*x_energy+y_energy*y_energy);
}





// @returns next position for @param v_id reducing its potential energy, ie the energy in the whole graph
// caused by its position.
// The position's delta depends on K (TODO bigger K = faster?).
// This is the complicated part of the algorithm.
ImVec2 KamadaKawai::ComputeNextVertexPosition(vertex_id_t aVertexId,const std::vector<ImVec2>& aPositions) const noexcept
{
  double xx_energy=0.0, xy_energy=0.0, yx_energy=0.0, yy_energy=0.0;
  double x_energy=0.0, y_energy=0.0;

  for(vertex_id_t other_id = 0; other_id < m_AdjList.size(); other_id++)
    {
      if(aVertexId==other_id)
        {
          continue;
        }

      ImVec2 delta=aPositions[aVertexId]-aPositions[other_id];
      double distance=norm(delta);
      double cubed_distance=distance * distance * distance;

      Spring spring=m_Springs[aVertexId][other_id];

      x_energy += delta.x * spring.m_Strength * (1.0 - spring.m_Length / distance);
      y_energy += delta.y * spring.m_Strength * (1.0 - spring.m_Length / distance);
      xy_energy += spring.m_Strength * spring.m_Length * delta.x * delta.y / cubed_distance;
      xx_energy += spring.m_Strength * (1.0 - spring.m_Length * delta.y * delta.y / cubed_distance);
      yy_energy += spring.m_Strength * (1.0 - spring.m_Length * delta.x * delta.x / cubed_distance);
    }
  yx_energy = xy_energy;

  ImVec2 position = aPositions[aVertexId];
  double denom = xx_energy * yy_energy - xy_energy * yx_energy;
  position.x += static_cast<float>((xy_energy * y_energy - yy_energy * x_energy) / denom);
  position.y += static_cast<float>((xy_energy * x_energy - xx_energy * y_energy) / denom);

  return position;
}




void KamadaKawai::CenterAndScale(float aWidth, float aHeight,std::vector<ImVec2>& aPositions) const noexcept
{
    // find current dimensions
    float x_min = std::numeric_limits<float>::max();
    float x_max = std::numeric_limits<float>::lowest();
    float y_min = std::numeric_limits<float>::max();
    float y_max = std::numeric_limits<float>::lowest();

    for(vertex_id_t v_id=0; v_id<m_Positions.size(); v_id++)
      {
        if(m_Positions[v_id].x<x_min)
          {
            x_min=m_Positions[v_id].x;
          }
        if(m_Positions[v_id].x>x_max)
          {
            x_max=m_Positions[v_id].x;
          }

        if(m_Positions[v_id].y<y_min)
          {
            y_min=m_Positions[v_id].y;
          }
        if(m_Positions[v_id].y>y_max)
          {
            y_max=m_Positions[v_id].y;
          }
      }

    float cur_width =x_max-x_min;
    float cur_height=y_max-y_min;

    // compute scale factor (0.9: keep some margin)
    float x_scale = aWidth / cur_width;
    float y_scale = aHeight / cur_height;
    float scale = 0.9f * (x_scale < y_scale ? x_scale : y_scale);

    // compute offset and apply it to every position
    ImVec2 center = { x_max + x_min, y_max + y_min };
    ImVec2 offset = center / 2.0 * scale;
    for(vertex_id_t v_id = 0; v_id < m_Positions.size(); v_id++)
      {
        ImVec2 pos_scaled{ scale*m_Positions[v_id].x ,scale* m_Positions[v_id].y };
        aPositions[v_id] = pos_scaled - offset;
      }
}


}
