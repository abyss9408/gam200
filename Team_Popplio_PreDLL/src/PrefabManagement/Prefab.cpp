/******************************************************************************/
/*!
\file   Prefab.cpp
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/14
\brief
    This source file defines the Prefab class within the Popplio namespace. 
    A Prefab is a template for creating entities with predefined components in a game engine. 
    The class provides methods to instantiate these prefabs into a registry, validate their state, 
    and manage their components.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include "Prefab.h"
#include "PrefabManager.h"
#include "../Hierarchy/HierarchySystem.h"

namespace Popplio
{
	Prefab::Prefab(const std::string& name)
		: name(name), isValid(true)
	{
        // Add default ActiveComponent
        AddComponent<ActiveComponent>();
		// Add default TransformComponent
		AddComponent<TransformComponent>();
	}

	[[nodiscard]]
	Entity Prefab::InstantiateBase(Registry& registry) const
	{
        if (!IsValid())
        {
            Logger::Error("Cannot instantiate invalid prefab: " + name, LogDestination::IMGUI_CONSOLE);
            return Entity(-1); // Return invalid entity
        }

        // Create a brand new entity with a unique ID (don't specify ID)
        Entity entity = registry.CreateEntity();

        try
        {
            // Apply tag with instance counter
            static std::unordered_map<std::string, int> instanceCounters;
            int instanceNum = ++instanceCounters[name];
            entity.Tag(name + "_Instance_" + std::to_string(instanceNum));

            // Apply all component factories to create components on the new entity
            for (const auto& [type, factory] : componentFactories)
            {
                factory(entity);
            }

            // Add the PrefabInstanceComponent to track this instance
            entity.AddComponent<PrefabInstanceComponent>(name);

            Logger::Info("Successfully instantiated prefab base: " + name +
                " as Entity " + std::to_string(entity.GetId()), LogDestination::IMGUI_CONSOLE);
        }
        catch (const std::exception& e)
        {
            Logger::Error("Failed to instantiate prefab: " + name +
                " Error: " + e.what(), LogDestination::IMGUI_CONSOLE);
            registry.KillEntity(entity);
            return Entity(-1);
        }

        return entity;
	}

    [[nodiscard]]
    Entity Prefab::Instantiate(Registry& registry, PrefabManager& prefabManager) const
	{
        // Create the root entity for this prefab
        Entity entity = InstantiateBase(registry);
        if (entity.GetId() == -1)
        {
            return entity;
        }

        registry.Update(); // Process any pending entity changes

        // Instantiate child prefabs recursively with completely new entity IDs
        InstantiateChildPrefabs(entity, childPrefabs, registry, prefabManager);

        registry.Update(); // Make sure all entity changes are processed
        return entity;
	}

	void Prefab::InstantiateChildPrefabs(Entity& parentEntity, const std::vector<PrefabRef>& childRefs, Registry& registry, PrefabManager& prefabManager) const
	{
        auto& parentTransform = parentEntity.GetComponent<TransformComponent>();

        for (const auto& childRef : childRefs)
        {
            std::string prefabName = std::filesystem::path(childRef.prefabPath).stem().string();
            auto prefabOpt = prefabManager.GetPrefab(prefabName);

            if (!prefabOpt)
            {
                Logger::Warning("Could not find prefab: " + prefabName);
                continue;
            }

            // Create a completely new entity instance with a unique ID
            Entity childEntity = prefabOpt->get().InstantiateBase(registry);
            if (childEntity.GetId() == -1)
            {
                Logger::Warning("Failed to instantiate child prefab: " + prefabName);
                continue;
            }

            registry.Update(); // Process pending entity changes

            // Setup parent-child relationship
            // Make sure parent has ParentComponent
            if (!parentEntity.HasComponent<ParentComponent>())
            {
                parentEntity.AddComponent<ParentComponent>();
            }

            // Make sure child has proper ParentComponent
            if (!childEntity.HasComponent<ParentComponent>())
            {
                childEntity.AddComponent<ParentComponent>(parentEntity);
            }
            else
            {
                auto& childParentComp = childEntity.GetComponent<ParentComponent>();
                childParentComp.parent = parentEntity;

                // Clear any previous children the component might have had
                childParentComp.children.clear();
            }

            // Add child to parent's children list
            parentEntity.GetComponent<ParentComponent>().AddChild(childEntity, parentEntity);

            registry.Update(); // Process entity changes

            // Update child's transform based on prefab reference
            auto& childTransform = childEntity.GetComponent<TransformComponent>();
            auto& childParentComp = childEntity.GetComponent<ParentComponent>();

            // Set local transform values from the prefab reference
            childParentComp.localPosition = childRef.localPosition;
            childParentComp.localScale = childRef.localScale;
            childParentComp.localRotation = childRef.localRotation;

            // Update world transform based on parent
            childParentComp.UpdateWorldTransform(childTransform, parentTransform);

            // Log successful child instantiation
            Logger::Debug("Instantiated child prefab " + prefabName +
                " as Entity " + std::to_string(childEntity.GetId()) +
                " with parent " + std::to_string(parentEntity.GetId()));

            // Recursively instantiate this child's nested prefabs if any
            if (prefabOpt)
            {
                const auto& nestedChildRefs = prefabOpt->get().GetChildPrefabs();
                if (!nestedChildRefs.empty())
                {
                    InstantiateChildPrefabs(childEntity, nestedChildRefs, registry, prefabManager);
                }
            }
        }
	}

	[[nodiscard]]
	const std::string& Prefab::GetName() const noexcept
	{
		return name;
	}

    [[nodiscard]]
    bool Prefab::IsValid() const noexcept
    {
        return isValid && HasComponent<TransformComponent>() && HasComponent<ActiveComponent>();
    }

    void Prefab::ValidatePrefabState()
    {
        // Check for required components
        if (!HasComponent<TransformComponent>())
        {
            Logger::Warning("Prefab missing required TransformComponent: " + name);
            isValid = false;
            return;
        }

        if (!HasComponent<ActiveComponent>())
        {
            Logger::Warning("Prefab missing required ActiveComponent: " + name);
            isValid = false;
            return;
        }

        // Additional validation as needed
        isValid = true;
    }

	void Prefab::AddChildPrefab(const PrefabRef& prefabRef)
	{
		childPrefabs.push_back(prefabRef);
	}

	void Prefab::RemoveChildPrefab(const std::string& prefabPath)
	{
        childPrefabs.erase(
			std::remove_if(
                childPrefabs.begin(), 
                childPrefabs.end(),
				[&prefabPath](const PrefabRef& prefabRef) { return prefabRef.prefabPath == prefabPath; }
            ),
			childPrefabs.end()
        );
	}

    [[nodiscard]] 
    const std::vector<PrefabRef>& Prefab::GetChildPrefabs() const
    {
        return childPrefabs;
    }
}