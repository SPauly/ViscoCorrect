#include "Calculator.h"

namespace ViscoCorrect
{
    Calculator::Calculator() : m_cq(m_raw_data.cq), m_cv(m_raw_data.cv)
    {
    }

    Project *Calculator::Calculate(Project *_prj)
    {
        _prj->func_totalhead = CreateLinearF(m_raw_data.totalhead, m_raw_data.pitch_th, _prj->parameters.total_head_m, m_raw_data.startpos_th);
        _prj->func_visco = CreateLinearF(m_raw_data.viscosity, m_raw_data.pitch_v, _prj->parameters.viscosity_v, m_raw_data.startpos_v, false);
        //do the rest
        
        return _prj;
    }

    LinearFunction *Calculator::CreateLinearF(const std::map<int, int> &_raw_scale, const double _m, const int _value, const int* _startpos, bool _scale_on_x)
    {
        double absolute_position = (_startpos) ? (double)_startpos[0] : 0.0;
        int prev_value = 0;
        bool bfound = false;

        for(const auto &pair : _raw_scale)
        {
            int value = pair.first;
            
            if(value == _value)
            {
                absolute_position += pair.second;
                bfound = true;
                break;
            }
            else if(value > _value)
            {
                int range = value - prev_value;
                int relative_value = _value - prev_value;

                absolute_position += ((double)relative_value / (double)range) * (double)pair.second;
                bfound = true;
                break;
            }

            absolute_position += pair.second;
            prev_value = value;
        }

        if(bfound)
        {
            return new LinearFunction(_m, (_scale_on_x) ? absolute_position : _startpos[0], (!_scale_on_x) ? absolute_position : _startpos[1]);
        }   
        else
            return nullptr; //throw exception
    }

} // namespace ViscoCorrect
