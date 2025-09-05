/******************************************************************************/
/*!
\file   BoxColliderComponent.h
\author Team Popplio
\author Rio Shannon Yvon Leonardo - 100%
\par    Course : CSD2400/CSD2401
\par    Section : A
\date   2024/10/03
\brief
        This header file defines the BoxColliderComponent struct, which presents
        the data for a rectangular (or square) collider that can be used for
        collision detection

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
    struct BoxColliderComponent
    {
        bool isActive;

        PopplioMath::Vec2f size;
        PopplioMath::Vec2f offset;
        PopplioMath::M3x3f colliderMatrix;

        bool isTrigger;     //acts a trigger for collision
        bool isEnabled;     //checking if collider is enabled

        // runtime // do not serialize
        bool isColliding;
        bool isTriggering;

        BoxColliderComponent (
            float w = 1,
            float h = 1, 
            float offsetX = 0,
            float offsetY = 0, 
            bool trigger = false, 
            bool enabled = true
        ) :
            isActive(true),
            size(w, h), 
            offset(offsetX, offsetY), 
            isTrigger(trigger), 
            isEnabled(enabled),
            isColliding(false),
            isTriggering(false)
        {}
    };
}
