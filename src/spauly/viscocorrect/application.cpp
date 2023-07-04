#include "spauly/viscocorrect/application.h"

namespace viscocorrect
{
    Application::Application(ApplicationBase *application_implementaion) : frontend_impl{application_implementaion}
    {
        event_callback_ = std::make_shared<std::function<void(std::unique_ptr<util::EventBase>)>>(std::bind(&PushEvent, this, std::placeholders::_1));
        projects_ = std::make_shared<std::vector<Project>>(1);

        frontend_impl->set_event_callback(event_callback_);
        frontend_impl->Init();

        HandleEvents(); // Handle events registered during initialization so that the process is completed
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

    void Application::PushEvent(std::unique_ptr<util::EventBase> event)
    {
        util::PushEvent(&event_que_, std::move(event));
    }

    void Application::HandleEvents()
    {
        for (int i = 0; i < event_que_.size(); i++)
        {
            auto event = std::move(event_que_.front());
            switch (event->get_event_type_())
            {
            case util::kCalcReq:
                calculator_.Calculate(event->GetData<Project>());
                break;

            case util::kProjectListReq:
                *event->GetData<std::shared_ptr<std::vector<Project>>>() = projects_;
                break;

            default:
                break;
            }
            event_que_.pop_front();
        }
    }
} // namespace viscocorrect
