// ViscoCorrect - Correction factors for centrifugal pumps
// Copyright (C) 2023  Simon Pauly
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
// Contact via <https://github.com/SPauly/ViscoCorrect> 

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

 private:
  const float FitToScale(const std::map<int, int> &raw_scale_units,
                         const float input, const int startpos = 0);
  util::LinearFunction *CreateLinearF(const std::map<int, int> &_raw_scale,
                                      const double _m, const float _input,
                                      const int *_startpos = nullptr,
                                      bool _scale_on_x = true);

  class  ConversionContext
  {
  public:
    ConversionContext() = delete;
    ConversionContext(Project *project);
    ~ConversionContext();

    ConversionContext(const ConversionContext &) = delete;
    ConversionContext &operator=(const ConversionContext&) = delete;
  
  private:
    Project *prj_;
    void ConvertInput(Project *project);
    void UndoConversion(Project *project);
  };

 private:
  util::PolynomialFunction func_eta_;
  util::PolynomialFunction func_q_;
  std::vector<util::LogisticalFunction> func_h_;
};
}  // namespace viscocorrect

#endif  // SPAULY_VISCOCORRECT_CALCULATOR_H