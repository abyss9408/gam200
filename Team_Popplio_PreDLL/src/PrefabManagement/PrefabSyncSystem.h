/******************************************************************************/
/*!
\file   PrefabSyncSystem.h
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/06
\brief
	This header file defines the PrefabSyncSystem class within the Popplio namespace. 
	The PrefabSyncSystem class is responsible for synchronizing entities with their 
	corresponding prefabs. It listens for prefab events and processes synchronization 
	requests to update entities based on changes in the prefabs.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include "../ECS/ECS.h"
#include "PrefabInstanceComponent.h"
#include "PrefabManager.h"
#include "PrefabEvent.h"
#include "../Logging/Logger.h"
#include "../EventBus/EventBus.h"
#include "../Serialization/SceneLoadedEvent.h"
#include <queue>

namespace Popplio
{
	/**
	* The PrefabSyncSystem class is responsible for synchronizing entities with their corresponding prefabs.
	* It listens for prefab events and processes sync requests to update entities based on the changes in the prefabs.
	*/
	class PrefabSyncSystem : public System
	{
	public:
		/**
		* Constructs a PrefabSyncSystem object.
		* @param reg The registry used to access entities and components.
		* @param prefabMgr The prefab manager used to retrieve prefabs.
		*/
		PrefabSyncSystem(Registry& reg, PrefabManager& prefabMgr);

		/**
		* Subscribes to prefab events from the event bus.
		* @param eventBus The event bus to subscribe to.
		*/
		void SubscribeToEvents(EventBus& eventBus);

		/**
		* Updates the PrefabSyncSystem.
		* This method is called every frame to process sync requests.
		*/
		void Update();

		void OnSceneLoaded(SceneLoadedEvent& event);

	private:
		Registry& reg; /**< The registry used to access entities and components. */
		PrefabManager& prefabMgr; /**< The prefab manager used to retrieve prefabs. */

		/**
		* Represents a sync request to update an entity based on changes in a prefab.
		*/
		struct SyncRequest
		{
			std::string prefabName; /**< The name of the prefab. */
			PrefabEventType eventType; /**< The type of prefab event. */
			std::optional<std::type_index> modifiedComponentType; /**< The type of the modified component, if applicable. */

			/**
			* Constructs a SyncRequest object.
			* @param name The name of the prefab.
			* @param type The type of prefab event.
			* @param componentType The type of the modified component, if applicable.
			*/
			SyncRequest(const std::string& name, PrefabEventType type, std::optional<std::type_index> componentType = std::nullopt)
				: prefabName(name), eventType(type), modifiedComponentType(componentType) {}
		};

		std::queue<SyncRequest> syncQueue; /**< The queue of sync requests to be processed. */

		/**
		* Handles the PrefabEvent.
		* @param event The PrefabEvent to handle.
		*/
		void OnPrefabEvent(PrefabEvent& event);

		/**
		* Processes a sync request.
		* @param request The sync request to process.
		*/
		void ProcessSyncRequest(const SyncRequest& request);

		/**
		* Syncs a specific component of an entity with the corresponding component in a prefab.
		* @param entity The entity to sync.
		* @param prefab The prefab to sync with.
		* @param componentType The type of the component to sync.
		*/
		void SyncSpecificComponent(Entity& entity, const Prefab& prefab, const std::type_index& componentType);

		/**
		* Syncs an entity with its corresponding prefab.
		* @param entity The entity to sync.
		* @param prefab The prefab to sync with.
		*/
		void SyncEntityWithPrefab(Entity& entity, const Prefab& prefab);

		/**
		* Syncs a component of an entity with the corresponding component in a prefab.
		* @tparam TComponent The type of the component to sync.
		* @param entity The entity to sync.
		* @param prefab The prefab to sync with.
		*/
		template <typename TComponent>
		void SyncComponent(Entity& entity, const Prefab& prefab);

		/**
		* Handles the deletion of a prefab.
		* @param prefabName The name of the prefab to handle deletion for.
		*/
		void HandlePrefabDeletion(const std::string& prefabName);
	};

	template <typename TComponent>
	void PrefabSyncSystem::SyncComponent(Entity& entity, const Prefab& prefab)
	{
		if (prefab.HasComponent<TComponent>())
		{
			const auto& prefabComponent = prefab.GetComponent<TComponent>();
			if (entity.HasComponent<TComponent>())
			{
				// Update existing component
				entity.GetComponent<TComponent>() = prefabComponent;
			}
			else
			{
				// Add component if it doesn't exist
				entity.AddComponent<TComponent>(prefabComponent);
			}
		}
		else if (entity.HasComponent<TComponent>())
		{
			// Remove component if prefab doesn't have it
			entity.RemoveComponent<TComponent>();
		}
	}
}