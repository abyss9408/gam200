/******************************************************************************/
/*!
\file           TransformationSystem.cpp
\author         Team Popplio
\author         Bryan Ang Wei Ze
\contribution   Bryan - 100%
\par            Course : CSD2401 / CSD2400
\par            Section : A
\date           2024/11/06
\brief
	This is the source file for the Transformation system of entities in the engine

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>
#include "TransformationSystem.h"

#include "../Editor/Changes.h"

namespace Popplio
{
	TransformationSystem::TransformationSystem()
	{
		RequireComponent<TransformComponent>();
	}

	void TransformationSystem::Update()
	{
		for (auto& entity : GetSystemEntities())
		{
			auto& transform = entity.GetComponent<TransformComponent>();

   //         static PopplioMath::M3x3f prevModelMatrix;
			//prevModelMatrix = transform.modelMatrix;

			UpdateModelMatrix(transform);

			//// If entity is a child and inherits transform
			//if (prevModelMatrix != transform.modelMatrix)
			//{
			//	if (entity.HasComponent<ParentComponent>() && entity.GetComponent<ParentComponent>().parent.Exists() &&
			//		entity.GetComponent<ParentComponent>().inheritTransform)
			//	{
			//		entity.GetComponent<ParentComponent>().UpdateLocalTransform(
			//			transform, entity.GetComponent<ParentComponent>().parent.GetComponent<TransformComponent>());
			//	}
			//}

			// If the entity has a BoxColliderComponent, update its matrix too
			if (entity.HasComponent<BoxColliderComponent>())
			{
				auto& collider = entity.GetComponent<BoxColliderComponent>();
				UpdateColliderMatrix(transform, collider);
			}
		}
	}

	void TransformationSystem::UpdateModelMatrix(TransformComponent& transform)
	{
		// Calculate the offset based on the anchor
		PopplioMath::Vec2f anchorOffset;
		switch (transform.anchor) {
		case Anchor::TopLeft: anchorOffset = { -transform.scale.x / 2, transform.scale.y / 2 }; break;
		case Anchor::TopCenter: anchorOffset = { 0, transform.scale.y / 2 }; break;
		case Anchor::TopRight: anchorOffset = { transform.scale.x / 2, transform.scale.y / 2 }; break;
		case Anchor::MiddleLeft: anchorOffset = { -transform.scale.x / 2, 0 }; break;
		case Anchor::MiddleCenter: anchorOffset = { 0, 0 }; break; // No offset needed
		case Anchor::MiddleRight: anchorOffset = { transform.scale.x / 2, 0 }; break;
		case Anchor::BottomLeft: anchorOffset = { -transform.scale.x / 2, -transform.scale.y / 2 }; break;
		case Anchor::BottomCenter: anchorOffset = { 0, -transform.scale.y / 2 }; break;
		case Anchor::BottomRight: anchorOffset = { transform.scale.x / 2, -transform.scale.y / 2 }; break;
		}

		//Logger::Debug("Anchor Offset: (" + std::to_string(anchorOffset.x) + ", " + std::to_string(anchorOffset.y) + ")");

		// Compute transformation matrices
		PopplioMath::M3x3f translateMat, rotateMat, scaleMat;

		// Apply translation with correct offset
		translateMat.Translate(
			transform.position.x + anchorOffset.x,
			transform.position.y + anchorOffset.y
		);

		rotateMat.RotDeg(static_cast<float>(transform.rotation));
		scaleMat.Scale(transform.scale.x, transform.scale.y);

		// Compute final transformation matrix
		transform.modelMatrix = translateMat * rotateMat * scaleMat;

		//Logger::Debug("Position after Anchor Offset: (" + std::to_string(transform.position.x + anchorOffset.x) + ", " + std::to_string(transform.position.y + anchorOffset.y) + ")");
	}




	void TransformationSystem::UpdateColliderMatrix(const TransformComponent& transform, BoxColliderComponent& collider)
	{
		PopplioMath::M3x3f translateMat, scaleMat;

		// Combine entity position and collider offset in a single translation
		float totalOffsetX = transform.position.x + collider.offset.x;
		float totalOffsetY = transform.position.y + collider.offset.y;
		translateMat.Translate(totalOffsetX, totalOffsetY);

		// Scale to collider size
		scaleMat.Scale(collider.size.x, collider.size.y);

		// Combine all transformations
		collider.colliderMatrix = translateMat * scaleMat;
	}
}