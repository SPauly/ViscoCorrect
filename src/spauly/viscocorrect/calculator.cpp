#include "spauly/viscocorrect/calculator.h"

#include <stdexcept>

#include "spauly/viscocorrect/util/properties.h"

namespace viscocorrect {
Calculator::Calculator()
    : func_cq_(internal::kProperties.kCoefficientsQ),
      func_cn_(internal::kProperties.kCoefficientsN) {
  for (auto &pol : internal::kProperties.kCoefficientsH) {
    func_ch_.push_back(util::LogisticalFunction(pol));
  }
}

Project *Calculator::Calculate(Project *prj) {
  // verify the input
  if (!IsFlowrateInputOkay(prj->parameters.flowrate_q) ||
      !IsTotalHeadInputOkay(prj->parameters.total_head_m) ||
      !IsViscosityInputOkay(prj->parameters.viscosity_v))
    return prj;

  prj->func_totalhead = CreateLinearF(
      internal::kProperties.kTotalHeadScale, internal::kProperties.kPitchTotalH,
      prj->parameters.total_head_m, internal::kProperties.kStartTotalH, false);
  prj->func_visco = CreateLinearF(
      internal::kProperties.kViscoScale, internal::kProperties.kPitchVisco,
      prj->parameters.viscosity_v, internal::kProperties.kStartVisco, true);

  prj->flow_pos = FitToScale(internal::kProperties.kFlowrateScale,
                             prj->parameters.flowrate_q,
                             internal::kProperties.kStartFlowrate[0]);

  if (!prj->func_totalhead || !prj->func_visco) return prj;

  prj->correction_x =
      prj->func_visco->get_x(prj->func_totalhead->f(prj->flow_pos));

  GetCorrectionFactors(&prj->correction, prj->correction_x);

  prj->correction.c_h_selected =
      prj->correction.c_h_all[(int)prj->parameters.selected_h_curve];

  return prj;
}

CorrectionFactors *Calculator::GetCorrectionFactors(CorrectionFactors *obj,
                                                    const double _x) {
  if (internal::ValidateXN(_x)) {
    obj->c_n = (func_cn_.f(_x) /
                (double)internal::kProperties.kCorrectionScale / (double)10.0) +
               (double)0.2;
  } else {
    obj->c_n = (_x < internal::kProperties.kCutoffN[0]) ? 1.0 : 0.0;
  }

  if (internal::ValidateXQ(_x)) {
    obj->c_q =
        (func_cq_.f(_x) / (double)internal::kProperties.kCorrectionScale / 10) +
        0.2;
  } else {
    obj->c_q = (_x < internal::kProperties.kCutoffQ[0]) ? 1.0 : 0.0;
  }

  if (internal::ValidateXH(_x)) {
    for (int i = 0; i < func_ch_.size(); i++) {
      obj->c_h_all[i] = (func_ch_.at(i).f(_x) /
                         (double)internal::kProperties.kCorrectionScale / 10) -
                        0.3;
    }
  } else {
    for (int i = 0; i < func_ch_.size(); i++) {
      obj->c_h_all[i] = (_x < internal::kProperties.kCutoffH[0]) ? 1.0 : 0.0;
    }
  }

  return obj;
}

const double Calculator::FitToScale(const std::map<int, int> &_raw_scale,
                                    const int _input, const int _startpos) {
  double absolute_position = (double)_startpos;
  int prev_value = 0;
  bool bfound = false;

  for (const auto &pair : _raw_scale) {
    int value = pair.first;

    if (value == _input) {
      absolute_position += pair.second;
      bfound = true;
      break;
    } else if (value > _input) {
      int range = value - prev_value;
      int relative_value = _input - prev_value;

      absolute_position +=
          ((double)relative_value / (double)range) * (double)pair.second;
      bfound = true;
      break;
    }

    absolute_position += pair.second;
    prev_value = value;
  }

  if (bfound)
    return absolute_position;
  else
    throw std::runtime_error("Input not in range of scale");
}

util::LinearFunction *Calculator::CreateLinearF(
    const std::map<int, int> &_raw_scale, const double _m, const int _input,
    const int *_startpos, bool _scale_on_x) {
  double pos;
  try {
    pos = FitToScale(_raw_scale, _input,
                     (_scale_on_x)
                         ? _startpos[0]
                         : _startpos[1]);  // sometimes setting it to
                                           // _startpos[0] works better...
  } catch (const std::runtime_error &e) {
    return nullptr;
  }

  return new util::LinearFunction(_m, (_scale_on_x) ? pos : _startpos[0],
                                  (!_scale_on_x) ? pos : _startpos[1]);
}

}  // namespace viscocorrect
