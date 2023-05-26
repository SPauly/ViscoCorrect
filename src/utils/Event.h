#pragma once

#include <deque>
#include <memory>
#include <any>

#include "vc_config.h"

namespace ViscoCorrect
{
    namespace utils
    {
        class EventBase
        {
        public:
            EventBase() = default;
            EventBase(VCConfig::EventType _etype) : etype(_etype) {}
            virtual ~EventBase() {}

            inline const VCConfig::EventType &GetType() { return etype; }

            template<typename T>
            T *GetData() { 
                return static_cast<T*>(GetDataImpl());
            } 

        protected:  
            virtual void *GetDataImpl() { return nullptr; }
            
        private:
            VCConfig::EventType etype;
        };

        template <typename T>
        class Event : public EventBase
        {
        public:
            Event(VCConfig::EventType _etype, T *_data, std::function<void(T)> *_callback = nullptr) 
                : EventBase(_etype), data(_data), callback(_callback) {}
            ~Event() {}

        protected:
            virtual void *GetDataImpl() override { return data;}

        private:
            T *data;
            std::function<void(T)> *callback;
        };

        inline void PushEvent(std::deque<std::unique_ptr<EventBase>> *_que, std::unique_ptr<EventBase> _event)
        {
            _que->push_back(std::move(_event));
        };
    }
} // namespace ViscoCorrect
