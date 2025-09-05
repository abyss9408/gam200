/******************************************************************************/
/*!
\file   LayerManager.cpp
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/05
\brief
	This source file defines the LayerManager class within the Popplio namespace. 
	The LayerManager class is responsible for managing layers and entities within 
	those layers. It provides functionality to add, remove, and manipulate layers 
	and entities, as well as to retrieve information about them.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>

#include "LayerManager.h"

#include <algorithm>

namespace Popplio
{
	void LayerManager::AddLayer(const std::string& name, bool active)
	{
		if (name.empty())
		{
			Logger::Error("Cannot add layer with empty name", LogDestination::IMGUI_CONSOLE);
			return;
		}

		if (layers.find(name) != layers.end())
		{
			Logger::Warning("Layer '" + name + "' already exists", LogDestination::IMGUI_CONSOLE);
			return;
		}

		// Calculate the order of the new layer
		int order = 0;
		if (!layers.empty())
		{
			order = std::max_element(layers.begin(), layers.end(),
				[](const auto& a, const auto& b) 
			{ 
				return a.second.order < b.second.order; 
			})->second.order + 1;
		}

		layers.emplace(name, Layer(name, order, active));
		layerEntities[name] = std::set<Entity>();
		InitializeLayerInteractions(name);

		Logger::Debug("Added layer '" + name + "' with order " + std::to_string(order), LogDestination::IMGUI_CONSOLE);
	}

	void LayerManager::RemoveLayer(const std::string& name)
	{
		auto layerIt = layers.find(name);
		if (layerIt == layers.end())
		{
			Logger::Warning("Cannot remove non-existent layer: " + name, LogDestination::IMGUI_CONSOLE);
			return;
		}

		// Remove all entities from this layer
		auto& entities = layerEntities[name];
		for (const Entity& entity : entities)
		{
			entityLayerMap.erase(entity.GetId());
		}

		// Remove layer entries
		layers.erase(layerIt);
		layerEntities.erase(name);

		// Remove layer interactions
		ClearLayerInteractions(name);

		// Reorder remaining layers to maintain consecutive ordering
		ReorderLayers();

		Logger::Debug("Removed layer: " + name, LogDestination::IMGUI_CONSOLE);
	}

	void LayerManager::AddEntityToLayer(Entity entity, const std::string& layerName)
	{
		auto layerIt = layers.find(layerName);
		if (layerIt == layers.end())
		{
			Logger::Warning("Cannot add entity to non-existent layer: " + layerName,
				LogDestination::IMGUI_CONSOLE);
			return;
		}

		// Remove from previous layer if exists
		RemoveEntityFromCurrentLayer(entity);

		// Add to new layer
		entityLayerMap[entity.GetId()] = layerName;
		layerEntities[layerName].insert(entity);

		Logger::Debug("Added Entity " + std::to_string(entity.GetId()) +
			" to layer '" + layerName + "'", LogDestination::IMGUI_CONSOLE);
	}

	void LayerManager::RemoveEntityFromCurrentLayer(Entity entity)
	{
		auto it = entityLayerMap.find(entity.GetId());
		if (it != entityLayerMap.end())
		{
			std::string currentLayer = it->second;
			layerEntities[currentLayer].erase(entity);
			entityLayerMap.erase(it);

			Logger::Debug("Removed Entity " + std::to_string(entity.GetId()) +
				" from layer '" + currentLayer + "'", LogDestination::IMGUI_CONSOLE);
		}
	}

	std::string LayerManager::GetEntityLayer(Entity entity) const
	{
		auto it = entityLayerMap.find(entity.GetId());
		return it != entityLayerMap.end() ? it->second : "";
	}

	std::vector<Entity> LayerManager::GetEntitiesInLayer(const std::string& layerName) const
	{
		auto it = layerEntities.find(layerName);
		if (it == layerEntities.end())
		{
			return {};
		}
		return std::vector<Entity>(it->second.begin(), it->second.end());
	}

	std::vector<Entity> LayerManager::GetAllEntitiesSortedByLayer() const
	{
		std::vector<Entity> sortedEntities;
		sortedEntities.reserve(entityLayerMap.size());  // Reserve space for efficiency

		// Get sorted layers
		auto sortedLayers = GetSortedLayers();

		// Add entities from each active layer
		for (const auto& layer : sortedLayers)
		{
			if (layer.active)
			{
				auto entitiesIt = layerEntities.find(layer.name);
				if (entitiesIt != layerEntities.end())
				{
					sortedEntities.insert(sortedEntities.end(),
						entitiesIt->second.begin(), entitiesIt->second.end());
				}
			}
		}

		return sortedEntities;
	}

	void LayerManager::SetLayerOrder(const std::string& name, int order)
	{
		auto it = layers.find(name);
		if (it == layers.end())
		{
			Logger::Warning("Cannot set order for non-existent layer: " + name,
				LogDestination::IMGUI_CONSOLE);
			return;
		}

		if (order < 0)
		{
			Logger::Warning("Layer order cannot be negative: " + name,
				LogDestination::IMGUI_CONSOLE);
			return;
		}

		int oldOrder = it->second.order;
		it->second.order = order;

		// Adjust other layers' orders if needed
		for (auto& [layerName, layer] : layers)
		{
			if (layerName != name)
			{
				if (oldOrder < order && layer.order <= order && layer.order > oldOrder)
				{
					layer.order--;
				}
				else if (oldOrder > order && layer.order >= order && layer.order < oldOrder)
				{
					layer.order++;
				}
			}
		}

		Logger::Debug("Set layer '" + name + "' order to " + std::to_string(order),
			LogDestination::IMGUI_CONSOLE);
	}

	void LayerManager::SetLayerActive(const std::string& name, bool active)
	{
		auto it = layers.find(name);
		if (it == layers.end())
		{
			Logger::Warning("Cannot set active state for non-existent layer: " + name,
				LogDestination::IMGUI_CONSOLE);
			return;
		}

		it->second.active = active;
		Logger::Debug("Set layer '" + name + "' active state to " +
			std::to_string(active), LogDestination::IMGUI_CONSOLE);
	}

	bool LayerManager::IsLayerActive(const std::string& name) const
	{
		auto it = layers.find(name);
		return it != layers.end() && it->second.active;
	}

	int LayerManager::GetLayerOrder(const std::string& name) const
	{
		auto it = layers.find(name);
		return it != layers.end() ? it->second.order : -1;
	}

	std::vector<Layer> LayerManager::GetSortedLayers() const
	{
		std::vector<Layer> sortedLayers;
		sortedLayers.reserve(layers.size());

		for (const auto& [name, layer] : layers)
		{
			sortedLayers.push_back(layer);
		}

		std::sort(sortedLayers.begin(), sortedLayers.end(),
			[](const Layer& a, const Layer& b) {
			return a.order < b.order;
		});

		return sortedLayers;
	}

	void LayerManager::ReorderLayers()
	{
		std::vector<Layer*> layerPtrs;
		layerPtrs.reserve(layers.size());

		// Collect pointers to all layers
		for (auto& [name, layer] : layers)
		{
			layerPtrs.push_back(&layer);
		}

		// Sort by current order
		std::sort(layerPtrs.begin(), layerPtrs.end(),
			[](const Layer* a, const Layer* b) {
			return a->order < b->order;
		});

		// Reassign orders sequentially
		for (size_t i = 0; i < layerPtrs.size(); ++i)
		{
			layerPtrs[i]->order = static_cast<int>(i);
		}

		Logger::Debug("Reordered layers", LogDestination::IMGUI_CONSOLE);
	}

	void LayerManager::Clear(std::set<Entity> persistentEntities)
	{
		// Store list of unique entities across all layers
		std::set<Entity> affectedEntities;

		for (const auto& layerPair : layerEntities)
		{
			for (const auto& entity : layerPair.second)
			{
				affectedEntities.insert(entity);
			}
		}

		// Clear all internal data structures
		layers.clear();
		entityLayerMap.clear();
		layerEntities.clear();
		layerInteractions.clear();

  //      if (persistentEntities.empty()) // Clear all layers and entities
		//{
		//	// Clear all internal data structures
		//	layers.clear();
		//	entityLayerMap.clear();
		//	layerEntities.clear();
		//	layerInteractions.clear();
		//}
  //      else // Clear all layers and entities except persistent ones
		//{
		//	std::vector<std::string> layersToRemove{};
		//	for (auto& layer : layers)
		//	{
		//		auto& entities = layerEntities[layer.first];

		//		bool layerIsPersistent = false;
		//		for (auto it = entities.begin(); it != entities.end();)
		//		{
		//			if (std::find(persistentEntities.begin(), persistentEntities.end(), *it) != persistentEntities.end())
		//			{
  //                      layerIsPersistent = true;
		//			}
		//			else entityLayerMap.erase(it->GetId());
		//		}

		//		if (!layerIsPersistent)
		//		{
  //                  layersToRemove.push_back(layer.first);
		//		}

		//		for (auto it = entities.begin(); it != entities.end();)
		//		{
		//			if (std::find(persistentEntities.begin(), persistentEntities.end(), *it) == persistentEntities.end())
		//			{
		//				it = entities.erase(it);
		//			}
		//			else
		//			{
		//				++it;
		//			}
		//		}

		//		for (auto& [layerName, e] : layerEntities)
		//		{
  //                  if (e.empty())
  //                  {
  //                      layerEntities.erase(layerName);
  //                  }
		//		}
		//	}

  //          for (auto& layerName : layersToRemove)
  //          {
  //              layers.erase(layerName);

  //              layerInteractions.erase(layerName);
  //          }

		//	std::vector<std::string> layerNames{};

  //          for (auto& [layerName, layer] : layers)
  //          {
  //              layerNames.push_back(layerName);
  //          }

		//	for (auto& [layerName, interactions] : layerInteractions)
		//	{
		//		if (std::find(layerNames.begin(), layerNames.end(), layerName) == layerNames.end()) interactions.erase(layerName);
		//	}
		//}

		Logger::Debug("Cleared " + std::to_string(affectedEntities.size() - persistentEntities.size()) +
			" entities from layer assignments", LogDestination::IMGUI_CONSOLE);
	}

	size_t LayerManager::GetLayerCount() const
	{
		return layers.size();
	}

	std::string LayerManager::GetLayerInfo() const
	{
		std::stringstream ss;
		ss << "Layer Information:\n";

		auto sortedLayers = GetSortedLayers();
		for (const auto& layer : sortedLayers)
		{
			auto entitiesIt = layerEntities.find(layer.name);
			size_t entityCount = entitiesIt != layerEntities.end() ? entitiesIt->second.size() : 0;

			ss << "Layer: " << layer.name
				<< " (Order: " << layer.order
				<< ", Active: " << (layer.active ? "Yes" : "No")
				<< ", Entities: " << entityCount << ")\n";
		}

		return ss.str();
	}

	void LayerManager::SetLayerInteraction(const std::string& layer1, const std::string& layer2, bool interacts)
	{
		if (layers.find(layer1) == layers.end() || layers.find(layer2) == layers.end())
		{
			Logger::Warning("Cannot set interaction for non-existent layer");
			return;
		}
		layerInteractions[layer1][layer2] = interacts;
		layerInteractions[layer2][layer1] = interacts;
		Logger::Debug("Set interaction between layers '" + layer1 + "' and '" + layer2 +
			"' to " + (interacts ? "true" : "false"), LogDestination::IMGUI_CONSOLE);
	}

	bool LayerManager::ShouldLayersInteract(const std::string& layer1, const std::string& layer2) const
	{
		auto it1 = layerInteractions.find(layer1);
		if (it1 == layerInteractions.end())
		{
			// Default to true if not set
			return true;
		}

		auto it2 = it1->second.find(layer2);
		if (it2 == it1->second.end())
		{
			// Default to true if not set
			return true;
		}
		return it2->second;
	}

	void LayerManager::InitializeLayerInteractions(const std::string& layerName)
	{
		if (layers.find(layerName) == layers.end())
		{
			Logger::Warning("Cannot initialize interactions for non-existent layer");
			return;
		}
		// Initialize interactions with all other layers
		for (const auto& [name, layer] : layers)
		{
			if (name != layerName)
			{
				layerInteractions[layerName][name] = true;
				layerInteractions[name][layerName] = true;
			}
		}
		Logger::Debug("Initialized interactions for layer '" + layerName + "'", LogDestination::IMGUI_CONSOLE);
	}

	void LayerManager::ClearLayerInteractions(const std::string& layerName)
	{
		if (layers.find(layerName) == layers.end())
		{
			Logger::Warning("Cannot clear interactions for non-existent layer");
			return;
		}
		layerInteractions.erase(layerName);
		for (auto& [name, interactions] : layerInteractions)
		{
			interactions.erase(layerName);
		}
		Logger::Debug("Cleared interactions for layer '" + layerName + "'", LogDestination::IMGUI_CONSOLE);
	}

	const std::unordered_map<std::string, std::unordered_map<std::string, bool>>& LayerManager::GetLayerInteractions() const
	{
		return layerInteractions;
	}

	void LayerManager::LoadInteractions(const std::unordered_map<std::string, std::unordered_map<std::string, bool>>& interactions)
	{
		layerInteractions = interactions;
		Logger::Debug("Loaded layer interactions", LogDestination::IMGUI_CONSOLE);
	}
}