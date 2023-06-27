#include "graph.h"

#include "implot.h"

#include "util/properties.h"

namespace viscocorrect
{
    Graph::Graph()
    {
        plot_size_1_ = internal::kProperties.kTableParameterSize;
        plot_size_2_ = internal::kProperties.kTableCorrectionSize;
        // instanciate all the coordinates
        CreateLineCoords(&flowrate_coords_, &internal::kProperties.kFlowrateScale, 0, internal::kProperties.kStartFlowrate, true, true);
        CreateLineCoords(&totalhead_coords_, &internal::kProperties.kTotalHeadScale, internal::kProperties.kPitchTotalH, internal::kProperties.kStartTotalH, false);
        CreateLineCoords(&viscosity_coords_, &internal::kProperties.kViscoScale, internal::kProperties.kPitchVisco, internal::kProperties.kStartVisco);

        CreateCorrectionPoints();
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
        for (auto &pair : flowrate_coords_)
        {
            ImPlot::PlotLine("##flowrates", pair.second.get_array_notation().x_coords, pair.second.get_array_notation().y_coords, 2);
        }
    }

    void Graph::RenderFunctions()
    {
        for (auto &pair : totalhead_coords_)
        {
            ImPlot::PlotLine("##totalh", pair.second.get_array_notation().x_coords, pair.second.get_array_notation().y_coords, 2);
        }
        for (auto &pair : viscosity_coords_)
        {
            ImPlot::PlotLine("##viscosity", pair.second.get_array_notation().x_coords, pair.second.get_array_notation().y_coords, 2);
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

    void Graph::CreateLineCoords(std::unordered_map<int, util::LineCoordinates<int>> *coords, const std::map<int, int> *raw_points, const double rate, const int *startpos, bool scale_on_x, bool use_same_x)
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

                util::LinearFunction temp_func(rate, (scale_on_x) ? (int)it_total_dist : startpos[0], (scale_on_x) ? startpos[1] : (int)it_total_dist);

                coords->insert(std::make_pair(pair.first, temp_func.CreateLineCoordinates(0, static_cast<int>(plot_size_1_.x))));
            }
        }
        else
        {
            for (const auto &pair : *raw_points)
            {
                it_total_dist += pair.second * scaling_factor_;

                util::LineCoordinates<int> temp({0, 0}, {1, 1});

                temp.get_array_notation().x_coords[0] = it_total_dist;
                temp.get_array_notation().x_coords[1] = it_total_dist;
                temp.get_array_notation().y_coords[0] = 0;
                temp.get_array_notation().y_coords[1] = static_cast<int>(plot_size_1_.y);

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

    void Graph::CreateCorrectionPoints()
    {
        util::PolynomialFunction correct_v{internal::kProperties.kCoefficientsV};
        util::PolynomialFunction correct_q{internal::kProperties.kCoefficientsQ};
        std::vector<util::PolynomialFunction> correct_h;

        for (int i = internal::kProperties.kCutoffV[0]; i < internal::kProperties.kCutoffV[1]; i++)
        {
            x_coords_v_.push_back(i);
            y_coords_v_.push_back(correct_v.f((double)i));
        }

        for (int i = internal::kProperties.kCutoffQ[0]; i < internal::kProperties.kCutoffQ[1]; i++)
        {
            x_coords_q_.push_back(i);
            y_coords_q_.push_back(correct_q.f((double)i));
        }
    }
} // namespace viscocorrect
