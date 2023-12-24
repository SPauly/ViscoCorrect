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

#include <Windows.h>

#include "spauly/viscocorrect/application.h"
#include "spauly/viscocorrect/frontend/imgui_glfw/application_impl_imgui_glfw.h"
#include "spauly/viscocorrect/frontend/imgui_glfw/graph_impl_imgui_glfw.h"

namespace viscocorrect {
namespace frontend {
namespace imgui_glfw {
void ViscoCorrectMainImGuiGlfw() {
  viscocorrect::frontend::imgui_glfw::ApplicationImplImguiGlfw frontend_impl;
  viscocorrect::Application* app =
      viscocorrect::CreateApplication(&frontend_impl);
  app->Run();
  delete app;
}
}  // namespace imgui_glfw
}  // namespace frontend
}  // namespace viscocorrect

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
  viscocorrect::frontend::imgui_glfw::ViscoCorrectMainImGuiGlfw();
  return 0;
}
