#include "frontend/imgui_glfw/application_impl_imgui_glfw.h"
#include "frontend/imgui_glfw/graph_impl_imgui_glfw.h"

namespace viscocorrect
{
    // implemented in this file
    int main();

    namespace frontend
    {
        namespace imgui_glfw
        {
            int ViscoCorrectMainImGuiGlfw()
            {
                viscocorrect::Application *app = viscocorrect::CreateApplication();
                app->Run();
                delete app;
            }
        } // namespace imgui_glfw

    } // namespace frontend

    int main()
    {
        return frontend::imgui_glfw::ViscoCorrectMainImGuiGlfw();
    }

} // namespace viscocorrect
