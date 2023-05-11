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
            m_debug_curve.Render();

            ImGui::Begin("Debugging");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            for (auto &layer : mvec_tools)
            {
                layer->OnUIRender();
            }
            ImGui::End();
        }

        // DebugToolBase
        DebugToolBase::DebugToolBase(const std::string &_name) : m_name(_name)
        {
        }

        void DebugToolBase::OnUIRender()
        {
            if (ImGui::CollapsingHeader(m_name.c_str()))
            {
                Run();
                return;
            }
        }

        void DebugToolBase::AddCallback(std::unique_ptr<std::function<void()>> _callback)
        {
            mvec_callbacks.push_back(std::move(_callback));
        }


        void DebugToolBase::RunCallbacks()
        {
            for (const auto &_func : mvec_callbacks)
            {
                (*_func)();
            }
        }
    }
}