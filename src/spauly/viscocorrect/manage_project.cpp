#include "spauly/viscocorrect/manage_project.h"

namespace viscocorrect
{
    ProjectManager::ProjectManager(std::shared_ptr<std::vector<Project>> projects, std::shared_ptr<EventCallbackType> event_callback)
        : projects_(projects), event_callback_(event_callback)
    {
    }

}