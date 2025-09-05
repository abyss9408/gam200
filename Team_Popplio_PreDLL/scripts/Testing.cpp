/******************************************************************************/
/*!
\file   TemplateScript.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
        This is the source file for a Logic System template script
        Enables scripting functionality by separating game from engine code
        Implements C# Mono Framework (TODO)

        Use this as a C++ template for game scripts

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

// Includes
#include <pch.h>
#include "Testing.h" // header

#include "../src/Logging/Logger.h"

//REGISTER_SCRIPT(BassNKick, TemplateScript);

namespace BassNKick
{
    // Script here (Similar to Unity Monobehaviour)

    //std::string TemplateScript::ScriptName() { return "BassNKick::TemplateScript"; }

    void Testing::Deserialize(std::string const& s)
    {
		static_cast<void>(s);
    }

    void Testing::Serialize()
    {

    }

    Testing& Testing::GetScript()
    {
        return *this;
    }

    // On Initialize
    void Testing::Init(Entity const& obj)
    {
        static_cast<void>(obj);
        Logger::Debug("Testing is initializing!", LogDestination::IMGUI_CONSOLE);
    }

    // On Update
    void Testing::Update(Entity const& obj)
    {
        static_cast<void>(obj);
        Logger::Debug("Testing is updating!", LogDestination::IMGUI_CONSOLE);
    }

    // On Terminate
    void Testing::End(Entity const& obj)
    {
        static_cast<void>(obj);
        Logger::Debug("Testing is ending!", LogDestination::IMGUI_CONSOLE);
    }

    void Testing::OnCollision(CollisionEvent& event)
    {
        static_cast<void>(event);
        Logger::Debug("Testing is colliding!", LogDestination::IMGUI_CONSOLE);
    }
}