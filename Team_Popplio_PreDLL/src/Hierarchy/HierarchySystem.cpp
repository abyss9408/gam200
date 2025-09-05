/******************************************************************************/
/*!
\file   HierarchySystem.cpp
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/06
\brief
This file contains the implementation of the HierarchySystem class, which is 
responsible for managing the hierarchical relationships between entities in the 
game engine. It ensures that parent-child relationships are maintained and that 
transformations are correctly propagated through the hierarchy.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include "HierarchySystem.h"

namespace Popplio
{
	HierarchySystem::HierarchySystem()
	{
		RequireComponent<ParentComponent>();
		RequireComponent<TransformComponent>();
		RequireComponent<ActiveComponent>();
	}

	void HierarchySystem::Update()
	{
		for (auto entity : GetSystemEntities())
        {
            if (!entity.Exists()) continue;
            if (!entity.HasComponent<TransformComponent>() || !entity.HasComponent<ActiveComponent>()) 
                continue;
            if (!entity.HasComponent<ParentComponent>()) continue;
			auto& parentComp = entity.GetComponent<ParentComponent>();

			// Skip if this is a child being processed (to avoid double processing)
			if (parentComp.parent.GetId() != -1)
            {
				continue;
			}

            entity.GetComponent<ParentComponent>().SetLocalToWorld(entity.GetComponent<TransformComponent>());

			// Process entire hierarchy starting from root
			UpdateHierarchy(entity);
		}
	}

	void HierarchySystem::UpdateHierarchy(Entity& entity)
	{
        if (!entity.HasComponent<ParentComponent>())
            return;

        auto& parentComp = entity.GetComponent<ParentComponent>();
        auto& parentTransform = entity.GetComponent<TransformComponent>();

        // Update children with deltas
        for (auto child : parentComp.children)
        {
            if (!child.HasComponent<TransformComponent>() ||
                !child.HasComponent<ActiveComponent>())
                continue;

            //if (!child.GetComponent<ParentComponent>().IsModified())
            //    child.GetComponent<ParentComponent>().UpdateLocalTransform(
            //        child.GetComponent<TransformComponent>(), parentTransform);

            // Handle active state inheritance
            //if (parentComp.inheritActive)
            if (child.GetComponent<ParentComponent>().inheritActive)
            {
                auto& parentActive = entity.GetComponent<ActiveComponent>();
                auto& childActive = child.GetComponent<ActiveComponent>();
                childActive.isActive = parentActive.isActive;
            }

            // Handle transform inheritance
            //if (parentComp.inheritTransform)
            auto& childTransform = child.GetComponent<TransformComponent>();
            auto& childParentComp = child.GetComponent<ParentComponent>();

            if (child.GetComponent<ParentComponent>().inheritTransform)
            {
                //if (child.GetComponent<ParentComponent>().IsModified()) 
                //    childParentComp.SyncLocalToWorld(childTransform, parentTransform);
                //else
                
                //if (childParentComp.localModified)
                //{
                //    //childParentComp.UpdateLocalTransform(childTransform, parentTransform);
                //    childParentComp.localModified = false;
                //}

                //if (childParentComp.WorldIsModified(childTransform)) 
                //    childParentComp.SyncLocalToWorld(childTransform, parentTransform);

                // Update world transform based on local transform and parent's transform
                childParentComp.UpdateWorldTransform(childTransform, parentTransform);
            }
            else childParentComp.SyncLocalToWorld(childTransform, parentTransform);

            // Recursively update children
            UpdateHierarchy(child);
        }
	}

    void HierarchySystem::InitializeParentTransform(Entity parent)
    {
        if (parent.HasComponent<TransformComponent>())
        {
            //for (auto& [id, transform] : lastParentTransforms)
            //{
            //    if (id == parent.GetId()) return;
            //}
            lastParentTransforms[parent.GetId()] = parent.GetComponent<TransformComponent>();
        }
    }

    void HierarchySystem::StopParentTransform(Entity parent)
    {
        if (parent.HasComponent<TransformComponent>())
        {
            for (auto& [id, transform] : lastParentTransforms)
            {
                if (id == parent.GetId())
                {
                    lastParentTransforms.erase(parent.GetId());
                    break;
                }
            }
        }
    }

    std::vector<Entity> HierarchySystem::GetChildRecursive(Entity const parent)
    {
        if (!parent.HasComponent<ParentComponent>()) return std::vector<Entity>();
        std::vector<Entity>temp{};
        return GetChildRecursion(parent, temp);
    }

    Entity HierarchySystem::GetChildRecursive(Entity const parent, Entity child)
    {
        //if (child == parent) return child; // untested
        if (!parent.HasComponent<ParentComponent>()) return Entity(-1);

        std::vector<Entity> children = GetChildRecursive(parent);

        for (auto& c : children)
        {
            if (c.GetId() == child.GetId()) return c;
        }
        return Entity(-1);
    }

    void HierarchySystem::AddChild(Entity parent, Entity child)
    {
        if (!parent.HasComponent<ParentComponent>())
        {
            //return;
            parent.AddComponent<ParentComponent>();
        }

        if (!child.HasComponent<ParentComponent>())
        {
            child.AddComponent<ParentComponent>();
        }

        // Get the current world transform of the child before changing hierarchy
        auto& childTransform = child.GetComponent<TransformComponent>();
        //auto& parentTransform = parent.GetComponent<TransformComponent>();

        Entity childExistingParent = child.GetComponent<Popplio::ParentComponent>().parent;
        if (childExistingParent.Exists())
            childExistingParent.GetComponent<Popplio::ParentComponent>().RemoveChild(child);

        // Add child to parent's list
        parent.GetComponent<ParentComponent>().AddChild(child, parent);

        // Add or update ParentComponent on child
        //if (!child.HasComponent<ParentComponent>())
        //{
        //    child.AddComponent<ParentComponent>(parent);
        //}
        //else
        //{
        //    child.GetComponent<ParentComponent>().parent = parent;
        //}

        child.GetComponent<ParentComponent>().parent = parent;

        //child.GetComponent<ParentComponent>().InitLocalTransform(childTransform);

        // Calculate and set local transform for the child
        //child.GetComponent<ParentComponent>().UpdateLocalTransform(childTransform, parentTransform);
        child.GetComponent<ParentComponent>().SetLocalToWorld(childTransform);

        InitializeParentTransform(parent);
    }

    void HierarchySystem::RemoveChild(Entity parent, Entity child)
    {
        if (!parent.HasComponent<ParentComponent>())
            return;

        // Before removing, calculate the final world transform
        auto& childTransform = child.GetComponent<TransformComponent>();
        //auto& parentTransform = parent.GetComponent<TransformComponent>();
        auto& childParentComp = child.GetComponent<ParentComponent>();

        // Store the current world transform
        PopplioMath::Vec2f worldPos = childTransform.position;
        PopplioMath::Vec2f worldScale = childTransform.scale;
        double worldRotation = childTransform.rotation;

        // Remove child from parent
        parent.GetComponent<ParentComponent>().RemoveChild(child);

        // Set the child's transform to its world transform
        childTransform.position = worldPos;
        childTransform.scale = worldScale;
        childTransform.rotation = worldRotation;

        // Reset the child's local transform
        childParentComp.localPosition = { 0.0f, 0.0f };
        childParentComp.localScale = { 1.0f, 1.0f };
        childParentComp.localRotation = 0.0;
    }

    std::vector<Entity>& HierarchySystem::GetChildRecursion(Entity const parent, std::vector<Entity>& ents)
    {
        auto& parentComp = parent.GetComponent<ParentComponent>();
        for (auto c : parentComp.children)
        {
            ents.push_back(c);
            if (c.GetComponent<ParentComponent>().children.size() > 0)
            {
                ents = GetChildRecursion(c, ents);
            }
        }
        return ents;
    }

    //void HierarchySystem::SetChildParentIdRecursive(Entity e, Entity o)
    //{
    //    auto child = e.GetComponent<ParentComponent>().children.begin();
    //    auto originalChild = o.GetComponent<ParentComponent>().children.begin();

    //    while (child != e.GetComponent<ParentComponent>().children.end() &&
    //        originalChild != o.GetComponent<ParentComponent>().children.end())
    //    {
    //        child->SetClonedFrom(originalChild->GetId());

    //        child.GetComponent<ParentComponent>().parent = e;

    //        if (!GetChildRecursive(e, child).Exists()) e.GetComponent<ParentComponent>().AddChild(child);

    //        if (child.GetComponent<ParentComponent>().children.size() > 0)
    //        {
    //            SetChildParentIdRecursive(child, originalChild);
    //        }
    //    }

    //    //for (size_t i{}; i < e.GetComponent<ParentComponent>().children.size(); ++i)
    //    //{
    //    //    auto& child = e.GetComponent<ParentComponent>().children[i];
    //    //    auto& originalChild = o.GetComponent<ParentComponent>().children[i];

    //    //    child.SetClonedFrom(originalChild.GetId());

    //    //    child.GetComponent<ParentComponent>().parent = e;

    //    //    if (!GetChildRecursive(e, child).Exists()) e.GetComponent<ParentComponent>().AddChild(child);

    //    //    if (child.GetComponent<ParentComponent>().children.size() > 0)
    //    //    {
    //    //        SetChildParentIdRecursive(child, originalChild);
    //    //    }
    //    //}
    //}

    //void HierarchySystem::CloneSecondPass(Entity newEnt, Entity original)
    //{
    //    SetChildParentIdRecursive(newEnt, original);
    //}
}