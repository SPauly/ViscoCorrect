#include "Project.h"

namespace ViscoCorrect
{
    void Project::OnUIRender()
    {
        ImGui::Begin("Project");

        ImGui::InputInt("Flowrate Q", &m_parameters.flowrate_q);
        ImGui::InputInt("Total head H in m", &m_parameters.total_head_m);
        ImGui::InputInt("Kinematic viscosity v in mm^2/s", &m_parameters.viscosity_v);
        ImGui::Button("Calculate");

        ImGui::End();
    }
}