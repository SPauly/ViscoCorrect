#ifndef SPAULY_VISCOCORRECT_FRONTEND_IMGUI_GLFW_GRAPH_IMPL_IMGUI_GLFW_H
#define SPAULY_VISCOCORRECT_FRONTEND_IMGUI_GLFW_GRAPH_IMPL_IMGUI_GLFW_H

#include <functional>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include "imgui.h"

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

  inline bool set_use_autofit(bool fit = false) { return use_autofit_ = fit; }

 protected:
  void Autofit(){};

  void RenderFlowrate();
  void RenderFunctions();
  void RenderCorrection();

 private:
  bool use_autofit_ = false;
  float scaling_factor_ = 1.0f;

  // internal use
  ImVec2 plot_size_1_;  // gotta change these names to be more convinient
  ImVec2 plot_size_2_;

  GraphCoordsStorage *graph_coords_;
};
}  // namespace imgui_glfw

}  // namespace frontend

}  // namespace viscocorrect

#endif  // SPAULY_VISCOCORRECT_FRONTEND_IMGUI_GLFW_GRAPH_IMPL_IMGUI_GLFW_H