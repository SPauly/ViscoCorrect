// ViscoCorrect - Correction factors centrifugal pumps
// Copyright (C) 2023  Simon Pauly
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
// Contact via <https://github.com/SPauly/ViscoCorrect>

#ifndef SPAULY_VISCOCORRECT_FRONTEND_IMGUI_GLFW_GRAPH_IMPL_IMGUI_GLFW_H
#define SPAULY_VISCOCORRECT_FRONTEND_IMGUI_GLFW_GRAPH_IMPL_IMGUI_GLFW_H

#include <imgui.h>
#include <implot.h>

#include <functional>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include "spauly/viscocorrect/frontend/util_frontend/layer.h"
#include "spauly/viscocorrect/graph_base.h"
#include "spauly/viscocorrect/util/mathematical_functions.h"

namespace viscocorrect {
namespace frontend {
namespace imgui_glfw {

class GraphImplImGuiGlfw : public frontend::util_frontend::Layer,
                           public viscocorrect::GraphImplBase {
 public:
  GraphImplImGuiGlfw();
  virtual ~GraphImplImGuiGlfw();

  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnUIRender() override;

  inline void set_window_flags(const ImGuiWindowFlags &flags) {
    window_flags_ = flags;
  };

  inline bool set_use_autofit(bool fit = false) { return use_autofit_ = fit; }

 protected:
  void Autofit(){};

  void RenderFlowrate();
  void RenderFunctions();
  void RenderCorrection();

 private:
  // config
  bool use_autofit_ = false;
  float scaling_factor_ = 1.0f;

  // Style
  ImGuiWindowFlags window_flags_ = ImGuiWindowFlags_NoCollapse;
  ImPlotFlags plot_flags_ = ImPlotFlags_CanvasOnly | ImPlotFlags_Crosshairs;
  ImPlotAxisFlags axis_flags_ =
      ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_NoMenus;

  // internal use
  ImVec2 plot_size_1_;  // gotta change these names to be more convinient
  ImVec2 plot_size_2_;

  GraphCoordsStorage *graph_coords_;
};
}  // namespace imgui_glfw

}  // namespace frontend

}  // namespace viscocorrect

#endif  // SPAULY_VISCOCORRECT_FRONTEND_IMGUI_GLFW_GRAPH_IMPL_IMGUI_GLFW_H