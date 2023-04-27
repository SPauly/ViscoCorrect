#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "utils/LayerStack.h"
#include "Graph.h"
#include "Project.h"

#if defined(DEBUG_BUILD)
#include "utils/Debug/DebugTools.h"
#endif

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

        void SetStyle();

    private:
        // Demo dependencies
        GLFWwindow *window;
        ImGuiContext *ctx;

        float m_TimeStep = 0.0f;
		float m_FrameTime = 0.0f;
		float m_LastFrameTime = 0.0f;   

        //Style
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        ImVec4 *colors = nullptr;
        
        LayerStack m_layer_stack;  
        std::shared_ptr<ViscoCorrect::Graph> m_graph;
        std::vector<std::shared_ptr<ViscoCorrect::Project>> mvec_projects;

    private:
        static Application *s_Instance;
    };

    Application *CreateApplication();
}