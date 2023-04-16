#pragma once
#include "nodesoup.hpp"
#include <vector>
#include <tuple>

namespace nodesoup
{
// https://gist.github.com/terakun/b7eff90c889c1485898ec9256ca9f91d
// https://graphsharp.codeplex.com/SourceControl/latest#Source/Graph#/Algorithms/Layout/Simple/FDP/KKLayoutAlgorithm.cs



class KamadaKawai
{
public:

  KamadaKawai(const adj_list_t& aAdjList,double aK=300.0,double aEnergyThreshold=1e-2);

  void Start(const std::vector<ImVec2>& aPositions);
  void Step(float aWidth,float aHeight,std::vector<ImVec2>& aPositions) const;



private:

  struct Spring
  {
    double m_Length;
    double m_Strength;
  };


  const adj_list_t& m_AdjList;
  const double m_EnergyThreshold;
  double m_K;
  mutable unsigned int m_SteadyEnergyCount;
  mutable double m_MaxVertexEnergy;
  mutable vertex_id_t m_VertexId;

  std::vector<std::vector<Spring>> m_Springs;
  mutable std::vector<ImVec2> m_Positions;

  // p m
  std::tuple<double,vertex_id_t> FindMaxVertexEnergy(const std::vector<ImVec2>& aPositions) const noexcept;
  // delta m
  double ComputeVertexEnergy(vertex_id_t aVertexId,const std::vector<ImVec2>& aPositions) const noexcept;
  ImVec2 ComputeNextVertexPosition(vertex_id_t aVertexId,const std::vector<ImVec2>& aPositions) const noexcept;

  void CenterAndScale(float aWidth,float aHeight,std::vector<ImVec2>& aPositions) const noexcept;
};


}
