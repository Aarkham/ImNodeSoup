#pragma once
#include "nodesoup.hpp"
#include <vector>

namespace nodesoup
{


class FruchtermanReingold
{
public:
  FruchtermanReingold(const adj_list_t& aAdjList,double aK=15.0);

  void Start();
  void Step(int aStepSize,int aMaxStep,std::vector<ImVec2>& aPositions);

  int GetCurrIter() const noexcept;
  int GetMaxIters() const noexcept;

  double GetK() const noexcept;
  void   SetK(double aK) noexcept;

private:

  const adj_list_t& m_AdjList;
  double m_K;
  double m_KSquared;
  double m_Temp;
  std::vector<ImVec2> m_Mvmts;

  int m_CurrIter,m_MaxIter;
  std::vector<ImVec2> m_TempBuffer;

  void DoStep(std::vector<ImVec2>& aPositions);
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


}
