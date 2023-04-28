#pragma once

#include <memory>

#include "utils/Types.h"

namespace ViscoCorrect
{
    class Calculator
    {
    public:
        Calculator() = default;
        ~Calculator() = default;

        std::shared_ptr<CorrectionFactors> calc();
        std::shared_ptr<CorrectionFactors> calc(std::shared_ptr<CalcParameters>);
        
    private:
        std::shared_ptr<CalcParameters> m_param;
        std::shared_ptr<CorrectionFactors> m_correction;
    };
}