#ifndef VISCOCORRECT_SRC_PROJECTMANAGER_H
#define VISCOCORRECT_SRC_PROJECTMANAGER_H

#include <memory>
#include <vector>
#include <functional>

#include "imgui.h"

#include "graph.h"
#include "utils/types.h"
#include "utils/layer.h"
#include "utils/event.h"
#include "utils/properties.h"

namespace viscocorrect
{

    class ProjectManager : public Layer
    {
    public:
        ProjectManager() = delete;
        ProjectManager(std::shared_ptr<std::vector<Project>> _projects, std::function<void(std::unique_ptr<utils::EventBase>)> _event_callback);
        ~ProjectManager() = default;

        virtual void OnUIRender() override;

    private:
        std::function<void(std::unique_ptr<utils::EventBase>)> event_callback_;

        std::shared_ptr<std::vector<Project>> projects_;
    };
}

#endif // VISCOCORRECT_SRC_PROJECTMANAGER_H