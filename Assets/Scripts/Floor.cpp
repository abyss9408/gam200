/******************************************************************************/
/*!
\file   Floor.cpp
\author Team Popplio
\author Rio Shannon Yvon Leonardo
\contribution Shannon - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/30 (Last Edit)
\brief
    This is the source file for the floor implementation for the game.
    Currently initializes the floor component for the game.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

// Includes
#include <pch.h>
#include "Floor.h" // header

#include "../src/Logging/Logger.h"
#include "../src/Physics/RigidBodyComponent.h"
#include "../src/Collision/BoxColliderComponent.h"

//REGISTER_SCRIPT(BassNKick, TemplateScript);

namespace BassNKick
{
    // Script here (Similar to Unity Monobehaviour)

    //std::string TemplateScript::ScriptName() { return "BassNKick::TemplateScript"; }

    void Floor::Deserialize(std::string const& s)
    {
        static_cast<void>(s);
    }

    void Floor::Serialize()
    {

    }

    Floor& Floor::GetScript()
    {
        return *this;
    }

    // On Initialize
    void Floor::Init(Entity const& obj)
    {
        if (!obj.HasComponent<RigidBodyComponent>()) return;
        if (!obj.HasComponent<BoxColliderComponent>()) return;

        // Add RigidBody if not exists
        //if (!obj->HasComponent<RigidBodyComponent>())
        //{
        //    obj->AddComponent<RigidBodyComponent>();
        //    auto& rigidBody = obj->GetComponent<RigidBodyComponent>();
        //    rigidBody.useGravity = false;  // Floor shouldn't fall
        //    rigidBody.mass = 1.0f;
        //    rigidBody.velocity.x = 0.0f;  // Floor doesn't move
        //    rigidBody.velocity.y = 0.0f;
        //}

        // Add BoxCollider if not exists
        //if (!obj->HasComponent<BoxColliderComponent>())
        //{
        //    obj->AddComponent<BoxColliderComponent>(
        //        FLOOR_WIDTH,    // width
        //        FLOOR_HEIGHT,   // height
        //        0.0f,          // offset X
        //        0.0f,          // offset Y
        //        false,         // isTrigger - false for physical collisions
        //        true           // isEnabled
        //    );
        //}

        auto& rigidBody = obj.GetComponent<RigidBodyComponent>();
        rigidBody.useGravity = false;  // Floor shouldn't fall
        rigidBody.mass = 1.0f;
        rigidBody.velocity.x = 0.0f;  // Floor doesn't move
        rigidBody.velocity.y = 0.0f;

        auto& collider = obj.GetComponent<BoxColliderComponent>();
        collider.size.x = FLOOR_WIDTH;    // width
        collider.size.y = FLOOR_HEIGHT;   // height
        collider.offset.x = 0.f;          // offset X
        collider.offset.y = 0.f;          // offset Y
        collider.isTrigger = false;         // isTrigger - false for physical collisions
        collider.isEnabled = true;           // isEnabled

        Logger::Debug("Floor is initializing!", LogDestination::IMGUI_CONSOLE);
    }

    // On Update
    void Floor::Update(Entity const& obj)
    {
        static_cast<void>(obj);
        Logger::Debug("Floor is updating!", LogDestination::IMGUI_CONSOLE);
    }

    // On Terminate
    void Floor::End(Entity const& obj)
    {
        static_cast<void>(obj);
        Logger::Debug("Floor is ending!", LogDestination::IMGUI_CONSOLE);
    }

    void Floor::OnCollision(CollisionEvent& event)
    {
        static_cast<void>(event);
        Logger::Debug("Floor is colliding!", LogDestination::IMGUI_CONSOLE);
    }
}