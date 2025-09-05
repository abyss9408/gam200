/******************************************************************************/
/*!
\file   CircleColliderComponent.h
\author Team Popplio
\author Rio Shannon Yvon Leonardo - 100%
\par    Course : CSD2400/CSD2401
\par    Section : A
\date   2024/10/03
\brief
        This header file defines the BoxColliderComponent struct, which contains 
        the data for a circle-shaped collider to be used for collision detection

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "../Math/MatVec.h"

namespace Popplio
{
    struct CircleColliderComponent
    {
        bool isActive;

        float radius;       //radius of the circle collider
        PopplioMath::Vec2f offset;   //offset from entity position
        PopplioMath::M3x3f colliderMatrix;  //transform matrix for scaling

        bool isTrigger;     //acts a trigger for collision
        bool isEnabled;     //checking if collider is enabled

        // runtime // do not serialize
        bool isColliding;
        bool isTriggering;

        CircleColliderComponent(
            float r = 0.5f,
            float offsetX = 0,
            float offsetY = 0,
            bool trigger = false,
            bool enabled = true
        ) : 
            isActive(true), 
            radius(r), 
            offset(offsetX, offsetY), 
            isTrigger(trigger), 
            isEnabled(enabled),
            isColliding(false),
            isTriggering(false) 
        {}
    };
}
