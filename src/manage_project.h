#ifndef VISCOCORRECT_SRC_PROJECTMANAGER_H
#define VISCOCORRECT_SRC_PROJECTMANAGER_H

#include "project.h"

#include <memory>
#include <vector>
#include <functional>

#include "imgui.h"

#include "graph.h"
#include "util/layer.h"
#include "util/event.h"

namespace viscocorrect
{

    class ProjectManager : public util::Layer
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

#endif // VISCOCORRECT_SRC_PROJECTMANAGER_H