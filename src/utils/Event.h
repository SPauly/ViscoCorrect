#pragma once

#include <deque>
#include <memory>

#include "vc_config.h"

namespace ViscoCorrect
{
    namespace utils
    {
        class EventBase
        {
        public:
            EventBase() {}
            virtual ~EventBase() {}
        };

        template <typename T>
        class Event : public EventBase
        {
        public:
            Event(VCConfig::EventType _etype, T *_data, std::function<void(T)> *_callback = nullptr) {}
            ~Event() {}
        };

        inline void PushEvent(std::deque<std::unique_ptr<EventBase>> *_que, std::unique_ptr<EventBase> _event)
        {
            _que->push_back(std::move(_event));
        };
    }
} // namespace ViscoCorrect
