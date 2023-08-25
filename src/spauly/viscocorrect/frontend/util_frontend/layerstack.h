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

#ifndef SPAULY_VISCOCORRECT_UTIL_LAYERSTACK_H
#define SPAULY_VISCOCORRECT_UTIL_LAYERSTACK_H

#include <memory>
#include <vector>

#include "spauly/viscocorrect/frontend/util_frontend/layer.h"

namespace viscocorrect {
namespace frontend {
namespace util_frontend {
class LayerStack {
 public:
  LayerStack() = default;
  ~LayerStack();

  template <typename T>
  void PushLayer() {
    static_assert(std::is_base_of<Layer, T>::value,
                  "Pushed type is not subclass of Layer!");
    layers_.emplace(layers_.begin() + layer_insert_index_,
                    std::make_shared<T>());
    layers_.at(layers_.begin() + layer_insert_index_)->OnAttach();
    layer_insert_index_++;
  }

  void PushLayer(const std::shared_ptr<Layer> &layer);
  void PushOverlay(const std::shared_ptr<Layer> &overlay);
  void PopLayer(std::shared_ptr<Layer> layer);
  void PopOverlay(std::shared_ptr<Layer> overlay);
  void HideLayer(std::shared_ptr<Layer> layer);
  void ShowLayer(std::shared_ptr<Layer> layer);

  void clear();

  std::vector<std::shared_ptr<Layer>>::iterator begin() {
    return layers_.begin();
  }
  std::vector<std::shared_ptr<Layer>>::iterator end() { return layers_.end(); }
  std::vector<std::shared_ptr<Layer>>::reverse_iterator rbegin() {
    return layers_.rbegin();
  }
  std::vector<std::shared_ptr<Layer>>::reverse_iterator rend() {
    return layers_.rend();
  }

  std::vector<std::shared_ptr<Layer>>::const_iterator begin() const {
    return layers_.begin();
  }
  std::vector<std::shared_ptr<Layer>>::const_iterator end() const {
    return layers_.end();
  }
  std::vector<std::shared_ptr<Layer>>::const_reverse_iterator rbegin() const {
    return layers_.rbegin();
  }
  std::vector<std::shared_ptr<Layer>>::const_reverse_iterator rend() const {
    return layers_.rend();
  }

 private:
  std::vector<std::shared_ptr<Layer>> layers_;
  std::vector<std::shared_ptr<Layer>> hidden_layers_;
  unsigned int layer_insert_index_ = 0;
};
}  // namespace util_frontend
}  // namespace frontend
}  // namespace viscocorrect

#endif  // SPAULY_VISCOCORRECT_UTIL_LAYERSTACK_H