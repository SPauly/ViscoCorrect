#pragma once
#include <cmath>
#include "Layer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace ViscoCorrect
{
    class Viewport : public Layer
    {
    public:
        virtual void OnUIRender() override;

    private:
        
    };

}