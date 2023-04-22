#include "LayerStack.h"
#include <algorithm>

namespace Your_Project
{
    LayerStack::~LayerStack()
	{
		for (std::shared_ptr<Layer> layer : mvec_layers)
		{
			layer->OnDetach();
			layer.reset();
		}
	}

	void LayerStack::PushLayer(const std::shared_ptr<Layer> &layer)
	{
		mvec_layers.emplace(mvec_layers.begin() + m_layer_insert_index, layer);
		m_layer_insert_index++;
		layer->OnAttach();
	}

	void LayerStack::PushOverlay(const std::shared_ptr<Layer> &overlay)
	{
		mvec_layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopLayer(std::shared_ptr<Layer> layer)
	{
		auto it = std::find(mvec_layers.begin(), mvec_layers.begin() + m_layer_insert_index, layer);
		if (it != mvec_layers.begin() + m_layer_insert_index)
		{
			layer->OnDetach();
			mvec_layers.erase(it);
			m_layer_insert_index--;
		}
	}

	void LayerStack::PopOverlay(std::shared_ptr<Layer> overlay)
	{
		auto it = std::find(mvec_layers.begin() + m_layer_insert_index, mvec_layers.end(), overlay);
		if (it != mvec_layers.end())
		{
			overlay->OnDetach();
			mvec_layers.erase(it);
		}
	}
}