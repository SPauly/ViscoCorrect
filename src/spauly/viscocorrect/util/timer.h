#ifndef SPAULY_VISCOCORRECT_UTIL_TIMER_H
#define SPAULY_VISCOCORRECT_UTIL_TIMER_H

#include <chrono>

namespace viscocorrect {
namespace util {

class Timer {
 public:
  Timer() { Reset(); }

  inline void Reset() { start_ = std::chrono::high_resolution_clock::now(); }

  template <typename T>
  long long Elapsed() {
    static_assert(std::is_same_v<T, std::chrono::milliseconds> ||
                      std::is_same_v<T, std::chrono::microseconds> ||
                      std::is_same_v<T, std::chrono::nanoseconds>,
                  "T must be either std::chrono::milliseconds, microseconds or "
                  "std::chrono::nanoseconds");

    return std::chrono::duration_cast<T>(
               std::chrono::high_resolution_clock::now() - start_)
        .count();
  }

 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};

}  // namespace util
}  // namespace viscocorrect

#endif SPAULY_VISCOCORRECT_UTIL_TIMER_H