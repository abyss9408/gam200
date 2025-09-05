/******************************************************************************/
/*!
\file   Test1.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
        This is the source file of a Logic System script
        Tests Logic System functionality

        Use this as a C++ test1 for game scripts

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

// Includes

#include "Test1.h" // header

#include <../src/Transformation/TransformComponent.h>
#include <../src/Engine/Engine.h>

#include <../src/Input/InputSystem.h>

#include <../src/Logging/Logger.h>

namespace BassNKick
{
    // Script here (Similar to Unity Monobehaviour)

    //::string Test1::ScriptName() { return "BassNKick::Test1"; }

    // On Initialize
    void Test1::Init(Entity const& obj)
    {
        //obj->registry->GetSystem<AudioSystem>().GetAudio(obj->GetComponent<AudioComponent>().audio).Play();
        obj.GetComponent<AudioComponent>().data->Play();
    }

    // On Update
    void Test1::Update(Entity const& obj)
    {
        //obj->GetComponent<TransformComponent>().position.X() += Popplio::Engine::timer.GetFixedDeltaTime();
        //obj->GetComponent<TransformComponent>().position.Y() += Popplio::Engine::timer.GetFixedDeltaTime();

        obj.GetComponent<TransformComponent>().rotation += static_cast<float>(Popplio::Engine::timer.GetFixedDeltaTime()) * 20;
        Logger::Debug("rotating", LogDestination::IMGUI_CONSOLE);

        if (Popplio::InputSystem::IsKeyDown(GLFW_KEY_UP))
        {
            gObj->GetComponent<TransformComponent>().position.y += static_cast<float>(Popplio::Engine::timer.GetFixedDeltaTime()) * 200;
            Logger::Debug("moving", LogDestination::IMGUI_CONSOLE);
        }

        if (Popplio::InputSystem::IsKeyDown(GLFW_KEY_DOWN))
        {
            gObj->GetComponent<TransformComponent>().position.y -= static_cast<float>(Popplio::Engine::timer.GetFixedDeltaTime()) * 200;
            Logger::Debug("moving", LogDestination::IMGUI_CONSOLE);
        }

        if (Popplio::InputSystem::IsKeyDown(GLFW_KEY_LEFT))
        {
            gObj->GetComponent<TransformComponent>().position.x -= static_cast<float>(Popplio::Engine::timer.GetFixedDeltaTime()) * 200;
            Logger::Debug("moving", LogDestination::IMGUI_CONSOLE);
        }

        if (Popplio::InputSystem::IsKeyDown(GLFW_KEY_RIGHT))
        {
            gObj->GetComponent<TransformComponent>().position.x += static_cast<float>(Popplio::Engine::timer.GetFixedDeltaTime()) * 200;
            Logger::Debug("moving", LogDestination::IMGUI_CONSOLE);
        }
    }

    // On Terminate
    void Test1::End(Entity const& obj)
    {
        //obj->GetComponent<AudioComponent>().data->Stop();
        static_cast<void>(obj);
    }

    void Test1::OnCollision(CollisionEvent& event)
    {
        static_cast<void>(event);
        Logger::Debug("Test1 is colliding!", LogDestination::IMGUI_CONSOLE);
    }

    //void Test1::OnKeyPressed(KeyPressedEvent& event)
    //{

    //}

    //void Test1::OnKeyHeld(KeyRepeatedEvent& event)
    //{
    //    switch (event.key)
    //    {
    //    case GLFW_KEY_UP:
    //        gObj->GetComponent<TransformComponent>().position.Y() += Popplio::Engine::timer.GetFixedDeltaTime() * 200;
    //        Logger::Debug("moving", LogDestination::IMGUI_CONSOLE);
    //        break;
    //    case GLFW_KEY_DOWN:
    //        gObj->GetComponent<TransformComponent>().position.Y() -= Popplio::Engine::timer.GetFixedDeltaTime() * 200;
    //        Logger::Debug("moving", LogDestination::IMGUI_CONSOLE);
    //        break;
    //    case GLFW_KEY_LEFT:
    //        gObj->GetComponent<TransformComponent>().position.X() -= Popplio::Engine::timer.GetFixedDeltaTime() * 200;
    //        Logger::Debug("moving", LogDestination::IMGUI_CONSOLE);
    //        break;
    //    case GLFW_KEY_RIGHT:
    //        gObj->GetComponent<TransformComponent>().position.X() += Popplio::Engine::timer.GetFixedDeltaTime() * 200;
    //        Logger::Debug("moving", LogDestination::IMGUI_CONSOLE);
    //        break;
    //    }
    //}
    //void Test1::OnKeyReleased(KeyReleasedEvent& event)
    //{

    //}
}