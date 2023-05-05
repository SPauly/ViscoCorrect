#pragma once

#include <memory>
#include <map>
#include <unordered_map>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"

#include "Layer.h"
#include "Calculator.h"
#include "Types.h"

#if defined(DEBUG_BUILD)
#include "utils/Debugging/DebugTools.h"
#endif
namespace ViscoCorrect
{
    class Flowrate
    {
    public:
        Flowrate(const ImVec2&);
        ~Flowrate();

        void RenderFlowrates();

        void Resize(const ImVec2&, const double);

    private:
        ImVec2 m_win_size;
        double m_scaling_factor = 1.0;

        std::unordered_map<int, LinePoint> rates;

        std::map<int, int> raw_distances{
            {6, 14},
            {7, 9},
            {8, 9},
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

    class Graph : public Layer
    {
    public:
        Graph();
        virtual ~Graph(){};

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        virtual void OnUpdate(float) override;
        virtual void OnUIRender() override;

    private:
        // properties
        ImVec2 m_main_win_size;
        ImVec2 m_win_size{434,625};
        ImVec2 m_plot_size1{434,304};
        ImVec2 m_plot_size2{434, 284};

        const double m_win_ratio = 434.0/625.0;
        const double m_win_invratio = 625.0/434.0;
        const double m_plot1_yratio = 304.0/625;
        const double m_plot2_yratio = 284.0/625;
        double m_scalling_factor = 1.0;
        int padding = 10;
        

        std::shared_ptr<Calculator> m_calculator;
        Flowrate m_flowrate;

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