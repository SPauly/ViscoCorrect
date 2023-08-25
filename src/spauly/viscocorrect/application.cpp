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

#include "spauly/viscocorrect/application.h"

#include <chrono>
#include <memory>
#include <thread>

#include "spauly/viscocorrect/util/timer.h"

namespace viscocorrect {
Application::Application(ApplicationBase *application_implementaion)
    : frontend_impl_{application_implementaion} {
  event_callback_ = std::make_shared<EventCallbackType>(
      std::bind(&Application::PushEvent, this, std::placeholders::_1));
  projects_ = std::make_shared<std::vector<Project>>(1);

  frontend_impl_->set_event_callback(event_callback_);

  HandleEvents();  // Handle events registered during initialization so that the
                   // process is completed
}

Application::~Application() {
  // call projectmanager cleanups
}

void Application::Run() {
  long long sleep_time = 0;
  util::Timer frame_timer, sleep_calc_timer;

  if (!frontend_impl_->Init()) {
    event_que_.clear();
    projects_->clear();
    return;
  }

  HandleEvents();  // Handle events registered during initialization so that the
                   // process is completed

  should_close_ = !frontend_impl_->Render();

  while (!should_close_) {

    HandleEvents();
    should_close_ = !frontend_impl_->Render();
    
  }
  Shutdown();
}

void Application::Shutdown() {
  event_que_.clear();
  frontend_impl_->Shutdown();
  projects_->clear();
}

void Application::PushEvent(std::unique_ptr<util::EventBase> event) {
  util::PushEvent(&event_que_, std::move(event));
}

void Application::HandleEvents() {
  for (size_t i = 0; i < event_que_.size(); i++) {
    auto event = std::move(event_que_.front());
    switch (event->get_event_type_()) {
      case util::kCalcReq:
        calculator_.Calculate(event->GetData<Project>());
        break;

      case util::kProjectListReq:
        *event->GetData<std::shared_ptr<std::vector<Project>>>() = projects_;
        break;

      case util::kGetGraphInstance:
        *event->GetData<std::shared_ptr<GraphImplBase>>() =
            frontend_impl_->get_graph();
        break;

      default:
        break;
    }
    event_que_.pop_front();
  }
}
}  // namespace viscocorrect
