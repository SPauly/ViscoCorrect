#ifndef SPAULY_VISCOCORRECT_CALCULATOR_H
#define SPAULY_VISCOCORRECT_CALCULATOR_H

#include <vector>
#include <memory>
#include <map>

#include "spauly/viscocorrect/project.h"
#include "spauly/viscocorrect/util/mathematical_functions.h"

namespace viscocorrect
{
    class Calculator
    {
    public:
        Calculator();
        virtual ~Calculator() = default;

        Project *Calculate(Project *project);
        CorrectionFactors *GetCorrectionFactors(CorrectionFactors *cor_factors, const double x_pos);

    protected:
        // have to make these more convenient anyways
        const double FitToScale(const std::map<int, int> &raw_scale_units, const int input, const int startpos = 0);
        util::LinearFunction *CreateLinearF(const double rate, const double pos_var, bool _scale_on_x = true, const int _other_coordinate = 0);
        util::LinearFunction *CreateLinearF(const std::map<int, int> &_raw_scale, const double _m, const int _input, const int *_startpos = nullptr, bool _scale_on_x = true);

    private:
        util::PolynomialFunction poly_cv_;
        util::PolynomialFunction poly_cq_;
        std::vector<util::PolynomialFunction> poly_ch_;
    };
} // namespace viscocorrect

#endif // SPAULY_VISCOCORRECT_CALCULATOR_H