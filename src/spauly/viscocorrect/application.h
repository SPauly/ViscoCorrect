#ifndef SPAULY_VISCOCORRECT_APPLICATION_H
#define SPAULY_VISCOCORRECT_APPLICATION_H

#include <deque>
#include <functional>
#include <vector>
#include <chrono>

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
  long long sleep_time_ = 0;

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