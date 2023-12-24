// ViscoCorrect - Correction factors for centrifugal pumps
// Copyright (C) 2023  Simon Pauly
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
// Contact via <https://github.com/SPauly/ViscoCorrect> 

#ifndef SPAULY_VISCOCORRECT_UTILS_EVENT_H
#define SPAULY_VISCOCORRECT_UTILS_EVENT_H

#include <deque>
#include <functional>
#include <memory>

#include "spauly/viscocorrect/util/properties.h"

namespace viscocorrect {
// defined in this file
namespace util {
class EventBase;
template <typename T>
class Event;
}  // namespace util

using EventCallbackType = std::function<void(std::unique_ptr<util::EventBase>)>;

namespace util {
enum EventType {
  kCalcReq,
  kManageProjectReq,
  kGetProjectReq,
  kProjectListReq,
  kGetGraphInstance
};

class EventBase {
 public:
  EventBase() = default;
  EventBase(util::EventType etype) : event_type_(etype) {}
  virtual ~EventBase() {}

  inline const util::EventType &get_event_type_() { return event_type_; }

  template <typename T>
  T *GetData() {
    return static_cast<T *>(GetDataImpl());
  }

 protected:
  virtual void *GetDataImpl() { return nullptr; }

 private:
  util::EventType event_type_;
};

template <typename T>
class Event : public EventBase {
 public:
  Event(util::EventType etype, T *data,
        std::function<void(T)> *callback = nullptr)
      : EventBase(etype), data_(data), callback_(callback) {}
  ~Event() {}

 protected:
  virtual void *GetDataImpl() override { return data_; }

 private:
  T *data_;
  std::function<void(T)> *callback_;
};

inline void PushEvent(std::deque<std::unique_ptr<EventBase>> *que,
                      std::unique_ptr<EventBase> event) {
  que->push_back(std::move(event));
};
}  // namespace util
}  // namespace viscocorrect

#endif  // SPAULY_VISCOCORRECT_UTILS_EVENT_H