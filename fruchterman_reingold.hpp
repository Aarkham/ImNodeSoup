#pragma once
#include "nodesoup.hpp"
#include <vector>

namespace nodesoup
{


class FruchtermanReingold
{
public:
  FruchtermanReingold(const adj_list_t& aAdjList,double aK=15.0);

  void Start(bool aStartCircle=true);
  void Step(int aStepSize,int aMaxStep,std::vector<NsPosition>& aPositions);

  int GetCurrIter() const noexcept;
  int GetMaxIters() const noexcept;

  double GetK() const noexcept;
  void   SetK(double aK) noexcept;

  double GetEnergy() const noexcept;

  void   MovePos(vertex_id_t aVertexId,const ImVec2& aDisp,bool aRecalculate);

private:

  const adj_list_t& m_AdjList;
  double m_K;
  double m_KSquared;
  double m_Temp;
  std::vector<ImVec2> m_Mvmts;

  bool m_StartCircle;
  int m_CurrIter,m_MaxIter;

  std::vector<NsPosition> m_Positions;

  void DoStep();
  void SetInitPositions();
};




inline int FruchtermanReingold::GetCurrIter() const noexcept
{
  return m_CurrIter;
}

inline int FruchtermanReingold::GetMaxIters() const noexcept
{
  return m_MaxIter;
}

inline double FruchtermanReingold::GetK() const noexcept
{
  return m_K;
}

inline double FruchtermanReingold::GetEnergy() const noexcept
{
  return m_Temp;
}




}
