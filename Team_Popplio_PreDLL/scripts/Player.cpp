/******************************************************************************/
/*!
\file   Player.cpp
\author Team Popplio
\author Rio Shannon Yvon Leonardo
\contribution Shannon - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/30 (Last Edit)
\brief
    This is the source file that currently implements the player 
    functionality for the game.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

// Includes
#include <pch.h>
#include "Player.h" // header

#include "../src/Logging/Logger.h"
#include "../src/Physics/MovementSystem.h"
#include "../src/Collision/BoxColliderComponent.h"
#include "../src/Input/InputSystem.h"

//REGISTER_SCRIPT(BassNKick, TemplateScript);

// Notes:
// Shouldn't be interfacing with the system! use the components instead
// For now, you shouldn't add components via script

namespace BassNKick
{
    // Script here (Similar to Unity Monobehaviour)

    //std::string TemplateScript::ScriptName() { return "BassNKick::TemplateScript"; }

    void Player::Deserialize(std::string const& s)
    {
        static_cast<void>(s);
    }

    void Player::Serialize()
    {

    }

    Player& Player::GetScript()
    {
        return *this;
    }

    // On Initialize
    void Player::Init(Entity const& obj)
    {
        // Get or add the RigidBodyComponent
        //if (!obj.HasComponent<RigidBodyComponent>())
        //{
        //    obj.AddComponent<RigidBodyComponent>();
        //}

        if (!obj.HasComponent<RigidBodyComponent>()) return;

        auto& rigidBody = obj.GetComponent<RigidBodyComponent>();
        auto& boxCollider = obj.GetComponent<BoxColliderComponent>();

        boxCollider.isTrigger = false;

        //enable Gravity
        rigidBody.useGravity = true;
        //adjust the values as needed
        rigidBody.mass = 1.0f;
        rigidBody.gravityScale = 4.0f;
        rigidBody.dragCoefficient = 6.0f;

       //m_entity = obj;  // Store the entity pointer
        Logger::Debug("Player is initializing!", LogDestination::IMGUI_CONSOLE);
    }

    // On Update
    void Player::Update(Entity const& obj)
    {
        //get movement system from registry through the entity?
        auto& movementSystem = obj.registry->GetSystem<Popplio::MovementSystem>();

        ////handle key inputs for movement 
        //position is random for now
        if (Popplio::InputSystem::IsKeyDown(GLFW_KEY_R)) //press the R key
        {
            movementSystem.SetPositionY(obj, 250.0f);
            Logger::Debug("R button is pressed!", LogDestination::IMGUI_CONSOLE);

        }
        else if (Popplio::InputSystem::IsKeyDown(GLFW_KEY_F)) //press the F key
        {
            movementSystem.SetPositionY(obj, 100.0f);
            Logger::Debug("F button is pressed!", LogDestination::IMGUI_CONSOLE);
        }
        else if (Popplio::InputSystem::IsKeyDown(GLFW_KEY_V)) //press the V key
        {
            movementSystem.SetPositionY(obj, -50.0f);
            Logger::Debug("V button is pressed!", LogDestination::IMGUI_CONSOLE);
        }
        
        //Logger::Debug("Player is updating!", LogDestination::IMGUI_CONSOLE);
    }

    // On Terminate
    void Player::End(Entity const& obj)
    {
        static_cast<void>(obj);
        Logger::Debug("Player is ending!", LogDestination::IMGUI_CONSOLE);
    }

    void Player::OnCollision(CollisionEvent& event)
    {
        static_cast<void>(event);
        Logger::Debug("Player is colliding!", LogDestination::IMGUI_CONSOLE);
    }
}