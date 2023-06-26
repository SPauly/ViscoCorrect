#ifndef VISCOCORRECT_SRC_TYPES_H
#define VISCOCORRECT_SRC_TYPES_H

#include <string>
#include <functional>
#include <cmath>

namespace viscocorrect
{
    struct CalcParameters
    {
        int flowrate_q = 0;
        int total_head_m = 0;
        int viscosity_v = 0;
    };

    struct CorrectionFactors
    {
        double c_q = 0.0f;
        double c_v = 0.0f;
        double c_h[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    };

    struct LineCoordinates
    {
        int x_coords[2]; // Why did I use ints here? why not float?
        int y_coords[2];

        LineCoordinates(int x1 = 0, int x2 = 0, int y1 = 0, int y2 = 0) : x_coords{x1, x2}, y_coords{y1, y2} {}
        ~LineCoordinates() = default;
    };

    class LinearFunction
    {
    public:
        LinearFunction(LineCoordinates coords, int xmin = 0, int xmax = 100);
        LinearFunction(double pitch, double x, double y);

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
        inline void SetRange(int xmin, int xmax)
        {
            xmin_ = xmin;
            xmax_ = xmax;
            get_render_coords(xmin, xmax);
        }

        // this is unclean --> has to be revised
        inline LineCoordinates &get_render_coords() { return render_coords_; }
        LineCoordinates &get_render_coords(int, int);

    private:
        double m_ = 0.0, b_ = 0.0;
        double y_axis_scale_ = 1.0f;
        int xmin_ = 0, xmax_ = 100;

        LineCoordinates render_coords_;
    };

    class Polynom
    {
    public:
        template <typename... Args>
        Polynom(Args... _args) : coefficients_({_args...}) {}
        Polynom(std::vector<double> &_ply) : coefficients_(_ply) {}
        ~Polynom() {}

        template <typename T>
        T f(const T x)
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

    private:
        std::vector<double> coefficients_;
    };

    // remove this class since no longer needed
    class LogisticSigmoid
    {
    public:
        LogisticSigmoid(double _l = 1, double _k = 1, double _x0 = 1) : L(_l), k(_k), x0(_x0) {}
        ~LogisticSigmoid() {}

        template <typename T>
        T f(const T _x)
        {
            return static_cast<T>(L / (1 + exp(-k * (_x - x0))));
        }

    private:
        double L, k, x0;
    };

    struct Project
    {
        Project() = default;
        ~Project()
        {
            if (func_totalhead)
                delete func_totalhead;
            if (func_visco)
                delete func_visco;
            if (render_params)
                delete render_params;
            if (render_correction)
                delete render_correction;
        }
        // metadata
        std::string name = "";

        // data
        CalcParameters parameters;
        CorrectionFactors correction;

        // Calculated Data
        LinearFunction *func_totalhead = nullptr;
        LinearFunction *func_visco = nullptr;
        double flow_pos = 0;
        double correction_x = 0;

        // render functions
        std::function<void()> *render_params = nullptr;
        std::function<void()> *render_correction = nullptr;
    };
}

#endif // VISCOCORRECT_SRC_TYPES_H