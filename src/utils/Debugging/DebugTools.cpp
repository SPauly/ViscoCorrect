#include "DebugTools.h"

namespace ViscoCorrect
{
    namespace Debug
    {

        DebugTools::DebugTools()
        {
            s_DebugInstance = this;
        }

        DebugTools::~DebugTools()
        { 
            s_DebugInstance = nullptr;
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

        template <typename T>
        void DebugTools::AddTool(const T *_t)
        {
            static_assert(std::is_base_of<ViscoCorrect::Debug::DebugToolBase, T>::value, "Pushed type is not subclass of DebugToolBase!");
            mvec_tools.PushLayer(_t);
        }

        // DebugToolBase
        DebugToolBase::DebugToolBase(const char *_name) : name(_name)
        {
        }

        void DebugToolBase::OnUIRender()
        {
            if (ImGui::CollapsingHeader(name))
                return;

            Run();
        }

        void DebugToolBase::AddCallback(std::function<void()> *_callback)
        {
            mvec_callbacks.push_back(*_callback);
        }

        void DebugToolBase::RemoveCallback(std::function<void()> *_rm_callback)
        {
        }
    }
}