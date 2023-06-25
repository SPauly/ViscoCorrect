#include "Application.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

namespace viscocorrect
{
    Application *Application::s_instance_ = nullptr;

    Application *CreateApplication()
    {
        return new Application;
    }

    Application::Application()
    {
        s_instance_ = this;
        projects_ = std::make_shared<std::vector<Project>>(1);
        event_callback_ = std::bind(&PushEvent, this, std::placeholders::_1);

#if defined(DEBUG_BUILD)
        debug_tools_ = std::make_shared<Debug::DebugTools>();
#endif
    }

    bool Application::Init()
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
#elif defined(__APPLE__)
        // GL 3.2 + GLSL 150
        const char *glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
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

        // layer init graph
        graph_ = std::make_shared<Graph>();
        this->PushLayer(graph_);

        // layer init Projects
        project_manager_ = std::make_shared<ProjectManager>(projects_, event_callback_);
        this->PushLayer(project_manager_);

#if defined(DEBUG_BUILD)
        // layer init ExampleLayer
        this->PushLayer(debug_tools_);
#endif
        return true;
    }

    void Application::Shutdown()
    {
        // clear layerstack
        layer_stack_.clear();
        event_que_.clear();
        
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window_);
        glfwTerminate();
    }

    void Application::Run()
    {
        if (!Init())
            return;

        ImGuiIO &io = ImGui::GetIO();
        // Main loop
        while (!glfwWindowShouldClose(window_))
        {
            // Poll and handle events (inputs, window resize, etc.)
            // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
            // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
            // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
            // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
            glfwPollEvents();

            //Handle application events
            HandleEvents();

            for (auto &layer : layer_stack_)
            {
                layer->OnUpdate(time_step_);
            }

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

            for (auto &layer : layer_stack_)
            {
                layer->OnUIRender();
            }

            // Rendering
            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window_, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(clear_color_.x * clear_color_.w, clear_color_.y * clear_color_.w, clear_color_.z * clear_color_.w, clear_color_.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Update and Render additional Platform Windows
            // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
            //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow *backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }

            glfwSwapBuffers(window_);
        }

        // call shutdown
        Shutdown();
    }

    void Application::PushEvent(std::unique_ptr<utils::EventBase> _event)
    {
        utils::PushEvent(&event_que_, std::move(_event));
    }

    template <typename T>
    void Application::PushLayer()
    {
        layer_stack_.PushLayer<T>();
    }

    void Application::PushLayer(const std::shared_ptr<Layer> &layer)
    {
        layer_stack_.PushLayer(layer);
    }

    void Application::SetStyle()
    {
        ImGuiStyle &style = ImGui::GetStyle();
        style.WindowRounding = 3.0f;

        //Set the colors
        colors_ = ImGui::GetStyle().Colors;
        colors_[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors_[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors_[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
        colors_[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors_[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
        colors_[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
        colors_[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors_[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors_[ImGuiCol_FrameBgHovered] = ImVec4(0.98f, 0.69f, 0.26f, 0.95f);
        colors_[ImGuiCol_FrameBgActive] = ImVec4(0.99f, 0.59f, 0.16f, 0.67f);
        colors_[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
        colors_[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
        colors_[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
        colors_[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
        colors_[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
        colors_[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
        colors_[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.95f, 0.50f, 0.19f, 0.76f);
        colors_[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.97f, 0.39f, 0.00f, 1.00f);
        colors_[ImGuiCol_CheckMark] = ImVec4(0.97f, 0.59f, 0.14f, 1.00f);
        colors_[ImGuiCol_SliderGrab] = ImVec4(0.98f, 0.67f, 0.26f, 0.78f);
        colors_[ImGuiCol_SliderGrabActive] = ImVec4(0.46f, 0.54f, 0.80f, 0.60f);
        colors_[ImGuiCol_Button] = ImVec4(1.00f, 0.60f, 0.07f, 0.86f);
        colors_[ImGuiCol_ButtonHovered] = ImVec4(0.84f, 0.79f, 0.73f, 1.00f);
        colors_[ImGuiCol_ButtonActive] = ImVec4(0.73f, 0.61f, 0.44f, 1.00f);
        colors_[ImGuiCol_Header] = ImVec4(0.98f, 0.67f, 0.26f, 0.31f);
        colors_[ImGuiCol_HeaderHovered] = ImVec4(0.98f, 0.73f, 0.26f, 0.80f);
        colors_[ImGuiCol_HeaderActive] = ImVec4(0.99f, 0.57f, 0.08f, 0.95f);
        colors_[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
        colors_[ImGuiCol_SeparatorHovered] = ImVec4(0.95f, 0.71f, 0.16f, 0.78f);
        colors_[ImGuiCol_SeparatorActive] = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
        colors_[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
        colors_[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        colors_[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        colors_[ImGuiCol_Tab] = ImVec4(1.00f, 0.66f, 0.11f, 0.93f);
        colors_[ImGuiCol_TabHovered] = ImVec4(0.95f, 0.53f, 0.03f, 0.85f);
        colors_[ImGuiCol_TabActive] = ImVec4(1.00f, 0.78f, 0.54f, 1.00f);
        colors_[ImGuiCol_TabUnfocused] = ImVec4(0.92f, 0.93f, 0.94f, 0.99f);
        colors_[ImGuiCol_TabUnfocusedActive] = ImVec4(0.74f, 0.82f, 0.91f, 1.00f);
        colors_[ImGuiCol_DockingPreview] = ImVec4(0.98f, 0.57f, 0.26f, 0.22f);
        colors_[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors_[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        colors_[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors_[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors_[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
        colors_[ImGuiCol_TableHeaderBg] = ImVec4(0.78f, 0.87f, 0.98f, 1.00f);
        colors_[ImGuiCol_TableBorderStrong] = ImVec4(0.57f, 0.57f, 0.64f, 1.00f);
        colors_[ImGuiCol_TableBorderLight] = ImVec4(0.68f, 0.68f, 0.74f, 1.00f);
        colors_[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors_[ImGuiCol_TableRowBgAlt] = ImVec4(0.30f, 0.30f, 0.30f, 0.09f);
        colors_[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors_[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        colors_[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        colors_[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
        colors_[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
        colors_[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
    }

    void Application::HandleEvents()
    {
        for(int i = 0; i < event_que_.size(); i++)
        {
            auto event = std::move(event_que_.front());
            switch (event->get_event_type_())
            {
            case utils::kCalcReq:
                calculator_.Calculate(event->GetData<Project>());
                break;
            
            default:
                break;
            }
            event_que_.pop_front();
        }
    }
}