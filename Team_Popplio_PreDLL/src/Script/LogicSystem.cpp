/******************************************************************************/
/*!
\file   LogicSystem.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\co-author Rio Shannon Yvon Leonardo
\contribution Shawn - 98% Shannon - 2%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the source file for engine Logic System
        Enables scripting functionality by separating game from engine code
        Implements C# Mono Framework 

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>

#include "LogicSystem.h"

#include "../Logging/Logger.h"

//#include "../Utilities/Any.h"

#include "../Utilities/QueueSystem.h"
#include "../Hierarchy/HierarchySystem.h"

#include "../Engine/Engine.h"

#ifndef IMGUI_DISABLE
#include "../Editor/Editor.h"
#endif

#include <unordered_map>
#include <functional>
#include <string>

// events
#include "../Collision/CollisionEvent.h"

namespace Popplio
{
    // Create a data type
    std::shared_ptr<ILogicData> DataRegistry::Create(std::string const& dataName) const
    {
        auto it = factories.find(dataName);

        if (it != factories.end()) return (it->second)();

        Logger::Error("LogicSystem: Failed to create data " + dataName + " in DataRegistry", LogDestination::IMGUI_CONSOLE);
        throw std::runtime_error("LogicSystem: Failed to create data " + dataName + " in DataRegistry");
    }

    // ******************************************************************************* //

    LogicSystem::LogicSystem(Registry& registry, EventBus& event, LayerManager& layer, PopplioScriptAPI::MonoAPI& mono)
        : scriptReg(), reg(registry), eventBus(event), lay(layer), monoAPI(mono)
    {
        RequireComponent<LogicComponent>();
    }

    LogicSystem::~LogicSystem() // delete scripts
    {

    }

    void LogicSystem::SubscribeToEvents(EventBus& event)
    {
        event.SubscribeToEvent<CollisionEvent>(&(*this), &LogicSystem::CPPOnCollision);
        event.SubscribeToEvent<CollisionEvent>(&(*this), &LogicSystem::CSOnCollision);
        event.SubscribeToEvent<TriggerEvent>(&(*this), &LogicSystem::CPPOnTrigger);
        event.SubscribeToEvent<TriggerEvent>(&(*this), &LogicSystem::CSOnTrigger);
        event.SubscribeToEvent<EntityRuntimeChangeEvent>(&(*this), &LogicSystem::CPPOnEntityChange);
        event.SubscribeToEvent<EntityRuntimeChangeEvent>(&(*this), &LogicSystem::CSOnEntityChange);
        event.SubscribeToEvent<PopplioUtil::OnAppQuit>(&(*this), &LogicSystem::CPPOnAppQuit);
        event.SubscribeToEvent<PopplioUtil::OnAppQuit>(&(*this), &LogicSystem::CSOnAppQuit);
    }

    void LogicSystem::Initialization()
    {
        SubscribeToEvents(eventBus);
    }

    void LogicSystem::Setup() // setup scripts & data
    {
        Logger::Info("LogicSystem | Setup()");

        if (reloadMono)
        {
            monoAPI.ReloadAssembly(); // reload assembly
            reloadMono = false;
        }

		entities = GetSystemEntities(); // get entities

        // initialize

        currentCycleElapsedTime = 0.0;
        currentGCCycleElapsedTime = 0.0;

        //fixedSteps = Engine::timer.GetCurrentNumberOfSteps();

        if (Popplio::Engine::Config::verbose) monoAPI.PrintMemoryStats("Scene Setup");

        //monoAPI.PrintMemoryStats("1");

        // C++ Scripts
        // scripts & data
        for (auto i = entities.begin(); i != entities.end(); ++i)
        {
            if (!i->HasComponent<ActiveComponent>()) continue;
            if (!i->HasComponent<LogicComponent>()) continue;

            auto& logic = reg.GetComponent<LogicComponent>(*i);

            if (logic.scriptLang != ScriptLang::CPP) continue; // skip non C++ scripts

            if (scripts.find(logic.scriptName) == scripts.end())
            {
                Logger::Info("LogicSystem: Setup | C++ Script invalid", LogDestination::IMGUI_CONSOLE);
                continue;
            }

            InstScriptByName(logic.scriptName, &(*i)); // instantiate script and add to instScripts

            // set runtime vars to component
            SetLogicComponentRuntimeVars(*i);
            //i->GetComponent<LogicComponent>().entityID = i->GetId();
            //i->GetComponent<LogicComponent>().logicSystem = this;
            //i->GetComponent<LogicComponent>().currIsActive = i->GetComponent<ActiveComponent>().isActive;

            if (i->GetComponent<ActiveComponent>().isActive == false) continue;
            if (!(i->GetComponent<LogicComponent>().isActive)) continue;

            instScripts[i->GetId()]->gObj = &(*i);
        }

        for (auto i = entities.begin(); i != entities.end(); ++i)
        {
            if (!i->HasComponent<ActiveComponent>()) continue;
            if (!i->HasComponent<LogicComponent>()) continue;

            auto& logic = reg.GetComponent<LogicComponent>(*i);

            if (logic.scriptLang != ScriptLang::CPP) continue; // skip non C++ scripts

            if (scripts.find(logic.scriptName) == scripts.end())
            {
                Logger::Info("LogicSystem: Setup | C++ Script invalid", LogDestination::IMGUI_CONSOLE);
                continue;
            }

            if (i->GetComponent<ActiveComponent>().isActive == false) continue;
            if (!(i->GetComponent<LogicComponent>().isActive)) continue;

            try
            {
                instScripts[i->GetId()]->Setup(*i);
            }
            catch (const std::exception& e)
            {
                std::stringstream ss{ "" };
                ss << "LogicSystem: Setup | Exception in C++ script: " << std::string(e.what());
                Logger::Error(ss.str());
            }
        }

        //monoAPI.PrintMemoryStats("2");

        // C# scripts

        //monoAPI.ReloadAssembly(); // reload assembly

        // clear invalid gc
        monoAPI.CleanupStaleHandles();
        monoAPI.ClearAllGC();
        monoAPI.ForceGC(); // might die
        monoAPI.CheckMemoryStatus();


        //csScriptsOverridden.clear();
        //csScriptsOverridden = std::vector<std::unordered_map<std::string, bool>>(5);

        //csScriptNames.clear();
        //csEntityLoadedScripts.clear();
        //csLoadedScriptsEntityId.clear();
        //csScriptsToBeDeleted.clear();

        try
        {
            LoadCSScripts();
        }
        catch (const std::exception& e)
        {
            std::stringstream ss{ "" };
            ss << "LogicSystem: Setup | Exception while loading C# scripts: " << std::string(e.what());
            Logger::Error(ss.str());
        }

        //static std::vector<MonoObject*> scriptsToBeDeleted{};

        csScriptsOverridden.clear();
        csScriptsOverridden = std::vector<std::unordered_map<std::string, bool>>(5);

        csScriptNames.clear();
        csEntityLoadedScripts.clear();
        csLoadedScriptsEntityId.clear();
        csScriptsToBeDeleted.clear();

        for (uint32_t handle : csLoadedScripts)
        {
            //uint32_t handle = monoAPI.GetGC(obj, false);
            //if (handle != 0) currentLoopHandles.push_back(handle);
            //else
            //{
            //    handle = monoAPI.GetGC(obj, true);
            //    if (handle != 0) currentLoopHandles.push_back(handle);
            //    else
            //    {
            //        monoAPI.AddGC(obj, false);
            //        currentLoopHandles.push_back(monoAPI.GetGC(obj, false));
            //    }
            //}
            CSLoadScriptAtRun(mono_gchandle_get_target(handle));
        }


        static std::vector<void*> params{};
        params.clear();
        monoAPI.CallMethod(monoAPI.GetAssClass("Team_Popplio.ECS", "Entity"), "AddEntity", params, -1); // add invalid entity

        // add existing entities
        for (auto i = reg.GetAllEntities().begin(); i != reg.GetAllEntities().end(); ++i)
        {
            params.clear();
            monoAPI.CallMethod(monoAPI.GetAssClass("Team_Popplio.ECS", "Entity"), "AddEntity", params, i->GetId());
        }

        monoAPI.CallMethod(monoAPI.GetAssClass("Team_Popplio", "PopplioCS"), "CompleteInit");

        // get all scripts loaded in with "BassNKick" namespace and base class of "PopplioMonoScript"
        // use mono funcs
        currentLoop = csLoadedScripts;
        for (uint32_t handle : currentLoop)
        {
            MonoObject* obj = mono_gchandle_get_target(handle);

            auto ent = reg.GetEntityById(csLoadedScriptsEntityId[handle]);
            if (!ent.Exists())
            {
                csScriptsToBeDeleted.push_back(handle);
                continue;
            }
            
            //if (obj == nullptr || obj == reinterpret_cast<MonoObject*>(-1)) continue;

            //Logger::Critical(mono_class_get_name(mono_object_get_class(obj)));
            //Logger::Critical(std::to_string(monoAPI.Get<int>(obj, mono_class_get_field_from_name(mono_object_get_class(obj), "EntID"))));
            
            // check if loop functions are overridden

            //CSLoadScriptAtRun(obj);

            if (!(ent.HasComponent<ActiveComponent>()))
            {
                //scriptsToBeDeleted.push_back(obj);
                csScriptsToBeDeleted.push_back(handle);
                continue;
            }
            if (!(ent.HasComponent<LogicComponent>()))
            {
                csScriptsToBeDeleted.push_back(handle);
                continue;
            }

            if (ent.GetComponent<ActiveComponent>().isActive == false) continue;

            if (!(ent.GetComponent<LogicComponent>().isActive)) continue;


            if (!csScriptsOverridden[0][csScriptNames[handle]]) continue;

            try
            {
                monoAPI.CallMethod(obj, "Setup");
            }
            catch (const std::exception& e)
            {
                std::stringstream ss{ "" };
                ss << "LogicSystem: Setup | Exception in C# script: " << std::string(e.what());
                Logger::Error(ss.str());
            }
        }

        // delete scripts
        CSDeleteScripts();
    }

    void LogicSystem::Init() // init scripts
    {
        Logger::Info("LogicSystem | Init()");

        for (auto& inst : instScripts)
        {
            Entity i = reg.GetEntityById(inst.first);

            if (!i.HasComponent<ActiveComponent>()) continue;
            if (!i.HasComponent<LogicComponent>()) continue;

            //if (i.GetComponent<ActiveComponent>().isActive == false) continue;
            //if (!(i.GetComponent<LogicComponent>().isActive)) continue;

            if (!IsScriptActive(i)) continue;

            //auto& logic = i.GetComponent<LogicComponent>();

            try
            {
                if (i.GetComponent<LogicComponent>().currIsActive != IsScriptActive(i))
                {
                    CPPOnActiveChange(i); // OnEnable / OnDisable
                }
                else instScripts[i.GetId()]->Init(i);
            }
            catch (const std::exception& e)
            {
                std::stringstream ss{ "" };
                ss << "LogicSystem: Init | Exception in C++ script: " << std::string(e.what());
                Logger::Error(ss.str());
            }
        }

        // C# scripts
        
        // get all scripts loaded in with "BassNKick" namespace and base class of "PopplioMonoScript"
        // use mono funcs
        //static std::vector<MonoObject*> scriptsToBeDeleted{};
        // update logic components with current object addresses
        CSUpdateLogicObjects();
        currentLoop = csLoadedScripts;
        for (uint32_t handle : currentLoop)
        {
            MonoObject* obj = mono_gchandle_get_target(handle);
            auto ent = reg.GetEntityById(csLoadedScriptsEntityId[handle]);

            if (!ent.Exists())
            {
                csScriptsToBeDeleted.push_back(handle);
                continue;
            }
            

            if (!(ent.HasComponent<ActiveComponent>()))
            {
                csScriptsToBeDeleted.push_back(handle);
                continue;
            }
            if (!(ent.HasComponent<LogicComponent>()))
            {
                csScriptsToBeDeleted.push_back(handle);
                continue;
            }

            if (ent.GetComponent<LogicComponent>().currIsActive != IsScriptActive(ent))
            {
                CSOnActiveChange(ent, obj); // OnEnable / OnDisable
                continue;
            }

            if (!ent.GetComponent<LogicComponent>().currIsActive) continue;

            //if (monoAPI.GetPtr<Entity>(obj, mono_object_get_class(obj), "EntP")->
            //    GetComponent<ActiveComponent>().isActive == false) continue;

            //if (!(monoAPI.GetPtr<Entity>(obj, mono_object_get_class(obj), "EntP")->
            //    GetComponent<LogicComponent>().isActive)) continue;

            try
            {
                //if (ent.GetComponent<LogicComponent>().currIsActive != IsScriptActive(ent))
                //{
                //    CSOnActiveChange(ent, obj); // OnEnable / OnDisable
                //}
                //else
                //{
                //    if (!csScriptsOverridden[1][csScriptNames[obj]]) continue;

                //    monoAPI.CallMethod(obj, "Init");
                //}

                if (!csScriptsOverridden[1][csScriptNames[handle]]) continue;

                monoAPI.CallMethod(obj, "Init");
            }
            catch (const std::exception& e)
            {
                std::stringstream ss{ "" };
                ss << "LogicSystem: Init | Exception in C# script: " << std::string(e.what());
                Logger::Error(ss.str());
            }
        }

        // delete scripts
        CSDeleteScripts();
    }

    void LogicSystem::Update() // update scripts
    {
        for (auto& inst : instScripts)
        {
            Entity i = reg.GetEntityById(inst.first);

            if (!i.HasComponent<ActiveComponent>()) continue;
            if (i.GetComponent<ActiveComponent>().isActive == false) continue;

            if (!i.HasComponent<LogicComponent>()) continue;
            if (!(i.GetComponent<LogicComponent>().isActive)) continue;

            //auto& logic = i.GetComponent<LogicComponent>();

            try
            {
                if (i.GetComponent<LogicComponent>().currIsActive != IsScriptActive(i))
                {
                    CPPOnActiveChange(i); // OnEnable / OnDisable
                }
                else instScripts[i.GetId()]->Update(i);
                //scripts[logic.scriptName]->Update(&i);
            }
            catch (const std::exception& e)
            {
                std::stringstream ss{ "" };
                ss << "LogicSystem: Update | Exception in C++ script: " << std::string(e.what());
                Logger::Error(ss.str());
            }
        }

        // C# scripts
        // get all scripts loaded in with "BassNKick" namespace and base class of "PopplioMonoScript"
        // use mono funcs
        //static std::vector<MonoObject*> scriptsToBeDeleted{};
        // update logic components with current object addresses
        CSUpdateLogicObjects();
        currentLoop = csLoadedScripts;
        for (uint32_t handle : currentLoop)
        {
            MonoObject* obj = mono_gchandle_get_target(handle);
            auto ent = reg.GetEntityById(csLoadedScriptsEntityId[handle]);

            if (!ent.Exists())
            {
                csScriptsToBeDeleted.push_back(handle);
                continue;
            }

            //auto& ent = *monoAPI.GetPtr<Entity>(obj, mono_object_get_class(obj), "EntP");
            //auto& ent = *monoAPI.GetPtr<Entity>(obj, monoAPI.GetAssClass("Team_Popplio", "PopplioMonoScript"), "EntP");

            if (!reg.EntityExists(csLoadedScriptsEntityId[handle]) || !monoAPI.IsObjValid(obj))
            {
                csScriptsToBeDeleted.push_back(handle);
                continue;
            }

            if (!(ent.HasComponent<ActiveComponent>()))
            {
                csScriptsToBeDeleted.push_back(handle);
                continue;
            }
            if (!(ent.HasComponent<LogicComponent>()))
            {
                csScriptsToBeDeleted.push_back(handle);
                continue;
            }

            if (ent.GetComponent<LogicComponent>().currIsActive != IsScriptActive(ent))
            {
                CSOnActiveChange(ent, obj); // OnEnable / OnDisable
                continue;
            }

            if (!ent.GetComponent<LogicComponent>().currIsActive) continue;

            try
            {
                //if (ent.GetComponent<LogicComponent>().currIsActive != IsScriptActive(ent))
                //{
                //    CSOnActiveChange(ent, obj); // OnEnable / OnDisable
                //}
                //else
                //{
                //    if (!csScriptsOverridden[2][csScriptNames[obj]]) continue;
                //    std::string str = csScriptNames[obj];
                //    monoAPI.CallMethod(obj, "Update");
                //}
                if (!csScriptsOverridden[2][csScriptNames[handle]]) continue;
                std::string str = csScriptNames[handle];
                monoAPI.CallMethod(obj, "Update");
            }
            catch (const std::exception& e)
            {
                std::stringstream ss{ "" };
                ss << "LogicSystem: Update | Exception in C# script: " << std::string(e.what());
                Logger::Error(ss.str());
            }
        }

        // delete scripts
        CSDeleteScripts();
    }

    void LogicSystem::FixedUpdate() // fixedupdate scripts todo: make it actually fixed update?
    {
        static int fixedSteps{};
        fixedSteps = 0;

        while (fixedSteps < Engine::timer.GetCurrentNumberOfSteps())
        {
            for (auto& inst : instScripts)
            {
                Entity i = reg.GetEntityById(inst.first);

                if (!i.HasComponent<ActiveComponent>()) continue;
                if (i.GetComponent<ActiveComponent>().isActive == false) continue;

                if (!i.HasComponent<LogicComponent>()) continue;
                if (!(i.GetComponent<LogicComponent>().isActive)) continue;

                //auto& logic = i.GetComponent<LogicComponent>();

                try
                {
                    if (i.GetComponent<LogicComponent>().currIsActive != IsScriptActive(i))
                    {
                        CPPOnActiveChange(i); // OnEnable / OnDisable
                    }
                    else instScripts[i.GetId()]->FixedUpdate(i);
                }
                catch (const std::exception& e)
                {
                    std::stringstream ss{ "" };
                    ss << "LogicSystem: FixedUpdate | Exception in C++ script: " << std::string(e.what());
                    Logger::Error(ss.str());
                }
            }

            // C# scripts
            // get all scripts loaded in with "BassNKick" namespace and base class of "PopplioMonoScript"
            // use mono funcs
            //static std::vector<MonoObject*> scriptsToBeDeleted{};
            // update logic components with current object addresses
            CSUpdateLogicObjects();
            currentLoop = csLoadedScripts;
            for (uint32_t handle : currentLoop)
            {
                MonoObject* obj = mono_gchandle_get_target(handle);
                auto ent = reg.GetEntityById(csLoadedScriptsEntityId[handle]);

                if (!ent.Exists())
                {
                    csScriptsToBeDeleted.push_back(handle);
                    continue;
                }

                //auto& ent = *monoAPI.GetPtr<Entity>(obj, mono_object_get_class(obj), "EntP");

                if (!reg.EntityExists(csLoadedScriptsEntityId[handle]) || !monoAPI.IsObjValid(obj))
                {
                    csScriptsToBeDeleted.push_back(handle);
                    continue;
                }

                if (!(ent.HasComponent<ActiveComponent>()))
                {
                    csScriptsToBeDeleted.push_back(handle);
                    continue;
                }
                if (!(ent.HasComponent<LogicComponent>()))
                {
                    csScriptsToBeDeleted.push_back(handle);
                    continue;
                }

                if (ent.GetComponent<LogicComponent>().currIsActive != IsScriptActive(ent))
                {
                    CSOnActiveChange(ent, obj); // OnEnable / OnDisable
                    continue;
                }

                if (!ent.GetComponent<LogicComponent>().currIsActive) continue;

                try
                {
                    //if (ent.GetComponent<LogicComponent>().currIsActive != IsScriptActive(ent))
                    //{
                    //    CSOnActiveChange(ent, obj); // OnEnable / OnDisable
                    //}
                    //else
                    //{
                    //    if (!csScriptsOverridden[3][csScriptNames[obj]]) continue;

                    //    monoAPI.CallMethod(obj, "FixedUpdate");
                    //}

                    if (!csScriptsOverridden[3][csScriptNames[handle]]) continue;

                    monoAPI.CallMethod(obj, "FixedUpdate");
                }
                catch (const std::exception& e)
                {
                    std::stringstream ss{ "" };
                    ss << "LogicSystem: FixedUpdate | Exception in C# script: " << std::string(e.what());
                    Logger::Error(ss.str());
                }
            }

            // delete scripts
            CSDeleteScripts();

            currentCycleElapsedTime += Engine::timer.GetFixedDeltaTime();
            currentGCCycleElapsedTime += Engine::timer.GetFixedDeltaTime();

            if (currentGCCycleElapsedTime >= POPPLIO_GC_CYCLE)
            {
                monoAPI.CheckMemoryStatus();
                currentGCCycleElapsedTime = 0.0;
            }

            ++fixedSteps;
        }
    }

    void LogicSystem::End() // script loop end
    {
        Logger::Info("LogicSystem | End()");

        for (auto& inst : instScripts)
        {
            Entity i = reg.GetEntityById(inst.first);

            if (!i.HasComponent<ActiveComponent>()) continue;
            if (i.GetComponent<ActiveComponent>().isActive == false) continue;

            if (!i.HasComponent<LogicComponent>()) continue;
            if (!(i.GetComponent<LogicComponent>().isActive)) continue;

            //auto& logic = i.GetComponent<LogicComponent>();

            try
            {
                instScripts[i.GetId()]->End(i);
            }
            catch (const std::exception& e)
            {
                std::stringstream ss{ "" };
                ss << "LogicSystem: End | Exception in C++ script: " << std::string(e.what());
                Logger::Error(ss.str());
            }
        }
        instScripts.clear();

        // C# scripts
        // get all scripts loaded in with "BassNKick" namespace and base class of "PopplioMonoScript"
        // use mono funcs
        //static std::vector<MonoObject*> scriptsToBeDeleted{};
        // update logic components with current object addresses
        CSUpdateLogicObjects();
        currentLoop = csLoadedScripts;
        for (uint32_t handle : currentLoop)
        {
            MonoObject* obj = mono_gchandle_get_target(handle);
            auto ent = reg.GetEntityById(csLoadedScriptsEntityId[handle]);
            if (!ent.Exists())
            {
                csScriptsToBeDeleted.push_back(handle);
                continue;
            }

            if (!(ent.HasComponent<ActiveComponent>()))
            {
                //scriptsToBeDeleted.push_back(obj);
                continue;
            }

            if (ent.GetComponent<ActiveComponent>().isActive == false) continue;

            if (!(ent.GetComponent<LogicComponent>().isActive)) continue;

            if (!csScriptsOverridden[4][csScriptNames[handle]]) continue;

            try
            {
                monoAPI.CallMethod(obj, "End");
            }
            catch (const std::exception& e)
            {
                std::stringstream ss{ "" };
                ss << "LogicSystem: End | Exception in C# script: " << std::string(e.what());
                Logger::Error(ss.str());
            }
        }

        CSDeleteScripts();

        csScriptsOverridden.clear();
        csScriptNames.clear();
        csEntityLoadedScripts.clear();
        csLoadedScriptsEntityId.clear();
        csScriptsToBeDeleted.clear();

        currentLoop.clear();

        try
        {
            UnloadCSScripts();

            Logger::Info("LogicSystem: End | Clearing GC...");
            monoAPI.ClearAllGC();
            monoAPI.ForceGC();
        }
        catch (const std::exception& e)
        {
            std::stringstream ss{ "" };
            ss << "LogicSystem: End | Exception while unloading C# scripts: " << std::string(e.what());
            Logger::Error(ss.str());
        }
    }

    void LogicSystem::SetLogicComponentRuntimeVars(Entity e)
    {
        //if (!e.HasComponent<LogicComponent>())
        //{
        //    Logger::Error("LogicSystem::SetLogicComponentRuntimeVars | Entity does not have LogicComponent");
        //    return;
        //}

        auto& comp = e.GetComponent<LogicComponent>();
        switch (comp.scriptLang)
        {
            case ScriptLang::CPP:
            case ScriptLang::CS:
                comp.entityID = e.GetId();
                comp.logicSystem = this;
                comp.currIsActive = IsScriptActive(e);
                break;
            case ScriptLang::UNASSIGNED:
                break;
        }
    }

    bool LogicSystem::IsScriptActive(Entity& e)
    {
        return e.GetComponent<ActiveComponent>().isActive && e.GetComponent<LogicComponent>().isActive;
    }

//    void LogicSystem::KillEntity(Entity& e)
//    {
//        auto kill = [&](Entity& ent)
//        {
//            std::vector<Entity> entitiesToDestroy;
//            if (ent.HasComponent<ParentComponent>())
//            {
//                // Recursive function to collect all children
//                std::function<void(const Entity&)> collectChildren = [&](const Entity& ent) {
//                    if (ent.HasComponent<ParentComponent>())
//                    {
//                        for (const auto& child : ent.GetComponent<ParentComponent>().children)
//                        {
//                            entitiesToDestroy.push_back(child);
//                            collectChildren(child);
//                        }
//                    }
//                    };
//
//                collectChildren(ent);
//            }
//            // Add the parent entity itself
//            entitiesToDestroy.push_back(ent);
//
//            // Remove all collected entities from their layers and then destroy them
//            for (const auto& entityToDestroy : entitiesToDestroy)
//            {
//                lay.RemoveEntityFromCurrentLayer(entityToDestroy);
//
//#ifndef IMGUI_DISABLE
//                reg.GetSystem<Popplio::Editor>().ResetSelectedEntity(entityToDestroy);
//#endif
//
//                reg.KillEntity(entityToDestroy);
//            }
//            //lay.RemoveEntityFromCurrentLayer(ent);
//            //ent.Kill();
//        };
//
//        reg.GetSystem<QueueSystem>().Add(std::bind(kill, e));
//    }


    // copy behaviour
    // special for entities (if entity is child)
    // Set entity references in logic component that are children of the original to the children of the new entity
    void LogicSystem::UpdateFromCopy(const Entity e, const Entity other, 
        const Entity rootE, const Entity rootOther) // e is this
    {
        //(void)rootE;

        if (!e.HasComponent<LogicComponent>() || !other.HasComponent<LogicComponent>()) return;

        //if (!e.HasComponent<ParentComponent>() || e.GetComponent<ParentComponent>().children.empty()) return;
        //if (!other.HasComponent<ParentComponent>() || other.GetComponent<ParentComponent>().children.empty()) return;

        // if no parent component or children, copy entities and entity lists and return
        if (!e.HasComponent<ParentComponent>() || e.GetComponent<ParentComponent>().children.empty() ||
            !other.HasComponent<ParentComponent>() || other.GetComponent<ParentComponent>().children.empty())
        {
            std::vector<std::pair<std::string, int>> ents = other.GetComponent<LogicComponent>().entities;
            std::vector<std::pair<std::string, std::vector<int>>> entityLists = other.GetComponent<LogicComponent>().entityLists;

            e.GetComponent<LogicComponent>().entities = ents;
            e.GetComponent<LogicComponent>().entityLists = entityLists;
            return;
        }

        //auto& ents = e.GetComponent<LogicComponent>().entities;
        //auto& entityLists = e.GetComponent<LogicComponent>().entityLists;
        auto const otherEntities = other.GetComponent<LogicComponent>().entities;
        auto const otherEntityLists = other.GetComponent<LogicComponent>().entityLists;

        std::vector<std::pair<std::string, int>> ents = std::vector<std::pair<std::string, int>>();
        std::vector<std::pair<std::string, std::vector<int>>> entityLists = std::vector<std::pair<std::string, std::vector<int>>>();

        for (auto& i : otherEntities)
        {
            if (!reg.EntityExists(i.second))
            {
                //Logger::Critical("1 " + i.first);
                ents.push_back(i);
                continue;
            }

            // if reference is original root entity, make new reference the new root entity
            if (reg.GetEntityById(i.second) == rootOther)
            {
                ents.push_back(std::make_pair(i.first, rootE.GetId()));
                continue;
            }

            Entity child = reg.GetSystem<HierarchySystem>().GetChildRecursive(rootOther, reg.GetEntityById(i.second));
            //Entity child = reg.GetSystem<HierarchySystem>().GetChildRecursive(other, reg.GetEntityById(i.second));
            //Logger::Error(std::to_string(child.GetId()));
            if (!child.Exists())
            {
                //Logger::Critical("2 " + i.first);

                //if (rootOther == reg.GetEntityById(i.second)) 
                //    ents.push_back(std::make_pair(i.first, rootE.GetId()));
                //else 
                //HERE
                ents.push_back(i);
            }
            else
            {
                for (Entity c : reg.GetSystem<HierarchySystem>().GetChildRecursive(rootE))
                {
                    //Logger::Warning(std::to_string(c.GetClonedFrom()));
                    if (c.GetClonedFrom() == child)
                    {
                        //Logger::Warning(std::to_string(c.GetId()));
                        ents.push_back(std::make_pair(i.first, c.GetId()));
                        break;
                    }
                }
            }
        }

        for (size_t i{}; i < otherEntityLists.size(); ++i) // UNTESTED CODE
        {
            entityLists.push_back(std::make_pair(otherEntityLists[i].first, std::vector<int>()));
            for (auto& j : otherEntityLists[i].second)
            {
                if (!reg.EntityExists(j))
                {
                    entityLists[i].second.push_back(static_cast<int>(i));
                    continue;
                }

                // if reference is original root entity, make new reference the new root entity
                if (reg.GetEntityById(j) == rootOther)
                {
                    entityLists[i].second.push_back(rootE.GetId());
                    continue;
                }

                Entity child = reg.GetSystem<HierarchySystem>().GetChildRecursive(rootOther, reg.GetEntityById(j));
                //Entity child = reg.GetSystem<HierarchySystem>().GetChildRecursive(other, reg.GetEntityById(j));
                if (!child.Exists())
                {
                    //if (reg.GetEntityById(j) == rootOther)
                    //    entityLists[i].second.push_back(rootE.GetId());
                    //else 
                    //HERE
                    entityLists[i].second.push_back(j);
                }
                else
                {
                    for (Entity c : reg.GetSystem<HierarchySystem>().GetChildRecursive(rootE))
                    {
                        if (c.GetClonedFrom() == child)
                        {
                            entityLists[i].second.push_back(c.GetId());
                            break;
                        }
                    }
                }
            }
        }

        e.GetComponent<LogicComponent>().entities = ents;
        e.GetComponent<LogicComponent>().entityLists = entityLists;

        reg.ForceRuntimeUpdate(e);
    }
}