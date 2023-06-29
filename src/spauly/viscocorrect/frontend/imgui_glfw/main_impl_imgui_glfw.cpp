#include "spauly/viscocorrect/frontend/imgui_glfw/application_impl_imgui_glfw.h"
#include "spauly/viscocorrect/frontend/imgui_glfw/graph_impl_imgui_glfw.h"
#include "spauly/viscocorrect/application.h"

namespace viscocorrect
{
    // implemented in this file
    int main();

    namespace frontend
    {
        namespace imgui_glfw
        {
            void ViscoCorrectMainImGuiGlfw()
            {
                viscocorrect::frontend::imgui_glfw::ApplicationImplImguiGlfw frontend_impl;
                viscocorrect::Application *app = viscocorrect::CreateApplication(&frontend_impl);
                app->Run();
                delete app;
            }
        } // namespace imgui_glfw

    } // namespace frontend
} // namespace viscocorrect

int main()
{
    viscocorrect::frontend::imgui_glfw::ViscoCorrectMainImGuiGlfw();
    return 0;
}