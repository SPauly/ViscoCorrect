// ViscoCorrect - Correction factors centrifugal pumps
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