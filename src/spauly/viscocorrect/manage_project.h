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