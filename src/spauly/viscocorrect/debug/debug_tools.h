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

#ifndef SPAULY_VISCOCORRECT_DEBUG_DEBUG_TOOLS_H
#define SPAULY_VISCOCORRECT_DEBUG_DEBUG_TOOLS_H

#include <imgui.h>

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "spauly/viscocorrect/debug/curve_fitting.h"
#include "spauly/viscocorrect/frontend/util_frontend/layer.h"
#include "spauly/viscocorrect/frontend/util_frontend/layerstack.h"
#include "spauly/viscocorrect/util/event.h"

namespace viscocorrect {
namespace debug {
class DebugTools : public viscocorrect::frontend::util_frontend::Layer {
 public:
  DebugTools(std::shared_ptr<EventCallbackType> callback);
  virtual ~DebugTools();

  virtual void OnUIRender() override;

  template <typename T>
  void AddTool(const std::shared_ptr<T> &_t) {
    tool_stack_.PushLayer(_t);
  }

 private:
  viscocorrect::frontend::util_frontend::LayerStack tool_stack_;
  CurveFitting curve_fitting_;
};

class DebugToolBase : public viscocorrect::frontend::util_frontend::Layer {
 public:
  DebugToolBase() = default;
  DebugToolBase(const std::string &);
  virtual ~DebugToolBase() = default;

  virtual void OnUIRender() final;

  // overload this function
  virtual void Run() = 0;

  template <typename Func, typename Obj>
  std::unique_ptr<std::function<void()>> GetCallback(Func func, Obj *obj) {
    auto callback =
        std::make_unique<std::function<void()>>(std::bind(func, obj));
    return callback;
  }

  void AddCallback(std::unique_ptr<std::function<void()>>);

 protected:
  std::vector<std::unique_ptr<std::function<void()>>> callbacks_;

  void RunCallbacks();

 private:
  std::string name_;
};

}  // namespace debug
}  // namespace viscocorrect

#endif  // SPAULY_VISCOCORRECT_DEBUG_DEBUG_TOOLS_H