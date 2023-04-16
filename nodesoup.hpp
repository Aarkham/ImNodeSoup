#pragma once
#include <vector>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"



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
std::vector<float> size_radiuses(const adj_list_t& aAdjList,float aMinRadius=4.0f, float aK=300.0f);


// Distribute vertices equally on a 1.0 radius circle
void circle(const adj_list_t& aAdjList,std::vector<ImVec2>& aPositions);


// Center and scale vertices so the graph fits on a canvas of given dimensions
void center_and_scale(unsigned int aWidth,unsigned int aHeight,std::vector<ImVec2>& aPositions);


}

