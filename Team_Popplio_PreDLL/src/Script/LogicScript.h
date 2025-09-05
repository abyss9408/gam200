/******************************************************************************/
/*!
\file   LogicScript.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
        This is the header file for engine Logic System script interface
        Enables scripting functionality by separating game from engine code
        Able to be attached to entities (gameobjects)
        Implements C# Mono Framework (TODO)

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "../ECS/ECS.h"

#include "../EventBus/Event.h"

#include "../Collision/CollisionEvent.h"

#include "LogicData.h"

#include <memory>
#include <vector>
#include <typeinfo>

#define VAR_NAME(var) #var // get variable name

namespace Popplio
{
    class ILogicScript // scripts inherit this
    {
        private:
        static std::unordered_map<std::string, ILogicScript&> scripts; // scripts
        
        protected:
        //Entity* gObj{ nullptr };
        //Entity gObj;

        public:

        Entity* gObj{ nullptr };

        static std::unordered_map<std::string, ILogicScript&>& GetAllScripts() { return scripts; }

        ILogicScript(Entity* obj) : gObj(obj) {}

        /*
        *   get name of script (currently doesnt work)
        */
        std::string GetName()
        {
            //for (std::pair<std::string const, ILogicScript&> s : scripts)
            //{
            //    if (typeid(s.second).name() == typeid(*this).name()) return s.first;
            //}
            return "";
        }

        /*
        *   deserialize script (WIP)
        */
        virtual void Deserialize(std::string const& s)  { scripts.emplace(s, *this); }
        /*
        *   serialize script (WIP)
        */
        virtual void Serialize()                        {}

        /*
        *   get script as ILogicScript
        */
        virtual ILogicScript& GetScript() { return *this; }

        /*
        *   base logic loop | setup
        */
        virtual void Setup(Entity const& obj)         { static_cast<void>(obj); }
        /*
        *   base logic loop | initialization
        */
        virtual void Init(Entity const& obj)          { static_cast<void>(obj); }
        /*
        *   base logic loop | update
        */
        virtual void Update(Entity const& obj)        { static_cast<void>(obj); }
        /*
        *   base logic loop | fixed update
        */
        virtual void FixedUpdate(Entity const& obj)   { static_cast<void>(obj); }
        /*
        *   base logic loop | end
        */
        virtual void End(Entity const& obj)           { static_cast<void>(obj); }

        /*
        *   collision callee in the event of collision (currently not limited to between colliding objects only)
        */
        virtual void OnCollision(CollisionEvent& event) { static_cast<void>(event); }

        virtual void OnTrigger(TriggerEvent& event) { static_cast<void>(event); }

        //virtual void OnKeyPressed(KeyPressedEvent& event) { static_cast<void>(event); }
        //virtual void OnKeyHeld(KeyRepeatedEvent& event) { static_cast<void>(event); }
        //virtual void OnKeyReleased(KeyReleasedEvent& event) { static_cast<void>(event); }
    };

    //std::unordered_map<std::string, ILogicScript&> ILogicScript::scripts {};
}