#include "Graph.h"
#include "Application.h"

namespace ViscoCorrect
{
    // Flowrate
    Flowrate::Flowrate(const ImVec2 &_win_size) : m_win_size(_win_size)
    {
        int it_total_dist = 0;
        for (const auto &pair : raw_distances)
        {
            it_total_dist += pair.second*m_scaling_factor;

            LinePoint temp;
            temp.relative_distance = pair.second;
            temp.total_distance = it_total_dist;
            temp.tag = pair.first;

            temp.x_coords[0] = it_total_dist;
            temp.x_coords[1] = it_total_dist;
            temp.y_coords[0] = 0;
            temp.y_coords[1] = m_win_size.y;

            rates.insert({pair.first, temp});
        }
    }

    Flowrate::~Flowrate()
    {
    }

    void Flowrate::RenderFlowrates()
    {
        for (const auto &pair : rates)
        {
            ImPlot::PlotLine("##", pair.second.x_coords, pair.second.y_coords, 2);
        }
    }

    void Flowrate::Resize(const ImVec2 &_new_size, const double _scal)
    {
        m_win_size = _new_size;
        m_scaling_factor = _scal;

        int it_total_distance = 0;
        for(auto &pair : raw_distances)
        {
            it_total_distance += pair.second * m_scaling_factor;

            auto it = rates.find(pair.first);
            it->second.total_distance = it_total_distance;
            it->second.x_coords[0] = it_total_distance;
            it->second.x_coords[1] = it_total_distance;
            it->second.y_coords[1] = m_win_size.y;
        }
    }

    Graph::Graph() : m_flowrate(m_win_size)
    {
    }

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

    void Graph::OnUpdate(float ts)
    {
    }

    void Graph::OnUIRender()
    {
        ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoCollapse);
    

        if (ImGui::GetIO().DisplaySize.x != m_main_win_size.x || ImGui::GetIO().DisplaySize.y != m_main_win_size.y)
        {
            m_main_win_size = ImGui::GetIO().DisplaySize;

            // Check the maximum size available
            int maxWidth = m_main_win_size.x;
            int maxHeight = m_main_win_size.y;

            int targetWidth = maxHeight * m_win_ratio;
            int targetHeight = maxWidth * m_win_invratio;

            if (targetWidth <= maxWidth)
            {
                m_win_size.x = targetWidth;
                m_win_size.y = maxHeight;
            }
            else
            {
                m_win_size.x = maxWidth;
                m_win_size.y = targetHeight;
            }

            //set the sizes
            m_plot_size1.x = m_win_size.x - padding;
            m_plot_size1.y = (m_win_size.y * m_plot1_yratio) - padding;

            m_plot_size2.x = m_win_size.x - padding;
            m_plot_size2.y = (m_win_size.y * m_plot2_yratio) - padding;

            m_scalling_factor = m_plot_size1.x / 434;

            m_flowrate.Resize(m_plot_size1, m_scalling_factor);
            //Resize other plot
        }

        if (ImPlot::BeginPlot("##plot1", m_plot_size1, ImPlotFlags_NoLegend))
        {
            // Set up Graph
            ImPlot::SetupAxis(ImAxis_X1, nullptr);
            ImPlot::SetupAxis(ImAxis_Y1, nullptr);
            ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, m_plot_size1.x);
            ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0, m_plot_size1.y);

            // Render the different functions
            m_flowrate.RenderFlowrates();

            ImPlot::EndPlot();
        }

        ImGui::End();
    }

#if defined(DEBUG_BUILD)
    void Graph::DebugGraph::Run()
    {
        RunCallbacks();
    }

    void Graph::debug_func()
    {
        ImPlot::ShowDemoWindow();
        ImGui::Text("Window Height: %.1f Window Width: %.1f", m_win_size.y, m_win_size.x);
        ImGui::InputInt("Padding", &padding);
    }
#endif
}