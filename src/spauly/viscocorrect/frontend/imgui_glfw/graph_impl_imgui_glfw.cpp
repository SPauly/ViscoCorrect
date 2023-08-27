// ViscoCorrect - Correction factors for centrifugal pumps
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

#include "spauly/viscocorrect/frontend/imgui_glfw/graph_impl_imgui_glfw.h"

namespace viscocorrect {
namespace frontend {
namespace imgui_glfw {
GraphImplImGuiGlfw::GraphImplImGuiGlfw() {
  // this has to be changed internal:: should not be accessed here
  plot_size_1_ = ImVec2(get_graph_ctx().GetTableWith(),
                        get_graph_ctx().GetTableParameterHeight());
  plot_size_2_ = ImVec2(get_graph_ctx().GetTableWith(),
                        get_graph_ctx().GetTableCorrectionHeight());

  graph_coords_ = &get_graph_ctx().GetGraphCoords(scaling_factor_);
}

GraphImplImGuiGlfw::~GraphImplImGuiGlfw() {}

void GraphImplImGuiGlfw::OnAttach() {
  ImPlot::CreateContext();
  get_graph_ctx().set_scaling_factor(1.0);
}

void GraphImplImGuiGlfw::OnDetach() { ImPlot::DestroyContext(); }

void GraphImplImGuiGlfw::OnUIRender() {
  ImGui::Begin("Viewport", nullptr, window_flags_);

  if (use_autofit_) Autofit();

  if (ImPlot::BeginPlot("##plot2", plot_size_2_, plot_flags_)) {
    // Set up GraphImplImGuiGlfw
    ImPlot::SetupAxis(ImAxis_X1, nullptr, axis_flags_);
    ImPlot::SetupAxis(ImAxis_Y1, nullptr, axis_flags_);
    ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, plot_size_2_.x);
    ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0, plot_size_2_.y);

    // Render the different functions
    RenderFlowrate();
    RenderCorrection();

    // Call the callbacks
    RunCallbacksPlot2();

    ImPlot::EndPlot();
  }

  if (ImPlot::BeginPlot("##plot1", plot_size_1_, plot_flags_)) {
    // Set up GraphImplImGuiGlfw
    ImPlot::SetupAxis(ImAxis_X1, nullptr, axis_flags_);
    ImPlot::SetupAxis(ImAxis_Y1, nullptr, axis_flags_);
    ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, plot_size_1_.x);
    ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0, plot_size_1_.y);

    // Render the different functions
    RenderFlowrate();
    RenderFunctions();

    // Call the callbacks
    RunCallbacksPlot1();

    ImPlot::EndPlot();
  }

  ImGui::End();
}

void GraphImplImGuiGlfw::RenderFlowrate() {
  for (auto &pair :
       get_graph_ctx().GetGraphCoords(scaling_factor_).flowrate_coords) {
    ImPlot::PlotLine("##flowrates", pair.second.get_array_notation().x_coords,
                     pair.second.get_array_notation().y_coords, 2);
  }
}

void GraphImplImGuiGlfw::RenderFunctions() {
  for (auto &pair :
       get_graph_ctx().GetGraphCoords(scaling_factor_).totalhead_coords) {
    ImPlot::PlotLine("##totalh", pair.second.get_array_notation().x_coords,
                     pair.second.get_array_notation().y_coords, 2);
  }
  for (auto &pair :
       get_graph_ctx().GetGraphCoords(scaling_factor_).viscosity_coords) {
    ImPlot::PlotLine("##viscosity", pair.second.get_array_notation().x_coords,
                     pair.second.get_array_notation().y_coords, 2);
  }
}

void GraphImplImGuiGlfw::RenderCorrection() {
  ImPlot::PlotLine("##c_v", graph_coords_->x_coords_v.data(),
                   graph_coords_->y_coords_v.data(),
                   graph_coords_->x_coords_v.size());
  ImPlot::PlotLine("##Q", graph_coords_->x_coords_q.data(),
                   graph_coords_->y_coords_q.data(),
                   graph_coords_->x_coords_q.size());
  for (int i = 0; i < graph_coords_->x_coords_h.size(); i++) {
    ImPlot::PlotLine("##ch", graph_coords_->x_coords_h.at(i).data(),
                     graph_coords_->y_coords_h.at(i).data(),
                     graph_coords_->x_coords_h.at(i).size());
  }
}
}  // namespace imgui_glfw
}  // namespace frontend

}  // namespace viscocorrect
