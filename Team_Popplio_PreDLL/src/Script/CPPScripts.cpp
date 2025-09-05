/******************************************************************************/
/*!
\file   CPPScripts.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\co-author Rio Shannon Yvon Leonardo
\contribution Shawn - 10% | Shannon - 90%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the source file for C++ scripts to be loaded into the engine
        TODO : Try to not hard code the script registration like here

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>

#include "LogicSystem.h"

// Scripts

#include "../../scripts/TemplateScript.h" // temp todo: fix
#include "../../scripts/Test1.h" // temp todo: fix

#include "../../scripts/TemplateComponent.h"
#include "../../scripts/Testing.h"

#include "../../scripts/Player.h" //player script 
#include "../../scripts/Audio.h" //audio script
#include "../../scripts/Background.h" //background script
#include "../../scripts/Floor.h" //floor script (idk why we need one but we just need one)

namespace Popplio
{
    void LogicSystem::LoadByName(std::string const& script) // C++ scripts
    {
        if (script == "BassNKick::TemplateScript")
        {
            LoadScript<BassNKick::TemplateScript>(script);
            return;
        }
        if (script == "BassNKick::Test1")
        {
            LoadScript<BassNKick::Test1>(script);
            return;
        }

        //for Testing Script
        if (script == "BassNKick::Testing")
        {
            LoadScript<BassNKick::Testing>(script);
            return;
        }

        //for PlayerScript
        if (script == "BassNKick::Player")
        {
            LoadScript<BassNKick::Player>(script);
            return;
        }

        //for AudioScript
        if (script == "BassNKick::Audio")
        {
            LoadScript<BassNKick::Audio>(script);
            return;
        }

        //for BackgroundScript
        if (script == "BassNKick::Background")
        {
            LoadScript<BassNKick::Background>(script);
            return;
        }

        //for floor script (WIP)
        if (script == "BassNKick::Floor")
        {
            LoadScript<BassNKick::Floor>(script);
            return;
        }
    }

    void LogicSystem::InstScriptByName(std::string const& scriptType, Entity* e)
    {
        if (scriptType == "BassNKick::TemplateScript")
        {
            InstScript<BassNKick::TemplateScript>(scriptType, e);
            return;
        }
        if (scriptType == "BassNKick::Test1")
        {
            InstScript<BassNKick::Test1>(scriptType, e);
            return;
        }

        //for Testing Script
        if (scriptType == "BassNKick::Testing")
        {
            InstScript<BassNKick::Testing>(scriptType, e);
            return;
        }

        //for PlayerScript
        if (scriptType == "BassNKick::Player")
        {
            InstScript<BassNKick::Player>(scriptType, e);
            return;
        }

        //for AudioScript
        if (scriptType == "BassNKick::Audio")
        {
            InstScript<BassNKick::Audio>(scriptType, e);
            return;
        }

        //for BackgroundScript
        if (scriptType == "BassNKick::Background")
        {
            InstScript<BassNKick::Background>(scriptType, e);
            return;
        }

        //for floor script (WIP)
        if (scriptType == "BassNKick::Floor")
        {
            InstScript<BassNKick::Floor>(scriptType, e);
            return;
        }
    }

    // Register script to registry
    void ScriptRegistry::RegisterScript(std::string const& scriptName)
    {
        scripts.push_back(scriptName);
    }

    ScriptRegistry::ScriptRegistry() : scripts() {}

    void LogicSystem::CPPOnCollision(CollisionEvent& event)
    {
        for (auto& [i, s] : instScripts)
        {
            s->OnCollision(event);
            //eventBus.SubscribeToEvent<CollisionEvent>(&(*s), &ILogicScript::OnCollision);
        }
    }

    void LogicSystem::CPPOnTrigger(TriggerEvent& event)
    {
        for (auto& [i, s] : instScripts)
        {
            s->OnTrigger(event);
        }
    }

    void LogicSystem::CPPOnEntityChange(EntityRuntimeChangeEvent& event)
    {
        static_cast<void>(event);

        if (instScripts.empty()) return;
        Logger::Warning("LogicSystem: CPPOnEntityChange | Not implemented");
    }

    void LogicSystem::CPPOnEntityCreate(int const& entity)
    {
        static_cast<void>(entity);

        if (instScripts.empty()) return;
        Logger::Warning("LogicSystem: CPPOnEntityCreate | Not implemented");
    }

    void LogicSystem::CPPOnEntityDestroy(int const& entity)
    {
        if (instScripts.empty()) return;

        // Call script OnDestroy

        // Remove from instScripts
        instScripts.erase(entity);

        Logger::Warning("LogicSystem: CPPOnEntityDestroy | Not implemented");
    }

    void LogicSystem::CPPOnActiveChange(Entity& e)
    {
        static_cast<void>(e);

        if (instScripts.empty()) return;
        Logger::Warning("LogicSystem: CPPOnActiveChange | Not implemented");
    }

    void LogicSystem::CPPOnAppQuit(PopplioUtil::OnAppQuit& event)
    {
        static_cast<void>(event);

        if (instScripts.empty()) return;
        Logger::Warning("LogicSystem: CPPOnAppQuit | Not implemented");
    }
}