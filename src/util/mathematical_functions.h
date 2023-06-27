#ifndef VISCOCORRECT_SRC_UTIL_MATHEMATICAL_FUNCTIONS_H
#define VISCOCORRECT_SRC_UTIL_MATHEMATICAL_FUNCTIONS_H

#include <vector>
#include <cmath>

namespace viscocorrect
{
    namespace util
    {
        // declared later in this file
        template <typename T>
        class LineCoordinates;
        class LinearFunction;
        class PolynomialFunction;

        template <typename T = float>
        class LineCoordinates
        {
        public:
            LineCoordinates();
            LineCoordinates(const std::vector<T> &x, const std::vector<T> &y);
            ~LineCoordinates();

            struct StorageNotation
            {
                std::vector<T> x_coords, y_coords;
            };
            struct ArrayNotation
            {
                T *x_coords, *y_coords;
            };
            typedef std::vector<std::pair<T, T>> PairNotation; // PairNotation is an array of x and y values

            inline const StorageNotation &get() const { return storage_notation_; }
            inline ArrayNotation &get_array_notation() { return array_notation_; }
            inline const PairNotation &get_pair_notation() const { return pair_notation_; }
            inline const size_t get_number_coordinates() const { return number_coordinates_; }

        private:
            StorageNotation storage_notation_;
            ArrayNotation array_notation_;
            PairNotation pair_notation_;

            size_t number_coordinates_;
        };

        class LinearFunction
        {
        public:
            LinearFunction(LineCoordinates<float> coords)
            {
                m_ = static_cast<double>(coords.get().x_coords[1] - coords.get().y_coords[0]) / (coords.get().x_coords[1] - coords.get().x_coords[0]);
                b_ = static_cast<double>(coords.get().y_coords[0] - (coords.get().x_coords[0] * m_));
            }

            LinearFunction(double pitch, double x, double y) : m_(pitch)
            {
                b_ = static_cast<double>(y - (m_ * x));
            }
            ~LinearFunction() = default;

            template <typename T>
            T f(const T x)
            {
                return static_cast<T>(m_ * static_cast<double>(x) + (b_ * y_axis_scale_));
            }

            template <typename T>
            T get_x(const T y)
            {
                return static_cast<T>((static_cast<double>(y) - (b_ * y_axis_scale_)) / m_);
            }

            inline void set_y_axis_scale(double scale = 1.0) { y_axis_scale_ = scale; }

            template <typename T>
            LineCoordinates<T> CreateLineCoordinates(T min, T max);

        private:
            double m_ = 0.0, b_ = 0.0;
            double y_axis_scale_ = 1.0f;
        };

        class PolynomialFunction
        {
        public:
            template <typename... Args>
            PolynomialFunction(Args... _args) : coefficients_({_args...}) {}
            PolynomialFunction(std::vector<double> &_ply) : coefficients_(_ply) {}
            ~PolynomialFunction() {}

            template <typename T>
            T f(const T x);

        private:
            std::vector<double> coefficients_;
        };

        //----------------------------------------------------------
        // template functions implementation
        template <typename T>
        LineCoordinates<T>::LineCoordinates() : LineCoordinates<T>({0}, {0}) {}

        template <typename T>
        LineCoordinates<T>::LineCoordinates(const std::vector<T> &x, const std::vector<T> &y) : storage_notation_{x, y}
        {
            number_coordinates_ = storage_notation_.x_coords.size();

            array_notation_.x_coords = storage_notation_.x_coords.data();
            array_notation_.y_coords = storage_notation_.y_coords.data();

            for (int i = 0; i < number_coordinates_; i++)
            {
                pair_notation_.push_back(std::make_pair(storage_notation_.x_coords.at(i), storage_notation_.y_coords.at(i)));
            }
        }

        template <typename T>
        LineCoordinates<T>::~LineCoordinates()
        {
            array_notation_.x_coords = nullptr;
            array_notation_.y_coords = nullptr;

            pair_notation_.clear();
            storage_notation_.x_coords.clear();
            storage_notation_.y_coords.clear();
        }

        template <typename T>
        LineCoordinates<T> LinearFunction::CreateLineCoordinates(T min, T max)
        {
            return LineCoordinates<T>({min, max}, {f<T>(min), f<T>(max)});
        }

        template <typename T>
        T PolynomialFunction::f(const T x)
        {
            T y = 0;
            int inverse_iter = coefficients_.size() - 1;

            for (int i = 0; i < coefficients_.size(); i++)
            {
                y += static_cast<T>(coefficients_.at(i) * std::pow((double)x, (double)inverse_iter));
                --inverse_iter;
            }

            return y;
        }
    } // namespace util

} // namespace viscocorrect

#endif // VISCOCORRECT_SRC_UTIL_MATHEMATICAL_FUNCTIONS_H