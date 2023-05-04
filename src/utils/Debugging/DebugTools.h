#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "Layer.h"
#include "LayerStack.h"
#include "imgui.h"

namespace ViscoCorrect
{
    namespace Debug
    {
        class DebugTools : public Layer
        {
        public:
            DebugTools();
            virtual ~DebugTools();

            virtual void OnUIRender() override;

            template <typename T>
            void AddTool(const std::shared_ptr<T> &_t)
            {
                mvec_tools.PushLayer(_t);
            }

        private:
            LayerStack mvec_tools;
        };

        class DebugToolBase : public Layer
        {
        public:
            DebugToolBase() = default;
            DebugToolBase(const char *);
            virtual ~DebugToolBase() = default;

            virtual void OnUIRender() final;

            // overload this function
            virtual void Run(){};
            template <typename Func, typename Obj>
            std::unique_ptr<std::function<void()>> GetCallback(Func func, Obj *obj)
            {
                auto callback = std::make_unique<std::function<void()>>(std::bind(func, obj));
                return callback;
            }

            void AddCallback(std::unique_ptr<std::function<void()>>);
            void RemoveCallback(std::function<void()> &);

        protected:
            std::vector<std::unique_ptr<std::function<void()>>> mvec_callbacks;

        private:
            const char *name;
        };

    }
}