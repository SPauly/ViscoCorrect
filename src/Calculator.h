#pragma once

#include <vector>
#include <memory>

#include "utils/Types.h"
#include "utils/vc_config.h"

namespace ViscoCorrect
{
    class Calculator
    {
    public:
        Calculator();
        virtual ~Calculator() = default;

        Project *Calculate(Project *_prj);
        CorrectionFactors *GetCorrectionFactors(CorrectionFactors *_obj, const double _x);

    protected:
        const double FitToScale(const std::map<int, int> &_raw_scale, const int _input, const int _startpos = 0);
        LinearFunction *CreateLinearF(const double _m, const double _value, bool _scale_on_x = true, const int _other_coordinate = 0);
        LinearFunction *CreateLinearF(const std::map<int, int> &_raw_scale, const double _m, const int _input, const int *_startpos = nullptr, bool _scale_on_x = true);

    private:
        VCConfig::RawData m_raw_data;

        Polynom m_cv;
        Polynom m_cq;
        std::vector<Polynom> m_ch;

        std::shared_ptr<LinearFunction> current_head, current_visco;
    };
} // namespace ViscoCorrect
