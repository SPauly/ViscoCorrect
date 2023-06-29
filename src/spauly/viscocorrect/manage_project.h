#ifndef SPAULY_VISCOCORRECT_SRC_PROJECTMANAGER_H
#define SPAULY_VISCOCORRECT_SRC_PROJECTMANAGER_H

#include "spauly/viscocorrect/project.h"

#include <memory>
#include <vector>
#include <functional>

#include "spauly/viscocorrect/graph.h"
#include "spauly/viscocorrect/util/event.h"

namespace viscocorrect
{

    class ProjectManager : public frontend::util_frontend::Layer
    {
    public:
        ProjectManager() = delete;
        ProjectManager(std::shared_ptr<std::vector<Project>> _projects, std::function<void(std::unique_ptr<util::EventBase>)> _event_callback);
        ~ProjectManager() = default;

        virtual void OnUIRender() override;

    private:
        std::function<void(std::unique_ptr<util::EventBase>)> event_callback_;

        std::shared_ptr<std::vector<Project>> projects_;
    };
}

#endif // SPAULY_VISCOCORRECT_SRC_PROJECTMANAGER_H