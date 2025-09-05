/******************************************************************************/
/*!
\file   HierarchySystem.h
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/06
\brief
This file contains the interface of the HierarchySystem class, which is
responsible for managing the hierarchical relationships between entities in the
game engine. It ensures that parent-child relationships are maintained and that
transformations are correctly propagated through the hierarchy.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include "../ECS/ECS.h"
#include "ParentComponent.h"

namespace Popplio
{
	class HierarchySystem : public System
	{
	public:
		HierarchySystem();

		void Update();

		// Call this when a new parent-child relationship is established
		void InitializeParentTransform(Entity parent);

		void StopParentTransform(Entity parent);

		std::vector<Entity> GetChildRecursive(Entity const parent);
		Entity GetChildRecursive(Entity const parent, Entity child);

		void AddChild(Entity e, Entity child);

		void RemoveChild(Entity e, Entity child);

		void SetChildParentIdRecursive(Entity e, Entity o);

		void CloneSecondPass(Entity newEnt, Entity original);

	private:
		std::unordered_map<int, TransformComponent> lastParentTransforms;

		void UpdateHierarchy(Entity& entity);

		std::vector<Entity>& GetChildRecursion(Entity const parent, std::vector<Entity>& ents);
	};
}