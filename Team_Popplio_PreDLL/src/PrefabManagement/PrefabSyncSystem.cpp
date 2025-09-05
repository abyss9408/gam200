/******************************************************************************/
/*!
\file   PrefabSyncSystem.cpp
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/06
\brief
	This source file implements the PrefabSyncSystem class within the Popplio 
	namespace. The PrefabSyncSystem class is responsible for synchronizing 
	entities with their corresponding prefabs. It handles events related to 
	prefab modifications and deletions, ensuring that any changes to prefabs 
	are propagated to their instances in the game world.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include "PrefabSyncSystem.h"
#include "../Utilities/Components.h"

namespace Popplio
{
	PrefabSyncSystem::PrefabSyncSystem(Registry& reg, PrefabManager& prefabMgr)
		: reg(reg), prefabMgr(prefabMgr)
	{
		RequireComponent<PrefabInstanceComponent>();
	}

	void PrefabSyncSystem::SubscribeToEvents(EventBus& eventBus)
	{
		eventBus.SubscribeToEvent<PrefabEvent>(this, &PrefabSyncSystem::OnPrefabEvent);
		eventBus.SubscribeToEvent<SceneLoadedEvent>(this, &PrefabSyncSystem::OnSceneLoaded);
	}

	void PrefabSyncSystem::Update()
	{
		// Process any pending sync requests
		while (!syncQueue.empty())
		{
			ProcessSyncRequest(syncQueue.front());
			syncQueue.pop();
		}
	}

	void PrefabSyncSystem::OnSceneLoaded(SceneLoadedEvent& event)
	{
		Logger::Info("Scene loaded, synchronizing prefab instances: " + event.scenePath);

		for (auto entity : GetSystemEntities())
		{
			auto& instanceComponent = entity.GetComponent<PrefabInstanceComponent>();

			// Create a sync request for this prefab
			syncQueue.push(SyncRequest(
				instanceComponent.prefabName,
				PrefabEventType::Modified
			));
		}

		// Process all sync requests immediately
		Update();
	}

	void PrefabSyncSystem::OnPrefabEvent(PrefabEvent& event)
	{
		syncQueue.push(SyncRequest(event.prefabName, event.eventType, event.modifiedComponentType));
	}

	void PrefabSyncSystem::ProcessSyncRequest(const SyncRequest& request)
	{
		switch (request.eventType)
		{
		case PrefabEventType::Modified:
		{
			auto prefabOpt = prefabMgr.GetPrefab(request.prefabName);
			if (!prefabOpt)
			{
				Logger::Error("Cannot sync instances: Prefab not found: " + request.prefabName);
				return;
			}
			const Prefab& prefab = prefabOpt->get();

			for (auto entity : GetSystemEntities())
			{
				auto& instanceComponent = entity.GetComponent<PrefabInstanceComponent>();
				if (instanceComponent.prefabName == request.prefabName &&
					instanceComponent.syncWithPrefab)
				{
					if (request.modifiedComponentType)
					{
						// Sync only the modified component
						SyncSpecificComponent(entity, prefab, *request.modifiedComponentType);
					}
					else
					{
						// If no specific component was modified, sync all components
						SyncEntityWithPrefab(entity, prefab);
					}
				}
			}
			break;
		}
		case PrefabEventType::Deleted:
			HandlePrefabDeletion(request.prefabName);
			break;
		}
	}

	void PrefabSyncSystem::SyncSpecificComponent(Entity& entity, const Prefab& prefab, const std::type_index& componentType)
	{
		if (componentType == typeid(ActiveComponent))
		{
			SyncComponent<ActiveComponent>(entity, prefab);
		}
		else if (componentType == typeid(RenderComponent))
		{
			SyncComponent<RenderComponent>(entity, prefab);
		}
		else if (componentType == typeid(RigidBodyComponent))
		{
			SyncComponent<RigidBodyComponent>(entity, prefab);
		}
		else if (componentType == typeid(BoxColliderComponent))
		{
			SyncComponent<BoxColliderComponent>(entity, prefab);
		}
		else if (componentType == typeid(TextComponent))
		{
			SyncComponent<TextComponent>(entity, prefab);
		}
		else if (componentType == typeid(LogicComponent))
		{
			SyncComponent<LogicComponent>(entity, prefab);
		}
		else if (componentType == typeid(AudioComponent))
		{
			SyncComponent<AudioComponent>(entity, prefab);
		}
		else if (componentType == typeid(AnimationComponent))
		{
			SyncComponent<AnimationComponent>(entity, prefab);
		}
        else if (componentType == typeid(PersistOnLoadComponent))
        {
            SyncComponent<PersistOnLoadComponent>(entity, prefab);
        }
        else if (componentType == typeid(ParticleComponent))
        {
            SyncComponent<ParticleComponent>(entity, prefab);
        }
		else if (componentType == typeid(UIComponent))
		{
			SyncComponent<UIComponent>(entity, prefab);
		}
		else
		{
			Logger::Warning("Cannot sync component of type: " + std::string(componentType.name()),
				LogDestination::IMGUI_CONSOLE);
		}
	}

	void PrefabSyncSystem::SyncEntityWithPrefab(Entity& entity, const Prefab& prefab)
	{
		// Sync all components
		SyncComponent<ActiveComponent>(entity, prefab);
		SyncComponent<RenderComponent>(entity, prefab);
		SyncComponent<RigidBodyComponent>(entity, prefab);
		SyncComponent<BoxColliderComponent>(entity, prefab);
		SyncComponent<TextComponent>(entity, prefab);
		SyncComponent<LogicComponent>(entity, prefab);
		SyncComponent<AnimationComponent>(entity, prefab);
		SyncComponent<ActiveComponent>(entity, prefab);
		SyncComponent<AudioComponent>(entity, prefab);
        SyncComponent<PersistOnLoadComponent>(entity, prefab);
		SyncComponent<UIComponent>(entity, prefab);
        SyncComponent<ParticleComponent>(entity, prefab);
	}

	void PrefabSyncSystem::HandlePrefabDeletion(const std::string& prefabName)
	{
		int instancesDetached = 0;
		for (auto entity : GetSystemEntities())
		{
			auto& instanceComponent = entity.GetComponent<PrefabInstanceComponent>();
			if (instanceComponent.prefabName == prefabName)
			{
				entity.RemoveComponent<PrefabInstanceComponent>();
				++instancesDetached;
			}
		}

		if (instancesDetached > 0)
		{
			Logger::Warning("Detached " + std::to_string(instancesDetached) +
				" instances due to prefab deletion: " + prefabName,
				LogDestination::IMGUI_CONSOLE);
		}
	}
}