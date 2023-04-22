


#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <cassert>


#include "imgui.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include <nodesoup.hpp>
#include "fruchterman_reingold.hpp"
#include "kamada_kawai.hpp"


const char* k6_dot=R"str(graph {
v0 -- v1;
v1 -- v2;
v2 -- v3;
v3 -- v4;
v4 -- v5;
v0 -- v5;
v0 -- v2;
v0 -- v3;
v0 -- v4;
v1 -- v3;
v1 -- v4;
v1 -- v5;
v2 -- v4;
v2 -- v5;
v3 -- v5;
})str";




const char* k6_2_dot = R"str(graph {
v0 -- v1;
v1 -- v2;
v2 -- v3;
v3 -- v4;
v4 -- v5;
v5 -- v0;
v5 -- v6;
v6 -- v7;
v7 -- v8;
v8 -- v9;
v9 -- v10;
v10 -- v5;
})str";




const char* small_dense_dot = R"str(graph {
v0 -- v1;
v0 -- v2;
v0 -- v3;
v1 -- v2;
v1 -- v4;
v2 -- v4;
v2 -- v5;
v3 -- v5;
v3 -- v6;
v4 -- v7;
v5 -- v7;
v5 -- v8;
v5 -- v9;
v5 -- v6;
v6 -- v10;
v7 -- v14;
v7 -- v11;
v8 -- v11;
v8 -- v12;
v8 -- v9;
v9 -- v12;
v9 -- v13;
v9 -- v10;
v10 -- v13;
v10 -- v17;
v11 -- v14;
v11 -- v12;
v12 -- v14;
v12 -- v15;
v12 -- v13;
v13 -- v16;
v13 -- v17;
v14 -- v18;
v14 -- v15;
v15 -- v18;
v15 -- v19;
v15 -- v16;
v16 -- v19;
v16 -- v17;
v17 -- v19;
v18 -- v20;
v19 -- v20;
})str";




const char* bin_tree_dot = R"str(graph G {
v0
v0 -- v1
v1
v1 -- v2
v2
v2 -- v3
v2 -- v4
v3
v3 -- v5
v3 -- v6
v4
v4 -- v7
v4 -- v8
v5
v5 -- v9
v5 -- v10
v6
v6 -- v11
v6 -- v12
v7
v7 -- v13
v7 -- v14
v8
v8 -- v15
v9
v9 -- v16
v9 -- v17
v10
v10 -- v18
v10 -- v19
v11
v11 -- v20
v12
v12 -- v21
v13
v13 -- v22
v13 -- v23
v14
v14 -- v24
v15
v15 -- v25
v16
v16 -- v26
v17
v17 -- v27
v18
v18 -- v28
v19
v19 -- v29
v20
v20 -- v30
v21
v21 -- v31
v21 -- v32
v22
v22 -- v33
v22 -- v34
v23
v23 -- v35
v24
v24 -- v36
v24 -- v37
v25
v25 -- v38
v26
v26 -- v39
v27
v27 -- v40
v28
v28 -- v41
v29
v29 -- v42
v29 -- v43
v30
v30 -- v44
v30 -- v45
v31
v31 -- v46
v31 -- v47
v32
v32 -- v48
v33
v33 -- v49
v34
v34 -- v50
v34 -- v51
v35
v35 -- v52
v36
v36 -- v53
v37
v37 -- v54
v38
v38 -- v55
v38 -- v56
v39
v39 -- v57
v39 -- v58
v40
v40 -- v59
v41
v41 -- v60
v41 -- v61
v42
v42 -- v62
v43
v43 -- v63
v44
v44 -- v64
v45
v45 -- v65
v46
v46 -- v66
v47
v47 -- v67
v47 -- v68
v48
v48 -- v69
v49
v49 -- v70
v50
v50 -- v71
v50 -- v72
v51
v51 -- v73
v52
v52 -- v74
v53
v53 -- v75
v54
v54 -- v76
v55
v55 -- v77
v55 -- v78
v56
v56 -- v79
v56 -- v80
v57
v57 -- v81
v57 -- v82
v58
v58 -- v83
v59
v59 -- v84
v60
v60 -- v85
v61
v61 -- v86
v61 -- v87
v62
v62 -- v88
v62 -- v89
v63
v63 -- v90
v63 -- v91
v64
v64 -- v92
v64 -- v93
v65
v65 -- v94
v66
v66 -- v95
v67
v67 -- v96
v68
v68 -- v97
v68 -- v98
v69
v69 -- v99
v70
v71
v72
v73
v74
v75
v76
v77
v78
v79
v80
v81
v82
v83
v84
v85
v86
v87
v88
v89
v90
v91
v92
v93
v94
v95
v96
v97
v98
v99
})str";




const char* quad_tree_dot = R"str(graph G {
v0
v0 -- v1
v0 -- v2
v0 -- v3
v0 -- v4
v1
v1 -- v5
v2
v2 -- v6
v2 -- v7
v3
v3 -- v8
v3 -- v9
v4
v4 -- v10
v4 -- v11
v5
v5 -- v12
v5 -- v13
v6
v6 -- v14
v6 -- v15
v6 -- v16
v6 -- v17
v7
v7 -- v18
v7 -- v19
v7 -- v20
v8
v8 -- v21
v8 -- v22
v8 -- v23
v9
v9 -- v24
v10
v10 -- v25
v11
v11 -- v26
v11 -- v27
v12
v12 -- v28
v12 -- v29
v12 -- v30
v12 -- v31
v13
v13 -- v32
v13 -- v33
v13 -- v34
v14
v14 -- v35
v14 -- v36
v15
v15 -- v37
v15 -- v38
v15 -- v39
v16
v16 -- v40
v17
v17 -- v41
v17 -- v42
v17 -- v43
v18
v18 -- v44
v18 -- v45
v18 -- v46
v19
v19 -- v47
v19 -- v48
v20
v20 -- v49
v20 -- v50
v20 -- v51
v21
v21 -- v52
v22
v22 -- v53
v23
v23 -- v54
v23 -- v55
v24
v24 -- v56
v24 -- v57
v24 -- v58
v25
v25 -- v59
v26
v26 -- v60
v26 -- v61
v27
v27 -- v62
v27 -- v63
v28
v28 -- v64
v28 -- v65
v29
v29 -- v66
v29 -- v67
v29 -- v68
v29 -- v69
v30
v30 -- v70
v30 -- v71
v30 -- v72
v31
v31 -- v73
v31 -- v74
v31 -- v75
v32
v32 -- v76
v33
v33 -- v77
v33 -- v78
v33 -- v79
v34
v34 -- v80
v35
v35 -- v81
v35 -- v82
v36
v36 -- v83
v37
v37 -- v84
v37 -- v85
v37 -- v86
v38
v38 -- v87
v38 -- v88
v38 -- v89
v39
v39 -- v90
v39 -- v91
v39 -- v92
v40
v40 -- v93
v40 -- v94
v41
v41 -- v95
v42
v42 -- v96
v42 -- v97
v43
v43 -- v98
v43 -- v99
v44
v45
v46
v47
v48
v49
v50
v51
v52
v53
v54
v55
v56
v57
v58
v59
v60
v61
v62
v63
v64
v65
v66
v67
v68
v69
v70
v71
v72
v73
v74
v75
v76
v77
v78
v79
v80
v81
v82
v83
v84
v85
v86
v87
v88
v89
v90
v91
v92
v93
v94
v95
v96
v97
v98
v99
})str";






// Read not-too-complicated dot files
nodesoup::adj_list_t read_from_dot(const char* aDotData)
{
  nodesoup::adj_list_t adj_list;

  std::istringstream ifs(aDotData);
  if (!ifs.good())
    {
      return adj_list;
    }

  std::unordered_map<std::string, nodesoup::vertex_id_t> names;

  auto name_to_vertex_id = [&adj_list,&names](std::string name) -> nodesoup::vertex_id_t
  {
    if(name[name.size() - 1] == ';')
      {
        name.erase(name.end() - 1, name.end());
      }

    nodesoup::vertex_id_t v_id;
    auto it = names.find(name);
    if (it != names.end())
      {
        return (*it).second;
      }

    v_id = adj_list.size();
    names.insert({ name, v_id });
    adj_list.resize(v_id + 1);
    return v_id;
  };

  std::string line;
  // skip first line
  std::getline(ifs, line);

  while (std::getline(ifs, line))
    {
      if (line[0] == '}')
        {
          break;
        }

      std::istringstream iss(line);
      std::string name, edge_sign, adj_name;
      iss >> name >> edge_sign >> adj_name;

      // add vertex if new
      nodesoup::vertex_id_t v_id = name_to_vertex_id(name);

      assert(edge_sign == "--" || edge_sign.size() == 0);
      if (edge_sign != "--")
        {
          continue;
        }

      // add adjacent vertex if new
      nodesoup::vertex_id_t adj_id = name_to_vertex_id(adj_name);

      // add edge if new
      if (find(adj_list[v_id].begin(),adj_list[v_id].end(), adj_id) == adj_list[v_id].end())
        {
          adj_list[v_id].push_back(adj_id);
          adj_list[adj_id].push_back(v_id);
        }
    }

  return adj_list;
}





const float kWindowInitWidth = 800.0f;
const float kWindowInitHeight= 600.0f;
static ImVec2 gDisp{0.0f,0.0f};
static float  gScale=1.0f;




struct MoveRes
{
  bool   m_Moved;
  bool   m_Recalculate;
  nodesoup::vertex_id_t m_Index;
  ImVec2 m_Disp;
};





static inline float sq_dist(const ImVec2& aPos1,const ImVec2& aPos2) noexcept
{
  ImVec2 d=aPos2-aPos1;
  return d.x*d.x + d.y*d.y;
}



static ImVec2 GetStartPos() noexcept
{
  ImGuiWindow* window = ImGui::GetCurrentWindowRead();
  return window->Pos+gDisp;
}



static nodesoup::vertex_id_t GetPosAt(const ImVec2& aPos,const std::vector<NsPosition>& aPositions)
{
  ImVec2 origin(kWindowInitWidth / 2.0, kWindowInitHeight / 2.0);
  ImVec2 cursor_pos=GetStartPos();

  for(nodesoup::vertex_id_t v_id=0; v_id<aPositions.size(); v_id++)
    {
      ImVec2 v_pos = aPositions[v_id].m_Pos*gScale+origin+cursor_pos;

      if(sq_dist(v_pos,aPos)< 2.0f*aPositions[v_id].m_Radius*aPositions[v_id].m_Radius)
        {
          return v_id;
        }
    }

  return -1;
}



static MoveRes MovePos(const std::vector<NsPosition>& aPositions)
{
  MoveRes res;

  ImGuiIO& io = ImGui::GetIO();

  if(!io.MouseDown[1] && !io.MouseReleased[1])
    {
      res.m_Moved=false;
      res.m_Recalculate=false;

      return res;
    }

  ImVec2 mouse_pos=io.MousePos;
  nodesoup::vertex_id_t v_id=GetPosAt(mouse_pos,aPositions);
  if(v_id==-1) // Not over a node
    {
      res.m_Moved=false;
      res.m_Recalculate=false;
      return res;
    }

  if(io.MouseDown[1])
    {
      res.m_Index=v_id;
      res.m_Disp=io.MouseDelta/gScale;
      res.m_Moved=true;
      res.m_Recalculate=false;
      return res;
    }

  if(io.MouseReleased[1])
    {
      res.m_Index=v_id;
      res.m_Recalculate=true;

      if(io.MouseDragMaxDistanceSqr[1]<10)
        {
          res.m_Disp={kInvalidPos,kInvalidPos};
        }
      else
        {
          res.m_Disp=io.MouseDelta/gScale;
        }

      return res;
    }

  assert(0);

  res.m_Moved=false;
  res.m_Recalculate=false;

  return res;


}




static void DrawData(const nodesoup::adj_list_t& aAdjList,const std::vector<NsPosition>& aPositions,bool aAllowMove
                    ,bool aDrawDebug)
{
  ImGuiWindow* w=ImGui::GetCurrentWindow();
  ImGuiIO& io=ImGui::GetIO();

  if(io.MouseDownDuration[1]>0.0 && aAllowMove)
    {
      if(w->InnerClipRect.Contains(io.MousePos))
        {
          gDisp+=io.MouseDelta;
        }
    }

  if(io.MouseWheel>0.0f)
    {
      gScale*=1.1f;
    }
  else if(io.MouseWheel<0.0f)
    {
      gScale*=0.9f;
    }

  ImDrawList* draw_list=ImGui::GetWindowDrawList();
  ImVec2 cursor_pos=GetStartPos();

  ImVec2 origin(kWindowInitWidth/2.0,kWindowInitHeight/2.0);

  const ImU32 node_col=ImGui::GetColorU32(ImGuiCol_ScrollbarGrabActive);
  const ImU32 node_fix_col=ImGui::GetColorU32(ImGuiCol_NavHighlight);
  const ImU32 arc_col =ImGui::GetColorU32(ImGuiCol_ScrollbarGrab);
  const ImU32 txt_col =ImGui::GetColorU32(ImGuiCol_PlotLinesHovered);


  for(nodesoup::vertex_id_t v_id=0; v_id<aAdjList.size(); v_id++)
    {
      const NsPosition& curr_pos=aPositions[v_id];
      ImVec2 v_pos=curr_pos.m_Pos*gScale+origin;

      for(auto adj_id:aAdjList[v_id])
        {
          if(adj_id < v_id)
            {
              continue;
            }

          ImVec2 adj_pos=aPositions[adj_id].m_Pos*gScale+origin;
          draw_list->AddLine(cursor_pos+v_pos,cursor_pos+adj_pos,arc_col);
        }


      draw_list->AddCircleFilled(cursor_pos+ImVec2(v_pos.x,v_pos.y),curr_pos.m_Radius, curr_pos.m_Fixed?node_fix_col:node_col);
      if(aDrawDebug)
        {
          draw_list->AddText(cursor_pos+ImVec2(v_pos.x,v_pos.y), txt_col, std::to_string(v_id).c_str());
          draw_list->AddText(cursor_pos+ImVec2(v_pos.x,v_pos.y+20.0f), txt_col, std::to_string(v_pos.x).c_str());
          draw_list->AddText(cursor_pos+ImVec2(v_pos.x,v_pos.y+40.0f), txt_col, std::to_string(v_pos.y).c_str());
        }
    }

  ImGuiContext& g = *GImGui;
  ImGui::SetCursorPos({20.0f,w->InnerClipRect.GetHeight()-g.FontSize });
  ImGui::Text("x:%.3f  y:%.3f  scale:%.3f",gDisp.x,gDisp.y,gScale);
}




void ShowNodeSoup()
{
  static std::vector<NsPosition> positions;
  static std::vector<float> radiuses;

  float k=15.0;

  static nodesoup::adj_list_t adj_list;
  static nodesoup::FruchtermanReingold fr(adj_list,k);
  static nodesoup::KamadaKawai ka(adj_list,k);

  constexpr int kFruchtermanReingold=0;
  constexpr int kKamadaKawai=1;
  static int method=kFruchtermanReingold;

  constexpr int kCircle=0;
  constexpr int kRandom=1;
  static int init_mode=kCircle;

  static bool draw_debug=false;


  ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Appearing);
  ImGui::SetNextWindowSize(ImVec2(kWindowInitWidth,kWindowInitHeight), ImGuiCond_Appearing);

  if (ImGui::Begin("NodeSoup",nullptr))
    {
      int prev_method=method;

      ImGui::BeginGroup();
        ImGui::RadioButton("Fruchterman Reingold",&method,kFruchtermanReingold);
        ImGui::RadioButton("Kamada Kawai",&method,kKamadaKawai);
      ImGui::EndGroup();

      ImGui::SameLine(350.0f);

      ImGui::BeginGroup();
        ImGui::RadioButton("Init circle",&init_mode,kCircle);
        ImGui::RadioButton("Init random",&init_mode,kRandom);
      ImGui::EndGroup();

      const char* items[]={"None","K6","K6-2","Small dense","Bin tree","Quad tree"};
      const char* items_data[] = {"", k6_dot,k6_2_dot,small_dense_dot,bin_tree_dot,quad_tree_dot};
      static int item_current=0;
      bool change=ImGui::Combo("Data", &item_current,items,IM_ARRAYSIZE(items));
      ImGui::SameLine();
      change|=ImGui::SmallButton("R");

      ImGui::NewLine();
      ImGui::Checkbox("Show debug info",&draw_debug);
      if(draw_debug)
        {
          ImGui::NewLine();
          ImGui::Text("Energy: %.3f",static_cast<float>(method==kFruchtermanReingold?fr.GetEnergy() : ka.GetEnergy()  ));
        }

      if(prev_method!=method || change)
        {
          adj_list=read_from_dot(items_data[item_current]);
          positions.resize(adj_list.size());
          nodesoup::SetRadiuses(adj_list,positions);


          if(method==kFruchtermanReingold)
            {
              fr.Start(init_mode==kCircle);
            }
          else
            {
              ka.Start(init_mode==kCircle);
            }
        }

        if(!adj_list.empty())
          {
            if(method==kFruchtermanReingold)
              {
                fr.Step(15,0,positions);
              }
            else
              {
                ka.Step(kWindowInitWidth,kWindowInitHeight,positions);
              }
          }

      MoveRes r=MovePos(positions);
      if(r.m_Moved)
        {
          if(method==kFruchtermanReingold)
            {
              fr.MovePos(r.m_Index,r.m_Disp,r.m_Recalculate);
            }
          else
            {
              ka.MovePos(r.m_Index,r.m_Disp,r.m_Recalculate);
            }
        }

      DrawData(adj_list,positions,!r.m_Moved,draw_debug);

      ImGui::End();
    }

}