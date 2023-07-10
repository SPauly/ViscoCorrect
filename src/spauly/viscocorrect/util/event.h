#ifndef SPAULY_VISCOCORRECT_UTILS_EVENT_H
#define SPAULY_VISCOCORRECT_UTILS_EVENT_H

#include <deque>
#include <functional>
#include <memory>

#include "spauly/viscocorrect/util/properties.h"

namespace viscocorrect
{
    // defined in this file
    namespace util
    {
        class EventBase;
        template <typename T>
        class Event;
    }

    using EventCallbackType = std::function<void(std::unique_ptr<util::EventBase>)>;

    namespace util
    {
        enum EventType
        {
            kCalcReq,
            kManageProjectReq,
            kGetProjectReq,
            kProjectListReq,
            kGetGraphInstance
        };

        class EventBase
        {
        public:
            EventBase() = default;
            EventBase(util::EventType etype) : event_type_(etype) {}
            virtual ~EventBase() {}

            inline const util::EventType &get_event_type_() { return event_type_; }

            template <typename T>
            T *GetData()
            {
                return static_cast<T *>(GetDataImpl());
            }

        protected:
            virtual void *GetDataImpl() { return nullptr; }

        private:
            util::EventType event_type_;
        };

        template <typename T>
        class Event : public EventBase
        {
        public:
            Event(util::EventType etype, T *data, std::function<void(T)> *callback = nullptr)
                : EventBase(etype), data_(data), callback_(callback) {}
            ~Event() {}

        protected:
            virtual void *GetDataImpl() override { return data_; }

        private:
            T *data_;
            std::function<void(T)> *callback_;
        };

        inline void PushEvent(std::deque<std::unique_ptr<EventBase>> *que, std::unique_ptr<EventBase> event)
        {
            que->push_back(std::move(event));
        };
    }
} // namespace ViscoCorrect

#endif // SPAULY_VISCOCORRECT_UTILS_EVENT_H