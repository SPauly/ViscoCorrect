#pragma once

#include <memory>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"

#include "Layer.h"
#include "Calculator.h"
#include "Types.h"
namespace ViscoCorrect
{
    class Graph : public Layer
    {
    public:
        virtual void OnUIRender() override;

    private:
        std::shared_ptr<Calculator> m_calculator;
    };

}