#include "Graph.h"
#include "Application.h"

namespace ViscoCorrect
{
    // Flowrate
    Flowrate::Flowrate(const ImVec2 &_win_size) : m_plot_size(_win_size)
    {
        int it_total_dist = 0;
        for (const auto &pair : raw_distances)
        {
            it_total_dist += pair.second * m_scaling_factor;

            FlowrateLinePoints temp;
            temp.relative_distance = pair.second;
            temp.total_distance = it_total_dist;
            temp.tag = pair.first;

            temp.x_coords[0] = it_total_dist;
            temp.x_coords[1] = it_total_dist;
            temp.y_coords[0] = 0;
            temp.y_coords[1] = m_plot_size.y * m_scaling_factor;

            rates.insert(std::make_pair(pair.first, temp));
        }
    }

    Flowrate::~Flowrate()
    {
    }

    void Flowrate::RenderFlowrate()
    {
        for (const auto &pair : rates)
        {
            ImPlot::PlotLine("##flowrates", pair.second.x_coords, pair.second.y_coords, 2);
        }
    }

    void Flowrate::Resize(const double _scal)
    {
        m_scaling_factor = _scal;

        int it_total_distance = 0;
        for (auto &pair : raw_distances)
        {
            it_total_distance += pair.second * m_scaling_factor;

            auto it = rates.find(pair.first);
            it->second.total_distance = it_total_distance;
            it->second.x_coords[0] = it_total_distance;
            it->second.x_coords[1] = it_total_distance;
            it->second.y_coords[1] = m_plot_size.y * m_scaling_factor;
        }
    }

    const double Flowrate::ConvertFromInput(const int _input)
    {
        double absolute_position = 0;
        int prev_value = 0;
        bool bfound = false;

        for(const auto &pair : raw_distances)
        {
            int value = pair.first;
            
            if(value == _input)
            {
                absolute_position += pair.second;
                bfound = true;
                break;
            }
            else if(value > _input)
            {
                int range = value - prev_value;
                int relative_value = _input - prev_value;

                absolute_position += ((double)relative_value / (double)range) * (double)pair.second;
                bfound = true;
                break;
            }

            absolute_position += pair.second;
            prev_value = value;
        }

        if(bfound)
            return absolute_position;
        else
            return 0.0f; //throw exception
    }

    // LinearFunctionWrapper
    LinearFunctionWrapper::LinearFunctionWrapper(const std::map<int, int> &_raw_distances, const double _m, const ImVec2 &_starting_pos, const ImVec2 &_range, const int _xaxis, const int _yaxis) 
        : raw_distances(_raw_distances), m(_m), starting_pos(_starting_pos), range(_range), xaxis(_xaxis), yaxis(_yaxis)
    { 
        int totalx = starting_pos.x;
        int totaly = starting_pos.y;
        for (const auto &pair : raw_distances)
        {
            totalx += xaxis * pair.second;
            totaly += yaxis * pair.second;
            functions.emplace(pair.first, LinearFunction{m, totalx, totaly});
        }
    }

    void LinearFunctionWrapper::RenderFunctions()
    {
        for (auto &pair : functions)
        {
            ImPlot::PlotLine("##totalhead", pair.second.GetRenderCoords().x_coords, pair.second.GetRenderCoords().y_coords, 2);
        }
    }

    void LinearFunctionWrapper::Resize(const double _scale, int _xmin, int _xmax)
    {
        for (auto &pair : functions)
        {
            pair.second.ScaleYAxis(_scale);
            pair.second.SetRange(_xmin, _xmax);
        }
    }

    LinearFunction *LinearFunctionWrapper::CreateFromInput(const int _input)
    {
        double absolute_position = starting_pos.x;
        int prev_value = 0;
        bool bfound = false;

        for(const auto &pair : raw_distances)
        {
            int value = pair.first;
            
            if(value == _input)
            {
                absolute_position += pair.second;
                bfound = true;
                break;
            }
            else if(value > _input)
            {
                int range = value - prev_value;
                int relative_value = _input - prev_value;

                absolute_position += ((double)relative_value / (double)range) * (double)pair.second;
                bfound = true;
                break;
            }

            absolute_position += pair.second;
            prev_value = value;
        }

        if(bfound)
        {
            return new LinearFunction(m, (xaxis > 0 ) ? absolute_position : starting_pos.x, (yaxis > 0) ? absolute_position : starting_pos.y);
        }   
        else
            return nullptr; //throw exception
    }

    // Graph
    Graph::Graph() 
        : m_flowrate(m_win_size), m_totalhead(raw_totalhead, mth, m_startpos_th, (ImVec2){0, mor_plot_size1.x}, 0, 1), 
        m_viscosity(raw_viscosity, mv, m_startpos_v, (ImVec2){0,mor_plot_size1.x})
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

        if (b_autofit)
            Autofit();

        if (ImPlot::BeginPlot("##plot2", m_plot_size2, ImPlotFlags_NoLegend))
        {
            // Set up Graph
            ImPlot::SetupAxis(ImAxis_X1, nullptr);
            ImPlot::SetupAxis(ImAxis_Y1, nullptr);
            ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, m_plot_size2.x);
            ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0, m_plot_size2.y);

            // Render the different functions
            m_flowrate.RenderFlowrate();
            for (const auto &_func : mvec_callbacks_plot2)
            {
                (*_func)();
            }

            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("##plot1", m_plot_size1, ImPlotFlags_NoLegend))
        {
            // Set up Graph
            ImPlot::SetupAxis(ImAxis_X1, nullptr);
            ImPlot::SetupAxis(ImAxis_Y1, nullptr);
            ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, m_plot_size1.x);
            ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0, m_plot_size1.y);

            // Render the different functions
            m_flowrate.RenderFlowrate();
            m_totalhead.RenderFunctions();
            m_viscosity.RenderFunctions();
            for (const auto &_func : mvec_callbacks_plot1)
            {
                (*_func)();
            }

            ImPlot::EndPlot();
        }

        ImGui::End();
    }

    void Graph::AddCallbackToPlot(std::shared_ptr<std::function<void()>> _func, int _plot)
    {
        switch (_plot)
        {
        case 0:
            mvec_callbacks_plot1.push_back(_func);
            break;

        case 1:
            mvec_callbacks_plot2.push_back(_func);
            break;

        default:
            break;
        }
    };

    void Graph::RemoveCallbackFromPlot(std::shared_ptr<std::function<void()>> _func, int _plot)
    {
        switch (_plot)
        {
        case 0:
        {
            auto it = std::find(mvec_callbacks_plot1.begin(), mvec_callbacks_plot1.end(), _func);
            try
            {
                mvec_callbacks_plot1.erase(it);
            }
            catch (std::runtime_error &)
            {
                break;
            }
            break;
        }
        case 1:
        {
            auto it = std::find(mvec_callbacks_plot2.begin(), mvec_callbacks_plot2.end(), _func);
            try
            {
                mvec_callbacks_plot2.erase(it);
            }
            catch (std::runtime_error &)
            {
                break;
            }
            break;
        }
        default:
            break;
        }
    };

    void Graph::Autofit()
    {
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

            m_scalling_factor = m_win_size.x / 434;

            Resize(m_scalling_factor);
        }
    }

    void Graph::Resize(double _scale)
    {
        m_scalling_factor = _scale;

        m_plot_size1.x = mor_plot_size1.x * _scale;
        m_plot_size1.y = mor_plot_size1.y * _scale;

        m_plot_size2.x = mor_plot_size2.x * _scale;
        m_plot_size2.y = mor_plot_size2.y * _scale;

        m_flowrate.Resize(m_scalling_factor);
        m_totalhead.Resize(m_scalling_factor, 0, m_plot_size1.x);
        m_viscosity.Resize(m_scalling_factor, 0, m_plot_size1.x);
    }

    Project *Graph::PopulateProject(Project *_proj){
        _proj->func_totalhead = m_totalhead.CreateFromInput(_proj->parameters.total_head_m);
        _proj->func_visco = m_viscosity.CreateFromInput(_proj->parameters.viscosity_v);
        _proj->flow_pos = m_flowrate.ConvertFromInput(_proj->parameters.flowrate_q);

        return _proj;
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
        ImGui::RadioButton("Autofit", &b_autofit, 1);
        ImGui::RadioButton("No Autofit", &b_autofit, 0);
        if (!b_autofit)
        {
            ImGui::InputDouble("Scaling Factor", &m_scalling_factor);
            Resize(m_scalling_factor);
        }
    }
#endif
}