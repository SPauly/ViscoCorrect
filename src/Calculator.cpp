#include "Calculator.h"

#include <stdexcept>

namespace ViscoCorrect
{
    Calculator::Calculator() : m_cq(m_raw_data.cq), m_cv(m_raw_data.cv)
    {
        for(auto &pol : m_raw_data.ch)
        {
            m_ch.push_back(Polynom(pol));
        }
    }

    Project *Calculator::Calculate(Project *_prj)
    {
        _prj->func_totalhead = CreateLinearF(m_raw_data.totalhead, m_raw_data.pitch_th, _prj->parameters.total_head_m, m_raw_data.startpos_th);
        _prj->func_visco = CreateLinearF(m_raw_data.viscosity, m_raw_data.pitch_v, _prj->parameters.viscosity_v, m_raw_data.startpos_v, false);

        _prj->flow_pos = FitToScale(m_raw_data.flowrates, _prj->parameters.flowrate_q, m_raw_data.startpos_f);

        _prj->correction_x = _prj->func_visco->get_x(_prj->func_totalhead->f(_prj->flow_pos));

        GetCorrectionFactors(&_prj->correction, _prj->correction_x);

        return _prj;
    }

    CorrectionFactors *Calculator::GetCorrectionFactors(CorrectionFactors *_obj, const double _x)
    {
        _obj->c_v = (m_cv.f(_x) / (double)m_raw_data.stepsize_correction / 10) + 0.2;
        _obj->c_q = (m_cq.f(_x) / (double)m_raw_data.stepsize_correction / 10) + 0.2;
        for (int i = 0; i < m_ch.size(); i++)
        {
            _obj->c_h[i] = (m_ch.at(i).f(_x) / (double)m_raw_data.stepsize_correction / 10) - 0.3;
        }

        return _obj;
    }

    const double Calculator::FitToScale(const std::map<int, int> &_raw_scale, const int _input, const int *_startpos)
    {
        double absolute_position = (_startpos) ? (double)_startpos[0] : 0.0;
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
            pos = FitToScale(_raw_scale, _input, _startpos);
        }
        catch (const std::runtime_error &e)
        {
            return nullptr;
        }

        return new LinearFunction(_m, (_scale_on_x) ? pos : _startpos[0], (!_scale_on_x) ? pos : _startpos[1]);
    }

} // namespace ViscoCorrect
