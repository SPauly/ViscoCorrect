#include "spauly/viscocorrect/calculator.h"

#include "spauly/viscocorrect/util/properties.h"

namespace viscocorrect {
Calculator::Calculator()
    : func_q_(internal::kProperties.kCoefficientsQ),
      func_eta_(internal::kProperties.kCoefficientsEta) {
  for (auto &pol : internal::kProperties.kCoefficientsH) {
    func_h_.push_back(util::LogisticalFunction(pol));
  }
}

Project *Calculator::Calculate(Project *prj) {
  // Make sure the input is converted properly
  ConvertInput(prj);

  // verify the input
  if (!IsFlowrateInputOkay(prj->parameters.flowrate_q) ||
      !IsTotalHeadInputOkay(prj->parameters.total_head_m) ||
      !IsViscosityInputOkay(prj->parameters.viscosity_v)) {
    prj->correction.Clear();
    prj->parameters.has_input_error = true;
    UndoConversion(prj);
    return prj;
  }

  prj->func_totalhead = CreateLinearF(
      internal::kProperties.kTotalHeadScale, internal::kProperties.kPitchTotalH,
      prj->parameters.total_head_m, internal::kProperties.kStartTotalH, false);
  prj->func_visco = CreateLinearF(
      internal::kProperties.kViscoScale, internal::kProperties.kPitchVisco,
      prj->parameters.viscosity_v, internal::kProperties.kStartVisco, true);

  prj->flow_pos = FitToScale(internal::kProperties.kFlowrateScale,
                             prj->parameters.flowrate_q,
                             internal::kProperties.kStartFlowrate[0]);

  if (!prj->func_totalhead || !prj->func_visco) {
    prj->correction.Clear();
    prj->correction.has_calc_error = true;
    return prj;
  }

  prj->correction_x =
      prj->func_visco->get_x(prj->func_totalhead->f(prj->flow_pos));

  GetCorrectionFactors(&prj->correction, prj->correction_x);

  prj->correction.H_selected =
      prj->correction.H_all[(int)prj->parameters.selected_h_curve];

  // Undo conversion
  UndoConversion(prj);

  return prj;
}

CorrectionFactors *Calculator::GetCorrectionFactors(CorrectionFactors *obj,
                                                    const double _x) {
  if (internal::ValidateXEta(_x)) {
    obj->eta = (func_eta_.f(_x) /
                (double)internal::kProperties.kCorrectionScale / (double)10.0) +
               (double)0.2;
  } else {
    obj->eta = (_x < internal::kProperties.kCutoffEta[0]) ? 1.0 : 0.0;
  }

  if (internal::ValidateXQ(_x)) {
    obj->Q =
        (func_q_.f(_x) / (double)internal::kProperties.kCorrectionScale / 10) +
        0.2;
  } else {
    obj->Q = (_x < internal::kProperties.kCutoffQ[0]) ? 1.0 : 0.0;
  }

  if (internal::ValidateXH(_x)) {
    for (int i = 0; i < func_h_.size(); i++) {
      obj->H_all[i] = (func_h_.at(i).f(_x) /
                       (double)internal::kProperties.kCorrectionScale / 10) -
                      0.3;
    }
  } else {
    for (int i = 0; i < func_h_.size(); i++) {
      obj->H_all[i] = (_x < internal::kProperties.kCutoffH[0]) ? 1.0 : 0.0;
    }
  }

  return obj;
}

const float Calculator::FitToScale(const std::map<int, int> &_raw_scale,
                                   const float _input, const int _startpos) {
  float absolute_position = static_cast<float>(_startpos);
  float prev_value = 0;
  bool bfound = false;

  for (const auto &pair : _raw_scale) {
    float value = static_cast<float>(pair.first);

    if (value == _input) {
      absolute_position += static_cast<float>(pair.second);
      bfound = true;
      break;
    } else if (value > _input) {
      float range = value - prev_value;
      float relative_value = _input - prev_value;

      absolute_position +=
          (relative_value / range) * static_cast<float>(pair.second);
      bfound = true;
      break;
    }

    absolute_position += static_cast<float>(pair.second);
    prev_value = value;
  }

  if (bfound)
    return absolute_position;
  else
    return -1.0f;
}

util::LinearFunction *Calculator::CreateLinearF(
    const std::map<int, int> &_raw_scale, const double _m, const float _input,
    const int *_startpos, bool _scale_on_x) {
  float pos;
  pos =
      FitToScale(_raw_scale, _input,
                 (_scale_on_x) ? _startpos[0]
                               : _startpos[1]);  // sometimes setting it to
                                                 // _startpos[0] works better...

  if (pos < 0)
    return nullptr;
  else
    return new util::LinearFunction(_m, (_scale_on_x) ? pos : _startpos[0],
                                    (!_scale_on_x) ? pos : _startpos[1]);
}

void Calculator::ConvertInput(Project *prj) {
  prj->parameters.flowrate_q =
      prj->parameters.flowrate_q *
      internal::kConversionFlowrate.at(prj->parameters.flowrate_unit);
  prj->parameters.total_head_m =
      prj->parameters.total_head_m *
      internal::kConversionTotalHead.at(prj->parameters.total_head_unit);

  if (prj->parameters.viscosity_unit == util::ViscosityUnits::kCentiPoise ||
      prj->parameters.viscosity_unit ==
          util::ViscosityUnits::kMilliPascalSeconds) {
    prj->parameters.viscosity_v =
        prj->parameters.viscosity_v /
        (prj->parameters.density_cp *
         internal::kConversionDensity.at(prj->parameters.density_unit));
  }
}

void Calculator::UndoConversion(Project *prj) {
  prj->parameters.flowrate_q =
      prj->parameters.flowrate_q /
      internal::kConversionFlowrate.at(prj->parameters.flowrate_unit);
  prj->parameters.total_head_m =
      prj->parameters.total_head_m /
      internal::kConversionTotalHead.at(prj->parameters.total_head_unit);

  if (prj->parameters.viscosity_unit == util::ViscosityUnits::kCentiPoise ||
      prj->parameters.viscosity_unit ==
          util::ViscosityUnits::kMilliPascalSeconds) {
    prj->parameters.viscosity_v =
        prj->parameters.viscosity_v *
        (prj->parameters.density_cp *
         internal::kConversionDensity.at(prj->parameters.density_unit));
  }
}

}  // namespace viscocorrect
