#include "Graph.h"
#include "Application.h"

namespace ViscoCorrect
{
    void Graph::OnAttach()
    {
        ImPlot::CreateContext();
#if defined(DEBUG_BUILD)
        m_debug_graph = std::make_shared<DebugGraph>("Graph");
        Application::GetInstance()->GetDebugTools()->AddTool(m_debug_graph);
        m_debug_graph->AddCallback(m_debug_graph->GetCallback(&debug_func, this));
#endif
    }

    void Graph::OnDetach()
    {
        ImPlot::DestroyContext();
    }

    void Graph::OnUIRender()
    {
        ImGui::Begin("Viewport");

        if(ImPlot::BeginPlot("##", ImVec2(sz[0],sz[1]),ImPlotFlags_NoLegend))
        {
            ImPlot::SetupAxis(ImAxis_X1, nullptr);
            ImPlot::SetupAxis(ImAxis_Y1, nullptr);
            ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, x_min,x_max);
            ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, y_min,y_max);
            ImPlot::PlotLine("##data",v[0],v[1],v_size);
            m_flowrate.render_flowrates();
            ImPlot::EndPlot();
        }
        ImGui::End();

#if defined(DEBUG_BUILD)
        ImPlot::ShowDemoWindow();
#endif
    }

#if defined(DEBUG_BUILD)
    void Graph::DebugGraph::Run()
    {
        RunCallbacks();
    }

    void Graph::debug_func()
    {
        ImGui::InputFloat2("v[0]", v[0]);
        ImGui::InputFloat2("v[1]", v[1]);
        ImGui::InputInt("v_count", &v_size);
        ImGui::InputDouble("x_min", &x_min);
        ImGui::InputDouble("x_max", &x_max);
        ImGui::InputDouble("y_min", &y_min);
        ImGui::InputDouble("y_max", &y_max);
        ImGui::InputInt2("Size", sz);
    }
#endif

    //Flowrate
    Flowrate::Flowrate()
    {
        int it_total_dist = 0;
        for(const auto &pair : raw_distances)
        {
            it_total_dist += pair.second;

            LinePoint temp;
            temp.relative_distance = pair.second;
            temp.total_distance = it_total_dist;
            temp.tag = pair.first;

            temp.x_coords[0] = it_total_dist;
            temp.x_coords[1] = it_total_dist;
            temp.y_coords[0] = 0;
            temp.y_coords[1] = y_dim;

            rates.insert({pair.first, temp});
        }
    }

    Flowrate::~Flowrate()
    {
    }

    void Flowrate::render_flowrates()
    {
        for(const auto &pair : rates)
        {
            ImPlot::PlotLine("##", pair.second.x_coords, pair.second.y_coords,2);
        }
    }

}