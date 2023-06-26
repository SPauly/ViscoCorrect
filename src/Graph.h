#ifndef VISCOCORRECT_SRC_GRAPH_H
#define VISCOCORRECT_SRC_GRAPH_H

#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <functional>

#include "imgui.h"

#include "layer.h"
#include "types.h"
#include "properties.h"

namespace viscocorrect
{
    class Graph : public Layer
    {
    public:
        Graph();
        virtual ~Graph();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUIRender() override;

        void AddCallbackToPlot(std::shared_ptr<std::function<void()>>, int);
        void RemoveCallbackFromPlot(std::shared_ptr<std::function<void()>>, int);

        inline bool set_use_autofit(bool fit = false) { return use_autofit_ = fit; }
        inline const float set_scaling_factor(const float scale = 1.0f) { return scaling_factor_ = scale; }

    protected:
        void Autofit(){};

        void RenderFlowrate();
        void RenderFunctions();
        void RenderCorrection();

        void CreateLineCoords(std::unordered_map<int, LineCoordinates> *coords, std::map<int, int> *raw_points, const double rate, const int *startpos, bool scale_on_x = true, bool use_same_x = false);
        void CreateCorrectionPoints();

    private:
        // interface use
        bool use_autofit_ = false;
        double scaling_factor_ = 1.0f;

        std::vector<std::shared_ptr<std::function<void()>>> callbacks_plot_1_;
        std::vector<std::shared_ptr<std::function<void()>>> callbacks_plot_2_;

        // internal use
        ImVec2 plot_size_1_;
        ImVec2 plot_size_2_;

        util::RawData raw_data_;

        std::unordered_map<int, LineCoordinates> flowrate_coords_;
        std::unordered_map<int, LineCoordinates> totalhead_coords_;
        std::unordered_map<int, LineCoordinates> viscosity_coords_;

        std::vector<double> x_coords_q_, y_coords_q_, x_coords_v_, y_coords_v_;
        std::vector<std::vector<double>> x_coords_h_, y_coords_h_;
    };

} // namespace viscocorrect

#endif // VISCOCORRECT_SRC_GRAPH_H