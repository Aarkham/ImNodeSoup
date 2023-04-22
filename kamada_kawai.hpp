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

  void Start(bool aStartCircle=true);
  void Step(float aWidth,float aHeight,std::vector<NsPosition>& aPositions);

  void MovePos(vertex_id_t aVertexId,const ImVec2& aDisp,bool aRecalculate);


  double GetEnergy() const noexcept;

private:

  struct Spring
  {
    double m_Length;
    double m_Strength;
  };


  const adj_list_t& m_AdjList;
  const double m_EnergyThreshold;
  double m_K;
  unsigned int m_SteadyEnergyCount;
  double m_MaxVertexEnergy;
  vertex_id_t m_VertexId;

  std::vector<std::vector<Spring>> m_Springs;
  mutable std::vector<NsPosition> m_Positions;

  // p m
  std::tuple<double,vertex_id_t> FindMaxVertexEnergy() const noexcept;
  // delta m
  double ComputeVertexEnergy(vertex_id_t aVertexId) const noexcept;
  ImVec2 ComputeNextVertexPosition(vertex_id_t aVertexId) const noexcept;

  void RecalculateSprings(vertex_id_t aVertexId);

  void SetInitPositions(bool aStartCircle);

  void CenterAndScale(float aWidth,float aHeight,std::vector<NsPosition>& aPositions) const noexcept;
  mutable float m_Scale;
  mutable ImVec2 m_Offset;
};


}
