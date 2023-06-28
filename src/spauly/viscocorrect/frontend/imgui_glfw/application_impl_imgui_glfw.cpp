#include "frontend/imgui_glfw/application_impl_imgui_glfw.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace viscocorrect
{
    namespace frontend
    {
        namespace imgui_glfw
        {
            ApplicationImplImguiGlfw::ApplicationImplImguiGlfw()
            {
            }
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

            static void glfw_error_callback(int error, const char *description)
            {
                fprintf(stderr, "Glfw Error %d: %s\n", error, description);
            }

            bool ApplicationImplImguiGlfw::Init()
            {
                // Begin: ImGui Window Init

                // Setup window
                glfwSetErrorCallback(glfw_error_callback);
                if (!glfwInit())
                    return false;

                    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
                // GL ES 2.0 + GLSL 100
                const char *glsl_version = "#version 100";
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
                glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#else
                // GL 3.0 + GLSL 130
                const char *glsl_version = "#version 130";
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
                // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
                // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

                // Create window with graphics context
                window_ = glfwCreateWindow(1280, 720, "ViscoCorrect", NULL, NULL);
                if (window_ == NULL)
                    return false;
                glfwMakeContextCurrent(window_);
                glfwSwapInterval(1); // Enable vsync

                // Setup Dear ImGui context
                IMGUI_CHECKVERSION();
                ImGui::CreateContext();
                ImGuiIO &io = ImGui::GetIO();
                (void)io;
                io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
                // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
                io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
                io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
                // io.ConfigViewportsNoAutoMerge = true;
                // io.ConfigViewportsNoTaskBarIcon = true;

                // Set the style
                SetStyle();

                // Setup Platform/Renderer backends
                ImGui_ImplGlfw_InitForOpenGL(window_, true);
                ImGui_ImplOpenGL3_Init(glsl_version);

                // End ImGui Window Init

                return true;
            }

            void ApplicationImplImguiGlfw::Shutdown()
            {
                // Cleanup
                ImGui_ImplOpenGL3_Shutdown();
                ImGui_ImplGlfw_Shutdown();
                ImGui::DestroyContext();

                glfwDestroyWindow(window_);
                glfwTerminate();
            }

            bool ApplicationImplImguiGlfw::Render()
            {
            }
        } // namespace imgui_glfw

    } // namespace frontend

} // namespace viscocorrect
