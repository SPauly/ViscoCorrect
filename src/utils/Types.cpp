#include "types.h"

namespace viscocorrect
{
    LinearFunction::LinearFunction(LineCoordinates l, int xmin, int xmax) : render_coords_(l), xmin_(xmin), xmax_(xmax)
    {
        m_ = static_cast<double>(render_coords_.y_coords[1] - render_coords_.y_coords[0]) / (render_coords_.x_coords[1] - render_coords_.x_coords[0]);
        b_ = static_cast<double>(render_coords_.y_coords[0] - (render_coords_.x_coords[0] * m_));
    }

    LinearFunction::LinearFunction(double pitch, double x, double y) : m_(pitch)
    {
        b_ = static_cast<double>(y - (m_ * x));
        render_coords_ = get_render_coords(xmin_, xmax_);
    }

    LineCoordinates &LinearFunction::get_render_coords(int _xmin, int _xmax)
    {
        render_coords_.x_coords[0] = _xmin;
        render_coords_.x_coords[1] = _xmax;
        render_coords_.y_coords[0] = f(_xmin);
        render_coords_.y_coords[1] = f(_xmax);

        return render_coords_;
    }

} // namespace ViscoCorrect
