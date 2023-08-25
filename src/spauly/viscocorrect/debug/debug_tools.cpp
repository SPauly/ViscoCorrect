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

#include "spauly/viscocorrect/debug/debug_tools.h"

namespace viscocorrect {
namespace debug {
DebugTools::DebugTools(std::shared_ptr<EventCallbackType> callback)
    : curve_fitting_(callback) {}

DebugTools::~DebugTools() { tool_stack_.clear(); }

void DebugTools::OnUIRender() {
  ImGui::ShowDemoWindow();
  curve_fitting_.Render();

  ImGui::Begin("Debugging");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  for (auto &layer : tool_stack_) {
    layer->OnUIRender();
  }
  ImGui::End();
}

// DebugToolBase
DebugToolBase::DebugToolBase(const std::string &_name) : name_(_name) {}

void DebugToolBase::OnUIRender() {
  if (ImGui::CollapsingHeader(name_.c_str())) {
    Run();
    return;
  }
}

void DebugToolBase::AddCallback(
    std::unique_ptr<std::function<void()>> _callback) {
  callbacks_.push_back(std::move(_callback));
}

void DebugToolBase::RunCallbacks() {
  for (const auto &_func : callbacks_) {
    (*_func)();
  }
}
}  // namespace debug
}  // namespace viscocorrect