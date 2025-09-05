/******************************************************************************/
/*!
\file   MovementSystem.h
\author Team Popplio
\author Rio Shannon Yvon Leonardo - 100%
\par    Course : CSD2400/CSD2401
\par    Section : A
\date   2024/10/03
\brief
		This file contains the declarations for functions that manages the movement,
		collision and force integration of entities.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <pch.h>
#include "../Transformation/TransformComponent.h"
#include "../Physics/RigidBodyComponent.h"
#include "../Particles/ParticleComponent.h" //for Particles
#include "../Math/Vector2D.h"

namespace Popplio
{
	class MovementSystem : public System
	{
		public:
			MovementSystem();

			void ApplyForce(RigidBodyComponent& rigidBody, const PopplioMath::Vec2f& force);
			void Update(double deltaTime);
			void SetConstantVelocity(Entity entity, float velocityX, float velocityY);
			void SetPositionY(Entity entity, float positionY);

			static void HandleCollisionResponse(
				TransformComponent& transform1,
				RigidBodyComponent& rigidBody1,
				BoxColliderComponent& boxCollider1,
				TransformComponent& transform2,
				RigidBodyComponent& rigidBody2,
				BoxColliderComponent& boxCollider2,
				bool isTrigger1,
				bool isTrigger2,
				float firstCollisionTime
			);

			static void HandleCollisionResponse(
				TransformComponent& transform1,
				RigidBodyComponent& rigidBody1,
				TransformComponent& transform2,
				RigidBodyComponent& rigidBody2,
				bool isTrigger1,
				bool isTrigger2,
				float firstCollisionTime
			);

			void ApplyPhysicsParticles(
				Particle& particle,
				float mass,
				bool useGravity,
				float gravityScale,
				float dragCoefficient,
				float deltaTime
			);

		private:
			void IntegrateForces(RigidBodyComponent& rigidBody, TransformComponent& transform, float deltaTime);
			PopplioMath::Vec2f CalculateDragForce(const RigidBodyComponent& rigidBody);

	};
}