#ifndef VISCOCORRECT_SRC_UTILS_LAYER_H
#define VISCOCORRECT_SRC_UTILS_LAYER_H

namespace viscocorrect
{
    namespace util_frontend
    {
        class Layer
        {
        public:
            virtual ~Layer() = default;

            virtual void OnAttach(){};
            virtual void OnDetach(){};

            virtual void OnUpdate(float ts){};
            virtual void OnUIRender(){};
        };
    } // namespace util_frontend
} // namespace viscocorrect

#endif // VISCOCORRECT_SRC_UTILS_LAYER_H