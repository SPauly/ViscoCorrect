#ifndef VISCOCORRECT_SRC_PROJECT_H
#define VISCOCORRECT_SRC_PROJECT_H

#include <string>
#include <functional>

#include "util/mathematical_functions.h"

namespace viscocorrect
{
    struct CalcParameters
    {
        int flowrate_q = 0;
        int total_head_m = 0;
        int viscosity_v = 0;
    };

    struct CorrectionFactors
    {
        double c_q = 0.0f;
        double c_v = 0.0f;
        double c_h[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    };

    struct Project
    {
        Project() = default;
        ~Project()
        {
            if (func_totalhead)
                delete func_totalhead;
            if (func_visco)
                delete func_visco;
            if (render_params)
                delete render_params;
            if (render_correction)
                delete render_correction;
        }
        // metadata
        std::string name = "";

        // data
        CalcParameters parameters;
        CorrectionFactors correction;

        // Calculated Data
        util::LinearFunction<float> *func_totalhead = nullptr;
        util::LinearFunction<float> *func_visco = nullptr;
        double flow_pos = 0;
        double correction_x = 0;

        // render functions
        std::function<void()> *render_params = nullptr;
        std::function<void()> *render_correction = nullptr;
    };
}

#endif // VISCOCORRECT_SRC_PROJECT_H