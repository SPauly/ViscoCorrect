#ifndef SPAULY_VISCOCORRECT_FRONTEND_IMGUI_GLFW_APPLICATION_IMPL_IMGUI_GLFW_H
#define SPAULY_VISCOCORRECT_FRONTEND_IMGUI_GLFW_APPLICATION_IMPL_IMGUI_GLFW_H

#include "spauly/viscocorrect/application_base.h"

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

#include "imgui.h"
#include "spauly/viscocorrect/frontend/imgui_glfw/graph_impl_imgui_glfw.h"
#include "spauly/viscocorrect/frontend/util_frontend/layerstack.h"
#include "spauly/viscocorrect/project.h"

#ifdef DEBUG_BUILD
#include "spauly/viscocorrect/debug/debug_tools.h"
#endif

namespace viscocorrect {
namespace frontend {
namespace imgui_glfw {
class ApplicationImplImguiGlfw : public viscocorrect::ApplicationBase {
 public:
  ApplicationImplImguiGlfw();
  ~ApplicationImplImguiGlfw(){};

  virtual bool Init() override;
  virtual void Shutdown() override;

  virtual bool Render() override;

  virtual std::shared_ptr<GraphImplBase> get_graph() override { return graph_; }

 private:
  void RenderProjectManager();
  void SetStyle();

  void Feedback();

 private:
  GLFWwindow *window_;
  int display_w_, display_h_;
  ImGuiIO *io_;

  // Style
  ImVec4 clear_color_ = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  ImVec4 *colors_ = nullptr;

  // utils
  util_frontend::LayerStack layer_stack_;
  std::shared_ptr<GraphImplImGuiGlfw> graph_;
#ifdef DEBUG_BUILD
  std::shared_ptr<debug::DebugTools> debug_tools_;
#endif
};

}  // namespace imgui_glfw

}  // namespace frontend

}  // namespace viscocorrect

#endif  // SPAULY_VISCOCORRECT_FRONTEND_IMGUI_GLFW_APPLICATION_IMPL_IMGUI_GLFW_H