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
  float density_cp = 0;

  CurvesOrderH selected_h_curve;

  util::ViscosityUnits viscosity_unit =
      util::ViscosityUnits::kSquareMillimetersPerSecond;
  util::FlowrateUnits flowrate_unit = util::FlowrateUnits::kCubicMetersPerHour;
  util::TotalHeadUnits total_head_unit = util::TotalHeadUnits::kMeters;
  util::DensityUnits density_unit = util::DensityUnits::kGramPerLiter;

  bool has_input_error = false;
};

struct CorrectionFactors {
  double Q = 0.0;
  double eta = 0.0;
  double H_selected = 0.0;
  double H_all[4] = {0.0, 0.0, 0.0, 0.0};
  bool has_calc_error = false;

  void Clear() {
    Q = 0.0;
    eta = 0.0;
    H_selected = 0.0;
    for (int i = 0; i < 4; i++) H_all[i] = 0.0;
  }
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