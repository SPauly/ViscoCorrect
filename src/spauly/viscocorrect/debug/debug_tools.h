#ifndef SPAULY_VISCOCORRECT_DEBUG_DEBUG_TOOLS_H
#define SPAULY_VISCOCORRECT_DEBUG_DEBUG_TOOLS_H

#include <vector>
#include <memory>
#include <functional>
#include <string>

#include "imgui.h"

#include "spauly/viscocorrect/util/event.h"
#include "spauly/viscocorrect/debug/curve_fitting.h"
#include "spauly/viscocorrect/frontend/util_frontend/layer.h"
#include "spauly/viscocorrect/frontend/util_frontend/layerstack.h"

namespace viscocorrect
{
    namespace debug
    {
        class DebugTools : public viscocorrect::frontend::util_frontend::Layer
        {
        public:
            DebugTools(std::shared_ptr<EventCallbackType> callback);
            virtual ~DebugTools();

            virtual void OnUIRender() override;

            template <typename T>
            void AddTool(const std::shared_ptr<T> &_t)
            {
                tool_stack_.PushLayer(_t);
            }

        private:
            viscocorrect::frontend::util_frontend::LayerStack tool_stack_;
            CurveFitting curve_fitting_;
        };

        class DebugToolBase : public viscocorrect::frontend::util_frontend::Layer
        {
        public:
            DebugToolBase() = default;
            DebugToolBase(const std::string &);
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

        protected:
            std::vector<std::unique_ptr<std::function<void()>>> callbacks_;

            void RunCallbacks();

        private:
            std::string name_;
        };

    }
}

#endif // SPAULY_VISCOCORRECT_DEBUG_DEBUG_TOOLS_H