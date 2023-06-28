#include "spauly/viscocorrect/manage_project.h"

namespace viscocorrect
{
    ProjectManager::ProjectManager(std::shared_ptr<std::vector<Project>> projects, std::function<void(std::unique_ptr<util::EventBase>)> event_callback)
        : projects_(projects), event_callback_(event_callback)
    {
    }

    void ProjectManager::OnUIRender()
    {
        ImGui::Begin("Project");

        for (auto &proj : *projects_)
        {
            // start new heading et.
            ImGui::InputInt("Flowrate Q", &proj.parameters.flowrate_q);
            ImGui::InputInt("Total head H in m", &proj.parameters.total_head_m);
            ImGui::InputInt("Kinematic viscosity v in mm^2/s", &proj.parameters.viscosity_v);
            if (ImGui::Button("Calculate"))
            {
                event_callback_(std::make_unique<util::Event<Project>>(util::EventType::kCalcReq, &proj));
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