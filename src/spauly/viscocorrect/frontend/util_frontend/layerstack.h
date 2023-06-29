#ifndef SPAULY_VISCOCORRECT_SRC_UTIL_LAYERSTACK_H
#define SPAULY_VISCOCORRECT_SRC_UTIL_LAYERSTACK_H

#include "spauly/viscocorrect/frontend/util_frontend/layer.h"

#include <vector>
#include <memory>

namespace viscocorrect
{
	namespace frontend
	{
		namespace util_frontend
		{
			class LayerStack
			{
			public:
				LayerStack() = default;
				~LayerStack();

				template <typename T>
				void PushLayer()
				{
					static_assert(std::is_base_of<Layer, T>::value, "Pushed type is not subclass of Layer!");
					layers_.emplace_back(std::make_shared<T>());
					layers_.back()->OnAttach();
				}

				void PushLayer(const std::shared_ptr<Layer> &layer);
				void PushOverlay(const std::shared_ptr<Layer> &overlay);
				void PopLayer(std::shared_ptr<Layer> layer);
				void PopOverlay(std::shared_ptr<Layer> overlay);

				void clear();

				std::vector<std::shared_ptr<Layer>>::iterator begin() { return layers_.begin(); }
				std::vector<std::shared_ptr<Layer>>::iterator end() { return layers_.end(); }
				std::vector<std::shared_ptr<Layer>>::reverse_iterator rbegin() { return layers_.rbegin(); }
				std::vector<std::shared_ptr<Layer>>::reverse_iterator rend() { return layers_.rend(); }

				std::vector<std::shared_ptr<Layer>>::const_iterator begin() const { return layers_.begin(); }
				std::vector<std::shared_ptr<Layer>>::const_iterator end() const { return layers_.end(); }
				std::vector<std::shared_ptr<Layer>>::const_reverse_iterator rbegin() const { return layers_.rbegin(); }
				std::vector<std::shared_ptr<Layer>>::const_reverse_iterator rend() const { return layers_.rend(); }

			private:
				std::vector<std::shared_ptr<Layer>> layers_;
				unsigned int layer_insert_index_ = 0;
			};
		} // namespace util_frontend
	}	  // namespace frontend
} // namespace viscocorrect

#endif // SPAULY_VISCOCORRECT_SRC_UTIL_LAYERSTACK_H