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
        Flowrate();
        ~Flowrate();

        void render_flowrates();

    private:
        int x_dim = 434, y_dim = 656;


        std::map<int, int> raw_distances{
            {6,14},
            {7,9},
            {8,9},
            {10,9},
            {15,30} 
            //add the rest
        };

        std::unordered_map<int, LinePoint> rates;


    };
    class Graph : public Layer
    {
    public:
        virtual void OnAttach() override;
        virtual void OnDetach() override;

        virtual void OnUIRender() override;

    private:
    //properties
        float v[2][2] = {{50,0},{50,100}};
        int v_size = 2;
        double x_min=0, x_max=100, y_min=0, y_max=100;
        int sz[2] = {-1,-1};

        std::shared_ptr<Calculator> m_calculator;
        Flowrate m_flowrate;
    
    //debugging
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