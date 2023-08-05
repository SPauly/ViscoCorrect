#ifndef SPAULY_VISCOCORRECT_CALCULATOR_H
#define SPAULY_VISCOCORRECT_CALCULATOR_H

#include <map>
#include <memory>
#include <vector>

#include "spauly/viscocorrect/project.h"
#include "spauly/viscocorrect/util/mathematical_functions.h"

namespace viscocorrect {
class Calculator {
 public:
  Calculator();
  virtual ~Calculator() = default;

  Project *Calculate(Project *project);
  CorrectionFactors *GetCorrectionFactors(CorrectionFactors *cor_factors,
                                          const double x_pos);

 protected:
  const float FitToScale(const std::map<int, int> &raw_scale_units,
                          const float input, const int startpos = 0);
  util::LinearFunction *CreateLinearF(const std::map<int, int> &_raw_scale,
                                      const double _m, const float _input,
                                      const int *_startpos = nullptr,
                                      bool _scale_on_x = true);

 private:
  util::PolynomialFunction func_eta_;
  util::PolynomialFunction func_q_;
  std::vector<util::LogisticalFunction> func_h_;
};
}  // namespace viscocorrect

#endif  // SPAULY_VISCOCORRECT_CALCULATOR_H