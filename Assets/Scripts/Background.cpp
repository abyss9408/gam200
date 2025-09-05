/******************************************************************************/
/*!
\file   Background.cpp
\author Team Popplio
\author Rio Shannon Yvon Leonardo
\contribution Shannon - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/30 (Last Edit)
\brief
      This is the source file for the background functionality for the game.
      Currently implements a scrolling system for the background.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/


// Includes
#include <pch.h>
#include "Background.h" // header
#include "../Transformation/TransformationSystem.h"
#include "../Engine/Engine.h" //for delta Time

#include "../src/Logging/Logger.h"

//REGISTER_SCRIPT(BassNKick, TemplateScript);

namespace BassNKick
{
    // Script here (Similar to Unity Monobehaviour)

    //std::string TemplateScript::ScriptName() { return "BassNKick::TemplateScript"; }

    void Background::Deserialize(std::string const& s)
    {
        static_cast<void>(s);
    }

    void Background::Serialize()
    {

    }

    Background& Background::GetScript()
    {
        return *this;
    }

    // On Initialize
    void Background::Init(Entity const& obj)
    {
        //set initial position
        auto& transform = obj.GetComponent<TransformComponent>();
        // Set initial position based on tag
        if (obj.HasTag("Background-Between2"))
        {
            transform.position.x = INITIAL_X2;
        }
        else // Background-Between
        {
            transform.position.x = INITIAL_X1;
        }
        transform.position.y = RESET_Y;

        Logger::Debug("Background is initializing!", LogDestination::IMGUI_CONSOLE);
    }

    // On Update
    void Background::Update(Entity const& obj)
    {
        auto& transform = obj.GetComponent<TransformComponent>();
        //get the delta Time from Engine's static timer
        float deltaTime = static_cast<float>(Popplio::Engine::timer.GetFixedDeltaTime());

        //move left continously
        float currentX = transform.position.x;
        currentX -= MOVE_SPEED * deltaTime;

        //get the other background's position
           // Get the other background's position
        try
        {
            Entity otherBackground = obj.HasTag("Background-Between")
                ? obj.registry->GetEntityByTag("Background-Between2")
                : obj.registry->GetEntityByTag("Background-Between");

            auto& otherTransform = otherBackground.GetComponent<TransformComponent>();
            float otherX = otherTransform.position.x;

            // If this background goes too far left
            if (currentX <= -BACKGROUND_WIDTH)
            {
                // Place it right after the other background
                currentX = otherX + BACKGROUND_WIDTH;
            }
        }
        catch (const std::runtime_error&)
        {
            // Handle case where other background entity is not found
            Logger::Debug("Could not find other background entity");
        }

        //update the position of the background
        transform.position.x = currentX;
        transform.position.y = RESET_Y; //keep Y position constant

       // Logger::Debug("Background is updating!", LogDestination::IMGUI_CONSOLE);
    }

    // On Terminate
    void Background::End(Entity const& obj)
    {
        static_cast<void>(obj);
        Logger::Debug("Background is ending!", LogDestination::IMGUI_CONSOLE);
    }

    void Background::OnCollision(CollisionEvent& event)
    {
        static_cast<void>(event);
        Logger::Debug("Background is colliding!", LogDestination::IMGUI_CONSOLE);
    }
}