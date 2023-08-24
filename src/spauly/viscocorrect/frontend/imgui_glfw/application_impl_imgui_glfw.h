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
  virtual inline const float GetFramerate() override { return io_->Framerate; };

  virtual std::shared_ptr<GraphImplBase> get_graph() override { return graph_; }

 private:
  void ProjectManager();
  void MenuBar();
  void Feedback();

  void SetStyle();

  bool NeedRender();

 private:
  // config
  bool use_open_workspace = false;
  bool show_graph_ = false;

  // internal use
  GLFWwindow *window_;
  ImGuiViewport *viewport_;
  ImGuiIO *io_;
  const int update_frames_size_ = 20;
  int update_frames_counter_ = 0;
  ImVec2 prev_mouse_pos_;

  // Appearence
  const int display_w_ = 450;
  const int display_h_ = 650;
  const int display_w_offset_graph_ = 450;
  int temp_display_w_, temp_display_h_;  // for temporary use
  const ImGuiWindowFlags closed_workspace_flags_ =
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
  const ImGuiWindowFlags open_workspace_flags_ = ImGuiWindowFlags_NoCollapse;

  // Style
  ImVec4 clear_color_ = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  ImVec4 *colors_ = nullptr;
  bool use_dark_mode = false;

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