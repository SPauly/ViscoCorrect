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

#ifndef SPAULY_VISCOCORRECT_UTIL_MATHEMATICAL_FUNCTIONS_H
#define SPAULY_VISCOCORRECT_UTIL_MATHEMATICAL_FUNCTIONS_H

#include <cmath>
#include <vector>

namespace viscocorrect {
namespace util {
// declared later in this file
template <typename T>
class LineCoordinates;
class LinearFunction;
class PolynomialFunction;
class LogisticalFunction;

template <typename T = float>
class LineCoordinates {
 public:
  LineCoordinates() : number_coordinates_(0) {}
  LineCoordinates(const std::vector<T> &x, const std::vector<T> &y);
  ~LineCoordinates();

  struct StorageNotation {
    std::vector<T> x_coords, y_coords;
  };
  struct ArrayNotation {
    T *x_coords, *y_coords;
  };

  inline const StorageNotation &get() const { return storage_notation_; }
  inline ArrayNotation &get_array_notation() {
    array_notation_.x_coords = storage_notation_.x_coords.data();
    array_notation_.y_coords = storage_notation_.y_coords.data();
    return array_notation_;
  }
  inline const size_t get_number_coordinates() const {
    return number_coordinates_;
  }

 private:
  StorageNotation storage_notation_;
  ArrayNotation array_notation_;

  size_t number_coordinates_;
};

class LinearFunction {
 public:
  LinearFunction(LineCoordinates<float> coords) {
    m_ = static_cast<double>(coords.get().x_coords[1] -
                             coords.get().y_coords[0]) /
         (coords.get().x_coords[1] - coords.get().x_coords[0]);
    b_ = static_cast<double>(coords.get().y_coords[0] -
                             (coords.get().x_coords[0] * m_));
  }

  LinearFunction(double pitch, double x, double y) : m_(pitch) {
    b_ = static_cast<double>(y - (m_ * x));
  }
  ~LinearFunction() = default;

  template <typename T>
  T f(const T x) {
    return static_cast<T>(m_ * static_cast<double>(x) + (b_ * y_axis_scale_));
  }

  template <typename T>
  T get_x(const T y) {
    return static_cast<T>((static_cast<double>(y) - (b_ * y_axis_scale_)) / m_);
  }

  inline void set_y_axis_scale(double scale = 1.0) { y_axis_scale_ = scale; }

  template <typename T>
  LineCoordinates<T> CreateLineCoordinates(T min, T max);

 private:
  double m_ = 0.0, b_ = 0.0;
  double y_axis_scale_ = 1.0f;
};

class PolynomialFunction {
 public:
  PolynomialFunction() = delete;
  template <typename... Args>
  PolynomialFunction(Args... args) : coefficients_({args...}) {}
  PolynomialFunction(std::vector<double> &coefficients) : coefficients_(coefficients) {}
  ~PolynomialFunction() {coefficients_.clear();}

  template <typename T>
  T f(const T x);

 private:
  std::vector<double> coefficients_;
};

class LogisticalFunction
{
public:
  LogisticalFunction() = delete;
  LogisticalFunction(const std::vector<double> &params)
    : LogisticalFunction(params.at(0), params.at(1), params.at(2)) {}
  LogisticalFunction(double l, double k, double x0) 
    : l_(l), k_(k), x0_(x0) {}

  template <typename T>
  T f(const T x);

private:
  double l_, k_, x0_;
};

//----------------------------------------------------------
// template functions implementation
template <typename T>
LineCoordinates<T>::LineCoordinates(const std::vector<T> &x,
                                    const std::vector<T> &y)
    : storage_notation_{x, y} {
  number_coordinates_ = storage_notation_.x_coords.size();

  array_notation_.x_coords = storage_notation_.x_coords.data();
  array_notation_.y_coords = storage_notation_.y_coords.data();
}

template <typename T>
LineCoordinates<T>::~LineCoordinates() {
  array_notation_.x_coords = nullptr;
  array_notation_.y_coords = nullptr;

  storage_notation_.x_coords.clear();
  storage_notation_.y_coords.clear();
}

template <typename T>
LineCoordinates<T> LinearFunction::CreateLineCoordinates(T min, T max) {
  return LineCoordinates<T>({min, max}, {f<T>(min), f<T>(max)});
}

template <typename T>
T PolynomialFunction::f(const T x) {
  T y = 0;
  int inverse_iter = coefficients_.size() - 1;

  for (int i = 0; i < coefficients_.size(); i++) {
    y += static_cast<T>(coefficients_.at(i) *
                        std::pow((double)x, (double)inverse_iter));
    --inverse_iter;
  }

  return y;
}

template <typename T>
T LogisticalFunction::f(const T x)
{
  return static_cast<T>(l_ / (1 + exp(-k_ * (static_cast<double>(x) - x0_))));
}

}  // namespace util

}  // namespace viscocorrect

#endif  // SPAULY_VISCOCORRECT_UTIL_MATHEMATICAL_FUNCTIONS_H