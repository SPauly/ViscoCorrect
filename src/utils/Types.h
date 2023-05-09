#pragma once

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
        float c_q = 0.0f;
        float c_v = 0.0f;
        float c_h[4] = {0.0f,0.0f,0.0f,0.0f};
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
        LinearFunction(LineCoordinates);

        template<typename T>
        T f(const T _x){
            return m * _x + (b*y_scale);
        }

        inline void ScaleYAxis(double _scale) { y_scale = _scale; }
        inline void SetRange(int _xmin, int _xmax) { xmin = _xmin; xmax = _xmax; }

        const LineCoordinates &GetCoordinates();
        const LineCoordinates &GetCoordinates(int, int);

    private:
        double m = 0.0, b = 0.0;
        double y_scale = 1.0f;
        int xmin = 0, xmax = 1;

        LineCoordinates m_initial_coords;
        LineCoordinates m_current_coords;
    };
    
}