// ViscoCorrect - Correction factors centrifugal pumps
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

#include "spauly/viscocorrect/util/properties.h"

namespace viscocorrect {
namespace internal {
const Properties kProperties;

bool ValidateXQ(const double &x) {
  return (x >= static_cast<double>(kProperties.kCutoffQ[0]) &&
          x <= static_cast<double>(kProperties.kCutoffQ[1]));
}

bool ValidateXEta(const double &x) {
  return (x >= static_cast<double>(kProperties.kCutoffEta[0]) &&
          x <= static_cast<double>(kProperties.kCutoffEta[1]));
}

bool ValidateXH(const double &x) {
  return (x >= static_cast<double>(kProperties.kCutoffH[0]) &&
          x <= static_cast<double>(kProperties.kCutoffH[1]));
}

}  // namespace internal

bool IsFlowrateInputOkay(const float &flowrate) {
  return (flowrate >= internal::kProperties.kInputFlowrate[0] &&
          flowrate <= internal::kProperties.kInputFlowrate[1]);
}

bool IsTotalHeadInputOkay(const float &total_head) {
  return (total_head >= internal::kProperties.kInputTotalHead[0] &&
          total_head <= internal::kProperties.kInputTotalHead[1]);
}

bool IsViscosityInputOkay(const float &viscosity) {
  return (viscosity >= internal::kProperties.kInputViscosity[0] &&
          viscosity <= internal::kProperties.kInputViscosity[1]);
}

}  // namespace viscocorrect
