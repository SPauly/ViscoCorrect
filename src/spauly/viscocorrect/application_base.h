#ifndef SPAULY_VISCOCORRECT_APPLICATION_BASE_H
#define SPAULY_VISCOCORRECT_APPLICATION_BASE_H

#include <functional>
#include <memory>
#include <vector>

#include "spauly/viscocorrect/util/event.h"

namespace viscocorrect {
class GraphImplBase;  // defined in graph_base.h
struct Project;       // defined in project.h

class ApplicationBase {
 public:
  ApplicationBase() = default;
  virtual ~ApplicationBase() = default;

  virtual bool Init() = 0;
  virtual void Shutdown() = 0;
  virtual bool Render() = 0;
  virtual inline const float GetFramerate() = 0;

  virtual std::shared_ptr<GraphImplBase> get_graph() {
    return nullptr;
  }  // overload this for debugging purposes

  inline void set_event_callback(std::shared_ptr<EventCallbackType> callback) {
    register_event_ = callback;
  }

 protected:
  inline std::shared_ptr<std::vector<Project>> &get_projects() {
    return projects_;
  }
  inline std::shared_ptr<EventCallbackType> &get_register_event() {
    return register_event_;
  }

  inline bool set_should_close(bool close = true) {
    return should_close_ = close;
  }
  inline bool get_should_close() { return should_close_; }

 private:
  bool should_close_ = false;

  float time_step_ = 0.0f;
  float frame_time_ = 0.0f;
  float last_frame_time_ = 0.0f;

  std::shared_ptr<std::vector<Project>> projects_;
  std::shared_ptr<EventCallbackType> register_event_;
};

}  // namespace viscocorrect

#endif  // SPAULY_VISCOCORRECT_APPLICATION_BASE_H