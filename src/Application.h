#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "LayerStack.h"
#include "Viewport.h"

namespace ViscoCorrect
{
    class Application
    {
    public:
        Application();
        ~Application() = default;

        void Run();

        template<typename T>
        void PushLayer();
        void PushLayer(const std::shared_ptr<Layer> &);

        static Application& Get() {return *s_Instance; };  

        void Close();
    private:
        bool Init();
        void Shutdown();
    private:
        // Demo dependencies
        GLFWwindow *window;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        float m_TimeStep = 0.0f;
		float m_FrameTime = 0.0f;
		float m_LastFrameTime = 0.0f;   
        
        LayerStack m_layer_stack;
    private:
        static Application *s_Instance;
    };

    Application *CreateApplication();

    class ExampleLayer : public ViscoCorrect::Layer
    {
    public:
        virtual void OnUIRender() override; 

    private:
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);  
    };
}