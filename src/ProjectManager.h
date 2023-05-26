#pragma once

#include <string>
#include <memory>
#include <vector>
#include <functional>

#include "imgui.h"
#include "utils/Layer.h"
#include "utils/Event.h"
#include "utils/vc_config.h"
#include "Types.h"
#include "Graph.h"

namespace ViscoCorrect
{

    class ProjectManager : public Layer
    {
    public:
        ProjectManager() = delete;
        ProjectManager(std::shared_ptr<std::vector<Project>> _projects, std::function<void(std::unique_ptr<utils::EventBase>)> _event_callback);
        ~ProjectManager() = default;

        virtual void OnUIRender() override;

    private:
        std::function<void(std::unique_ptr<utils::EventBase>)> m_event_callback;
        
        std::shared_ptr<std::vector<Project>> m_projects;
    };
}