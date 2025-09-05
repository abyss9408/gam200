/******************************************************************************/
/*!
\file   CollisionSystem.h
\author Team Popplio
\author Rio Shannon Yvon Leonardo - 100%
\par    Course : CSD2400/CSD2401
\par    Section : A
\date   2024/10/03
\brief
        This is the header file to handle collisions responses. It contains
        declarations for CollisionSystem.h

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "../ECS/ECS.h"
#include "../Collision/BoxColliderComponent.h"
#include "../Collision/CircleColliderComponent.h"
#include "../Transformation/TransformComponent.h"
#include "../Physics/RigidBodyComponent.h"
#include "../Layering/LayerManager.h"

#include <iostream>

namespace Popplio
{
    // System
    class CollisionSystem : public System
    {
        EventBus& event;
		LayerManager& layerManager;

        std::unordered_map<int, int> collisionCount{};
        std::unordered_map<int, int> triggerCount{};
        std::set<std::pair<int, int>> collisions{};
        std::set<std::pair<int, int>> triggers{};

    public:
        CollisionSystem(EventBus& e, LayerManager& layerMgr);

        void Update(Popplio::Registry& registry, float deltaTime);
    private: 

        bool CheckBoxToBoxCollision(
            const TransformComponent& transform1,
            const BoxColliderComponent& box1,
            const TransformComponent& transform2,
            const BoxColliderComponent& box2,
            const PopplioMath::Vec2f& vel1,
            const PopplioMath::Vec2f& vel2,
            float& firstCollisionTime, float deltaTime
        );

        bool CheckCircleToCircleCollision(
            const TransformComponent& transform1,
            const CircleColliderComponent& circle1,
            const TransformComponent& transform2,
            const CircleColliderComponent& circle2
        );
        bool CheckBoxToCircleCollision(
            const TransformComponent& transformBox,
            const BoxColliderComponent& box,
            const TransformComponent& transformCircle,
            const CircleColliderComponent& circle
        );
    };
}