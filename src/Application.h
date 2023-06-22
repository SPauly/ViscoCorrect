#ifndef VISCOCORRECT_SRC_APPLICATION_H
#define VISCOCORRECT_SRC_APPLICATION_H

#include <vector>
#include <deque>
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
#include "utils/Event.h"
#include "Graph.h"
#include "ProjectManager.h"
#include "Calculator.h"

#if defined(DEBUG_BUILD)
#include "utils/Debugging/DebugTools.h"
#endif

namespace ViscoCorrect
{
    class Application
    {
    public:
        Application();
        ~Application() = default;

        void Run();
        void PushEvent(std::unique_ptr<utils::EventBase> _event);

        template<typename T>
        void PushLayer();
        void PushLayer(const std::shared_ptr<Layer> &);

        inline std::shared_ptr<Graph> GetGraph() { return m_graph; }

        static Application *GetInstance() {return s_Instance; };

#if defined(DEBUG_BUILD)
        inline std::shared_ptr<Debug::DebugTools> GetDebugTools()
        {
            return m_debug_tools;
        }
#endif
        void Close();
    private:
        bool Init();
        void Shutdown();

        void SetStyle();

        void HandleEvents();

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
        std::shared_ptr<Graph> m_graph;
        std::shared_ptr<ProjectManager> m_project_man;

        std::shared_ptr<std::vector<Project>> m_projects;
        std::function<void(std::unique_ptr<utils::EventBase>)> m_event_callback;

        std::deque<std::unique_ptr<utils::EventBase>> m_event_que;
        Calculator m_calculator;

#if defined(DEBUG_BUILD)
        std::shared_ptr<Debug::DebugTools> m_debug_tools;
#endif
    
    private:
        static Application *s_Instance;
    };

    Application *CreateApplication();
}

#endif //VISCOCORRECT_SRC_APPLICATION_H