
#include "nodesoup.hpp"
#include <cmath>
#include <limits>


#ifndef M_PIf
  #define M_PIf 3.14159265358979323846f
#endif





namespace nodesoup
{


std::vector<float> size_radiuses(const adj_list_t& aAdjList,float aMinRadius,float aK)
{
  std::vector<float> radiuses;
  radiuses.reserve(aAdjList.size());

  for (vertex_id_t v_id=0; v_id< aAdjList.size(); v_id++)
    {
      float delta=log2f(aK * aAdjList[v_id].size() / aAdjList.size());
      float radius= aMinRadius+std::max(0.0f,delta);
      radiuses.push_back(radius);
    }
  return radiuses;
}




void circle(const adj_list_t& aAdjList,std::vector<ImVec2>& aPositions)
{
  float angle = 2.0f*M_PIf / aAdjList.size();
  for (vertex_id_t v_id = 0; v_id < aAdjList.size(); v_id++)
    {
      aPositions[v_id].x=cosf(v_id*angle);
      aPositions[v_id].y=sinf(v_id*angle);
    }
}




void center_and_scale(unsigned int aWidth,unsigned int aHeight,std::vector<ImVec2>& aPositions)
{
  // find current dimensions
  float x_min=std::numeric_limits<float>::max();
  float x_max=std::numeric_limits<float>::lowest();
  float y_min=std::numeric_limits<float>::max();
  float y_max=std::numeric_limits<float>::lowest();

  for(vertex_id_t v_id=0; v_id< aPositions.size(); v_id++)
    {
      if(aPositions[v_id].x<x_min)
        {
          x_min=aPositions[v_id].x;
        }
      if(aPositions[v_id].x>x_max)
        {
          x_max=aPositions[v_id].x;
        }

      if(aPositions[v_id].y<y_min)
        {
          y_min=aPositions[v_id].y;
        }
      if(aPositions[v_id].y>y_max)
        {
          y_max=aPositions[v_id].y;
        }
    }

  float cur_width =x_max-x_min;
  float cur_height=y_max-y_min;

  // compute scale factor (0.9: keep some margin)
  float x_scale= aWidth/cur_width;
  float y_scale= aHeight/cur_height;
  float scale=0.9f * (x_scale<y_scale ? x_scale : y_scale);

  // compute offset and apply it to every position
  ImVec2 center={ x_max+x_min, y_max+y_min };
  ImVec2 offset=center / 2.0 * scale;
  for (vertex_id_t v_id=0; v_id< aPositions.size(); v_id++)
    {
      ImVec2 pos_scaled{ scale*aPositions[v_id].x ,scale*aPositions[v_id].y };
      aPositions[v_id]=pos_scaled-offset;
    }
}



}
