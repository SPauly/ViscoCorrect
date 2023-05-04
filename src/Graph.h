#pragma once

#include <memory>

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
    class Graph : public Layer
    {
    public:
        virtual void OnAttach() override;
        virtual void OnDetach() override;

        virtual void OnUIRender() override;

    private:
        std::shared_ptr<Calculator> m_calculator;

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