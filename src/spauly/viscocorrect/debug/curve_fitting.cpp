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

#include "spauly/viscocorrect/debug/curve_fitting.h"

#include "spauly/viscocorrect/debug/debug_tools.h"

namespace viscocorrect {
namespace debug {
// FullDataCurve
FullDataCurve::FullDataCurve(std::map<int, int> &_data, bool _bpoly,
                             int _params) {
  // populate x and y Data
  for (const auto &pair : _data) {
    xData.push_back(pair.first);
    yData.push_back(pair.second);
  }
  compressed_data.dataSize = xData.size();
  compressed_data.xData = xData.data();
  compressed_data.yData = yData.data();

  compressed_data.b_poly = _bpoly;
  parameters.resize(_params);
}

CurveFitting::CurveFitting(std::shared_ptr<EventCallbackType> callback)
    : event_callback_(callback) {
  // request graph instance
  (*event_callback_)(
      std::make_unique<util::Event<std::shared_ptr<GraphImplBase>>>(
          util::EventType::kGetGraphInstance, &graph_instance_));

  curves.push_back(FullDataCurve(raw_points.at(0), true, 6));
  curves.push_back(FullDataCurve(raw_points.at(1), true, 6));
  curves.push_back(FullDataCurve(raw_points.at(2), true, 6));
  curves.push_back(FullDataCurve(raw_points.at(3), true, 6));
  curves.push_back(FullDataCurve(raw_points.at(4), true, 6));
  curves.push_back(FullDataCurve(raw_points.at(5), true, 6));

  PlotRender_func = std::make_shared<std::function<void()>>(
      std::bind(&CurveFitting::RenderInPlot, this));
}

CurveFitting::~CurveFitting() {}

void CurveFitting::Render() {
  ImGui::Begin("Curve Fitting");
  if (ImGui::Button("Enable Render") && !b_renderplot) {
    graph_instance_->AddCallbackToPlot(PlotRender_func, 1);
    b_renderplot = true;
  }
  ImGui::SameLine();
  if (ImGui::Button("Disable Render") && b_renderplot) {
    graph_instance_->RemoveCallbackFromPlot(PlotRender_func, 1);
    b_renderplot = false;
  }

  ImGui::RadioButton("1Polynom", &use_poly_, 1);
  ImGui::SameLine();
  ImGui::RadioButton("1Logistical", &use_poly_, 0);
  ImGui::InputInt("1Parameters", &num_params_);

  if (ImGui::Button("Refresh Curve")) {
    if (b_function) {
      curves.clear();
      curves.push_back(
          FullDataCurve(raw_points.at(0), (bool)use_poly_, num_params_));
      curves.push_back(
          FullDataCurve(raw_points.at(1), (bool)use_poly_, num_params_));
      curves.push_back(
          FullDataCurve(raw_points.at(2), (bool)use_poly_, num_params_));
      curves.push_back(
          FullDataCurve(raw_points.at(3), (bool)use_poly_, num_params_));
      curves.push_back(
          FullDataCurve(raw_points.at(4), (bool)use_poly_, num_params_));
      curves.push_back(
          FullDataCurve(raw_points.at(5), (bool)use_poly_, num_params_));
    }
    for (int i = 0; i < curves.size(); i++) {
      FitCurve(curves.at(i));
    }
    b_function = true;
  }

  ImGui::End();
}

void CurveFitting::RenderInPlot() {
  for (int i = 0; i < curves.size(); i++) {
    ImPlot::PlotScatter("raw_points", curves.at(i).xData.data(),
                        curves.at(i).yData.data(), curves.at(i).xData.size());
    ImPlot::PlotLine("fitted", curves.at(i).fittedX.data(),
                     curves.at(i).fittedY.data(), curves.at(i).fittedX.size());
  }
}

// Curve Fitting

int CurveFitting::ResidualFunction(const gsl_vector *x, void *params,
                                   gsl_vector *f) {
  CompressedCurveData *_data = static_cast<CompressedCurveData *>(params);

  for (size_t i = 0; i < _data->dataSize; i++) {
    double _yModel = _data->ModelFunction(_data->xData[i], x);
    gsl_vector_set(f, i, _data->yData[i] - _yModel);
  }

  return GSL_SUCCESS;
}

void CurveFitting::FitCurve(FullDataCurve &_data) {
  size_t _datasize = _data.xData.size();

  gsl_vector *xVec = gsl_vector_alloc(_datasize);
  gsl_vector *yVec = gsl_vector_alloc(_datasize);

  for (size_t i = 0; i < _datasize; i++) {
    gsl_vector_set(xVec, i, _data.xData.at(i));
    gsl_vector_set(yVec, i, _data.yData.at(i));
  }

  gsl_vector *_initial_params = gsl_vector_alloc(_data.parameters.size());

  switch (_data.compressed_data.b_poly) {
    case true:
      for (int i = 0; i < _initial_params->size; i++) {
        gsl_vector_set(_initial_params, i, 1);
      }
      break;

    case false:
      gsl_vector_set(_initial_params, 0, 174.0);
      gsl_vector_set(_initial_params, 1, 0.1);
      gsl_vector_set(_initial_params, 2, 300);
      break;

    default:
      break;
  }

  gsl_multifit_nlinear_parameters fittingParams =
      gsl_multifit_nlinear_default_parameters();
  gsl_multifit_nlinear_workspace *workspace =
      gsl_multifit_nlinear_alloc(gsl_multifit_nlinear_trust, &fittingParams,
                                 _datasize, _initial_params->size);

  gsl_multifit_nlinear_fdf fittingFunction;
  fittingFunction.f = &ResidualFunction;
  fittingFunction.df = nullptr;   // Set to nullptr for numerical derivatives
  fittingFunction.fvv = nullptr;  // Set to nullptr for numerical derivatives
  fittingFunction.n = _datasize;
  fittingFunction.p = _initial_params->size;
  fittingFunction.params = static_cast<void *>(&_data.compressed_data);

  int info;

  gsl_multifit_nlinear_init(_initial_params, &fittingFunction, workspace);

  if (gsl_multifit_nlinear_driver(100, 1e-6, 1e-6, 1e-6, nullptr, nullptr,
                                  &info, workspace) != GSL_SUCCESS) {
    gsl_vector_free(xVec);
    gsl_vector_free(yVec);
    gsl_vector_free(_initial_params);
    gsl_multifit_nlinear_free(workspace);
    return;
  }

  // Retrieve the optimized parameter values
  gsl_vector *solution = gsl_multifit_nlinear_position(workspace);
  for (int i = 0; i < _initial_params->size; i++) {
    _data.parameters.at(i) = gsl_vector_get(solution, i);
  }

  // Produce fitted data
  for (int i = _data.xData.at(0); i < _data.xData.back(); i++) {
    _data.fittedX.push_back(i);
    _data.fittedY.push_back(
        _data.compressed_data.ModelFunction((double)i, _data.parameters));
  }

  gsl_vector_free(xVec);
  gsl_vector_free(yVec);
  gsl_vector_free(_initial_params);
  gsl_multifit_nlinear_free(workspace);
}
}  // namespace debug

}  // namespace viscocorrect
