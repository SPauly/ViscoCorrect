#pragma once

#include <string>
#include <memory>
#include <vector>

#include "imgui.h"
#include "Layer.h"
#include "Types.h"
#include "Graph.h"

namespace ViscoCorrect
{

    class ProjectManager : public Layer
    {
    public:
        ProjectManager() = delete;
        ProjectManager(std::shared_ptr<Graph> _graph);
        ~ProjectManager() = default;

        virtual void OnUIRender() override;

    private:
        std::shared_ptr<Graph> m_graph;
        
        std::vector<std::shared_ptr<Project>> m_projects;
    };
}