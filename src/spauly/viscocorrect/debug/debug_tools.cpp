#include "spauly/viscocorrect/debug/debug_tools.h"

namespace viscocorrect {
namespace debug {
DebugTools::DebugTools(std::shared_ptr<EventCallbackType> callback)
    : curve_fitting_(callback) {}

DebugTools::~DebugTools() { tool_stack_.clear(); }

void DebugTools::OnUIRender() {
  // ImGui::ShowDemoWindow();  //this leads to an error fix in #8
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