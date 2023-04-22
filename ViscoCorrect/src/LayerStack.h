#pragma once

#include "Layer.h"

#include <vector>
#include <memory>

namespace Your_Project
{
	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		template <typename T>
		void PushLayer()
		{
			static_assert(std::is_base_of<Your_Project::Layer, T>::value, "Pushed type is not subclass of Layer!");
			mvec_layers.emplace_back(std::make_shared<T>());
			mvec_layers.back()->OnAttach();
		}

		void PushLayer(const std::shared_ptr<Layer> &layer);
		void PushOverlay(const std::shared_ptr<Layer> &overlay);
		void PopLayer(std::shared_ptr<Layer> layer);
		void PopOverlay(std::shared_ptr<Layer> overlay);

		std::vector<std::shared_ptr<Layer>>::iterator begin() { return mvec_layers.begin(); }
		std::vector<std::shared_ptr<Layer>>::iterator end() { return mvec_layers.end(); }
		std::vector<std::shared_ptr<Layer>>::reverse_iterator rbegin() { return mvec_layers.rbegin(); }
		std::vector<std::shared_ptr<Layer>>::reverse_iterator rend() { return mvec_layers.rend(); }

		std::vector<std::shared_ptr<Layer>>::const_iterator begin() const { return mvec_layers.begin(); }
		std::vector<std::shared_ptr<Layer>>::const_iterator end() const { return mvec_layers.end(); }
		std::vector<std::shared_ptr<Layer>>::const_reverse_iterator rbegin() const { return mvec_layers.rbegin(); }
		std::vector<std::shared_ptr<Layer>>::const_reverse_iterator rend() const { return mvec_layers.rend(); }

	private:
		std::vector<std::shared_ptr<Layer>> mvec_layers;
		unsigned int m_layer_insert_index = 0;
	};
}