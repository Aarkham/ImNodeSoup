
#include "nodesoup.hpp"
#include <cmath>
#include <cassert>





namespace nodesoup
{




void SetRadiuses(const adj_list_t& aAdjList,std::vector<NsPosition>& aPositions,float aMinRadius,float aK)
{
  assert(aPositions.size() == aAdjList.size());

  for (vertex_id_t v_id=0; v_id<aAdjList.size(); v_id++)
    {
      float delta = log2f(aK * aAdjList[v_id].size() / aAdjList.size());
      float radius = aMinRadius + 2.0f*std::max(0.0f, delta);
      aPositions[v_id].m_Radius=radius;
    }
}




void SetInitPositions(bool aCircleMode, std::vector<NsPosition>& aPositions)
{
  constexpr float kPIf=3.14159265358979323846f;

  if(aCircleMode)
    {
      float angle = 2.0f*kPIf/aPositions.size();
      for (vertex_id_t v_id=0; v_id<aPositions.size(); v_id++)
        {
          aPositions[v_id].m_Pos.x=cosf(v_id*angle);
          aPositions[v_id].m_Pos.y=sinf(v_id*angle);
          aPositions[v_id].m_Fixed=false;
        }
    }
  else
    {
      for(vertex_id_t v_id=0; v_id<aPositions.size(); v_id++)
        {
          float v1=(static_cast<float>(rand())/static_cast<float>(RAND_MAX));
          float v2=(static_cast<float>(rand())/static_cast<float>(RAND_MAX));

          aPositions[v_id].m_Pos.x=v1;
          aPositions[v_id].m_Pos.y=v2;
          aPositions[v_id].m_Fixed=false;
        }
    }
}



}
