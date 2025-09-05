/******************************************************************************/
/*!
\file   EntityReordering.cpp
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the source file for the Editor's entities reordering logic

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include "Editor.h"
#include "../Hierarchy/HierarchySystem.h"

namespace Popplio
{
	void Editor::HandleEntityReordering(Entity& dragEntity, Entity& targetEntity, bool insertBefore)
	{
        // Find the parents of both entities
        Entity draggedParent(-1);
        Entity targetParent(-1);

        // Check if dragged entity has a parent
        if (dragEntity.HasComponent<ParentComponent>())
        {
            auto& draggedParentComp = dragEntity.GetComponent<ParentComponent>();
            if (draggedParentComp.parent.GetId() != -1)
            {
                draggedParent = draggedParentComp.parent;
            }
        }

        // Check if target entity has a parent
        if (targetEntity.HasComponent<ParentComponent>())
        {
            auto& targetParentComp = targetEntity.GetComponent<ParentComponent>();
            if (targetParentComp.parent.GetId() != -1)
            {
                targetParent = targetParentComp.parent;
            }
        }

        // If both entities have the same parent, reorder within that parent
        if (draggedParent.GetId() != -1 && draggedParent.GetId() == targetParent.GetId())
        {
            auto& parentComp = draggedParent.GetComponent<ParentComponent>();

            // Find positions of both entities in parent's children list
            auto draggedIt = std::find(parentComp.children.begin(), parentComp.children.end(), draggedEntity);
            auto targetIt = std::find(parentComp.children.begin(), parentComp.children.end(), targetEntity);

            if (draggedIt != parentComp.children.end() && targetIt != parentComp.children.end())
            {
                // Store the entity being moved
                Entity draggedCopy = *draggedIt;

                // Remove the dragged entity from its current position
                parentComp.children.erase(draggedIt);

                // Recalculate target position (since we removed an element)
                targetIt = std::find(parentComp.children.begin(), parentComp.children.end(), targetEntity);

                // Insert at the new position
                if (insertBefore)
                {
                    parentComp.children.insert(targetIt, draggedCopy);
                }
                else
                {
                    parentComp.children.insert(targetIt + 1, draggedCopy);
                }

                // Emit change event for undo/redo
                eventBus.EmitEvent<SceneChangeEvent<std::any>>(draggedParent, draggedParent, draggedParent,
                    ChangeAction::MODIFY_ENTITY, "Child reordering");

                Logger::Info("Reordered entity " + std::to_string(dragEntity.GetId()) +
                    (insertBefore ? " before " : " after ") +
                    std::to_string(targetEntity.GetId()));
            }
        }
        // If both are root entities, implement root-level reordering
        else if (draggedParent.GetId() == -1 && targetParent.GetId() == -1)
        {
            // Access the entities vector from the registry
            auto& allEntities = reg.GetAllEntitiesMutable();

            // Find positions of both entities in the entities vector
            auto draggedIt = std::find(allEntities.begin(), allEntities.end(), dragEntity);
            auto targetIt = std::find(allEntities.begin(), allEntities.end(), targetEntity);

            if (draggedIt != allEntities.end() && targetIt != allEntities.end())
            {
                // Store the entity being moved
                Entity draggedCopy = *draggedIt;

                // Remove the dragged entity from its current position
                allEntities.erase(draggedIt);

                // Recalculate target position (since we removed an element)
                targetIt = std::find(allEntities.begin(), allEntities.end(), targetEntity);

                // Calculate insert position
                auto insertPos = insertBefore ? targetIt : targetIt + 1;

                // Insert at the new position
                allEntities.insert(insertPos, draggedCopy);

                // Emit change event for undo/redo
                eventBus.EmitEvent<SceneChangeEvent<std::any>>(dragEntity, dragEntity, dragEntity,
                    ChangeAction::MODIFY_ENTITY, "Root entity reordering");

                Logger::Info("Reordered root entity " + std::to_string(dragEntity.GetId()) +
                    (insertBefore ? " before " : " after ") +
                    std::to_string(targetEntity.GetId()));
            }
        }
        // If moving between different parents, make the dragged entity a sibling of the target
        else
        {
            // Remove from current parent if it exists
            if (draggedParent.GetId() != -1)
            {
                reg.GetSystem<HierarchySystem>().RemoveChild(draggedParent, dragEntity);
            }

            // Determine the new parent and position
            if (targetParent.GetId() != -1)
            {
                // Add to target's parent
                reg.GetSystem<HierarchySystem>().AddChild(targetParent, dragEntity);

                // Get updated parent component after adding the child
                auto& parentComp = targetParent.GetComponent<ParentComponent>();

                // Find the positions in the children array
                auto draggedIt = std::find(parentComp.children.begin(), parentComp.children.end(), draggedEntity);
                auto targetIt = std::find(parentComp.children.begin(), parentComp.children.end(), targetEntity);

                if (draggedIt != parentComp.children.end() && targetIt != parentComp.children.end())
                {
                    // Remove from current position
                    Entity draggedCopy = *draggedIt;
                    parentComp.children.erase(draggedIt);

                    // Find target position again
                    targetIt = std::find(parentComp.children.begin(), parentComp.children.end(), targetEntity);

                    // Insert at appropriate position
                    if (insertBefore)
                    {
                        parentComp.children.insert(targetIt, draggedCopy);
                    }
                    else
                    {
                        parentComp.children.insert(targetIt + 1, draggedCopy);
                    }

                    Logger::Info("Entity " + std::to_string(dragEntity.GetId()) +
                        " moved as sibling " + (insertBefore ? "before" : "after") +
                        " entity " + std::to_string(targetEntity.GetId()));

                    // Emit change event
                    eventBus.EmitEvent<SceneChangeEvent<std::any>>(targetParent, targetParent, targetParent,
                        ChangeAction::MODIFY_ENTITY, "Child reordering");
                }
            }
        }

        // Reset reordering mode after handling
        isReorderingMode = false;
	}
}