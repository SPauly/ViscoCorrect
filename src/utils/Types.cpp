#include "Types.h"

namespace ViscoCorrect
{
    LinearFunction::LinearFunction(LineCoordinates _l, int _xmin, int _xmax) : m_render_coords(_l), xmin(_xmin), xmax(_xmax)
    {
        m = static_cast<double>(_l.y_coords[1]-_l.y_coords[0])/(_l.x_coords[1] - _l.x_coords[0]);
        b = static_cast<double>(_l.y_coords[0] - (_l.x_coords[0] * m));
    }

    LinearFunction::LinearFunction(double _m, int _x, int _y) : m(_m)
    {
        b = static_cast<double>(_y - (_m*_x));
        m_render_coords = GetRenderCoords(xmin,xmax);
    }

    LineCoordinates &LinearFunction::GetRenderCoords(int _xmin, int _xmax)
    {
        m_render_coords.x_coords[0] = _xmin;
        m_render_coords.x_coords[1] = _xmax;
        m_render_coords.y_coords[0] = f(_xmin);
        m_render_coords.y_coords[1] = f(_xmax);

        return m_render_coords;
    }
    
} // namespace ViscoCorrect
