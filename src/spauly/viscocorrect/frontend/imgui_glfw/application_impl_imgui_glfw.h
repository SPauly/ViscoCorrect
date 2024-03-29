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
  void Disclaimer();

  void ConfigWindow();
  void SetStyle();
  void DarkMode();
  void LightMode();
  void HelpMarker(const char *description, const char *marker = nullptr);
  void HyperLink(const char *link, const char *marker = nullptr);

 private:
  // config
  bool use_open_workspace = false;
  bool show_graph_ = false;
  bool use_dark_mode = false;

  // internal use
  GLFWwindow *window_;
  ImGuiViewport *viewport_;
  ImGuiIO *io_;
  bool submitting_feedback_ = false;

  // Appearence
  const int display_w_ = 445;
  const int display_h_ = 650;
  const int display_w_offset_graph_ = 445;
  int temp_display_w_, temp_display_h_;  // for temporary use
  const ImGuiWindowFlags closed_workspace_flags_ = ImGuiWindowFlags_NoCollapse |
                                                   ImGuiWindowFlags_NoMove |
                                                   ImGuiWindowFlags_NoResize;
  const ImGuiWindowFlags open_workspace_flags_ =
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
  const float rounding_ = 2.0;

  // Style
  ImVec4 clear_color_ = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  ImVec4 *colors_ = nullptr;
  ImGuiStyle *style_ = nullptr;

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