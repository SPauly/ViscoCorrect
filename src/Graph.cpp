#include "Graph.h"

#include "implot.h"

namespace viscocorrect
{
    Graph::Graph()
    {
        plot_size_1_ = raw_data_.table_param_size;
        plot_size_2_ = raw_data_.table_correct_size;
        // instanciate all the coordinates
        InstanceCoords(&flowrate_coords_, &raw_data_.flowrates, 0, raw_data_.startpos_f, true, true);
        InstanceCoords(&totalhead_coords_, &raw_data_.totalhead, raw_data_.pitch_th, raw_data_.startpos_th, false);
        InstanceCoords(&viscosity_coords_, &raw_data_.viscosity, raw_data_.pitch_v, raw_data_.startpos_v);

        InstanceCorrection();
    }

    Graph::~Graph()
    {
    }

    void Graph::OnAttach()
    {
        ImPlot::CreateContext();
        set_scaling_factor(1.0);
    }

    void Graph::OnDetach()
    {
        ImPlot::DestroyContext();
    }

    void Graph::OnUIRender()
    {
        ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoCollapse);

        if (use_autofit_)
            Autofit();

        if (ImPlot::BeginPlot("##plot2", plot_size_2_, ImPlotFlags_NoLegend))
        {
            // Set up Graph
            ImPlot::SetupAxis(ImAxis_X1, nullptr);
            ImPlot::SetupAxis(ImAxis_Y1, nullptr);
            ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, plot_size_2_.x);
            ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0, plot_size_2_.y);

            // Render the different functions
            RenderFlowrate();
            RenderCorrection();

            // Call the callbacks
            for (const auto &func : callbacks_plot_2_)
            {
                (*func)();
            }

            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("##plot1", plot_size_1_, ImPlotFlags_NoLegend))
        {
            // Set up Graph
            ImPlot::SetupAxis(ImAxis_X1, nullptr);
            ImPlot::SetupAxis(ImAxis_Y1, nullptr);
            ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, plot_size_1_.x);
            ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0, plot_size_1_.y);

            // Render the different functions
            RenderFlowrate();
            RenderFunctions();

            // Call the callbacks
            for (const auto &func : callbacks_plot_1_)
            {
                (*func)();
            }

            ImPlot::EndPlot();
        }

        ImGui::End();
    }

    void Graph::AddCallbackToPlot(std::shared_ptr<std::function<void()>> func, int plot_num)
    {
        switch (plot_num)
        {
        case 0:
            callbacks_plot_1_.push_back(func);
            break;

        case 1:
            callbacks_plot_2_.push_back(func);
            break;

        default:
            break;
        }
    };

    void Graph::RemoveCallbackFromPlot(std::shared_ptr<std::function<void()>> func, int plot_num)
    {
        switch (plot_num)
        {
        case 0:
        {
            auto it = std::find(callbacks_plot_1_.begin(), callbacks_plot_1_.end(), func);
            try
            {
                callbacks_plot_1_.erase(it);
            }
            catch (std::runtime_error &)
            {
                break;
            }
            break;
        }
        case 1:
        {
            auto it = std::find(callbacks_plot_2_.begin(), callbacks_plot_2_.end(), func);
            try
            {
                callbacks_plot_2_.erase(it);
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
        for (const auto &pair : flowrate_coords_)
        {
            ImPlot::PlotLine("##flowrates", pair.second.x_coords, pair.second.y_coords, 2);
        }
    }

    void Graph::RenderFunctions()
    {
        for (const auto &pair : totalhead_coords_)
        {
            ImPlot::PlotLine("##totalh", pair.second.x_coords, pair.second.y_coords, 2);
        }
        for (const auto &pair : viscosity_coords_)
        {
            ImPlot::PlotLine("##viscosity", pair.second.x_coords, pair.second.y_coords, 2);
        }
    }

    void Graph::RenderCorrection()
    {
        ImPlot::PlotLine("##c_v", x_coords_v_.data(), y_coords_v_.data(), x_coords_v_.size());
        ImPlot::PlotLine("##c_q", x_coords_q_.data(), y_coords_q_.data(), x_coords_q_.size());
        for (int i = 0; i < x_coords_h_.size(); i++)
        {
            ImPlot::PlotLine("##ch", x_coords_h_.at(i).data(), y_coords_h_.at(i).data(), x_coords_h_.at(i).size());
        }
    }

    void Graph::InstanceCoords(std::unordered_map<int, LineCoordinates> *coords, std::map<int, int> *raw_points, const double rate, const int *startpos, bool scale_on_x, bool use_same_x)
    {
        coords->clear();
        bool set_start = false;
        
        if (!startpos)
        {
            startpos = new int[2]{0, 0};
            set_start = true;
        }

        double it_total_dist = (scale_on_x) ? startpos[0] : startpos[1];

        if (!use_same_x)
        {
            for (const auto &pair : *raw_points)
            {
                it_total_dist += pair.second * scaling_factor_;

                LinearFunction temp_func(rate, (scale_on_x) ? (int)it_total_dist : startpos[0], (scale_on_x) ? startpos[1] : (int)it_total_dist);

                coords->insert(std::make_pair(pair.first, temp_func.get_render_coords(0, plot_size_1_.x)));
            }
        }
        else
        {
            for (const auto &pair : *raw_points)
            {
                it_total_dist += pair.second * scaling_factor_;

                LineCoordinates temp;

                temp.x_coords[0] = it_total_dist;
                temp.x_coords[1] = it_total_dist;
                temp.y_coords[0] = 0;
                temp.y_coords[1] = plot_size_1_.y;

                coords->insert(std::make_pair(pair.first, temp));
            }
        }

        if (set_start)
        {
            try
            {
                delete[] startpos;
            }
            catch (const std::exception &e)
            {
                return;
            }
        }
    }

    void Graph::InstanceCorrection()
    {
        Polynom correct_v{raw_data_.cv};
        Polynom correct_q{raw_data_.cq};
        std::vector<Polynom> correct_h;

        for (int i = raw_data_.cutoff_cv[0]; i < raw_data_.cutoff_cv[1]; i++)
        {
            x_coords_v_.push_back(i);
            y_coords_v_.push_back(correct_v.f((double)i));
        }

        for (int i = raw_data_.cutoff_cq[0]; i < raw_data_.cutoff_cq[1]; i++)
        {
            x_coords_q_.push_back(i);
            y_coords_q_.push_back(correct_q.f((double)i));
        }
    }
} // namespace viscocorrect
