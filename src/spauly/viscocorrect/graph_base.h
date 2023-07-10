#ifndef SPAULY_VISCOCORRECT_GRAPH_H
#define SPAULY_VISCOCORRECT_GRAPH_H

#include <functional>
#include <map>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "spauly/viscocorrect/project.h"
#include "spauly/viscocorrect/util/mathematical_functions.h"
#include "spauly/viscocorrect/util/properties.h"

namespace viscocorrect {
// declared in this file
class GraphContext;
class GraphImplBase;
struct GraphCoordsStorage;         // helper for sharing data with the gui
struct GraphProjectCoordsStorage;  // helper for sharing project data with the
                                   // gui

typedef int GraphValueTag;
typedef float GraphExactValueTag;
typedef float GraphCoordType;

typedef util::LineCoordinates<GraphCoordType> GraphLineCoords;

typedef std::pair<GraphValueTag, GraphLineCoords> GraphLineType;
typedef std::pair<GraphExactValueTag, GraphLineCoords> GraphExactLineType;

typedef std::unordered_map<GraphValueTag, GraphLineCoords> GraphLineStorage;

struct GraphCoordsStorage {
  float scaling_factor;

  GraphLineStorage flowrate_coords;
  GraphLineStorage totalhead_coords;
  GraphLineStorage viscosity_coords;

  std::vector<double> x_coords_q, y_coords_q, x_coords_v, y_coords_v;
  std::vector<std::vector<double>> x_coords_h, y_coords_h;

  void clear();
};

struct GraphProjectCoordsStorage {
  GraphExactLineType prj_flowrate_coords;
  GraphExactLineType prj_totalhead_coords;
  GraphExactLineType prj_viscosity_coords;

  GraphExactLineType prj_calculated_x_coords;
};

class GraphContext {
 public:
  GraphContext();
  virtual ~GraphContext();

  GraphCoordsStorage &GetGraphCoords(const float scale = 1.0f);
  GraphProjectCoordsStorage &GetProjectCoords(const Project &project) {
    return prj_data_;
  }  // only a temporary solution! DO NOT USE. Function is yet to be implemented

  inline const float GetTableWith() {
    return internal::kProperties.kTableWidth;
  }
  inline const float GetTableParameterHeight() {
    return internal::kProperties.kTableParameterHeight;
  }
  inline const float GetTableCorrectionHeight() {
    return internal::kProperties.kTableCorrectionHeight;
  }

  inline const float CalcScalingFactorX(const float &x) {
    return (x / internal::kProperties.kTableWidth);
  }
  inline const float set_scaling_factor(const float scale = 1.0f) {
    return scaling_factor_ = scale;
  }

 private:
  GraphCoordsStorage &InitData();
  void CreateLineCoords(GraphLineStorage *coords,
                        const std::map<int, int> *raw_points,
                        const double pitch, const int *startpos,
                        bool scale_on_x = true, bool use_same_x = false);
  void CreateCorrectionPoints();

 private:
  // interface use
  float scaling_factor_ = 1.0f;

  GraphCoordsStorage data_;
  GraphProjectCoordsStorage prj_data_;
};

class GraphImplBase {
 public:
  GraphImplBase();
  virtual ~GraphImplBase();

  void DisplayProject(std::shared_ptr<Project> project);
  void DisplayProject();
  void HideProject();

  void AddCallbackToPlot(std::shared_ptr<std::function<void()>>, int);
  void RemoveCallbackFromPlot(std::shared_ptr<std::function<void()>>, int);

 protected:
  void RunCallbacksPlot1();
  void RunCallbacksPlot2();

  inline bool get_show_project() { return show_project_; }
  inline bool set_show_project(bool show = true) {
    return show_project_ = show;
  }

  inline GraphContext &get_graph_ctx() { return graph_ctx_; }
  inline const GraphProjectCoordsStorage &get_project_coords() {
    return project_coords_;
  }

 private:
  GraphContext graph_ctx_;

  bool show_project_ = false;
  std::shared_ptr<Project> project_;
  GraphProjectCoordsStorage project_coords_;

  std::vector<std::shared_ptr<std::function<void()>>> callbacks_plot_1_;
  std::vector<std::shared_ptr<std::function<void()>>> callbacks_plot_2_;
};

}  // namespace viscocorrect

#endif  // SPAULY_VISCOCORRECT_GRAPH_H