#pragma once

#include <memory>
#include <map>
#include <unordered_map>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"

#include "Layer.h"
#include "Types.h"

#if defined(DEBUG_BUILD)
#include "utils/Debugging/DebugTools.h"
#endif
namespace ViscoCorrect
{
    class Flowrate
    {
    public:
        Flowrate(const ImVec2 &);
        ~Flowrate();

        void RenderFlowrate();

        void Resize(const double);

        const double ConvertFromInput(const int);

    private:
        struct FlowrateLinePoints
        {
            int x_coords[2];
            int y_coords[2];
            int relative_distance = 0;
            int total_distance = 0;
            int tag = 0;
        };

    private:
        ImVec2 m_plot_size;
        double m_scaling_factor = 1.0;

        std::unordered_map<int, FlowrateLinePoints> rates;

        std::map<int, int> raw_distances{
            {6, 0},
            {7, 14},
            {8, 9},
            {9, 9},
            {10, 9},
            {15, 30},
            {20, 21},
            {30, 30},
            {40, 21},
            {50, 17},
            {60, 13},
            {70, 12},
            {80, 9},
            {90, 9},
            {100, 9},
            {150, 30},
            {200, 21},
            {300, 30},
            {400, 21},
            {500, 17},
            {600, 14},
            {700, 11},
            {800, 10},
            {900, 8},
            {1000, 8},
            {1500, 30},
            {2000, 22}};
    };

    class LinearFunctionWrapper
    {
    public:
        LinearFunctionWrapper(const std::map<int, int> &_raw_distances, const double _m, const ImVec2 & _starting_pos, const ImVec2 &_range = (ImVec2){0,100}, const int _xaxis = 1, const int _yaxis = 0);
        ~LinearFunctionWrapper(){};

        void RenderFunctions();

        void Resize(const double, int, int);

        LinearFunction *CreateFromInput(const int);

    private:
        std::unordered_map<int, LinearFunction> functions;

        std::map<int, int> raw_distances;
        
        const double m;
        const ImVec2 starting_pos;
        const ImVec2 range;
        const int xaxis;
        const int yaxis;
    };

    class Graph : public Layer
    {
    public:
        Graph();
        virtual ~Graph(){};

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        virtual void OnUpdate(float) override;
        virtual void OnUIRender() override;

        Project *PopulateProject(Project*);

        void AddCallbackToPlot(std::shared_ptr<std::function<void()>>, int);
        void RemoveCallbackFromPlot(std::shared_ptr<std::function<void()>>, int);

    private:
        void Resize(double);
        void Autofit();

    private:
        // general properties
        ImVec2 m_main_win_size;
        const ImVec2 mor_win_size{434, 625};
        const ImVec2 mor_plot_size1{434, 304};
        const ImVec2 mor_plot_size2{434, 284}; 
        double m_scalling_factor = 1.0;
        int b_autofit = 1;
        
        // current sizes
        ImVec2 m_win_size{mor_win_size};
        ImVec2 m_plot_size1{mor_plot_size1};
        ImVec2 m_plot_size2{mor_plot_size2};

        //ratios
        const double m_win_ratio = 434.0 / 625.0;
        const double m_win_invratio = 625.0 / 434.0;
        const double m_plot1_yratio = 304.0 / 625.0;
        const double m_plot2_yratio = 284.0 / 625.0;

        //Raw data totalhead
        ImVec2 m_startpos_th{4,1}; //have to check if that is correct
        const double mth = (553.0-327.0)/430.0;
        std::map<int, int> raw_totalhead{
            {5, 0},
            {10, 15},
            {20, 12}
        };
        
        ImVec2 m_startpos_v{234, 304}; //have to check this
        const double mv = (31.0-304.0)/(377.0-234.0);
        std::map<int, int> raw_viscosity{
            {300, 0}
        };

        //Needed objects
        Flowrate m_flowrate;
        LinearFunctionWrapper m_totalhead;
        LinearFunctionWrapper m_viscosity;

        std::vector<std::shared_ptr<std::function<void()>>> mvec_callbacks_plot1;
        std::vector<std::shared_ptr<std::function<void()>>> mvec_callbacks_plot2;
    
    private:
        // debugging
#if defined(DEBUG_BUILD)
        class DebugGraph : public Debug::DebugToolBase
        {
        public:
            DebugGraph(const std::string &_name) : DebugToolBase(_name){};

            virtual void Run() override;
        };

        std::shared_ptr<DebugGraph> m_debug_graph;

        void debug_func();
#endif
    };
}