#ifndef VISCOCORRECT_SRC_APPLICATION_H
#define VISCOCORRECT_SRC_APPLICATION_H

#include <stdio.h>

#include <vector>
#include <deque>
#include <memory>
#include <functional>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "graph.h"
#include "manage_project.h"
#include "calculator.h"
#include "utils/layerstack.h"
#include "utils/event.h"
#if defined(DEBUG_BUILD)
#include "utils/Debugging/DebugTools.h"
#endif

namespace viscocorrect
{
    class Application
    {
    public:
        Application();
        ~Application() = default;

        void Run();
        void PushEvent(std::unique_ptr<util::EventBase> _event);

        template <typename T>
        void PushLayer();
        void PushLayer(const std::shared_ptr<Layer> &);

        inline std::shared_ptr<Graph> get_graph() { return graph_; }
        static Application *get_instance() { return s_instance_; };

#if defined(DEBUG_BUILD)
        inline std::shared_ptr<Debug::DebugTools> GetDebugTools()
        {
            return debug_tools_;
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
        GLFWwindow *window_;

        float time_step_ = 0.0f;
        float frame_time_ = 0.0f;
        float last_frame_time_ = 0.0f;

        // Style
        ImVec4 clear_color_ = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        ImVec4 *colors_ = nullptr;

        LayerStack layer_stack_;
        std::shared_ptr<Graph> graph_;
        std::shared_ptr<ProjectManager> project_manager_;

        std::shared_ptr<std::vector<Project>> projects_;
        std::function<void(std::unique_ptr<util::EventBase>)> event_callback_;

        std::deque<std::unique_ptr<util::EventBase>> event_que_;
        Calculator calculator_;

#if defined(DEBUG_BUILD)
        std::shared_ptr<Debug::DebugTools> debug_tools_;
#endif

    private:
        static Application *s_instance_;
    };

    Application *CreateApplication();
}

#endif // VISCOCORRECT_SRC_APPLICATION_H