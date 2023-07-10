#ifndef SPAULY_VISCOCORRECT_PROJECTMANAGER_H
#define SPAULY_VISCOCORRECT_PROJECTMANAGER_H

#include <functional>
#include <memory>
#include <vector>

#include "spauly/viscocorrect/project.h"
#include "spauly/viscocorrect/util/event.h"

namespace viscocorrect {

class ProjectManager {
 public:
  ProjectManager() = delete;
  ProjectManager(std::shared_ptr<std::vector<Project>> _projects,
                 std::shared_ptr<EventCallbackType> _event_callback);
  ~ProjectManager() = default;

 private:
  std::shared_ptr<EventCallbackType> event_callback_;

  std::shared_ptr<std::vector<Project>> projects_;
};
}  // namespace viscocorrect

#endif  // SPAULY_VISCOCORRECT_PROJECTMANAGER_H