#ifndef SPAULY_VISCOCORRECT_SRC_UTILS_EVENT_H
#define SPAULY_VISCOCORRECT_SRC_UTILS_EVENT_H

#include <deque>
#include <memory>

#include "spauly/viscocorrect/util/properties.h"

namespace viscocorrect
{
    namespace util
    {
        enum EventType
        {
            kCalcReq,
            kRendReq
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

#endif // SPAULY_VISCOCORRECT_SRC_UTILS_EVENT_H