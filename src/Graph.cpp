#include "Graph.h"

#include "implot.h"

namespace ViscoCorrect
{
    Graph::Graph()
    {
        m_plot_size1 = m_raw.table_param_size;
        m_plot_size2 = m_raw.table_correct_size;
        // instanciate all the coordinates
        InstanceCoords(&m_flow_points, &m_raw.flowrates, 0, m_raw.startpos_f, true, true);
        InstanceCoords(&m_totalh_points, &m_raw.totalhead, m_raw.pitch_th, m_raw.startpos_th, false);
        InstanceCoords(&m_visco_points, &m_raw.viscosity, m_raw.pitch_v, m_raw.startpos_v);

        InstanceCorrection();
    }

    Graph::~Graph()
    {
    }

    void Graph::OnAttach()
    {
        ImPlot::CreateContext();
        Resize(1.0);
    }

    void Graph::OnDetach()
    {
        ImPlot::DestroyContext();
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
            RenderFlowrate();
            RenderCorrection();

            // Call the callbacks
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
            RenderFlowrate();
            RenderFunctions();

            // Call the callbacks
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

    void Graph::RenderFlowrate()
    {
        for (const auto &pair : m_flow_points)
        {
            ImPlot::PlotLine("##flowrates", pair.second.x_coords, pair.second.y_coords, 2);
        }
    }

    void Graph::RenderFunctions()
    {
        for (const auto &pair : m_totalh_points)
        {
            ImPlot::PlotLine("##totalh", pair.second.x_coords, pair.second.y_coords, 2);
        }
        for (const auto &pair : m_visco_points)
        {
            ImPlot::PlotLine("##viscosity", pair.second.x_coords, pair.second.y_coords, 2);
        }
    }

    void Graph::RenderCorrection()
    {
        ImPlot::PlotLine("##c_v", m_x_cv.data(), m_y_cv.data(), m_x_cv.size());
        ImPlot::PlotLine("##c_q", m_x_cq.data(), m_y_cq.data(), m_x_cq.size());
        for (int i = 0; i < m_x_ch.size(); i++)
        {
            ImPlot::PlotLine("##ch", m_x_ch.at(i).data(), m_y_ch.at(i).data(), m_x_ch.at(i).size());
        }
    }

    void Graph::InstanceCoords(std::unordered_map<int, LineCoordinates> *_umap, std::map<int, int> *_raw, const double _m, const int *_startpos, bool _scale_on_x, bool _link_x)
    {
        _umap->clear();
        bool b_set_start = false;
        if (!_startpos)
        {
            _startpos = new int[2]{0, 0};
            b_set_start = true;
        }

        double it_total_dist = (_scale_on_x) ? _startpos[0] : _startpos[1];

        if (!_link_x)
        {
            for (const auto &pair : *_raw)
            {
                it_total_dist += pair.second * m_scaling_factor;

                LinearFunction temp_func(_m, (_scale_on_x) ? (int)it_total_dist : _startpos[0], (_scale_on_x) ? _startpos[1] : (int)it_total_dist);

                _umap->insert(std::make_pair(pair.first, temp_func.GetRenderCoords(0, m_plot_size1.x)));
            }
        }
        else
        {
            for (const auto &pair : *_raw)
            {
                it_total_dist += pair.second * m_scaling_factor;

                LineCoordinates temp;

                temp.x_coords[0] = it_total_dist;
                temp.x_coords[1] = it_total_dist;
                temp.y_coords[0] = 0;
                temp.y_coords[1] = m_plot_size1.y;

                _umap->insert(std::make_pair(pair.first, temp));
            }
        }

        if (b_set_start)
        {
            try
            {
                delete[] _startpos;
            }
            catch (const std::exception &e)
            {
                return;
            }
        }
    }

    void Graph::InstanceCorrection()
    {
        Polynom m_cv{m_raw.cv};
        Polynom m_cq{m_raw.cq};
        std::vector<Polynom> m_ch;

        for (int i = m_raw.cutoff_cv[0]; i < m_raw.cutoff_cv[1]; i++)
        {
            m_x_cv.push_back(i);
            m_y_cv.push_back(m_cv.f((double)i));
        }

        for (int i = m_raw.cutoff_cq[0]; i < m_raw.cutoff_cq[1]; i++)
        {
            m_x_cq.push_back(i);
            m_y_cq.push_back(m_cq.f((double)i));
        }
    }
} // namespace ViscoCorrect
