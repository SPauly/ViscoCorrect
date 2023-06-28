#include "spauly/viscocorrect/calculator.h"

#include <stdexcept>

#include "spauly/viscocorrect/util/properties.h"

namespace viscocorrect
{
    Calculator::Calculator() : poly_cq_(internal::kProperties.kCoefficientsQ), poly_cv_(internal::kProperties.kCoefficientsV)
    {
        for (auto &pol : internal::kProperties.kCoefficientsH)
        {
            poly_ch_.push_back(util::PolynomialFunction(pol));
        }
    }

    Project *Calculator::Calculate(Project *_prj)
    {
        _prj->func_totalhead = CreateLinearF(internal::kProperties.kTotalHeadScale, internal::kProperties.kPitchTotalH, _prj->parameters.total_head_m, internal::kProperties.kStartTotalH, false);
        _prj->func_visco = CreateLinearF(internal::kProperties.kViscoScale, internal::kProperties.kPitchVisco, _prj->parameters.viscosity_v, internal::kProperties.kStartVisco, true);

        _prj->flow_pos = FitToScale(internal::kProperties.kFlowrateScale, _prj->parameters.flowrate_q, internal::kProperties.kStartFlowrate[0]);

        _prj->correction_x = _prj->func_visco->get_x(_prj->func_totalhead->f(_prj->flow_pos));

        GetCorrectionFactors(&_prj->correction, _prj->correction_x);

        return _prj;
    }

    CorrectionFactors *Calculator::GetCorrectionFactors(CorrectionFactors *_obj, const double _x)
    {
        _obj->c_v = (poly_cv_.f(_x) / (double)internal::kProperties.kCorrectionScale / 10) + 0.2;
        _obj->c_q = (poly_cq_.f(_x) / (double)internal::kProperties.kCorrectionScale / 10) + 0.2;
        for (int i = 0; i < poly_ch_.size(); i++)
        {
            _obj->c_h[i] = (poly_ch_.at(i).f(_x) / (double)internal::kProperties.kCorrectionScale / 10) - 0.3;
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

    util::LinearFunction *Calculator::CreateLinearF(const double _m, const double _value, bool _scale_on_x, const int _other_coordinate)
    {
        return new util::LinearFunction(_m, (_scale_on_x) ? _value : _other_coordinate, (!_scale_on_x) ? _value : _other_coordinate);
    }

    util::LinearFunction *Calculator::CreateLinearF(const std::map<int, int> &_raw_scale, const double _m, const int _input, const int *_startpos, bool _scale_on_x)
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

        return new util::LinearFunction(_m, (_scale_on_x) ? pos : _startpos[0], (!_scale_on_x) ? pos : _startpos[1]);
    }

} // namespace viscocorrect
