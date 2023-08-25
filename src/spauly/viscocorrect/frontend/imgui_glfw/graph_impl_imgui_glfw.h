#ifndef SPAULY_VISCOCORRECT_FRONTEND_IMGUI_GLFW_GRAPH_IMPL_IMGUI_GLFW_H
#define SPAULY_VISCOCORRECT_FRONTEND_IMGUI_GLFW_GRAPH_IMPL_IMGUI_GLFW_H

#include <functional>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include <imgui.h>
#include <implot.h>

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