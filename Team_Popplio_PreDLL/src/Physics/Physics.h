/******************************************************************************/
/*!
\file   Physics.h
\author Team Popplio
\author Rio Shannon Yvon Leonardo - 100%
\par    Course : CSD2400/CSD2401
\par    Section : A
\date   2024/10/03
\brief
         This header file provides essential physics functionalities for the Popplio
         game engine, including collision detection and gravity application.

         The functions and classes within this file facilitate the implementation of
         physics interactions between two objects.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "../Math/MathLib.h"
#include "../Physics/RigidBodyComponent.h"
#include "../ECS/ECS.h"
#include <iostream>

namespace PopplioPhysics
{
    struct AABB
    {
        PopplioMath::Vec2f min; //bottom left corner (minimum x and y values)
        PopplioMath::Vec2f max; //top right hand corner (maximum x and y values)
    };

    bool PointRectangleCollision(PopplioMath::Vec2f const& point, float left, float right, float top, float bottom);

    bool RectangleToRectangleCollision(float leftA, float rightA, float topA, float bottomA, float leftB, float rightB, float topB, float bottomB);

    bool CircleToCircleCollision(PopplioMath::Vec2f const& centerA, float radiusA, PopplioMath::Vec2f centerB, float radiusB);

    bool BoxToCircleCollision(float boxLeft, float boxRight, float boxTop, float boxBottom, PopplioMath::Vec2f const& circleCenter, float circleRadius);

    //AABB Rect-Rect Collision
    bool AABB_RectangleRectangleCollision(const AABB& aabb1, const PopplioMath::Vec2f& vel1, const AABB& aabb2, const PopplioMath::Vec2f& vel2,
        float& firstTimeOfCollision, float GlobalDeltaTime);

    class Gravity
    {

    public:

        //overload constructor
        Gravity(double gravityAcceleration);

        //void ApplyGravity(Popplio::RigidBodyComponent& rigidBody, double deltaTime);

    private:

        double gravity;

    };
}