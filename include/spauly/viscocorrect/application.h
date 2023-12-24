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

#ifndef SPAULY_VISCOCORRECT_APPLICATION_H
#define SPAULY_VISCOCORRECT_APPLICATION_H

#include <chrono>
#include <deque>
#include <functional>
#include <vector>
#include <thread>

#include "spauly/viscocorrect/application_base.h"
#include "spauly/viscocorrect/calculator.h"
#include "spauly/viscocorrect/manage_project.h"
#include "spauly/viscocorrect/project.h"
#include "spauly/viscocorrect/util/event.h"

namespace viscocorrect {
// defined in this file
class Application;
inline Application *CreateApplication(ApplicationBase *frontend_impl);

class Application {
 public:
  Application(ApplicationBase *frontend_impl);
  ~Application();

  void Run();

  void PushEvent(std::unique_ptr<util::EventBase> event);

 private:
  void Shutdown();

  void HandleEvents();

 private:
  bool should_close_ = false;

  ApplicationBase *frontend_impl_;

  Calculator calculator_;

  std::shared_ptr<ProjectManager> project_manager_;
  std::shared_ptr<std::vector<Project>> projects_;

  std::shared_ptr<EventCallbackType> event_callback_;
  std::deque<std::unique_ptr<util::EventBase>> event_que_;
};

Application *CreateApplication(ApplicationBase *frontend_impl) {
  return new Application(frontend_impl);
}
}  // namespace viscocorrect

#endif  // SPAULY_VISCOCORRECT_APPLICATION_H