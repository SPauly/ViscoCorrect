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

            template<typename T>
            void AddTool(const T*);

        private:
            LayerStack mvec_tools;
        };

        class DebugToolBase : public Layer
        {
        public: 
            DebugToolBase() = delete;
            DebugToolBase(const char*);
            virtual ~DebugToolBase(){};

            virtual void OnUIRender() final;

            //overload this function
            virtual void Run();

            void AddCallback(std::function<void()> *);
            void RemoveCallback(std::function<void()> *);

        protected:
            std::vector<std::function<void()>> mvec_callbacks;

        private:
            const char *name;
        };

        DebugTools *s_DebugInstance = nullptr;
    }
}