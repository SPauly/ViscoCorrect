#ifndef SPAULY_VISCOCORRECT_FRONTEND_IMGUI_APPLICATION_IMPL_IMGUI_GLFW_H
#define SPAULY_VISCOCORRECT_FRONTEND_IMGUI_APPLICATION_IMPL_IMGUI_GLFW_H

#include "spauly/viscocorrect/application_base.h"

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include "imgui.h"

#include "spauly/viscocorrect/graph.h"
#include "spauly/viscocorrect/project.h"
#include "spauly/viscocorrect/manage_project.h"
#include "spauly/viscocorrect/frontend/util_frontend/layerstack.h"

namespace viscocorrect
{
    namespace frontend
    {
        namespace imgui_glfw
        {
            class ApplicationImplImguiGlfw : public viscocorrect::ApplicationBase
            {
            public:
                ApplicationImplImguiGlfw();
                ~ApplicationImplImguiGlfw(){};

                virtual bool Init() override;
                virtual void Shutdown() override;

                virtual bool Render() override;

            private:
                void SetStyle();

            private:
                GLFWwindow *window_;
                ImGuiIO *io_;

                // Style
                ImVec4 clear_color_ = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
                ImVec4 *colors_ = nullptr;

                // utils
                util_frontend::LayerStack layer_stack_;

                // for now
                std::shared_ptr<Graph> graph_;
                std::shared_ptr<ProjectManager> project_manager_;

                std::shared_ptr<std::vector<Project>> projects_;
            };

        } // namespace imgui_glfw

    } // namespace frontend

} // namespace viscocorrect

#endif // SPAULY_VISCOCORRECT_FRONTEND_IMGUI_APPLICATION_IMPL_IMGUI_GLFW_H