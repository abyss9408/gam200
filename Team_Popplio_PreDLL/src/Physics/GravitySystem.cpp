/******************************************************************************/
/*!
\file   GravitySystem.cpp
\author Team Popplio
\author Rio Shannon Yvon Leonardo - 100%
\par    Course : CSD2400/CSD2401
\par    Section : A
\date   2024/10/03
\brief
        This file defines the implementations of the functions declared in
        GravitySystem.h.

        It is responsible to stimulate the effects of Gravity in Physics

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include "../Math/Vector2D.h"
#include "../ECS/ECS.h" 
#include "../Physics/RigidBodyComponent.h" 
#include "../Physics/Physics.h"
#include "../Engine/Engine.h"
#include "../Logging/Logger.h"
#include "../Physics/GravitySystem.h"
#include <iostream>

namespace {
    const float base_gravity = -1000.0f;
}

namespace Popplio {

    GravitySystem::GravitySystem() {
        RequireComponent<RigidBodyComponent>();
    }

    //apply the force to a rigidBody
    void GravitySystem::ApplyForce(RigidBodyComponent& rigidBody, float forceX, float forceY) {
        rigidBody.force.x += forceX;
        rigidBody.force.y += forceY;
    }


    void GravitySystem::Update() {
        for (Entity entity : GetSystemEntities())
        {
            if (entity.GetComponent<ActiveComponent>().isActive == false) continue;
            if (!(entity.GetComponent<RigidBodyComponent>().isActive)) continue;

            auto& rigidBody = entity.GetComponent<RigidBodyComponent>();

            // to check for useGravity entity
            //Logger::Debug("Entity " + std::to_string(entity.GetId()) + " useGravity: " + std::to_string(rigidBody.useGravity)); //debug

            if (rigidBody.useGravity == true)
            {
                // Apply gravity force ( F = mg )
                float gravityForce = base_gravity * rigidBody.gravityScale * rigidBody.mass;
                ApplyForce(rigidBody, 0.0f, gravityForce);
            }

        }
    }

}

