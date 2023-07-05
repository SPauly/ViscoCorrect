#ifndef VISCOCORRECT_UTILS_DEBUGGING_DEBUGTOOLS_H
#define VISCOCORRECT_UTILS_DEBUGGING_DEBUGTOOLS_H

#include <vector>
#include <memory>
#include <functional>
#include <string>

#include "imgui.h"

#include "debug/CurveFitting.h"
#include "util/layer.h"
#include "util/layerstack.h"

namespace viscocorrect
{
    namespace Debug
    {
        class DebugTools : public util::Layer
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
            util::LayerStack mvec_tools;
            CurveFitting m_debug_curve;
        };

        class DebugToolBase : public util::Layer
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
            std::vector<std::unique_ptr<std::function<void()>>> mvec_callbacks;

            void RunCallbacks();

        private:
            std::string m_name;
        };

    }
}

#endif // VISCOCORRECT_UTILS_DEBUGGING_DEBUGTOOLS_H