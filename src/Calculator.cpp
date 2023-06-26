#include "calculator.h"

#include <stdexcept>

namespace viscocorrect
{
    Calculator::Calculator() : poly_cq_(raw_data_.cq), poly_cv_(raw_data_.cv)
    {
        for (auto &pol : raw_data_.ch)
        {
            poly_ch_.push_back(PolynomialFunction(pol));
        }
    }

    Project *Calculator::Calculate(Project *_prj)
    {
        _prj->func_totalhead = CreateLinearF(raw_data_.totalhead, raw_data_.pitch_th, _prj->parameters.total_head_m, raw_data_.startpos_th, false);
        _prj->func_visco = CreateLinearF(raw_data_.viscosity, raw_data_.pitch_v, _prj->parameters.viscosity_v, raw_data_.startpos_v, true);

        _prj->flow_pos = FitToScale(raw_data_.flowrates, _prj->parameters.flowrate_q, raw_data_.startpos_f[0]);

        _prj->correction_x = _prj->func_visco->get_x(_prj->func_totalhead->f(_prj->flow_pos));

        GetCorrectionFactors(&_prj->correction, _prj->correction_x);

        return _prj;
    }

    CorrectionFactors *Calculator::GetCorrectionFactors(CorrectionFactors *_obj, const double _x)
    {
        _obj->c_v = (poly_cv_.f(_x) / (double)raw_data_.stepsize_correction / 10) + 0.2;
        _obj->c_q = (poly_cq_.f(_x) / (double)raw_data_.stepsize_correction / 10) + 0.2;
        for (int i = 0; i < poly_ch_.size(); i++)
        {
            _obj->c_h[i] = (poly_ch_.at(i).f(_x) / (double)raw_data_.stepsize_correction / 10) - 0.3;
        }

        return _obj;
    }

    const double Calculator::FitToScale(const std::map<int, int> &_raw_scale, const int _input, const int _startpos)
    {
        double absolute_position = (double)_startpos;
        int prev_value = 0;
        bool bfound = false;

        for (const auto &pair : _raw_scale)
        {
            int value = pair.first;

            if (value == _input)
            {
                absolute_position += pair.second;
                bfound = true;
                break;
            }
            else if (value > _input)
            {
                int range = value - prev_value;
                int relative_value = _input - prev_value;

                absolute_position += ((double)relative_value / (double)range) * (double)pair.second;
                bfound = true;
                break;
            }

            absolute_position += pair.second;
            prev_value = value;
        }

        if (bfound)
            return absolute_position;
        else
            throw std::runtime_error("Input not in range of scale");
    }

    LinearFunction *Calculator::CreateLinearF(const double _m, const double _value, bool _scale_on_x, const int _other_coordinate)
    {
        return new LinearFunction(_m, (_scale_on_x) ? _value : _other_coordinate, (!_scale_on_x) ? _value : _other_coordinate);
    }

    LinearFunction *Calculator::CreateLinearF(const std::map<int, int> &_raw_scale, const double _m, const int _input, const int *_startpos, bool _scale_on_x)
    {
        double pos;
        try
        {
            pos = FitToScale(_raw_scale, _input, (_scale_on_x) ? _startpos[0] : _startpos[1]); // sometimes setting it to _startpos[0] works better...
        }
        catch (const std::runtime_error &e)
        {
            return nullptr;
        }

        return new LinearFunction(_m, (_scale_on_x) ? pos : _startpos[0], (!_scale_on_x) ? pos : _startpos[1]);
    }

} // namespace viscocorrect
