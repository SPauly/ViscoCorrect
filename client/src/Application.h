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

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "Your_Project_protocol.grpc.pb.h"

#include "LayerStack.h"

namespace Your_Project
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

        std::string m_server_address = "localhost:50000";
        std::shared_ptr<grpc::Channel> msprt_channel;
        std::unique_ptr<YourProject::YourProjectServer::Stub> muprt_stub;
    };

    Application *CreateApplication();

    class ExampleLayer : public Your_Project::Layer
    {
    public:
        virtual void OnUIRender() override; 

    private:
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);  
    };
}