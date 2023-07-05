#ifndef SPAULY_VISCOCORRECT_PROJECTMANAGER_H
#define SPAULY_VISCOCORRECT_PROJECTMANAGER_H

#include "spauly/viscocorrect/project.h"

#include <memory>
#include <vector>
#include <functional>

#include "spauly/viscocorrect/util/event.h"

namespace viscocorrect
{

    class ProjectManager
    {
    public:
        ProjectManager() = delete;
        ProjectManager(std::shared_ptr<std::vector<Project>> _projects, std::function<void(std::unique_ptr<util::EventBase>)> _event_callback);
        ~ProjectManager() = default;

    private:
        std::function<void(std::unique_ptr<util::EventBase>)> event_callback_;

        std::shared_ptr<std::vector<Project>> projects_;
    };
}

#endif // SPAULY_VISCOCORRECT_PROJECTMANAGER_H