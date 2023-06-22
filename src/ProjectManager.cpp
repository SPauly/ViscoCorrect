#include "ProjectManager.h"

namespace ViscoCorrect
{
    ProjectManager::ProjectManager(std::shared_ptr<std::vector<Project>> _projects, std::function<void(std::unique_ptr<utils::EventBase>)> _event_callback) 
        : m_projects(_projects), m_event_callback(_event_callback)
    {
    }

    void ProjectManager::OnUIRender()
    {
        ImGui::Begin("Project");

        for (auto &proj : *m_projects)
        {
            //start new heading et.
            ImGui::InputInt("Flowrate Q", &proj.parameters.flowrate_q);
            ImGui::InputInt("Total head H in m", &proj.parameters.total_head_m);
            ImGui::InputInt("Kinematic viscosity v in mm^2/s", &proj.parameters.viscosity_v);
            if(ImGui::Button("Calculate"))
            {
                m_event_callback(std::make_unique<utils::Event<Project>>(VCConfig::EventType::CALC_REQ, &proj));
            }
            ImGui::Text("Correction Q: %.2f", proj.correction.c_q);
            ImGui::Text("Correction V: %.2f", proj.correction.c_v);
            ImGui::Text("Correction C_H 0.6: %.2f", proj.correction.c_h[0]);
            ImGui::Text("Correction C_H 0.8: %.2f", proj.correction.c_h[1]);
            ImGui::Text("Correction C_H 1.0: %.2f", proj.correction.c_h[2]);
            ImGui::Text("Correction C_H 1.2: %.2f", proj.correction.c_h[3]);
        }

        ImGui::End();
    }
}