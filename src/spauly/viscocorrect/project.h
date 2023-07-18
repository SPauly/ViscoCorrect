#ifndef SPAULY_VISCOCORRECT_PROJECT_H
#define SPAULY_VISCOCORRECT_PROJECT_H

#include <functional>
#include <string>

#include "spauly/viscocorrect/util/input_units.h"
#include "spauly/viscocorrect/util/mathematical_functions.h"

namespace viscocorrect {
enum CurvesOrderH : int;  // defined in properties.h

// defined in this file
struct CalcParameters;
struct CorrectionFactors;
struct Project;

struct CalcParameters {
  float flowrate_q = 0;
  float total_head_m = 0;
  float viscosity_v = 0;
  CurvesOrderH selected_h_curve;
  util::ViscoUnits visco_unit = util::ViscoUnits::kMilliPascalSeconds;
  util::FlowrateUnits flowrate_unit = util::FlowrateUnits::kLitersPerMinute;
};

struct CorrectionFactors {
  double c_q = 0.0;
  double c_n = 0.0;
  double c_h_selected = 0.0;
  double c_h_all[4] = {0.0, 0.0, 0.0, 0.0};
};

struct Project {
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
  ~Project() {
    if (func_totalhead) delete func_totalhead;
    if (func_visco) delete func_visco;
    if (render_params) delete render_params;
    if (render_correction) delete render_correction;
  }
};
}  // namespace viscocorrect

#endif  // SPAULY_VISCOCORRECT_PROJECT_H