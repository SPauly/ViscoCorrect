#pragma once

namespace ViscoCorrect
{
    struct CalcParameters
    {
        int flowrate_q = 0;
        int total_head_m = 0;
        int viscosity_v = 0;
    };

    struct CorrectionFactors
    {
        float c_q = 0.0f;
        float c_v = 0.0f;
        float c_h[4] = {0.0f,0.0f,0.0f,0.0f};
    };
    
}