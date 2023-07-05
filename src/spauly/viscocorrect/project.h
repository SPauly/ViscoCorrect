#ifndef SPAULY_VISCOCORRECT_PROJECT_H
#define SPAULY_VISCOCORRECT_PROJECT_H

#include <string>
#include <functional>

#include "spauly/viscocorrect/util/mathematical_functions.h"
#include "spauly/viscocorrect/util/input_units.h"

namespace viscocorrect
{
    struct CalcParameters
    {
        int flowrate_q = 0;
        int total_head_m = 0;
        int viscosity_v = 0;
        util::ViscoUnits visco_unit = util::ViscoUnits::kMilliPascalSeconds;
        util::FlowrateUnits flowrate_unit = util::FlowrateUnits::kLitersPerMinute;
    };

    struct CorrectionFactors
    {
        double c_q = 0.0f;
        double c_v = 0.0f;
        double c_h[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    };

    struct Project
    {
        // metadata
        std::string name = "";

        // data
        CalcParameters parameters;
        CorrectionFactors correction;

        // Calculated Data
        util::LinearFunction *func_totalhead = nullptr;
        util::LinearFunction *func_visco = nullptr;
        double flow_pos = 0;
        double correction_x = 0;

        // render functions
        std::function<void()> *render_params = nullptr;
        std::function<void()> *render_correction = nullptr;

        // Ctor and dtor
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
    };
}

#endif // SPAULY_VISCOCORRECT_PROJECT_H