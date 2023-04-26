#pragma once
#include <cmath>
#include "Layer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"

namespace ViscoCorrect
{
    class Graph : public Layer
    {
    public:
        virtual void OnUIRender() override;

    private:
        
    };

}