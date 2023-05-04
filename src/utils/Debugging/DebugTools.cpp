#include "DebugTools.h"

namespace ViscoCorrect
{
    namespace Debug
    {
        DebugTools::DebugTools()
        {
        }

        DebugTools::~DebugTools()
        {
            mvec_tools.clear();
        }

        void DebugTools::OnUIRender()
        {
            ImGui::ShowDemoWindow();

            ImGui::Begin("Debugging");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            for (auto &layer : mvec_tools)
            {
                layer->OnUIRender();
            }
            ImGui::End();
        }

        // DebugToolBase
        DebugToolBase::DebugToolBase(const char *_name) : name(_name)
        {
        }

        void DebugToolBase::OnUIRender()
        {
            if (ImGui::CollapsingHeader(name))
                return;
            ImGui::Text("Hello from DebugToolBase");
            Run();
        }

        void DebugToolBase::AddCallback(std::unique_ptr<std::function<void()>> _callback)
        {
            mvec_callbacks.push_back(std::move(_callback));
        }

        void RemoveCallback(std::function<void()> *callback)
        {
        }

    }
}