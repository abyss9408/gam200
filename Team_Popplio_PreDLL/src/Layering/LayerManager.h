/******************************************************************************/
/*!
\file   LayerManager.h
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/05
\brief
	This header file defines the LayerManager class within the Popplio namespace. 
	The LayerManager class is responsible for managing layers and entities within 
	those layers. It provides functionality to add, remove, and manipulate layers 
	and entities, as well as to retrieve information about them.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include <unordered_map>
#include <vector>

namespace Popplio
{
	/*!
  * \brief Represents a layer in the LayerManager.
  */
	struct Layer
	{
		std::string name; //!< The name of the layer.
		int order; //!< The order of the layer.
		bool active; //!< Indicates whether the layer is active or not.

		/*!
        * \brief Constructs a Layer object.
        * \param name The name of the layer.
        * \param order The order of the layer.
        * \param active Indicates whether the layer is active or not. Default is true.
        */
		Layer(const std::string& name, int order, bool active = true) : name(name), order(order), active(active) {}
	};

	/*!
  * \brief Manages layers and entities within those layers.
  */
	class LayerManager
	{
	public:
        /**
        * \brief Adds a layer to the LayerManager.
        * \param name The name of the layer to add.
        * \param active Indicates whether the layer is active or not. Default is true.
        */
        void AddLayer(const std::string& name, bool active = true);

        /**
        * \brief Removes a layer from the LayerManager.
        * \param name The name of the layer to remove.
        */
        void RemoveLayer(const std::string& name);

        /**
        * \brief Sets the order of a layer in the LayerManager.
        * \param name The name of the layer to set the order for.
        * \param order The new order of the layer.
        */
        void SetLayerOrder(const std::string& name, int order);

        /**
        * \brief Sets the active state of a layer in the LayerManager.
        * \param name The name of the layer to set the active state for.
        * \param active Indicates whether the layer should be active or not.
        */
        void SetLayerActive(const std::string& name, bool active);

        /**
        * \brief Checks if a layer is active in the LayerManager.
        * \param name The name of the layer to check.
        * \return True if the layer is active, false otherwise.
        */
        bool IsLayerActive(const std::string& name) const;

        /**
        * \brief Gets the order of a layer in the LayerManager.
        * \param name The name of the layer to get the order for.
        * \return The order of the layer.
        */
        int GetLayerOrder(const std::string& name) const;

        /**
        * \brief Adds an entity to a specific layer.
        * \param entity The entity to add.
        * \param layerName The name of the layer to add the entity to.
        */
        void AddEntityToLayer(Entity entity, const std::string& layerName);

        /**
        * \brief Removes an entity from its current layer.
        * \param entity The entity to remove.
        */
        void RemoveEntityFromCurrentLayer(Entity entity);

        /**
        * \brief Gets the layer of an entity.
        * \param entity The entity to get the layer for.
        * \return The name of the layer that the entity belongs to.
        */
        std::string GetEntityLayer(Entity entity) const;

        /**
        * \brief Gets all entities in a specific layer.
        * \param layerName The name of the layer to get the entities from.
        * \return A vector containing all entities in the specified layer.
        */
        std::vector<Entity> GetEntitiesInLayer(const std::string& layerName) const;

        /**
        * \brief Gets all entities sorted by layer.
        * \return A vector containing all entities sorted by layer.
        */
        std::vector<Entity> GetAllEntitiesSortedByLayer() const;

        /**
        * \brief Gets all layers sorted by order.
        * \return A vector containing all layers sorted by order.
        */
        std::vector<Layer> GetSortedLayers() const;

        /**
        * \brief Gets the number of layers.
        * \return The number of layers.
        */
        size_t GetLayerCount() const;

        /**
        * \brief Clears all layers and entities.
        * \param persistentEntities A list of entities that should not be cleared. Default is an empty list.
        */
        void Clear(std::set<Entity> persistentEntities = std::set<Entity>());

        /**
        * \brief Gets information about the layers.
        * \return A string containing information about the layers.
        */
        std::string GetLayerInfo() const;

		void SetLayerInteraction(const std::string& layer1, const std::string& layer2, bool interacts);

		bool ShouldLayersInteract(const std::string& layer1, const std::string& layer2) const;

        void InitializeLayerInteractions(const std::string& layerName);

		void ClearLayerInteractions(const std::string& layerName);
        
        const std::unordered_map<std::string, std::unordered_map<std::string, bool>>& GetLayerInteractions() const;
	
        void LoadInteractions(const std::unordered_map<std::string, std::unordered_map<std::string, bool>>& interactions);
    private:
		std::unordered_map<std::string, Layer> layers; //!< The map of layer names to Layer objects.
		std::unordered_map<int, std::string> entityLayerMap; //!< The map of entity IDs to layer names.
		std::unordered_map<std::string, std::set<Entity>> layerEntities; //!< The map of layer names to sets of entities.
		std::unordered_map<std::string, std::unordered_map<std::string, bool>> layerInteractions; //!< The map of layer names to maps of layer interaction toggles.

		// Helper methods
		void ReorderLayers(); //!< Ensures layer orders remain sequential.
	};
}