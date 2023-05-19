#include "ProjectManager.h"

namespace ViscoCorrect
{
    ProjectManager::ProjectManager(std::shared_ptr<Graph> _graph) : m_graph(_graph)
    {
        m_projects.push_back(std::make_shared<Project>());
    }

    void ProjectManager::OnUIRender()
    {
        ImGui::Begin("Project");

        for (auto &proj : m_projects)
        {
            //start new heading et.
            ImGui::InputInt("Flowrate Q", &proj->parameters.flowrate_q);
            ImGui::InputInt("Total head H in m", &proj->parameters.total_head_m);
            ImGui::InputInt("Kinematic viscosity v in mm^2/s", &proj->parameters.viscosity_v);
            if(ImGui::Button("Calculate"))
            {
                m_graph->PopulateProject(proj.get());
            }
        }

        ImGui::End();
    }
}