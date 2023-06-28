#ifndef VISCOCORRECT_FRONTEND_IMGUI_APPLICATION_IMPL_IMGUI_GLFW_H
#define VISCOCORRECT_FRONTEND_IMGUI_APPLICATION_IMPL_IMGUI_GLFW_H

#include "application_base.h"

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

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
                ~ApplicationImplImguiGlfw();

                virtual void Init() override;
                virtual void Shutdown() override;

                virtual bool Render() override;

            private:
                void SetStyle();

            private:
                GLFWwindow *window_;
            };

        } // namespace imgui_glfw

    } // namespace frontend

} // namespace viscocorrect

#endif // VISCOCORRECT_FRONTEND_IMGUI_APPLICATION_IMPL_IMGUI_GLFW_H