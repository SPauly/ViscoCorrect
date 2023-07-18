#include "spauly/viscocorrect/util/properties.h"

namespace viscocorrect {
namespace internal {
const Properties kProperties;

bool ValidateXQ(const double &x) {
  return (x >= static_cast<double>(kProperties.kCutoffQ[0]) &&
          x <= static_cast<double>(kProperties.kCutoffQ[1]));
}

bool ValidateXN(const double &x) {
  return (x >= static_cast<double>(kProperties.kCutoffN[0]) &&
          x <= static_cast<double>(kProperties.kCutoffN[1]));
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
