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

    ImPlotPoint TestGetter(int i, void* data)
    {
        float f = *(float*)data;
        return ImPlotPoint(i, f);
    }


    void Graph::OnUIRender()
    {
        static float v[2][2] = {{50,0},{50,100}};
        static double x_min=0, x_max=100, y_min=0, y_max=100;
        ImGui::Begin("Viewport");
        ImGui::InputFloat2("v[0]", v[0]);
        ImGui::InputFloat2("v[1]", v[1]);
        ImGui::InputDouble("x_min", &x_min);
        ImGui::InputDouble("x_max", &x_max);
        ImGui::InputDouble("y_min", &y_min);
        ImGui::InputDouble("y_max", &y_max);
        static int sz[2] = {-1,-1};
        ImGui::InputInt2("Size", sz);

        if(ImPlot::BeginPlot("##", ImVec2(sz[0],sz[1]),ImPlotFlags_NoLegend))
        {
            ImPlot::SetupAxis(ImAxis_X1, nullptr);
            ImPlot::SetupAxis(ImAxis_Y1, nullptr);
            ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, x_min,x_max);
            ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, y_min,y_max);
            ImPlot::PlotLine("##data",v[0],v[1],sizeof(v));
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
        ImGui::Text("Hello from Run");
        for(const auto &_func : mvec_callbacks)
        {
            (*_func)();
        }
    }

    void Graph::debug_func()
    {
        ImGui::Text("Hello this is the callback");
    }
#endif
}