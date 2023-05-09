#include "Types.h"

namespace ViscoCorrect
{
    LinearFunction::LinearFunction(LineCoordinates _l) : m_initial_coords(_l), m_current_coords(_l)
    {
        m = static_cast<double>(_l.y_coords[1]-_l.y_coords[0])/(_l.x_coords[1] - _l.x_coords[0]);
        b = static_cast<double>(_l.y_coords[0] - (_l.x_coords[0] * m));
    }

    const LineCoordinates &LinearFunction::SetCoordinates(int _xmin, int _xmax)
    {
        xmin = _xmin; xmax = _xmax;

        m_current_coords.x_coords[0] = xmin;
        m_current_coords.x_coords[1] = xmax;
        m_current_coords.y_coords[0] = f(xmin);
        m_current_coords.y_coords[1] = f(xmax);

        return m_current_coords;
    }
    
} // namespace ViscoCorrect
