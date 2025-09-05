/******************************************************************************/
/*!
\file   ParentComponent.h
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 70%, Shawn - 30%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/06
\brief  This file contains the definition of the ParentComponent class, which is 
        used to manage parent-child relationships between entities in the ECS 
        system. It includes methods for adding and removing child entities, 
        updating local and world transforms, and synchronizing transformations 
        between parent and child entities.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include <vector>
#include "../ECS/ECS.h"
#include "../Math/MathLib.h"

namespace Popplio
{
	class ParentComponent
	{
		public:

		Entity parent{ -1 };
		std::vector<Entity> children{};
		bool inheritActive = true;
		bool inheritTransform = true;

		// Local transform data
		PopplioMath::Vec2f localPosition{ 0.0f, 0.0f };
		PopplioMath::Vec2f localScale{ 1.0f, 1.0f };
		double localRotation = 0.0;

		// do not serialize

		PopplioMath::Vec2f prevLocalPosition{ 0.0f, 0.0f };
		PopplioMath::Vec2f prevLocalScale{ 1.0f, 1.0f };
		double prevLocalRotation = 0.0;

		PopplioMath::Vec2f prevWorldPosition{ 0.0f, 0.0f };
		PopplioMath::Vec2f prevWorldScale{ 1.0f, 1.0f };
		double prevWorldRotation = 0.0;

		bool localModified = false;

		ParentComponent() = default;

		//ParentComponent()
		//{
  //          children = std::vector<Entity>();
		//	children.clear();
  //          children.reserve(100);
		//	children.push_back(Entity(-1));
		//}

		ParentComponent(Entity parentEntity, bool inheritActiveState = true, bool inheritTransformState = true)
			: parent(parentEntity)
			, inheritActive(inheritActiveState)
			, inheritTransform(inheritTransformState)
		{ 
			//children = std::vector<Entity>();
			//children.clear();
			//children.reserve(100);
			//children.push_back(Entity(-1));
		}

   //     void CloneFrom(const Entity& ent, const Entity& other)
   //     {
			////if (other.GetComponent<ParentComponent>().children.empty()) return;

			////if (children.size() != other.GetComponent<ParentComponent>().children.size()) return;

			//////if (parent.Exists()) parent.GetComponent<ParentComponent>().RemoveChild(ent);

			//////for (auto& child : other.GetComponent<ParentComponent>().children)
			//////{
   //////             Entity e = child.Clone();
			//////	AddChild(e);

			//////	e.GetComponent<ParentComponent>().CloneFrom(e, child);
			//////}

			////// TODO fix
			////for (size_t i{}; i < other.GetComponent<ParentComponent>().children.size(); ++i)
			////{
			////	Entity e = children[i];
   ////             other.GetComponent<ParentComponent>().RemoveChild(e);
			////	AddChild(e);
			////}

   //         if (other.GetComponent<ParentComponent>().parent == -1) return;
			//other.GetComponent<ParentComponent>().parent.GetComponent<ParentComponent>().RemoveChild(ent);
			//AddChild(ent);
   //     }
	
		void AddChild(Entity childEntity, Entity thisEntity)//, bool const& preserveTransform = true)
		{
			if (!childEntity.Exists()) return;

			if (std::find(children.cbegin(), children.cend(), childEntity) == children.end())
			{
				children.push_back(childEntity);

				if (!childEntity.HasComponent<ParentComponent>())
				{
					childEntity.AddComponent<ParentComponent>();
				}

				childEntity.GetComponent<ParentComponent>().parent = thisEntity;
			}
			

            //if (children.size() <= 0) children = std::vector<Entity>();

			//if (children[0] == Entity(-1)) children.clear();

			//if (children.empty()) return;
			//if (std::find(children.begin(), children.end(), childEntity) == children.end())
			//{
			//	children.push_back(childEntity);
			//}
			
			//children.push_back(childEntity);
			
			// Add ParentComponent to child if it doesn't exist
			//if (!childEntity.HasComponent<ParentComponent>())
			//{
			//	childEntity.AddComponent<ParentComponent>(parent);
			//}

			//childEntity.GetComponent<ParentComponent>().parent = parent;

            //if (preserveTransform) InitLocalTransform(childEntity.GetComponent<TransformComponent>());

            //childEntity.GetComponent<ParentComponent>().SetLocalToWorld(childEntity.GetComponent<TransformComponent>());

			//if (childEntity.GetComponent<ParentComponent>().inheritTransform)
			//{
   //             childEntity.GetComponent<ParentComponent>().SyncLocalToWorld(
			//		childEntity.GetComponent<TransformComponent>(), parent.GetComponent<TransformComponent>());
			//}
		}

   //     void InitLocalTransform(const TransformComponent& transform)
   //     {
			//localPosition = transform.position;
			//localScale = transform.scale;
			//localRotation = transform.rotation;
   //     }

		// Convert local transform to world transform
		void UpdateWorldTransform(TransformComponent& transform, const TransformComponent& parentTransform)
		{
			if (!inheritTransform)
			{
				return;
			}

			//if (prevLocalRotation != localRotation)
			//{
			// Calculate world rotation
			transform.rotation = parentTransform.rotation + localRotation;
			//}

			//if (prevLocalRotation != localRotation) SyncLocalToWorld(transform, parentTransform);

			//if (prevLocalScale != localScale)
			//{
			// Calculate world scale
			transform.scale.x = parentTransform.scale.x * localScale.x;
			transform.scale.y = parentTransform.scale.y * localScale.y;
			//}

			//if (prevLocalScale != localScale) SyncLocalToWorld(transform, parentTransform);

			if (prevLocalPosition != localPosition) // || prevLocalRotation != localRotation || prevLocalScale != localScale)
			{
				// Calculate world position using parent's rotation and scale
				double parentRadians = parentTransform.rotation * PopplioMath::M_PI / 180.0f;
				double cosTheta = std::cos(parentRadians);
				double sinTheta = std::sin(parentRadians);

				// Apply parent's rotation and scale to local position
				PopplioMath::Vec2f rotatedLocalPos = {
					static_cast<float>(static_cast<double>(localPosition.x) * cosTheta - 
						static_cast<double>(localPosition.y) * sinTheta),
					static_cast<float>(static_cast<double>(localPosition.x) * sinTheta + 
						static_cast<double>(localPosition.y) * cosTheta)
				};

				// Add parent's position
				transform.position.x = parentTransform.position.x + rotatedLocalPos.x;
				transform.position.y = parentTransform.position.y + rotatedLocalPos.y;

                prevLocalPosition = localPosition;

                SyncLocalToWorld(transform, parentTransform);
			}
			else
			{
				// Calculate world position using parent's rotation and scale
				double parentRadians = parentTransform.rotation * PopplioMath::M_PI / 180.0f;
				double cosTheta = std::cos(parentRadians);
				double sinTheta = std::sin(parentRadians);

				// Apply parent's rotation and scale to local position
				PopplioMath::Vec2f rotatedLocalPos = {
					static_cast<float>((static_cast<double>(localPosition.x) * cosTheta -
						static_cast<double>(localPosition.y) * sinTheta) *
					static_cast<double>(parentTransform.scale.x)),
					static_cast<float>((static_cast<double>(localPosition.x) * sinTheta +
						static_cast<double>(localPosition.y) * cosTheta) *
					static_cast<double>(parentTransform.scale.y))
				};

				// Add parent's position
				transform.position.x = parentTransform.position.x + rotatedLocalPos.x;
				transform.position.y = parentTransform.position.y + rotatedLocalPos.y;

			//	prevLocalPosition = localPosition;
			//	prevLocalScale = localScale;
			//	prevLocalPosition = localPosition;
			}

			prevWorldPosition = transform.position;
			prevWorldScale = transform.scale;
			prevWorldRotation = transform.rotation;
		}

		// Set local to absolute transform of the object
		void SetLocalToWorld(const TransformComponent& transform) // must be same entity
		{
            localPosition = transform.position;
            localScale = transform.scale;
            localRotation = transform.rotation;
		}

        void SetLocalPos(const PopplioMath::Vec2f& pos, 
			TransformComponent& transform, const TransformComponent& parentTransform, 
			const ParentComponent& parentLocalTransform)
        {
            static_cast<void>(parentLocalTransform);

            localPosition = pos;

			// Calculate world position using parent's rotation and scale
			float parentRadians = static_cast<float>(parentTransform.rotation * PopplioMath::M_PI / 180.0f);
			float cosTheta = std::cos(parentRadians);
			float sinTheta = std::sin(parentRadians);

			// Apply parent's rotation and scale to local position
            // do not apply parent's scale as it will mess up everything (do that only during update)
			PopplioMath::Vec2f rotatedLocalPos = 
			{
				(localPosition.x * cosTheta - localPosition.y * sinTheta) * parentTransform.scale.x, //* parentLocalTransform.localScale.x, 
				//* localScale.x, // * parentTransform.scale.x,
                (localPosition.x * sinTheta + localPosition.y * cosTheta) * parentTransform.scale.y //* parentLocalTransform.localScale.y
				//* localScale.y // * parentTransform.scale.y
			};

			// Add parent's position
			transform.position.x = parentTransform.position.x + rotatedLocalPos.x;
			transform.position.y = parentTransform.position.y + rotatedLocalPos.y;

            //SyncLocalToWorld(transform, parentTransform);
        }

		void SetLocalScale(const PopplioMath::Vec2f& scale,
			TransformComponent& transform, const TransformComponent& parentTransform)
		{
            localScale = scale;
			transform.scale.x = parentTransform.scale.x * localScale.x;
			transform.scale.y = parentTransform.scale.y * localScale.y;

			//SyncLocalToWorld(transform, parentTransform);
		}

		void SetLocalRot(const double& rot,
			TransformComponent& transform, const TransformComponent& parentTransform)
		{
            localRotation = rot;

            transform.rotation = parentTransform.rotation + localRotation;

			//SyncLocalToWorld(transform, parentTransform);
		}

		// Set local transform based on current world transform and parent's transform
		void UpdateLocalTransform(const TransformComponent& transform, const TransformComponent& parentTransform)
		{
			if (!inheritTransform)
			{
				return;
			}

            SyncLocalToWorld(transform, parentTransform);
		}

		void SyncLocalToWorld(const TransformComponent& transform, const TransformComponent& parentTransform)
		{
			// Calculate local rotation
			localRotation = transform.rotation - parentTransform.rotation;

			// Calculate local scale
			localScale.x = transform.scale.x / parentTransform.scale.x;
			localScale.y = transform.scale.y / parentTransform.scale.y;

			// Calculate relative position
			PopplioMath::Vec2f relativePosition = {
				transform.position.x - parentTransform.position.x,
				transform.position.y - parentTransform.position.y
			};

			// Reverse parent's rotation and scale
			float parentRadians = static_cast<float>(parentTransform.rotation * PopplioMath::M_PI / 180.0f);
			float cosTheta = std::cos(-parentRadians);
			float sinTheta = std::sin(-parentRadians);

			// Unrotate and unscale the position
			localPosition.x = (relativePosition.x * cosTheta - relativePosition.y * sinTheta) / parentTransform.scale.x;
			localPosition.y = (relativePosition.x * sinTheta + relativePosition.y * cosTheta) / parentTransform.scale.y;

			prevLocalPosition = localPosition;
			prevLocalScale = localScale;
			prevLocalRotation = localRotation;
		}

		void RemoveChild(Entity childEntity)
		{
			// Basic validation
			if (!childEntity.Exists() || childEntity.GetId() < 0)
			{
				Logger::Error("Attempted to remove invalid child entity");
				return;
			}

			auto it = std::find(children.begin(), children.end(), childEntity);
			if (it != children.end())
			{
				// Reset the child's parent reference first
				if (it->HasComponent<ParentComponent>())
				{
					it->GetComponent<ParentComponent>().parent = Entity(-1);
				}
				children.erase(it);
			}

			//while (true)
			//{
			//	auto it = std::find(children.begin(), children.end(), childEntity);
			//	if (it != children.end())
			//	{
			//		it->GetComponent<ParentComponent>().parent = Entity(-1);
			//		children.erase(it);
			//	}
			//	else break;
			//}

   //         if (children.empty()) children.push_back(Entity(-1));

			//if (children.empty()) children = std::vector<Entity>();
		}

		bool LocalIsModified()
		{
            return localPosition != prevLocalPosition || localScale != prevLocalScale || localRotation != prevLocalRotation;
		}

		bool WorldIsModified(const TransformComponent& transform)
		{
			return transform.position != prevWorldPosition || transform.scale != prevWorldScale || 
				transform.rotation != prevWorldRotation;
		}
	};
}