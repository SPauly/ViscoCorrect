#pragma once
#include <string>
#include <functional>
#include <cmath>

namespace ViscoCorrect
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
        double c_h[4] = {0.0f,0.0f,0.0f,0.0f};
    };

    struct LineCoordinates
    {
        int x_coords[2];
        int y_coords[2];

        LineCoordinates(int _x1 = 0, int _x2 = 0, int _y1 = 0, int _y2 = 0) : x_coords{_x1,_x2}, y_coords{_y1, _y2} {}
        ~LineCoordinates() = default;
    }; 

    class LinearFunction
    {
    public:
        LinearFunction(LineCoordinates, int _xmin = 0, int _xmax = 100);
        LinearFunction(double, int, int);

        template<typename T>
        T f(const T _x){
            return static_cast<T>(m * (double)_x + (b*y_scale));
        }

        template<typename T>
        T get_x(const T _y)
        {
            return static_cast<T>(((double)_y - (b*y_scale))/m);
        }

        inline void ScaleYAxis(double _scale) { y_scale = _scale; }

        inline void SetRange(int _xmin, int _xmax) { xmin = _xmin; xmax = _xmax; GetRenderCoords(xmin,xmax);}

        inline LineCoordinates &GetRenderCoords() { return m_render_coords; }
        LineCoordinates &GetRenderCoords(int, int);

    private:
        double m = 0.0, b = 0.0;
        double y_scale = 1.0f;
        int xmin = 0, xmax = 100;

        LineCoordinates m_render_coords;
    };

    class Polynom
    {
    public:
        template <typename... Args>
        Polynom(Args... _args) : polynoms({_args...}) {}
        Polynom(std::vector<double> &_ply) : polynoms(_ply){}
        ~Polynom(){}

        template<typename T>
        T f(const T _x){
            T y = 0;
            int inv_i = polynoms.size() - 1;

            for(int i = 0; i < polynoms.size(); i++)
            {
                y += static_cast<T>(polynoms.at(i)*std::pow((double)_x, (double)inv_i));
                --inv_i;
            }

            return y;
        }

    private:
        std::vector<double> polynoms;
    };

    struct Project{
        Project() = default;
        ~Project() {
            if(func_totalhead)
                delete func_totalhead;
            if(func_visco)
                delete func_visco;
            if(render_params)
                delete render_params;
            if(render_correction)
                delete render_correction;
        }
        //metadata
        std::string name = "";

        //data
        CalcParameters parameters;
        CorrectionFactors correction; 

        //Calculated Data
        LinearFunction *func_totalhead = nullptr;
        LinearFunction *func_visco = nullptr;
        double flow_pos = 0;
        double correction_x = 0;

        //render functions
        std::function<void()> *render_params = nullptr;
        std::function<void()> *render_correction = nullptr;
    };
    
}