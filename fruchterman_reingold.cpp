#include "fruchterman_reingold.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace nodesoup
{

using std::vector;

FruchtermanReingold::FruchtermanReingold(const adj_list_t& aAdjList,double aK)
    : m_AdjList(aAdjList)
    , m_K(aK)
    , m_KSquared(aK* aK)
    , m_Temp(10 * sqrt(aAdjList.size()))
    , m_Mvmts(m_AdjList.size())
    , m_StartCircle(true)
    , m_CurrIter(0), m_MaxIter(0)
{
}




void FruchtermanReingold::Start(bool aStartCircle)
{
  m_Mvmts.resize(m_AdjList.size());
  m_Positions.resize(m_AdjList.size());

  m_CurrIter=0;
  m_MaxIter=0;

  m_StartCircle=aStartCircle;
}



void FruchtermanReingold::DoStep()
{
  ImVec2 zero={0.0f,0.0f};
  fill(m_Mvmts.begin(),m_Mvmts.end(), zero);

  // Repulsion force between vertice pairs
  for(vertex_id_t v_id=0; v_id<m_AdjList.size(); v_id++)
    {
      for(vertex_id_t other_id=v_id+1; other_id<m_AdjList.size(); other_id++)
        {
          if(v_id==other_id)
            {
              continue;
            }

          ImVec2 delta = m_Positions[v_id].m_Pos-m_Positions[other_id].m_Pos;
          double distance = norm(delta);
          // TODO: handle distance == 0.0

          // > 1000.0: not worth computing
          if(distance > 1000.0)
            {
              continue;
            }

          double repulsion = m_KSquared / distance;

          m_Mvmts[v_id] += delta/distance * repulsion;
          m_Mvmts[other_id] -= delta/distance * repulsion;
        }

      // Attraction force between edges
      for(vertex_id_t adj_id:m_AdjList[v_id])
        {
          if(adj_id>v_id)
            {
              continue;
            }

          ImVec2 delta = m_Positions[v_id].m_Pos-m_Positions[adj_id].m_Pos;
          float distance=norm(delta);
          if(distance==0.0f)
            {
              continue;
            }

          double attraction = distance*distance / m_K;

          m_Mvmts[v_id] -= delta / distance * attraction;
          m_Mvmts[adj_id] += delta / distance * attraction;
        }
    }

  // Max movement capped by current temperature
  for(vertex_id_t v_id=0; v_id<m_AdjList.size(); v_id++)
    {
      if(!m_Positions[v_id].m_Fixed)
        {
          double mvmt_norm=norm(m_Mvmts[v_id]);
          // < 1.0: not worth computing
          if (mvmt_norm < 1.0)
            {
              continue;
            }
          double capped_mvmt_norm=std::min(mvmt_norm, m_Temp);
          ImVec2 capped_mvmt=m_Mvmts[v_id] / mvmt_norm * capped_mvmt_norm;

          m_Positions[v_id].m_Pos=m_Positions[v_id].m_Pos+capped_mvmt;
        }
    }

  // Cool down fast until we reach 1.5, then stay at low temperature
  if(m_Temp>0.1)
    {
      m_Temp*=0.85;
    }
  else
    {
      m_Temp=0.1;
    }
}




void FruchtermanReingold::Step(int aStepSize,int aMaxStep,vector<NsPosition>& aPositions)
{
  if(m_CurrIter>=aMaxStep && aMaxStep>0)
    {
      for(int k=0;k<aPositions.size();++k)
        {
          aPositions[k].m_Fixed=m_Positions[k].m_Fixed;
          aPositions[k].m_Pos=m_Positions[k].m_Pos;
        }
      return;
    }

  if (!m_CurrIter)
    {
      SetInitPositions();
    }

  for(int k=0;k<aStepSize && m_CurrIter<m_MaxIter;++k)
    {
      DoStep();
      m_CurrIter++;
    }


  if(m_CurrIter>=m_MaxIter)
    {
      m_CurrIter=1;
      m_MaxIter++;

      for(int k=0;k<aPositions.size();++k)
        {
          aPositions[k].m_Fixed=m_Positions[k].m_Fixed;
          aPositions[k].m_Pos=m_Positions[k].m_Pos;
        }
    }
}




void FruchtermanReingold::SetInitPositions()
{
  nodesoup::SetInitPositions(m_StartCircle,m_Positions);
}




void FruchtermanReingold::SetK(double aK) noexcept
{
  m_K=aK;
  m_KSquared=aK*aK;
}




void FruchtermanReingold::MovePos(vertex_id_t aVertexId,const ImVec2& aDisp,bool aRecalculate)
{
  // TODO: assert aVertexId en rango
  if(aRecalculate)
    {
      if(aDisp.x==kInvalidPos && aDisp.y==kInvalidPos)
        {
          if(m_Positions[aVertexId].m_Fixed)
            {
              m_Positions[aVertexId].m_Fixed=false;
            }
        }

      m_CurrIter=1;
      m_MaxIter=0;
      return;
    }

  m_Positions[aVertexId].m_Pos+=aDisp;
  m_Positions[aVertexId].m_Fixed=true;
}


}
