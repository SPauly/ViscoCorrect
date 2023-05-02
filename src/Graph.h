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
            virtual void Run() override;
        };

        DebugGraph m_debug_graph{"Graph"};

        void debug_func();
        std::function<void()> debug_func_callback = std::bind(&debug_func, this);
#endif
    };

}