#pragma once
#include <vector>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"



struct NsPosition
{
  ImVec2 m_Pos;
  float  m_Radius;
  bool   m_Fixed;
};


constexpr float kInvalidPos=-1000000.0f;

inline double norm(const ImVec2& aImVec2) noexcept
{
  return sqrt(aImVec2.x * aImVec2.x + aImVec2.y * aImVec2.y);
}

namespace nodesoup
{

// Simple adjaceny list graph structure
using vertex_id_t = std::size_t;
using adj_list_t = std::vector<std::vector<vertex_id_t>>;


// Assigns diameters to vertices based on their degree
void SetRadiuses(const adj_list_t& aAdjList,std::vector<NsPosition>& aPositions,float aMinRadius=4.0f,float aK=300.0f);

// Distribute vertices equally on a 1.0 radius circle (aCircleMode==true) or randomly in unit square (aCircleMode==false)
void SetInitPositions(bool aCircleMode,std::vector<NsPosition>& aPositions);

}

