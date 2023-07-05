#ifndef SPAULY_VISCOCORRECT_APPLICATION_BASE_H
#define SPAULY_VISCOCORRECT_APPLICATION_BASE_H

#include <functional>
#include <memory>
#include <vector>

namespace viscocorrect
{
    class GraphImplBase; // defined in graph_base.h
    struct Project;      // defined in project.h
    namespace util
    {
        class EventBase; // defined in util/event.h
    }

    class ApplicationBase
    {
    public:
        ApplicationBase() {}
        virtual ~ApplicationBase(){};

        virtual bool Init(){};
        virtual void Shutdown(){};

        virtual bool Render(){};

        virtual std::shared_ptr<GraphImplBase> get_graph() { return nullptr; } // mainly used for debugging

        inline void set_event_callback(std::shared_ptr<std::function<void(std::unique_ptr<util::EventBase>)>> callback)
        {
            register_event_ = callback;
        }

        inline std::shared_ptr<std::function<void(std::unique_ptr<util::EventBase>)>> get_register_event()
        {
            return register_event_;
        }

    protected:
        inline bool set_should_close(bool close = true) { return should_close_ = close; }
        inline bool get_should_close() { return should_close_; }

    protected:
        std::shared_ptr<std::vector<Project>> projects_;
        std::shared_ptr<std::function<void(std::unique_ptr<util::EventBase>)>> register_event_;

    private:
        bool should_close_ = false;

        float time_step_ = 0.0f;
        float frame_time_ = 0.0f;
        float last_frame_time_ = 0.0f;
    };

} // namespace viscocorrect

#endif // SPAULY_VISCOCORRECT_APPLICATION_BASE_H