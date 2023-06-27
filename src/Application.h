#ifndef VISCOCORRECT_SRC_APPLICATION_H
#define VISCOCORRECT_SRC_APPLICATION_H

#include "application_base.h"

#include <vector>
#include <functional>
#include <deque>

#include "calculator.h"
#include "manage_project.h"
#include "project.h"
#include "util/event.h"

namespace viscocorrect
{
    class Application
    {
    public:
        Application(ApplicationBase *frontend_impl);
        ~Application();

        void Run();

    private:
        void Shutdown();

        void HandleEvents();

    private:
        ApplicationBase *frontend_impl;

        std::shared_ptr<ProjectManager> project_manager_;

        std::shared_ptr<std::vector<Project>> projects_;
        std::shared_ptr<std::function<void(std::unique_ptr<util::EventBase>)>> event_callback_;

        std::deque<std::unique_ptr<util::EventBase>> event_que_;
    };
} // namespace viscocorrect

#endif // VISCOCORRECT_SRC_APPLICATION_H