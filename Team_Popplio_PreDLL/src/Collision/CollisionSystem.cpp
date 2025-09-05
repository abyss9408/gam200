/******************************************************************************/
/*!
\file   CollisionSystem.cpp
\author Team Popplio
\author Rio Shannon Yvon Leonardo - 95%
\co-author Ng Jun Heng Shawn - 5%
\par    Course : CSD2400/CSD2401
\par    Section : A
\date   2024/10/03
\brief
		This header file contains the implementations for the declarations in
		CollisionSystem.h

		This file checks for various collisions between different entities and
		calls the function accordingly for each collision detected

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>

#include "CollisionSystem.h"
#include "CollisionEvent.h"
#include "../Engine/Engine.h"
#include "../Physics/Physics.h"
#include "../Physics/MovementSystem.h"
#include "../Logging/Logger.h"

namespace Popplio
{
	CollisionSystem::CollisionSystem(EventBus& e, LayerManager& layerMgr) 
		: event(e), layerManager(layerMgr)
	{
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();
		//RequireComponent<BoxColliderComponent>(); //no need because components can have both circle and box

		//collisions.reserve(256);
  //      triggers.reserve(256);
        collisionCount.reserve(1024);
        triggerCount.reserve(1024);
	}

	//check for collision between two box colliders
	bool CollisionSystem::CheckBoxToBoxCollision(
		const TransformComponent& transform1, const BoxColliderComponent& box1,
		const TransformComponent& transform2, const BoxColliderComponent& box2,
		const PopplioMath::Vec2f& vel1, const PopplioMath::Vec2f& vel2,
		float& firstTimeOfCollision, float deltaTime)
	{
		//create Axis-Aligned Bounding Boxes (AABB) for collision check
		PopplioPhysics::AABB aabb1 = {
			PopplioMath::Vec2f(transform1.position.x + box1.offset.x - box1.size.x / 2,
				transform1.position.y + box1.offset.y - box1.size.y / 2),
			PopplioMath::Vec2f(transform1.position.x + box1.offset.x + box1.size.x / 2,
				transform1.position.y + box1.offset.y + box1.size.y / 2)
		};
		PopplioPhysics::AABB aabb2 = {
			PopplioMath::Vec2f(transform2.position.x + box2.offset.x - box2.size.x / 2,
				transform2.position.y + box2.offset.y - box2.size.y / 2),
			PopplioMath::Vec2f(transform2.position.x + box2.offset.x + box2.size.x / 2,
				transform2.position.y + box2.offset.y + box2.size.y / 2)
		};
		return PopplioPhysics::AABB_RectangleRectangleCollision(aabb1, vel1, aabb2, vel2, firstTimeOfCollision, deltaTime);
	}

	bool CollisionSystem::CheckCircleToCircleCollision(
		const TransformComponent& transform1, const CircleColliderComponent& circle1,
		const TransformComponent& transform2, const CircleColliderComponent& circle2
	)
	{
		PopplioMath::Vec2f center1(
			transform1.position.x + circle1.offset.x,
			transform1.position.y + circle1.offset.y
		);
		PopplioMath::Vec2f center2(
			transform2.position.x + circle2.offset.x,
			transform2.position.y + circle2.offset.y
		);

		return PopplioPhysics::CircleToCircleCollision(center1, circle1.radius, center2, circle2.radius);
	}

	bool CollisionSystem::CheckBoxToCircleCollision(
		const TransformComponent& transformBox, const BoxColliderComponent& box,
		const TransformComponent& transformCircle, const CircleColliderComponent& circle)
	{
		// Get box bounds
		float boxLeft = transformBox.position.x + box.offset.x - box.size.x / 2;
		float boxRight = transformBox.position.x + box.offset.x + box.size.x / 2;
		float boxTop = transformBox.position.y + box.offset.y + box.size.y / 2;
		float boxBottom = transformBox.position.y + box.offset.y - box.size.y / 2;

		// Get circle center
		PopplioMath::Vec2f circleCenter(
			transformCircle.position.x + circle.offset.x,
			transformCircle.position.y + circle.offset.y
		);

		return PopplioPhysics::BoxToCircleCollision(
			boxLeft, boxRight, boxTop, boxBottom, circleCenter, circle.radius
		);
	}

	void CollisionSystem::Update(Popplio::Registry& registry, float deltaTime)
	{
		for (int step = 0; step < Engine::timer.GetCurrentNumberOfSteps(); ++step)
		{
			auto systemEntities = GetSystemEntities();

			// expensive?
            //std::vector<std::vector<int>> collisionCount(systemEntities.size());
            //std::vector<std::vector<int>> triggerCount(systemEntities.size());

			unsigned int iIndex = 0;
			for (auto i = systemEntities.begin(); i != systemEntities.end(); ++i)
			{
				if (i->GetComponent<ActiveComponent>().isActive == false) continue;
				if (!(i->GetComponent<RigidBodyComponent>().isActive)) continue;

				collisionCount[iIndex] = 0;
                triggerCount[iIndex] = 0;

				// Get first entity's layer
				std::string layer1 = layerManager.GetEntityLayer(*i);

				auto& transform1 = registry.GetComponent<TransformComponent>(*i);
				auto& rigidBody1 = registry.GetComponent<RigidBodyComponent>(*i);

				//checking for box and circle components
				bool hasBox1 = registry.HasComponent<BoxColliderComponent>(*i);
				bool hasCircle1 = registry.HasComponent<CircleColliderComponent>(*i);

				if (!hasBox1 && !hasCircle1) continue;

				//if (!(i->GetComponent<BoxColliderComponent>().isActive)) continue;
				//if (!(i->GetComponent<CircleColliderComponent>().isActive)) continue;

    //            bool atLeastOneCollision = false;
				//bool atLeastOneTrigger = false;

				for (auto j = i + 1; j != systemEntities.end(); ++j)
				{
					if (j->GetComponent<ActiveComponent>().isActive == false) continue;
					if (!(j->GetComponent<RigidBodyComponent>().isActive)) continue;

					// Get second entity's layer
					std::string layer2 = layerManager.GetEntityLayer(*j);

					// Check if layers should interact
					// If either entity has no layer, allow collision (default behavior)
					// Otherwise, check layer interaction matrix
					if (!layer1.empty() && !layer2.empty() &&
						!layerManager.ShouldLayersInteract(layer1, layer2))
					{
						continue;  // Skip collision check if layers shouldn't interact
					}

					auto& transform2 = registry.GetComponent<TransformComponent>(*j);
					auto& rigidBody2 = registry.GetComponent<RigidBodyComponent>(*j);

					//checking for box and circle components
					bool hasBox2 = registry.HasComponent<BoxColliderComponent>(*j);
					bool hasCircle2 = registry.HasComponent<CircleColliderComponent>(*j);

					if (!hasBox2 && !hasCircle2) continue;

					//if (!(j->GetComponent<BoxColliderComponent>().isActive)) continue;
					//if (!(j->GetComponent<CircleColliderComponent>().isActive)) continue;

					bool collision = false;
					bool isTrigger1 = false;
					bool isTrigger2 = false;
					float firstCollisionTime = 0.0f;

					// Box - Box collision
					if (hasBox1 && hasBox2) 
					{
						if (!(i->GetComponent<BoxColliderComponent>().isActive)) continue;
                        if (!(j->GetComponent<BoxColliderComponent>().isActive)) continue;

						auto& box1 = registry.GetComponent<BoxColliderComponent>(*i);
						auto& box2 = registry.GetComponent<BoxColliderComponent>(*j);

						if (!box1.isEnabled || !box2.isEnabled) continue;

						isTrigger1 = box1.isTrigger;
						isTrigger2 = box2.isTrigger;

						PopplioMath::Vec2f vel1(rigidBody1.velocity.x, rigidBody1.velocity.y);
						PopplioMath::Vec2f vel2(rigidBody2.velocity.x, rigidBody2.velocity.y);

						collision = CheckBoxToBoxCollision(transform1, box1, transform2, box2, vel1, vel2, firstCollisionTime, deltaTime);
					}

					// Circle - Circle Collision
					else if (hasCircle1 && hasCircle2) 
					{
						if (!(i->GetComponent<CircleColliderComponent>().isActive)) continue;
						if (!(j->GetComponent<CircleColliderComponent>().isActive)) continue;

						auto& circle1 = registry.GetComponent<CircleColliderComponent>(*i);
						auto& circle2 = registry.GetComponent<CircleColliderComponent>(*j);

						if (!circle1.isEnabled || !circle2.isEnabled) continue;

						isTrigger1 = circle1.isTrigger;
						isTrigger2 = circle2.isTrigger;

						collision = CheckCircleToCircleCollision(transform1, circle1, transform2, circle2);
					}

					// Box - Circle Collision
					else if (hasBox1 && hasCircle2) 
					{
						if (!(i->GetComponent<BoxColliderComponent>().isActive)) continue;
						if (!(j->GetComponent<CircleColliderComponent>().isActive)) continue;

						auto& box = registry.GetComponent<BoxColliderComponent>(*i);
						auto& circle = registry.GetComponent<CircleColliderComponent>(*j);

						if (!box.isEnabled || !circle.isEnabled) continue;

						isTrigger1 = box.isTrigger;
						isTrigger2 = circle.isTrigger;

						collision = CheckBoxToCircleCollision(transform1, box, transform2, circle);
					}

					//circle - box collision
					else if (hasCircle1 && hasBox2) 
					{
						if (!(i->GetComponent<CircleColliderComponent>().isActive)) continue;
						if (!(j->GetComponent<BoxColliderComponent>().isActive)) continue;

						auto& circle = registry.GetComponent<CircleColliderComponent>(*i);
						auto& box = registry.GetComponent<BoxColliderComponent>(*j);

						if (!circle.isEnabled || !box.isEnabled) continue;

						isTrigger1 = circle.isTrigger;
						isTrigger2 = box.isTrigger;

						collision = CheckBoxToCircleCollision(transform2, box, transform1, circle);
					}

					//collision is detected
					if (collision)
					{
						if (!isTrigger1 && !isTrigger2) // collision w/o trigger
						{
                            collisions.insert(std::make_pair(static_cast<int>(iIndex), static_cast<int>(j - systemEntities.begin())));

                            ++collisionCount[iIndex];
                            ++collisionCount[static_cast<int>(j - systemEntities.begin())];

							bool isFirstCollision1 = true;
                            bool isFirstCollision2 = true;
							if (hasCircle1)
							{
								// emit collision event, clients need to differentiate entities
                                if (i->GetComponent<CircleColliderComponent>().isColliding) isFirstCollision1 = false;
								i->GetComponent<CircleColliderComponent>().isColliding = true;
							}
							else
							{
                                if (i->GetComponent<BoxColliderComponent>().isColliding) isFirstCollision1 = false;
								i->GetComponent<BoxColliderComponent>().isColliding = true;
							}

							if (hasCircle2)
							{
                                if (j->GetComponent<CircleColliderComponent>().isColliding) isFirstCollision2 = false;
								j->GetComponent<CircleColliderComponent>().isColliding = true;
							}
							else
							{
                                if (j->GetComponent<BoxColliderComponent>().isColliding) isFirstCollision2 = false;
								j->GetComponent<BoxColliderComponent>().isColliding = true;
							}

							Logger::Debug("Collision detected between Entity " + std::to_string(i->GetId()) +
								" and Entity " + std::to_string(j->GetId()));
							// emit collision event, clients need to differentiate entities
                            if (isFirstCollision1 || isFirstCollision2) // first collision if either
								event.EmitEvent<CollisionEvent>(i->GetId(), j->GetId(), 
									Engine::timer.GetAccumulatedTime(), CollisionEvent::ENTER);
							else event.EmitEvent<CollisionEvent>(i->GetId(), j->GetId(), 
								Engine::timer.GetAccumulatedTime(), CollisionEvent::STAY);

							MovementSystem::HandleCollisionResponse(transform1, rigidBody1,
								transform2, rigidBody2, isTrigger1, isTrigger2, firstCollisionTime);

                            // skip circle collider for now TODO: implement circle collider
							//if (hasBox1 && hasBox2)
							//{
       //                         BoxColliderComponent& box1 = i->GetComponent<BoxColliderComponent>();
       //                         BoxColliderComponent& box2 = j->GetComponent<BoxColliderComponent>();

							//	MovementSystem::HandleCollisionResponse(transform1, rigidBody1, box1,
							//		transform2, rigidBody2, box2, isTrigger1, isTrigger2, firstCollisionTime);
							//}
							//else
							//{
							//	MovementSystem::HandleCollisionResponse(transform1, rigidBody1, 
							//		transform2, rigidBody2, isTrigger1, isTrigger2, firstCollisionTime);
							//}
						}
						else // emit trigger event, clients need to differentiate entities
						{
							triggers.insert(std::make_pair(static_cast<int>(iIndex), static_cast<int>(j - systemEntities.begin())));

                            ++triggerCount[iIndex];
                            ++triggerCount[static_cast<int>(j - systemEntities.begin())];

							bool isFirstTrigger1 = true;
							bool isFirstTrigger2 = true;
							if (hasCircle1)
							{
								if (i->GetComponent<CircleColliderComponent>().isTriggering) isFirstTrigger1 = false;
								i->GetComponent<CircleColliderComponent>().isTriggering = true;
							}
							else
							{
								if (i->GetComponent<BoxColliderComponent>().isTriggering) isFirstTrigger1 = false;
								i->GetComponent<BoxColliderComponent>().isTriggering = true;
							}

							if (hasCircle2)
							{
								if (j->GetComponent<CircleColliderComponent>().isTriggering) isFirstTrigger2 = false;
								j->GetComponent<CircleColliderComponent>().isTriggering = true;
							}
							else
							{
								if (j->GetComponent<BoxColliderComponent>().isTriggering) isFirstTrigger2 = false;
								j->GetComponent<BoxColliderComponent>().isTriggering = true;
							}

							Logger::Debug("Trigger detected between Entity " + std::to_string(i->GetId()) +
								" and Entity " + std::to_string(j->GetId()));
							// emit trigger event, clients need to differentiate entities
                            if (isFirstTrigger1 || isFirstTrigger2) // first trigger if either
                                event.EmitEvent<TriggerEvent>(i->GetId(), j->GetId(), 
									Engine::timer.GetAccumulatedTime(), TriggerEvent::ENTER);
                            else event.EmitEvent<TriggerEvent>(i->GetId(), j->GetId(), 
								Engine::timer.GetAccumulatedTime(), TriggerEvent::STAY);
						}
						//MovementSystem::HandleCollisionResponse(transform1, rigidBody1, transform2, rigidBody2, isTrigger1, isTrigger2);
					}
					else // OnCollisionExit / OnTriggerExit
					{
						std::pair<int, int> pair1 = std::make_pair(static_cast<int>(iIndex), static_cast<int>(j - systemEntities.begin()));
                        std::pair<int, int> pair2 = std::make_pair(
							static_cast<int>(j - systemEntities.begin()), static_cast<int>(iIndex));
						if (collisions.contains(pair1) || collisions.contains(pair2))
						{
							event.EmitEvent<CollisionEvent>(i->GetId(), j->GetId(), 
								Engine::timer.GetAccumulatedTime(), CollisionEvent::EXIT);
                            collisions.erase(pair1);
                            collisions.erase(pair2);
						}
                        if (triggers.contains(pair1) || triggers.contains(pair2))
                        {
							event.EmitEvent<TriggerEvent>(i->GetId(), j->GetId(), 
                                Engine::timer.GetAccumulatedTime(), TriggerEvent::EXIT);
                            triggers.erase(pair1);
                            triggers.erase(pair2);
                        }
					}
				}

                // OnCollisionExit / OnTriggerExit

				if (collisionCount[iIndex] <= 0)
				{
					if (hasCircle1)
					{
						if (!i->GetComponent<CircleColliderComponent>().isTrigger && 
							i->GetComponent<CircleColliderComponent>().isColliding)
						{
							i->GetComponent<CircleColliderComponent>().isColliding = false;
                            // emit OnCollisionExit event
							//event.EmitEvent<CollisionEvent>(i->GetId(), -1, Engine::timer.GetAccumulatedTime(), CollisionEvent::EXIT);
						}
					}
					else
					{
						if (!i->GetComponent<BoxColliderComponent>().isTrigger && 
							i->GetComponent<BoxColliderComponent>().isColliding)
						{
                            i->GetComponent<BoxColliderComponent>().isColliding = false;
                            // emit OnCollisionExit event
							//event.EmitEvent<CollisionEvent>(i->GetId(), -1, Engine::timer.GetAccumulatedTime(), CollisionEvent::EXIT);
						}
					}
				}
				if (triggerCount[iIndex] <= 0)
                {
                    if (hasCircle1)
                    {
                        if (//i->GetComponent<CircleColliderComponent>().isTrigger && 
							i->GetComponent<CircleColliderComponent>().isTriggering)
                        {
                            i->GetComponent<CircleColliderComponent>().isTriggering = false;
                            // emit OnTriggerExit event
							//event.EmitEvent<TriggerEvent>(i->GetId(), -1, Engine::timer.GetAccumulatedTime(), TriggerEvent::EXIT);
                        }
                    }
                    else
                    {
                        if (//i->GetComponent<BoxColliderComponent>().isTrigger && 
							i->GetComponent<BoxColliderComponent>().isTriggering)
                        {
                            i->GetComponent<BoxColliderComponent>().isTriggering = false;
                            // emit OnTriggerExit event
							//event.EmitEvent<TriggerEvent>(i->GetId(), -1, Engine::timer.GetAccumulatedTime(), TriggerEvent::EXIT);
                        }
                    }
                }

				++iIndex;
			}
		}
	}
}