#include "spauly/viscocorrect/application.h"

namespace viscocorrect
{
    Application::Application(ApplicationBase *application_implementaion) : frontend_impl{application_implementaion}
    {
        event_callback_ = std::make_shared(std::bind(&PushEvent, this, std::placeholders::_1));
        projects_ = std::make_shared<std::vector<Project>>(1);

        frontend_impl->set_event_callback(event_callback_);
        frontend_impl->Init();
    }

    Application::~Application()
    {
        // call projectmanager cleanups
    }

    void Application::Run()
    {
        while (frontend_impl->Render())
        {
            HandleEvents();
        }
        Shutdown();
    }

    void Application::Shutdown()
    {
        event_que_.clear();
        frontend_impl->Shutdown();
        projects_->clear();
    }
} // namespace viscocorrect
