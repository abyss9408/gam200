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

#include "TemplateScript.h" // header

#include "../src/Logging/Logger.h"

//REGISTER_SCRIPT(BassNKick, TemplateScript);

namespace BassNKick
{
    // Script here (Similar to Unity Monobehaviour)

    //std::string TemplateScript::ScriptName() { return "BassNKick::TemplateScript"; }

    void TemplateScript::Deserialize(std::string const& s)
    {
		static_cast<void>(s);
    }

    void TemplateScript::Serialize()
    {

    }

    TemplateScript& TemplateScript::GetScript()
    {
        return *this;
    }

    // On Initialize
    void TemplateScript::Init(Entity const& obj)
    {
        static_cast<void>(obj);
        Logger::Debug("TemplateScript is initializing!", LogDestination::IMGUI_CONSOLE);
    }

    // On Update
    void TemplateScript::Update(Entity const& obj)
    {
        static_cast<void>(obj);
        Logger::Debug("TemplateScript is updating!", LogDestination::IMGUI_CONSOLE);
    }

    // On Terminate
    void TemplateScript::End(Entity const& obj)
    {
        static_cast<void>(obj);
        Logger::Debug("TemplateScript is ending!", LogDestination::IMGUI_CONSOLE);
    }

    void TemplateScript::OnCollision(CollisionEvent& event)
    {
        if (event.entity1Id != gObj->GetId() && event.entity2Id != gObj->GetId()) return;

        Logger::Debug("TemplateScript is colliding!", LogDestination::IMGUI_CONSOLE);
    }
}