/******************************************************************************/
/*!
\file   LogicSystem.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the header file for engine Logic System
        Enables scripting functionality by separating game from engine code
        Implements C# Mono Framework 

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <vector>
#include <memory>

#include "Logic.h" // avoid circular dependency
//#include "LogicScript.h"
//#include "LogicComponent.h"
//#include "LogicData.h"

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../AssetStore/AssetStore.h"
#include "../Layering/LayerManager.h"

#include "../Utilities/OnAppQuit.h"

#include "../Mono/MonoAPI.h" // C# Script API

#include <functional>
#include <algorithm>
//#include <any>

namespace Popplio
{
    class ScriptFactory
    {
        public:
        template <class T>
        static std::shared_ptr<T> Create(Entity* e);
    };

    /*
    *   script registry for c++ scripts
    */
    class ScriptRegistry
    {
        //using FactoryFunc = std::function<std::shared_ptr<std::any>(Entity* e)>;
        //using FactoryFunc = std::function<std::shared_ptr<ILogicScript>(Entity* e)>;

        public:

        ScriptRegistry();

        /*
        *   register c++ script
        */
        void RegisterScript(std::string const& scriptName);

        /*
        *   create instance of c++ script
        */
        template <class T = ILogicScript>
        std::shared_ptr<T> Create(std::string const& scriptName, Entity* e) const;

        private:

        //std::unordered_map<std::string, ScriptFactory&> factories; // registry
        std::vector<std::string> scripts; // registry
        //template <class T>    
        //static std::shared_ptr<ILogicScript> CPPScriptFactory(Entity* e);
    };

    //static ScriptRegistry scriptReg {}; // registry for scripts

    // macro to register script to registry
   /* #define REGISTER_SCRIPT(scriptClass) \
        static bool scriptClass##_registered = []() \
        { \
            scriptReg.RegisterScript<scriptClass>(#scriptClass); \
            return true; \
        } (); \*/

    //#define REGISTER_SCRIPT(NAMESPACE, SCRIPT) \
    //        static bool _##NAMESPACE##_##SCRIPT##_registered = []() \
    //        { \
    //            Popplio::scriptReg.RegisterScript<NAMESPACE::SCRIPT>(#NAMESPACE"::"#SCRIPT); \
    //            return true; \
    //        }(); \

    class DataRegistry // data (wip)
    {
        using FactoryFunc = std::function<std::shared_ptr<ILogicData>()>;

    public:

        template<typename T>
        void RegisterData(std::string const& dataName);

        std::shared_ptr<ILogicData> Create(std::string const& dataName) const;

    private:

        std::unordered_map<std::string, FactoryFunc> factories; // registry
    };

    static DataRegistry dataReg{}; // registry for scripts

    /*
    *   logic system of c++ and c# scripts
    */
    class LogicSystem : public System //: public ILogicSystem
    {
        private:
        Registry& reg;
        EventBus& eventBus;
        LayerManager& lay;

        double currentCycleElapsedTime{};
        double currentGCCycleElapsedTime{};

        // C++
        ScriptRegistry scriptReg;

        std::unordered_map<std::string, std::shared_ptr<ILogicScript>> scripts; // pointer to asset store scripts
        std::unordered_map<int, std::shared_ptr<ILogicScript>> instScripts; // instantiated scripts

        std::unordered_map<std::string, std::shared_ptr<ILogicData>> data; // components // doesnt work

        protected:
        std::vector<Entity> entities; // todo: remove overload in ECS since this solution has been found

        // C#
        std::vector<uint32_t> csLoadedScripts; // cs scripts loaded in current run
        std::vector<uint32_t> csScriptsToBeDeleted; // scripts to be deleted

        std::unordered_map<uint32_t, std::string> csScriptNames{}; // script names
        std::unordered_map<int, uint32_t> csEntityLoadedScripts{}; // entity scripts
        std::unordered_map<uint32_t, int> csLoadedScriptsEntityId{}; // script entity ids

        std::vector<uint32_t> currentLoop{}; // current loop scripts

        //std::vector<MonoClass*> csScripts; // cs scripts from mono
        //std::vector<MonoObject*> csLoadedScripts; // cs scripts loaded in current run
        //std::vector<MonoObject*> csScriptsToBeDeleted; // scripts to be deleted
        std::vector<std::unordered_map<std::string, bool>> csScriptsOverridden
            {std::vector<std::unordered_map<std::string, bool>>(5)}; // scripts that have overridden loop methods
        //std::unordered_map<MonoObject*, std::string> csScriptNames{}; // script names
        //std::unordered_map<int, MonoObject*> csEntityLoadedScripts{}; // entity scripts
        //std::unordered_map<MonoObject*, int> csLoadedScriptsEntityId{}; // script entity ids

        //std::vector<MonoObject*> currentLoop{}; // current loop scripts

        //int fixedSteps{}; // fixed update steps

        public:

        PopplioScriptAPI::MonoAPI& monoAPI;
        bool reloadMono{ false }; // reload mono assembly


        LogicSystem() = delete;
        /*
        *   logic system argument constructor
        */
        LogicSystem(Registry& registry, EventBus& event, LayerManager& layer, PopplioScriptAPI::MonoAPI& mono);
        /*
        *   logic system destructor to delete behaviours and components
        */
        ~LogicSystem();

        /*
        *   system base func to subscribe to engine events
        */
        void SubscribeToEvents(EventBus& event);

        //void Register(LogicComponent const& cmp); // register scripts and components hard coded for now // todo: serialize

        /*
        *   initialization
        */
        void Initialization();

        /*
        *   setup scripts
        */
        void Setup(); 
        /*
        *   initialize scripts
        */
        void Init(); 
        /*
        *   update scripts
        */
        void Update(); 
        /*
        *   fixed update scripts
        */
        void FixedUpdate(); 
        /*
        *   clean up scripts
        */
        void End();

        //void AddScript(std::shared_ptr<ILogicScript> scp);
        //void AddComponent(std::shared_ptr<LogicComponent> cmp);

        /*
        *	Sets inspector deserialized assigned values to script variables at setup
        */
        void SetLogicComponentRuntimeVars(Entity e);

        /*
        *	check if script is active on entity
        */
        bool IsScriptActive(Entity& e);

        /*
        *	update script vars from original entity
        */
        void UpdateFromCopy(const Entity e, const Entity other, const Entity rootE, const Entity rootOther);

        // C++

        /*
        *   load c++ script by name
        */
        void LoadByName(std::string const& script);

        /*
        *   load c++ script by type
        */
        template <class T = ILogicScript>
        void LoadScript(std::string const& scriptType);

        /*
        *   instantiate c++ script by type
        */
        template <class T = ILogicScript>
        std::shared_ptr<T> InstScript(std::string const& scriptType, Entity* e);

        /*
        *	instantiate script by name
        */
        void InstScriptByName(std::string const& scriptType, Entity* e);

        /*
        *   collision handler
        */
        void CPPOnCollision(CollisionEvent& event);

        /*
        *   trigger handler
        */
        void CPPOnTrigger(TriggerEvent& event);

        /*
        *   entity change handler
        */
        void CPPOnEntityChange(EntityRuntimeChangeEvent& event);

        /*
        *   entity create handler
        */
        void CPPOnEntityCreate(int const& entity);

        /*
        *   entity destroy handler
        */
        void CPPOnEntityDestroy(int const& entity);

        /*
        *   entity active handler
        */
        void CPPOnActiveChange(Entity& e);

        /*
        *   on app quit handler
        */
        void CPPOnAppQuit(PopplioUtil::OnAppQuit& event);

        // C#

        /*
        *   load c# scripts that are in assembly
        */
        void LoadCSScripts();
        bool LoadCSScript(Entity& i);
        /*
        *   unload c# scripts that are loaded
        */
        void UnloadCSScripts();

        /*
        *   setup c# script
        */
        MonoObject* SetupCSScript(Entity& i);

        /*
        *   delete c# scripts
        */
        void CSDeleteScripts();

        /*
        *   collision handler
        */
        void CSOnCollision(CollisionEvent& event);

        /*
        *   trigger handler
        */
        void CSOnTrigger(TriggerEvent& event);

        /*
        *   entity change handler
        */
        void CSOnEntityChange(EntityRuntimeChangeEvent& event);

        /*
        *   entity create handler
        */
        void CSOnEntityCreate(int const& entity, MonoObject* script);

        /*
        *   entity destroy handler
        */
        void CSOnEntityDestroy(int entity, MonoObject* script);

        /*
        *   entity active handler
        */
        void CSOnActiveChange(Entity& e, MonoObject* script);

        /*
        *   on app quit handler
        */
        void CSOnAppQuit(PopplioUtil::OnAppQuit& event);

        /*
        *   enqueue entity kill for scripts to use to avoid errors
        */
        //void KillEntity(Entity& e);

        void CSRemoveScriptObj(int const& entityID);

        /*
        *   load scripts at runtime
        */
        void CSLoadScriptAtRun(MonoObject* obj);

        /*
        *   add script to loaded scripts
        */
        void CSAddToLoadedScripts(int id);

        void CSUpdateLogicObjects();
    };
}

// Template Function Definitions //

#include "CPPScripts.h"