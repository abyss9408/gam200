/******************************************************************************/
/*!
\file   MovementSystem.cpp
\author Team Popplio
\author Rio Shannon Yvon Leonardo - 100%
\par    Course : CSD2400/CSD2401 - CSD2451/CSD2450
\par    Section : A
\date   2024/10/03, Last Update: 2025/01/26
\brief
		This file contains the implementations for the functions declared in
		MovementSystem.h 

		It is responsible for the movement, collision and force integration
		entities in the game and handles various aspects of physics (including
		collision) applying forces and integrating forces into entity motion.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include "../Engine/Engine.h"
#include "../Physics/MovementSystem.h"


namespace Popplio
{
	MovementSystem::MovementSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();

	}

	void MovementSystem::HandleCollisionResponse(
        TransformComponent& transform1, RigidBodyComponent& rigidBody1, BoxColliderComponent& boxCollider1,
        TransformComponent& transform2, RigidBodyComponent& rigidBody2, BoxColliderComponent& boxCollider2,
		bool isTrigger1, bool isTrigger2, float firstCollisionTime)
	{
		static_cast<void>(firstCollisionTime);

        float pos1X = transform1.position.x + boxCollider1.offset.x;
        float pos1Y = transform1.position.y + boxCollider1.offset.y;
        float pos2X = transform2.position.x + boxCollider2.offset.x;
        float pos2Y = transform2.position.y + boxCollider2.offset.y;
		float scale1X = boxCollider1.size.x; //transform1.scale.x;
		float scale1Y = boxCollider1.size.y; //transform1.scale.y;
		float scale2X = boxCollider2.size.x; //transform2.scale.x;
		float scale2Y = boxCollider2.size.y; //transform2.scale.y;

		if (!isTrigger1 && !isTrigger2) //only handle physical collisions
		{
			// Calculate direction vector between two objects
			float dx = pos2X - pos1X;
			float dy = pos2Y - pos1Y;
			bool isDynamic1 = rigidBody1.useGravity;
			bool isDynamic2 = rigidBody2.useGravity;
			// Add debug logging
			Logger::Debug("Passes through this function", LogDestination::IMGUI_CONSOLE);
			// Calculate minimum distance needed between objects based on their scales
			float minDistanceX = (scale1X + scale2X) * 0.5f;
			float minDistanceY = (scale1Y + scale2Y) * 0.5f;
			// Determine if collision is horizontal or vertical based on overlap ratios
			float overlapX = minDistanceX - std::abs(dx);
			float overlapY = minDistanceY - std::abs(dy);
			if (overlapX > 0 && overlapX < overlapY) {
				// Horizontal collision
				if (dx > 0) { // Object 1 is on the left
					if (isDynamic1) pos1X = pos2X - minDistanceX;
					if (isDynamic2) pos2X = pos1X + minDistanceX;
					if (rigidBody1.velocity.x > 0) rigidBody1.velocity.x = 0;
					if (rigidBody2.velocity.x < 0) rigidBody2.velocity.x = 0;
				}
				else { // Object 1 is on the right
					if (isDynamic1) pos1X = pos2X + minDistanceX;
					if (isDynamic2) pos2X = pos1X - minDistanceX;
					if (rigidBody1.velocity.x < 0) rigidBody1.velocity.x = 0;
					if (rigidBody2.velocity.x > 0) rigidBody2.velocity.x = 0;
				}
			}
			else if (overlapY > 0) {
				// Vertical collision
				if (dy > 0) { // Object 1 is below
					if (isDynamic2) {
						//pos2Y = pos1Y + minDistanceY;
						rigidBody2.velocity.y = 0;
						rigidBody2.force.y = 0;
						rigidBody2.acceleration.y = 0;
					}
					if (isDynamic1) {
						//pos1Y = pos2Y - minDistanceY;
						rigidBody1.velocity.y = 0;
						rigidBody1.force.y = 0;
						rigidBody1.acceleration.y = 0;
					}
				}
				else { // Object 1 is above
					if (isDynamic1) {
						//pos1Y = pos2Y + minDistanceY;
						rigidBody1.velocity.y = 0;
						rigidBody1.force.y = 0;
						rigidBody1.acceleration.y = 0;
					}
					if (isDynamic2) {
						//pos2Y = pos1Y - minDistanceY;
						rigidBody2.velocity.y = 0;
						rigidBody2.force.y = 0;
						rigidBody2.acceleration.y = 0;
					}
				}
			}
			// Reset horizontal forces
			rigidBody1.force.x = 0;
			rigidBody2.force.x = 0;
			rigidBody1.acceleration.x = 0;
			rigidBody2.acceleration.x = 0;
		}
	}

	void MovementSystem::HandleCollisionResponse(
		TransformComponent& transform1, RigidBodyComponent& rigidBody1,
		TransformComponent& transform2, RigidBodyComponent& rigidBody2,
		bool isTrigger1, bool isTrigger2, float firstCollisionTime)
	{
		static_cast<void>(firstCollisionTime);

		if (!isTrigger1 && !isTrigger2) //only handle physical collisions
		{
			// Calculate direction vector between two objects
			float dx = transform2.position.x - transform1.position.x;
			float dy = transform2.position.y - transform1.position.y;
			bool isDynamic1 = rigidBody1.useGravity;
			bool isDynamic2 = rigidBody2.useGravity;
			// Add debug logging
			Logger::Debug("Passes through this function", LogDestination::IMGUI_CONSOLE);
			// Calculate minimum distance needed between objects based on their scales
			float minDistanceX = (transform1.scale.x + transform2.scale.x) * 0.5f;
			float minDistanceY = (transform1.scale.y + transform2.scale.y) * 0.5f;
			// Determine if collision is horizontal or vertical based on overlap ratios
			float overlapX = minDistanceX - std::abs(dx);
			float overlapY = minDistanceY - std::abs(dy);
			if (overlapX > 0 && overlapX < overlapY) {
				// Horizontal collision
				if (dx > 0) { // Object 1 is on the left
					if (isDynamic1) transform1.position.x = transform2.position.x - minDistanceX;
					if (isDynamic2) transform2.position.x = transform1.position.x + minDistanceX;
					if (rigidBody1.velocity.x > 0) rigidBody1.velocity.x = 0;
					if (rigidBody2.velocity.x < 0) rigidBody2.velocity.x = 0;
				}
				else { // Object 1 is on the right
					if (isDynamic1) transform1.position.x = transform2.position.x + minDistanceX;
					if (isDynamic2) transform2.position.x = transform1.position.x - minDistanceX;
					if (rigidBody1.velocity.x < 0) rigidBody1.velocity.x = 0;
					if (rigidBody2.velocity.x > 0) rigidBody2.velocity.x = 0;
				}
			}
			else if (overlapY > 0) {
				// Vertical collision
				if (dy > 0) { // Object 1 is below
					if (isDynamic2) {
						transform2.position.y = transform1.position.y + minDistanceY;
						rigidBody2.velocity.y = 0;
						rigidBody2.force.y = 0;
						rigidBody2.acceleration.y = 0;
					}
					if (isDynamic1) {
						transform1.position.y = transform2.position.y - minDistanceY;
						rigidBody1.velocity.y = 0;
						rigidBody1.force.y = 0;
						rigidBody1.acceleration.y = 0;
					}
				}
				else { // Object 1 is above
					if (isDynamic1) {
						transform1.position.y = transform2.position.y + minDistanceY;
						rigidBody1.velocity.y = 0;
						rigidBody1.force.y = 0;
						rigidBody1.acceleration.y = 0;
					}
					if (isDynamic2) {
						transform2.position.y = transform1.position.y - minDistanceY;
						rigidBody2.velocity.y = 0;
						rigidBody2.force.y = 0;
						rigidBody2.acceleration.y = 0;
					}
				}
			}
			// Reset horizontal forces
			rigidBody1.force.x = 0;
			rigidBody2.force.x = 0;
			rigidBody1.acceleration.x = 0;
			rigidBody2.acceleration.x = 0;
		}
	}



	//function to only change position Y
	void MovementSystem::SetPositionY(Entity entity, float positionY)
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		transform.position.y = positionY; 

		//reset velocity and forces if entity has a rigidBody
		auto& rigidBody = entity.GetComponent<RigidBodyComponent>();
		rigidBody.velocity.x = 0.0f;
		rigidBody.velocity.y = 0.0f;
		rigidBody.force.x = 0.0f;
		rigidBody.force.y = 0.0f;
		rigidBody.acceleration.x = 0.0f;
		rigidBody.acceleration.y = 0.0f;
	}

	void MovementSystem::ApplyForce(RigidBodyComponent& rigidBody, const PopplioMath::Vec2f& force)
	{
		//apply external forces to a rigidBody
		rigidBody.force.x += force.x;
		rigidBody.force.y += force.y;
	}

	//this is to ensure the velocity comes to a constant 
	PopplioMath::Vec2f MovementSystem::CalculateDragForce(const RigidBodyComponent& rigidBody) {
		//calculate the square of velocity magnitude
		float velocityMagnitudeSquared = rigidBody.velocity.x * rigidBody.velocity.x + rigidBody.velocity.y * rigidBody.velocity.y;

		//if object is stationary, no drag force is needed
		if (velocityMagnitudeSquared < 0.01f) {
			return PopplioMath::Vec2f(0.0f, 0.0f);
		}

		//calculate the drag force opposite to velocity direction
		float dragForceX = -rigidBody.dragCoefficient * rigidBody.velocity.x;
		float dragForceY = -rigidBody.dragCoefficient * rigidBody.velocity.y;

		return PopplioMath::Vec2f(dragForceX, dragForceY);
	}

	//update the physics and resulting motion
	void MovementSystem::IntegrateForces(RigidBodyComponent& rigidBody, TransformComponent& transform, float deltaTime) {
		if (rigidBody.mass <= 0.0f) return;

		//update forces from ForceManager
		rigidBody.forceManager.UpdateForces(deltaTime);

		//get resultant force from ForceManager
		PopplioMath::Vec2f resultantForce = rigidBody.forceManager.GetResultantForce();

		//add drag force (only for those using gravity)
		if (rigidBody.useGravity) {
			PopplioMath::Vec2f dragForce = CalculateDragForce(rigidBody);
			resultantForce.x += dragForce.x;
			resultantForce.y += dragForce.y;
		}

		//apply gravity in negative y direction
		if (rigidBody.useGravity)
		{
			resultantForce.y -= rigidBody.gravityScale * 500.0f; // Gravity is downward
		}

		//calculate acceleration from force ( F = ma -> a = F/m)
		float inverseMass = 1.0f / rigidBody.mass;
		rigidBody.acceleration.x = resultantForce.x * inverseMass;
		rigidBody.acceleration.y = resultantForce.y * inverseMass;

		//update velocity using acceleration ( v = v0 + a)
		rigidBody.velocity.x += rigidBody.acceleration.x * deltaTime;
		rigidBody.velocity.y += rigidBody.acceleration.y * deltaTime;

		//update position here instead (p = p0 + vt)
		transform.position.x += rigidBody.velocity.x * static_cast<float>(deltaTime);
		transform.position.y += rigidBody.velocity.y * static_cast<float>(deltaTime);

		//reset forces for next frame
		rigidBody.force.x = 0.0f;
		rigidBody.force.y = 0.0f;
	}

	//for entities with constant velocity (no gravity applied)
	void MovementSystem::SetConstantVelocity(Entity entity, float velocityX, float velocityY) {
		auto& rigidBody = entity.GetComponent<RigidBodyComponent>();
		if (!rigidBody.useGravity) {
			rigidBody.velocity.x = velocityX;
			rigidBody.velocity.y = velocityY;
		}
	}

	void MovementSystem::ApplyPhysicsParticles(Particle& particle, float mass, bool useGravity,
		float gravityScale, float dragCoefficient, float deltaTime) {

		//IntegrateForces for particles
		if (mass <= 0.0f) return;

		//initialize resultant force vector
		PopplioMath::Vec2f resultantForce(0.0f, 0.0f);

		//apply gravity in negative y direction
		if (useGravity) {
			resultantForce.y += gravityScale * 9.8f;
		}

		//calculate the drag force
		if (useGravity) {
			float velocityMagnitudeSquared = particle.vel.x * particle.vel.x + particle.vel.y * particle.vel.y;

			if (velocityMagnitudeSquared > 0.01f) {
				float dragForceX = -dragCoefficient * particle.vel.x;
				float dragForceY = -dragCoefficient * particle.vel.y;

				resultantForce.x += dragForceX;
				resultantForce.y += dragForceY;
			}
		}

		//calculate acceleration (F = ma so a = F/m)
		float inverseMass = 1.0f / mass;
		particle.accel.x = resultantForce.x * inverseMass;
		particle.accel.y = resultantForce.y * inverseMass;

		//update velocity (v = v0 + at)
		particle.vel.x += particle.accel.x * deltaTime;
		particle.vel.y += particle.accel.y * deltaTime;

		//update position (p = p0 + vt)
		particle.pos.x += particle.vel.x * deltaTime;
		particle.pos.y += particle.vel.y * deltaTime;
	}

	void MovementSystem::Update(double deltaTime)
	{
		for (int step = 0; step < Engine::timer.GetCurrentNumberOfSteps(); ++step)
		{
			for (Entity entity : GetSystemEntities())
			{
				if (entity.GetComponent<ActiveComponent>().isActive == false) continue;
                if (!(entity.GetComponent<RigidBodyComponent>().isActive)) continue;

				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rigidBody = entity.GetComponent<RigidBodyComponent>();

				//Logger::Debug("Entity " + std::to_string(entity.GetId()) + " position x: " + std::to_string(transform.position.x)); //debug
				//Logger::Debug("Entity " + std::to_string(entity.GetId()) + " position y: " + std::to_string(transform.position.y)); //debug

				//update physics
				IntegrateForces(rigidBody, transform ,static_cast<float>(deltaTime));

				//update positions (p = p0 + vt)
				//transform.position.x += rigidBody.velocity.x * static_cast<float>(deltaTime);
				//transform.position.y += rigidBody.velocity.y * static_cast<float>(deltaTime);
			}
		}
	}
}