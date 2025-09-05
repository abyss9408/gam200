/******************************************************************************/
/*!
\file           TransformationSystem.h
\author         Team Popplio
\author         Bryan Ang Wei Ze
\contribution   Bryan - 100%
\par            Course : CSD2401 / CSD2400
\par            Section : A
\date           2024/11/06
\brief
    This is the header file for the Transformation system of entities in the engine

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "../ECS/ECS.h"
#include "TransformComponent.h"
#include "../Collision/BoxColliderComponent.h"
#include "../Math/Matrix2D.h"
#include "../EventBus/EventBus.h"

namespace Popplio
{
    class TransformationSystem : public System
    {
    public:
        TransformationSystem();

        void Update();

        void UpdateModelMatrix(TransformComponent& transform);

        void UpdateColliderMatrix(const TransformComponent& transform, BoxColliderComponent& collider);
    };
}