#include "spauly/viscocorrect/graph_base.h"

#include <stdexcept>

#include "spauly/viscocorrect/util/properties.h"

namespace viscocorrect {
void GraphCoordsStorage::clear() {
  flowrate_coords.clear();
  totalhead_coords.clear();
  viscosity_coords.clear();

  x_coords_v.clear();
  y_coords_v.clear();
  x_coords_q.clear();
  y_coords_q.clear();
  x_coords_h.clear();
  y_coords_h.clear();
}

//-----------------------------
// GraphContext
GraphContext::GraphContext() { InitData(); }

GraphContext::~GraphContext() {}

GraphCoordsStorage &GraphContext::GetGraphCoords(const float scale) {
  if (scale == scaling_factor_) return data_;

  set_scaling_factor(scale);
  data_.clear();

  return InitData();
}

GraphCoordsStorage &GraphContext::InitData() {
  data_.scaling_factor = scaling_factor_;

  // create data to share with GraphImplBase
  CreateLineCoords(&data_.flowrate_coords,
                   &internal::kProperties.kFlowrateScale, 0.0f,
                   internal::kProperties.kStartFlowrate, true, true);
  CreateLineCoords(&data_.totalhead_coords,
                   &internal::kProperties.kTotalHeadScale,
                   internal::kProperties.kPitchTotalH,
                   internal::kProperties.kStartTotalH, false, false);
  CreateLineCoords(&data_.viscosity_coords, &internal::kProperties.kViscoScale,
                   internal::kProperties.kPitchVisco,
                   internal::kProperties.kStartVisco, true, false);

  CreateCorrectionPoints();

  return data_;
}

void GraphContext::CreateLineCoords(GraphLineStorage *coords,
                                    const std::map<int, int> *raw_points,
                                    const double pitch, const int *startpos,
                                    bool scale_on_x, bool use_same_x) {
  bool set_start = false;

  if (!startpos) {
    startpos = new int[2]{0, 0};
    set_start = true;
  }

  GraphCoordType it_total_dist =
      static_cast<GraphCoordType>((scale_on_x) ? startpos[0] : startpos[1]);

  if (!use_same_x) {
    for (const auto &pair : *raw_points) {
      it_total_dist +=
          static_cast<GraphCoordType>(pair.second) * scaling_factor_;

      util::LinearFunction temp_func(
          pitch, (scale_on_x) ? it_total_dist : startpos[0],
          (scale_on_x) ? startpos[1] : it_total_dist);

      coords->insert(std::make_pair(
          pair.first, temp_func.CreateLineCoordinates<GraphCoordType>(
                          0.0f, static_cast<GraphCoordType>(
                                    internal::kProperties.kTableWidth) *
                                    scaling_factor_)));
    }
  } else {
    for (const auto &pair : *raw_points) {
      it_total_dist +=
          static_cast<GraphCoordType>(pair.second) * scaling_factor_;

      GraphLineCoords temp({.0f, .0f}, {1.0f, 1.0f});

      temp.get_array_notation().x_coords[0] = it_total_dist;
      temp.get_array_notation().x_coords[1] = it_total_dist;
      temp.get_array_notation().y_coords[0] = .0f;
      temp.get_array_notation().y_coords[1] =
          static_cast<GraphCoordType>(internal::kProperties.kTableWidth);

      coords->insert(std::make_pair(pair.first, temp));
    }
  }

  if (set_start) {
    try {
      delete[] startpos;
    } catch (const std::exception &e) {
      return;
    }
  }
}

void GraphContext::CreateCorrectionPoints() {
  // have to implement the scaling_factor here!
  util::PolynomialFunction correct_v{internal::kProperties.kCoefficientsN};
  util::PolynomialFunction correct_q{internal::kProperties.kCoefficientsQ};
  std::vector<util::LogisticalFunction> correct_h;
  for (const std::vector<double> &params :
       internal::kProperties.kCoefficientsH) {
    correct_h.push_back(params);
  }

  for (int i = internal::kProperties.kCutoffN[0];
       i < internal::kProperties.kCutoffN[1]; i++) {
    data_.x_coords_v.push_back(i);
    data_.y_coords_v.push_back(correct_v.f((double)i));
  }

  for (int i = internal::kProperties.kCutoffQ[0];
       i < internal::kProperties.kCutoffQ[1]; i++) {
    data_.x_coords_q.push_back(i);
    data_.y_coords_q.push_back(correct_q.f((double)i));
  }

  for (int i = 0; i < correct_h.size(); i++) {
    std::vector<double> temp_x;
    std::vector<double> temp_y;
    for (int ii = internal::kProperties.kCutoffH[0];
         ii < internal::kProperties.kCutoffH[1]; ii++) {
      temp_x.push_back(ii);
      temp_y.push_back(correct_h.at(i).f((double)ii));
    }
    data_.x_coords_h.push_back(temp_x);
    data_.y_coords_h.push_back(temp_y);
    temp_x.clear();
    temp_y.clear();
  }
}

//-------------------------------
// GraphImplBase

GraphImplBase::GraphImplBase() {}

GraphImplBase::~GraphImplBase() {}

void GraphImplBase::DisplayProject(std::shared_ptr<Project> project) {
  project_ = project;
  project_coords_ = graph_ctx_.GetProjectCoords(*project_);
  set_show_project(true);
}

void GraphImplBase::DisplayProject() {
  if (!project_) {
    set_show_project(false);
    return;
  } else {
    DisplayProject(project_);
  }
}

void GraphImplBase::HideProject() { set_show_project(false); }

void GraphImplBase::AddCallbackToPlot(
    std::shared_ptr<std::function<void()>> func, int plot_num) {
  switch (plot_num) {
    case 0:
      callbacks_plot_1_.push_back(func);
      break;

    case 1:
      callbacks_plot_2_.push_back(func);
      break;

    default:
      break;
  }
}

void GraphImplBase::RemoveCallbackFromPlot(
    std::shared_ptr<std::function<void()>> func, int plot_num) {
  switch (plot_num) {
    case 0: {
      auto it =
          std::find(callbacks_plot_1_.begin(), callbacks_plot_1_.end(), func);
      try {
        callbacks_plot_1_.erase(it);
      } catch (std::runtime_error &) {
        break;
      }
      break;
    }
    case 1: {
      auto it =
          std::find(callbacks_plot_2_.begin(), callbacks_plot_2_.end(), func);
      try {
        callbacks_plot_2_.erase(it);
      } catch (std::runtime_error &) {
        break;
      }
      break;
    }
    default:
      break;
  }
}

void GraphImplBase::RunCallbacksPlot1() {
  // Call the callbacks
  for (const auto &func : callbacks_plot_1_) {
    (*func)();
  }
}

void GraphImplBase::RunCallbacksPlot2() {
  // Call the callbacks
  for (const auto &func : callbacks_plot_2_) {
    (*func)();
  }
}
}  // namespace viscocorrect
