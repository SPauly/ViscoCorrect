#ifndef SPAULY_VISCOCORRECT_UTILS_LAYER_H
#define SPAULY_VISCOCORRECT_UTILS_LAYER_H

namespace viscocorrect {
namespace frontend {
namespace util_frontend {
class Layer {
 public:
  virtual ~Layer() = default;

  virtual void OnAttach(){};
  virtual void OnDetach(){};

  virtual void OnUpdate(float ts){};
  virtual void OnUIRender(){};
};
}  // namespace util_frontend
}  // namespace frontend
}  // namespace viscocorrect

#endif  // SPAULY_VISCOCORRECT_UTILS_LAYER_H