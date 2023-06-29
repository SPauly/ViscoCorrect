#ifndef SPAULY_VISCOCORRECT_SRC_APPLICATION_BASE_H
#define SPAULY_VISCOCORRECT_SRC_APPLICATION_BASE_H

#include <memory>
#include <functional>

namespace viscocorrect
{
    class GraphBase; // defined in graph_base.h
    namespace util
    {
        class EventBase; // defined in util/event.h
    }

    class ApplicationBase
    {
    public:
        ApplicationBase(){};
        virtual ~ApplicationBase(){};

        virtual bool Init(){};
        virtual void Shutdown(){};

        virtual bool Render(){};

        inline std::shared_ptr<GraphBase> get_graph() { return graph_base_; }

        inline void set_event_callback(std::shared_ptr<std::function<void(std::unique_ptr<util::EventBase>)>> callback)
        {
            event_callback_ = callback;
        }

    protected:
        inline std::shared_ptr<std::function<void(std::unique_ptr<util::EventBase>)>> get_event_callback()
        {
            return event_callback_;
        }

        inline bool set_should_close(bool close = true) { return should_close_ = close; }
        inline bool get_should_close() { return should_close_; }

    private:
        bool should_close_ = false;

        float time_step_ = 0.0f;
        float frame_time_ = 0.0f;
        float last_frame_time_ = 0.0f;

        std::shared_ptr<std::function<void(std::unique_ptr<util::EventBase>)>> event_callback_;

        std::shared_ptr<GraphBase> graph_base_;
    };

} // namespace viscocorrect

#endif // SPAULY_VISCOCORRECT_SRC_APPLICATION_BASE_H