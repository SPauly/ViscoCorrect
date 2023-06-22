#ifndef VISCOCORRECT_SRC_GRAPH_H
#define VISCOCORRECT_SRC_GRAPH_H

#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <functional>

#include "imgui.h"

#include "Layer.h"
#include "Types.h"
#include "vc_config.h"

namespace ViscoCorrect
{
    class Graph : public Layer
    {
    public:
        Graph();
        virtual ~Graph();

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        virtual void OnUIRender() override;

        void SetAutofit(bool _fit = false){};
        void Resize(const float _scale = 1.0f){};

        void AddCallbackToPlot(std::shared_ptr<std::function<void()>>, int);
        void RemoveCallbackFromPlot(std::shared_ptr<std::function<void()>>, int);
    
    protected:
        void Autofit(){};

        void RenderFlowrate();
        void RenderFunctions();
        void RenderCorrection();

        void InstanceCoords(std::unordered_map<int, LineCoordinates> *_umap, std::map<int, int> *_raw, const double _m, const int *_startpos, bool _scale_on_x = true, bool _link_x = false);
        void InstanceCorrection();
    private:

    private:
        //interface use
        bool b_autofit = false;
        double m_scaling_factor = 1.0f;

        std::vector<std::shared_ptr<std::function<void()>>> mvec_callbacks_plot1;
        std::vector<std::shared_ptr<std::function<void()>>> mvec_callbacks_plot2;
        
        //internal use
        ImVec2 m_plot_size1;
        ImVec2 m_plot_size2;

        VCConfig::RawData m_raw;

        std::unordered_map<int, LineCoordinates> m_flow_points;
        std::unordered_map<int, LineCoordinates> m_totalh_points;
        std::unordered_map<int, LineCoordinates> m_visco_points;

        std::vector<double> m_x_cq, m_y_cq, m_x_cv, m_y_cv;
        std::vector<std::vector<double>> m_x_ch, m_y_ch;
    };
    
} // namespace ViscoCorrect

#endif //VISCOCORRECT_SRC_GRAPH_H