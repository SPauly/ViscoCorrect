#ifndef VISCOCORRECT_FRONTENDS_IMGUI_APPLICATION_IMPL_IMGUI_GLFW_H
#define VISCOCORRECT_FRONTENDS_IMGUI_APPLICATION_IMPL_IMGUI_GLFW_H

#include "application_base.h"

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

                virtual void Render() override;

            private:
            };

        } // namespace imgui_glfw

    } // namespace frontend

} // namespace viscocorrect

#endif // VISCOCORRECT_FRONTENDS_IMGUI_APPLICATION_IMPL_IMGUI_GLFW_H