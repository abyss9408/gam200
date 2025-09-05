/******************************************************************************/
/*!
\file   CloneSystem.cpp
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/06
\brief
	This source file implements the CloneSystem class defined in CloneSystem.h. 
	The CloneSystem is responsible for cloning entities in a game engine. 
	It subscribes to events and handles the cloning of entities by copying 
	their components.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include "CloneSystem.h"

#include "../Utilities/Components.h"

#ifndef IMGUI_DISABLE
#include "../Editor/Editor.h"
#endif

#include "../Hierarchy/HierarchySystem.h"

namespace Popplio
{
	int CloneSystem::cloneCounter = 0;

	CloneSystem::CloneSystem(Registry& reg, LayerManager& lay)
		: registry(reg), layerManager(lay)
	{
		// Register component cloning functions
		RegisterComponentCloner<ActiveComponent>();
		RegisterComponentCloner<TransformComponent>();
		
		// Special handling for ParentComponent to avoid copying child references directly
		componentCloners[std::type_index(typeid(ParentComponent))] = 
			[this](Entity& original, Entity& clone)
		{
			if (original.HasComponent<ParentComponent>())
			{
				auto& originalParent = original.GetComponent<ParentComponent>();
				
				// Add component with only the inheritance flags - children will be handled separately
				clone.AddComponent<ParentComponent>(Entity(-1),
					originalParent.inheritActive,
					originalParent.inheritTransform);
			}
		};

		RegisterComponentCloner<RigidBodyComponent>();
		RegisterComponentCloner<BoxColliderComponent>();
		RegisterComponentCloner<CircleColliderComponent>();
		RegisterComponentCloner<RenderComponent>();
		RegisterComponentCloner<TextComponent>();
		RegisterComponentCloner<PrefabInstanceComponent>();
		RegisterComponentCloner<AudioComponent>();
		RegisterComponentCloner<AnimationComponent>();
		RegisterComponentCloner<LogicComponent>();
		RegisterComponentCloner<ActiveComponent>();
		RegisterComponentCloner<UIComponent>();
		RegisterComponentCloner<PersistOnLoadComponent>();
        RegisterComponentCloner<ParticleComponent>();
		// Add more component types here as needed
	}

	void CloneSystem::SubscribeToEvents(EventBus& eventBus)
	{
		eventBus.SubscribeToEvent<CloneEntityEvent>(this, &CloneSystem::OnCloneEntity);
	}

	void CloneSystem::CloneComponents(Entity& originalEntity, Entity& newEntity)
	{
		// Clone components
		for (const auto& [typeIndex, clonerFunc] : componentCloners)
		{
			clonerFunc(originalEntity, newEntity);
		}
	}

	int CloneSystem::CloneEntity(CloneEntityEvent& event) // might not work
	{
		// Validate entity ID before proceeding
		if (event.entityId < 0 || !registry.EntityExists(event.entityId))
		{
			Logger::Error("Invalid entity ID for cloning: " + std::to_string(event.entityId));
			return -1;
		}

		int currentClone = cloneCounter++;

		Logger::Debug("CloneEntity | Cloning entity with ID: " + std::to_string(event.entityId));

		// Check if the entity exists in the registry
		if (!registry.EntityExists(event.entityId))
		{
			Logger::Error("CloneEntity | Entity with ID " + std::to_string(event.entityId) + " does not exist in the registry.");
			return -1;
		}
		
		Entity originalEntity = registry.GetEntityById(event.entityId);
		Entity newEntity = registry.CreateEntity(originalEntity.GetId(), event.cloneToId);
		
		CopyEntityInfo(originalEntity, newEntity);

		// First pass: Clone basic components
		CloneComponents(originalEntity, newEntity);

		//registry.ForceRuntimeUpdate(newEntity);
		
		if (originalEntity.HasComponent<ParentComponent>())
		{
			// Root clone
            if (currentClone == 0 && originalEntity.GetComponent<ParentComponent>().parent.Exists())
            {
				registry.GetSystem<HierarchySystem>().AddChild(
					originalEntity.GetComponent<ParentComponent>().parent, newEntity);

     //           originalEntity.GetComponent<ParentComponent>().parent.GetComponent<ParentComponent>().AddChild(newEntity);
     //           newEntity.GetComponent<ParentComponent>().parent = 
					//originalEntity.GetComponent<ParentComponent>().parent;

                newEntity.GetComponent<ParentComponent>().localPosition = originalEntity.GetComponent<ParentComponent>().localPosition;
                newEntity.GetComponent<ParentComponent>().localRotation = originalEntity.GetComponent<ParentComponent>().localRotation;
                newEntity.GetComponent<ParentComponent>().localScale = originalEntity.GetComponent<ParentComponent>().localScale;
            }

			// Second pass: Set up parent-child relationships
			HandleHierarchyCloning(originalEntity, newEntity);

			// TODO FIX - DOESNT WORK
			// Third pass: Preserve script entity references to children
			//if (originalEntity.HasComponent<LogicComponent>() && 
			//	!originalEntity.GetComponent<ParentComponent>().children.empty())
			//{
			//	registry.GetSystem<LogicSystem>().UpdateFromCopy(newEntity, originalEntity);
			//}
		}

#ifndef IMGUI_DISABLE
		registry.GetSystem<Editor>().latestNewEntity = newEntity; // to signal change event
#endif

		if (currentClone == 0)
		{
            HandleLogicCloning(originalEntity, newEntity, originalEntity, newEntity);
			cloneCounter = 0;
		}

		Logger::Info("CloneEntity | Entity cloned successfully. New entity ID: " + std::to_string(newEntity.GetId()));

		//registry.ForceRuntimeUpdate(newEntity);

		return newEntity.GetId();
	}

	void CloneSystem::OnCloneEntity(CloneEntityEvent& event)
	{
		Logger::Debug("OnCloneEntity | Event triggered: ");
		CloneEntity(event);
	}

	void CloneSystem::CopyEntityInfo(Entity originalEntity, Entity newEntity)
	{
		layerManager.AddEntityToLayer(newEntity, layerManager.GetEntityLayer(originalEntity));
		
		// Handle tag - add numbered clone suffix
		if (registry.EntityHasAnyTag(originalEntity))
		{
			std::string originalTag = registry.GetEntityTag(originalEntity);
            if (cloneCounter > 1) // non-root clone
            {
                newEntity.Tag(originalTag);
            }
			else
			{
				// Find next available clone number
				int cloneNum = 1;
				std::string newTag;
				bool tagExists;
				do
				{
					newTag = originalTag + " {" + std::to_string(cloneNum) + "}";
					try
					{
						registry.GetEntityByTag(newTag);
						tagExists = true;
					}
					catch (...)
					{
						tagExists = false;
					}
					cloneNum++;
				} while (tagExists);
				newEntity.Tag(newTag);
			}
		}
        newEntity.Group(registry.GetEntityGroup(originalEntity));
	}

	void CloneSystem::HandleHierarchyCloning(Entity originalEntity, Entity newEntity)
	{
		if (!originalEntity.HasComponent<ParentComponent>())
		{
			return;
		}

		auto& originalParentComp = originalEntity.GetComponent<ParentComponent>();
		auto& newParentComp = newEntity.GetComponent<ParentComponent>();

		// Only handle parent-child relationship if original entity has a valid parent
		if (originalParentComp.parent.Exists())
		{
			// Set parent reference on the new entity
			Entity newParent = registry.GetEntityById(originalParentComp.parent.GetId());
			if (newParent.Exists()) {
				newParentComp.parent = newParent;
				// Update transforms based on parent
				newParentComp.UpdateLocalTransform(
					newEntity.GetComponent<TransformComponent>(),
					newParent.GetComponent<TransformComponent>()
				);
			}
		}

		// Clone all children recursively
		if (!originalParentComp.children.empty())
		{
			for (auto& child : originalParentComp.children)
			{
				CloneEntityEvent childEvent(child.GetId());
				int newChildId = CloneEntity(childEvent);

				if (newChildId != -1)
				{
					Entity newChild = registry.GetEntityById(newChildId);
					newParentComp.AddChild(newChild, newEntity);

					auto& childParentComp = newChild.GetComponent<ParentComponent>();
					childParentComp.parent = newEntity;

					// Update child transforms 
					if (childParentComp.inheritTransform)
					{
						childParentComp.UpdateLocalTransform(
							newChild.GetComponent<TransformComponent>(),
							newEntity.GetComponent<TransformComponent>()
						);
					}
				}
			}
		}
	}

	void CloneSystem::HandleLogicCloning(Entity originalEntity, Entity newEntity, 
		Entity rootOriginalEntity, Entity rootNewEntity)
	{
		// root entity is the eldest entity in the hierarchy being cloned

        if (!originalEntity.HasComponent<LogicComponent>()) return;

        if (originalEntity.HasComponent<ParentComponent>())
        {
			auto& originalParentComp = originalEntity.GetComponent<ParentComponent>();
			auto& newParentComp = newEntity.GetComponent<ParentComponent>();

			if (!originalParentComp.children.empty())
			{
				auto originalIt = originalParentComp.children.begin();
				auto newIt = newParentComp.children.begin();

				while (originalIt != originalParentComp.children.end() && 
					newIt != newParentComp.children.end())
				{
					//Entity originalChild = *originalIt;
					//Entity newChild = *newIt;
					HandleLogicCloning(*originalIt, *newIt, rootOriginalEntity, rootNewEntity);
					++originalIt;
					++newIt;
				}
			}
		}

		registry.GetSystem<LogicSystem>().UpdateFromCopy(newEntity, originalEntity, rootNewEntity, rootOriginalEntity);
	}

	template<>
	void CloneSystem::RegisterComponentCloner<LogicComponent>()
	{
		componentCloners[std::type_index(typeid(LogicComponent))] = [this](Entity& original, Entity& clone)
		{
			if (original.HasComponent<LogicComponent>())
			{
				LogicComponent& clonedComponent = original.GetComponent<LogicComponent>();
				clone.AddComponent<LogicComponent>(clonedComponent);

				clonedComponent.Clone(original.GetComponent<LogicComponent>(), clone.GetId());
			}
		};
	}
	//template<>
	//void CloneSystem::RegisterComponentCloner<ParentComponent>()
	//{
	//	componentCloners[std::type_index(typeid(ParentComponent))] = [this](Entity& original, Entity& clone)
	//	{
	//		if (original.HasComponent<ParentComponent>())
	//		{
	//			ParentComponent& clonedComponent = original.GetComponent<ParentComponent>();
	//			clone.AddComponent<ParentComponent>(clonedComponent);
 //               clone.SetClonedFrom(original.GetId());
	//		}
	//	};
	//}
}