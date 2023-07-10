#include "spauly/viscocorrect/application.h"

namespace viscocorrect
{
    Application::Application(ApplicationBase *application_implementaion) : frontend_impl_{application_implementaion}
    {
        event_callback_ = std::make_shared<EventCallbackType>(std::bind(&PushEvent, this, std::placeholders::_1));
        projects_ = std::make_shared<std::vector<Project>>(1);

        frontend_impl_->set_event_callback(event_callback_);

        HandleEvents(); // Handle events registered during initialization so that the process is completed
    }

    Application::~Application()
    {
        // call projectmanager cleanups
    }

    void Application::Run()
    {
        if (!frontend_impl_->Init())
        {
            event_que_.clear();
            projects_->clear();
            return;
        }

        HandleEvents(); // Handle events registered during initialization so that the process is completed

        while (frontend_impl_->Render())
        {
            HandleEvents();
        }
        Shutdown();
    }

    void Application::Shutdown()
    {
        event_que_.clear();
        frontend_impl_->Shutdown();
        projects_->clear();
    }

    void Application::PushEvent(std::unique_ptr<util::EventBase> event)
    {
        util::PushEvent(&event_que_, std::move(event));
    }

    void Application::HandleEvents()
    {
        for (size_t i = 0; i < event_que_.size(); i++)
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

            case util::kGetGraphInstance:
                *event->GetData<std::shared_ptr<GraphImplBase>>() = frontend_impl_->get_graph();
                break;

            default:
                break;
            }
            event_que_.pop_front();
        }
    }
} // namespace viscocorrect
