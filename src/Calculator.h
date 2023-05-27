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

    protected:
        LinearFunction *CreateLinearF(const std::map<int, int> &_raw_scale, const double _m, const int _value, const int *_startpos = nullptr, bool _scale_on_x = true);

    private:
        VCConfig::RawData m_raw_data;

        Polynom m_cq;
        Polynom m_cv;
        std::vector<Polynom> m_ch;

        std::shared_ptr<LinearFunction> current_head, current_visco;
    };
} // namespace ViscoCorrect
