/******************************************************************************/
/*!
\file   CSScripts.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the source file for C# script Logic System functions

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>

#include "LogicSystem.h"

namespace Popplio
{
    namespace
    {
        static std::vector<void*> csParams{}; // params for C# scripts
    }

    void LogicSystem::LoadCSScripts()
    {
        //csScripts = std::vector<MonoClass*>();

        for (auto i = entities.begin(); i != entities.end(); ++i)
        {
            LoadCSScript(*i);
        }
    }

    bool LogicSystem::LoadCSScript(Entity& i)
    {
        MonoObject* obj = nullptr;
        if ((obj = SetupCSScript(i)) == nullptr) return false;

        if (reg.HasComponent<LogicComponent>(i))
        {
            auto& logic = reg.GetComponent<LogicComponent>(i);

            // Variable Initialization // ===================================================== //

            MonoClassField* field = nullptr;
            // set instance values from logic component in fields
            for (auto const& [name, value] : logic.bools)
            {
                field = mono_class_get_field_from_name(mono_object_get_class(obj), name.c_str());
                if (field == nullptr)
                {
                    Logger::Warning("LogicSystem: LoadCSScript | Field not found: " + name);
                    //Logger::Warning("LogicSystem: LoadCSScript | Removing variable from serialization");
                    // TODO : remove from serialization for every var not found
                    continue;
                }
                monoAPI.Set(obj, field, value);
            }
            for (auto const& [name, value] : logic.ints)
            {
                field = mono_class_get_field_from_name(mono_object_get_class(obj), name.c_str());
                if (field == nullptr)
                {
                    Logger::Warning("LogicSystem: LoadCSScript | Field not found: " + name);
                    continue;
                }
                monoAPI.Set(obj, field, value);
            }
            for (auto const& [name, value] : logic.floats)
            {
                field = mono_class_get_field_from_name(mono_object_get_class(obj), name.c_str());
                if (field == nullptr)
                {
                    Logger::Warning("LogicSystem: LoadCSScript | Field not found: " + name);
                    continue;
                }
                monoAPI.Set(obj, field, value);
            }
            for (auto const& [name, value] : logic.doubles)
            {
                field = mono_class_get_field_from_name(mono_object_get_class(obj), name.c_str());
                if (field == nullptr)
                {
                    Logger::Warning("LogicSystem: LoadCSScript | Field not found: " + name);
                    continue;
                }
                monoAPI.Set(obj, field, value);
            }
            for (auto const& [name, value] : logic.chars)
            {
                field = mono_class_get_field_from_name(mono_object_get_class(obj), name.c_str());
                if (field == nullptr)
                {
                    Logger::Warning("LogicSystem: LoadCSScript | Field not found: " + name);
                    continue;
                }
                monoAPI.Set(obj, field, value);
            }
            for (auto const& [name, value] : logic.strings)
            {
                field = mono_class_get_field_from_name(mono_object_get_class(obj), name.c_str());
                if (field == nullptr)
                {
                    Logger::Warning("LogicSystem: LoadCSScript | Field not found: " + name);
                    continue;
                }
                monoAPI.Set(obj, field, value);
            }
            for (auto const& [name, value] : logic.entities)
            {
                //if (value < 0) continue; // skip invalid / unassigned entity

                //if (value < 0)
                //{
                //    Logger::Warning("LogicSystem: LoadCSScript | Invalid entity id: " + name);
                //    continue;
                //}

                field = mono_class_get_field_from_name(mono_object_get_class(obj), name.c_str());
                if (field == nullptr)
                {
                    Logger::Warning("LogicSystem: LoadCSScript | Field not found: " + name);
                    continue;
                }
                csParams.clear();

                monoAPI.CallMethod(monoAPI.GetAssClass("Team_Popplio.ECS", "Entity"), "SetEntityId", csParams, value,
                    monoAPI.Get<MonoObject*>(obj, field));

                //csParams.clear();

                //int id = monoAPI.CallMethod<int>(monoAPI.GetAssClass("Team_Popplio.ECS", "Entity"), "GetEntityId", csParams, 
                //    monoAPI.Get<MonoObject*>(obj, field));

                //Logger::Critical("Entity ID: " + std::to_string(id));
                
                //monoAPI.CallMethod(obj, )
                //monoAPI.Set(obj, field, monoAPI.CallMethodObj(
                //    monoAPI.GetAssClass("Team_Popplio.ECS", "Entity"), "SetEntityField", csParams, value));
                //monoAPI.Set(obj, field, monoAPI.CallMethodObj(
                //    monoAPI.GetAssClass("Team_Popplio.ECS", "Entity"), "InitEntity", csParams, value));
                //monoAPI.Set(obj, field, monoAPI.CallMethodObj(
                //    monoAPI.GetAssClass("Team_Popplio.ECS", "Entity"), "GetEntityById", csParams, value));
            }

            // Containers

            for (auto const& [name, value] : logic.boolLists)
            {
                field = mono_class_get_field_from_name(mono_object_get_class(obj), name.c_str());
                if (field == nullptr)
                {
                    Logger::Warning("LogicSystem: LoadCSScript | Field not found: " + name);
                    continue;
                }
                MonoTypeEnum typeEnum = static_cast<MonoTypeEnum>(mono_type_get_type(mono_field_get_type(field)));
                if (typeEnum == MONO_TYPE_GENERICINST)
                {
                    monoAPI.SetList<bool>(obj, field, value);
                }
                else
                {
                    monoAPI.SetArray<bool>(obj, field, value);
                }
            }
            for (auto const& [name, value] : logic.intLists)
            {
                field = mono_class_get_field_from_name(mono_object_get_class(obj), name.c_str());
                if (field == nullptr)
                {
                    Logger::Warning("LogicSystem: LoadCSScript | Field not found: " + name);
                    continue;
                }
                MonoTypeEnum typeEnum = static_cast<MonoTypeEnum>(mono_type_get_type(mono_field_get_type(field)));
                if (typeEnum == MONO_TYPE_GENERICINST)
                {
                    monoAPI.SetList<int>(obj, field, value);
                }
                else
                {
                    monoAPI.SetArray<int>(obj, field, value);
                }
            }
            for (auto const& [name, value] : logic.floatLists)
            {
                field = mono_class_get_field_from_name(mono_object_get_class(obj), name.c_str());
                if (field == nullptr)
                {
                    Logger::Warning("LogicSystem: LoadCSScript | Field not found: " + name);
                    continue;
                }
                MonoTypeEnum typeEnum = static_cast<MonoTypeEnum>(mono_type_get_type(mono_field_get_type(field)));
                if (typeEnum == MONO_TYPE_GENERICINST)
                {
                    monoAPI.SetList<float>(obj, field, value);
                }
                else
                {
                    monoAPI.SetArray<float>(obj, field, value);
                }
            }
            for (auto const& [name, value] : logic.doubleLists)
            {
                field = mono_class_get_field_from_name(mono_object_get_class(obj), name.c_str());
                if (field == nullptr)
                {
                    Logger::Warning("LogicSystem: LoadCSScript | Field not found: " + name);
                    continue;
                }
                MonoTypeEnum typeEnum = static_cast<MonoTypeEnum>(mono_type_get_type(mono_field_get_type(field)));
                if (typeEnum == MONO_TYPE_GENERICINST)
                {
                    monoAPI.SetList<double>(obj, field, value);
                }
                else
                {
                    monoAPI.SetArray<double>(obj, field, value);
                }
            }
            for (auto const& [name, value] : logic.charLists)
            {
                field = mono_class_get_field_from_name(mono_object_get_class(obj), name.c_str());
                if (field == nullptr)
                {
                    Logger::Warning("LogicSystem: LoadCSScript | Field not found: " + name);
                    continue;
                }
                MonoTypeEnum typeEnum = static_cast<MonoTypeEnum>(mono_type_get_type(mono_field_get_type(field)));
                if (typeEnum == MONO_TYPE_GENERICINST)
                {
                    monoAPI.SetList<char>(obj, field, value);
                }
                else
                {
                    monoAPI.SetArray<char>(obj, field, value);
                }
            }
            for (auto const& [name, value] : logic.stringLists)
            {
                field = mono_class_get_field_from_name(mono_object_get_class(obj), name.c_str());
                if (field == nullptr)
                {
                    Logger::Warning("LogicSystem: LoadCSScript | Field not found: " + name);
                    continue;
                }
                MonoTypeEnum typeEnum = static_cast<MonoTypeEnum>(mono_type_get_type(mono_field_get_type(field)));
                if (typeEnum == MONO_TYPE_GENERICINST)
                {
                    monoAPI.SetList<std::string>(obj, field, value);
                }
                else
                {
                    monoAPI.SetArray<std::string>(obj, field, value);
                }
            }
            for (auto const& [name, list] : logic.entityLists)
            {
                // entity array field
                field = mono_class_get_field_from_name(mono_object_get_class(obj), name.c_str());

                if (field == nullptr)
                {
                    Logger::Warning("LogicSystem: LoadCSScript | Field not found: " + name);
                    continue;
                }

                std::vector<MonoObject*> arr{};

                for (int const& id : list)
                {
                    csParams.clear();
                    arr.push_back(monoAPI.CallMethodObj(monoAPI.GetAssClass("Team_Popplio.ECS", "Entity"),
                        "GetEntity", csParams, id));
                }

                //std::vector<MonoObject*> arr = PopplioScriptAPI::MonoToArray1D<MonoObject*>(
                //    monoAPI.GetArray(monoAPI.Get(obj, field)), &monoAPI.GetAppDomain());

                //csParams.clear();
                //MonoObject* arr = monoAPI.CallMethodObj(monoAPI.GetAssClass("Team_Popplio.ECS", "Entity"), 
                //    "AddEntitiesToArr", csParams, PopplioScriptAPI::ArrayToMono1D<int>(list, &monoAPI.GetAppDomain()));

                MonoTypeEnum typeEnum = static_cast<MonoTypeEnum>(mono_type_get_type(mono_field_get_type(field)));
                if (typeEnum == MONO_TYPE_GENERICINST)
                {
                    monoAPI.SetList<MonoObject*>(obj, field, arr);
                }
                else
                {
                    monoAPI.SetArray<MonoObject*>(obj, field, arr);
                }
            }

            // ===================================================== //

            // Add script to loaded list
            //csScripts.push_back(classObj);
            //csLoadedScripts.push_back(obj);
            csLoadedScripts.push_back(monoAPI.GetGC(obj, false));

            // set runtime vars to component
            SetLogicComponentRuntimeVars(i);
            //i->GetComponent<LogicComponent>().entityID = i->GetId();
            //i->GetComponent<LogicComponent>().logicSystem = this;
            logic.scriptObject = obj;

            // message
            static std::stringstream ss{ "" };
            ss.str("");
            std::pair<std::string, std::string> scriptName = monoAPI.SplitName(logic.scriptName);
            ss << "LogicSystem: Loaded C# script: " << scriptName.first << "::" << scriptName.second
                << " | Entity " << i.GetId();
            Logger::Info(ss.str());
        }
        else // add non-script-attached entity sync to C# environment
        {
            Logger::Error("LogicSystem: LoadCSScript | LogicComponent not found");
            csParams.clear();
            monoAPI.CallMethod(monoAPI.GetAssClass("Team_Popplio.ECS", "Entity"),
                "AddEntity", csParams, i.GetId());
        }

        return true;
    }

    MonoObject* LogicSystem::SetupCSScript(Entity& i)
    {
        if (!reg.HasComponent<LogicComponent>(i)) return nullptr;

        auto& logic = reg.GetComponent<LogicComponent>(i);

        if (logic.scriptLang != ScriptLang::CS)
        {
            Logger::Info("LogicSystem: C# Script load - not C#");
            return nullptr;
        }

        std::pair<std::string, std::string> scriptName = monoAPI.SplitName(logic.scriptName);

        if (monoAPI.GetAssClass(scriptName.first.c_str(), scriptName.second.c_str()) == nullptr)
        {
            Logger::Warning("LogicSystem: C# Script name does not match, continuing...");
            return nullptr;
        }

        MonoObject* obj = nullptr;

        //if (!monoAPI.IsObjValid(logic.scriptObject))
            //logic.scriptObject = 
        obj = monoAPI.InstClass(scriptName.first.c_str(), scriptName.second.c_str());
        // else obj = logic.scriptObject;

        // set entity in script

        monoAPI.Set(obj, mono_class_get_field_from_name(mono_object_get_class(obj), "EntP"), &(i));

        monoAPI.Set(obj, mono_class_get_field_from_name(mono_object_get_class(obj), "EntID"), i.GetId());

        // test OK
        //int j = i->GetId();

        //int l = monoAPI.GetPtr<Entity>(obj, classObj, "EntP")->GetId();

        monoAPI.SetProp(obj, mono_class_get_property_from_name(mono_object_get_class(obj), "This"), i); // TODO FIX

        //Logger::Warning("Engine");
        //Logger::Warning(std::to_string(i->GetId()));
        //std::cout << i->GetId() << std::endl;

        //int m = monoAPI.GetProp<int>(obj, mono_class_get_property_from_name(classObj, "entityID"));
        // test OK

        // Call Set Script of base class from derived script class
        // requires special code
        MonoClass* popplioMonoScript = monoAPI.GetAssClass("Team_Popplio", "PopplioMonoScript");
        //MonoClass* popplioMonoScript = mono_class_from_name(&monoAPI.GetImage(), "Team_Popplio", "PopplioMonoScript");
        MonoMethod* setScript = mono_class_get_method_from_name(popplioMonoScript, "SetScript", 0);
        if (setScript == nullptr)
        {
            Logger::Error("LogicSystem: LoadCSScript | Failed to get method");
            return nullptr;
        }
        MonoObject* except{ nullptr }; // exception
        //std::vector<void*> params{};
        //params.push_back(static_cast<void*>(&(*i)));
        //int temp = i->GetId();
        //params.push_back(static_cast<void*>(&temp));
        mono_runtime_invoke(setScript, obj, nullptr, &except);
        if (except != nullptr) // failed
        {
            Logger::Error(monoAPI.GetExceptionMessage(except));
            return nullptr;
        }

        return obj;
    }

    void LogicSystem::UnloadCSScripts()
    {
        Logger::Info("LogicSystem: Unloading C# scripts...");
        std::stringstream ss{};

        ss << "LogicSystem: Unloading " << monoAPI.GetHandles().size() << " GC Handles...";

        Logger::Info(ss.str());

        // free all GC handles
        //for (auto [obj, handle] : monoAPI.GetHandles()) mono_gchandle_free(handle);

        //monoAPI.GetHandles().clear();

        monoAPI.ClearAllGC();

        Logger::Info("LogicSystem: Cleared GC Handles");

        csLoadedScripts.clear();
        csScriptsToBeDeleted.clear();
        csLoadedScriptsEntityId.clear();
        csEntityLoadedScripts.clear();
        csScriptNames.clear();
        csScriptsOverridden.clear();
    }

    void LogicSystem::CSDeleteScripts()
    {
        if (csScriptsToBeDeleted.empty()) return;

        //for (std::vector<MonoObject*>::const_iterator i{ csScriptsToBeDeleted.cbegin() }; i != csScriptsToBeDeleted.cend(); ++i)
        for (std::vector<uint32_t>::const_iterator i{ csScriptsToBeDeleted.cbegin() }; i != csScriptsToBeDeleted.cend(); ++i)
        {
            // CSOnEntityDestroy handled by ECS
            //CSOnEntityDestroy(monoAPI.GetPtr<Entity>(*i, mono_object_get_class(*i), "EntP")->GetId(), *i);

            auto iter1 = std::find(csLoadedScripts.begin(), csLoadedScripts.end(), *i);
            if (iter1 != csLoadedScripts.end()) csLoadedScripts.erase(iter1);
            else continue;

            //int id = csLoadedScriptsEntityId[*i];

            if (csEntityLoadedScripts.find(csLoadedScriptsEntityId[*i]) != csEntityLoadedScripts.end())
            {
                csEntityLoadedScripts.erase(csLoadedScriptsEntityId[*i]);
            }

            if (csLoadedScriptsEntityId.find(*i) != csLoadedScriptsEntityId.end())
            {
                csLoadedScriptsEntityId.erase(*i);
            }
        }

        // clear GC?
        //for (std::vector<MonoObject*>::const_iterator i{ csScriptsToBeDeleted.cbegin() }; 
        //  i != csScriptsToBeDeleted.cend(); ++i)
        for (std::vector<uint32_t>::const_iterator i{ csScriptsToBeDeleted.cbegin() };
            i != csScriptsToBeDeleted.cend(); ++i)
            monoAPI.RemoveGC(*i);

        csScriptsToBeDeleted.clear();
        //monoAPI.ClearInvalidGC(); // may cause crash?
    }

    void LogicSystem::CSOnCollision(CollisionEvent& event)
    {
        if (csLoadedScripts.empty()) return;
        try
        {
            csParams.clear();
            monoAPI.CallMethod(monoAPI.GetAssClass("Team_Popplio", "Events"), "OnCollision2D",
                csParams, event.entity1Id, event.entity2Id, event.collisionTime, static_cast<int>(event.eventID));
        }
        catch (const std::exception& e)
        {
            std::stringstream ss{ "" };
            ss << "LogicSystem: Update | Exception in C# script: " << std::string(e.what());
            Logger::Error(ss.str());
        }
    }

    void LogicSystem::CSOnTrigger(TriggerEvent& event)
    {
        if (csLoadedScripts.empty()) return;
        try
        {
            csParams.clear();
            monoAPI.CallMethod(monoAPI.GetAssClass("Team_Popplio", "Events"), "OnTrigger2D",
                csParams, event.entity1Id, event.entity2Id, event.triggerTime, static_cast<int>(event.eventID));
        }
        catch (const std::exception& e)
        {
            std::stringstream ss{ "" };
            ss << "LogicSystem: Update | Exception in C# script: " << std::string(e.what());
            Logger::Error(ss.str());
        }
    }

    void LogicSystem::CSOnEntityChange(EntityRuntimeChangeEvent& event)
    {
        if (csLoadedScripts.empty()) return;
        try
        {
            csParams.clear();
            monoAPI.CallMethod(monoAPI.GetAssClass("Team_Popplio", "Events"), "OnEntityChange",
                csParams, event.id, static_cast<int>(event.change));
        }
        catch (const std::exception& e)
        {
            std::stringstream ss{ "" };
            ss << "LogicSystem::CSOnEntityChange | Exception in C# script: " << std::string(e.what());
            Logger::Error(ss.str());
        }
    }

    // TODO
    void LogicSystem::CSOnEntityCreate(int const& entity, MonoObject* script)
    {
        if (script != nullptr)
        {
            CSAddToLoadedScripts(entity);
            try
            {
                csParams.clear();
                monoAPI.CallMethod(monoAPI.GetAssClass("Team_Popplio", "Events"), "OnEntityCreate", csParams, entity);
            }
            catch (const std::exception& e)
            {
                Logger::Error("LogicSystem::CSOnEntityCreate | Exception in C# script: " + std::string(e.what()));
            }
        }
    }

    void LogicSystem::CSOnEntityDestroy(int entity, MonoObject* script)
    {
        if (csLoadedScripts.empty()) return;
        try
        {
            csParams.clear();
            monoAPI.CallMethod(monoAPI.GetAssClass("Team_Popplio", "Events"), "OnEntityDestroy", csParams, entity);

            // free GC handle?

            // remove from loaded scripts
            for (auto i = csLoadedScripts.begin(); i != csLoadedScripts.end(); ++i)
            {
                if (script == mono_gchandle_get_target(*i))
                {
                    //csLoadedScripts.erase(i);
                    csScriptsToBeDeleted.push_back(*i);
                    break;
                }
            }
        }
        catch (const std::exception& e)
        {
            std::stringstream ss{ "" };
            ss << "LogicSystem::CSOnEntityDestroy | Exception in C# script: " << std::string(e.what());
            Logger::Error(ss.str());
        }
    }

    void LogicSystem::CSOnActiveChange(Entity& e, MonoObject* script)
    {
        (void)script; // TODO

        e.GetComponent<LogicComponent>().currIsActive = IsScriptActive(e);
        //bool b = IsScriptActive(e);
        if (csLoadedScripts.empty()) return;
       
        try
        {
            csParams.clear();

            if (IsScriptActive(e))
            {
                monoAPI.CallMethod(monoAPI.GetAssClass("Team_Popplio", "Events"), "OnEnable", csParams, e.GetId());
            }
            else
            {
                monoAPI.CallMethod(monoAPI.GetAssClass("Team_Popplio", "Events"), "OnDisable", csParams, e.GetId());
            }
        }
        catch (const std::exception& e)
        {
            std::stringstream ss{ "" };
            ss << "LogicSystem::CSOnActiveChange | Exception in C# script: " << std::string(e.what());
            Logger::Error(ss.str());
        }
    }

    void LogicSystem::CSOnAppQuit(PopplioUtil::OnAppQuit& event)
    {
        (void)event;

        if (csLoadedScripts.empty()) return;
        try
        {
            monoAPI.CallMethod(monoAPI.GetAssClass("Team_Popplio", "Events"), "OnAppQuit");
        }
        catch (const std::exception& e)
        {
            std::stringstream ss{ "" };
            ss << "LogicSystem::CSOnAppQuit | Exception in C# script: " << std::string(e.what());
            Logger::Error(ss.str());
        }
    }

    void LogicSystem::CSRemoveScriptObj(int const& entityID)
    {
        csScriptsToBeDeleted.push_back(csEntityLoadedScripts[entityID]);
    }

    void LogicSystem::CSLoadScriptAtRun(MonoObject* obj)
    {
        uint32_t handle = monoAPI.GetGC(obj, false);

        if (handle == 0)
        {
            Popplio::Logger::Error("LogicSystem: CSLoadScriptAtRun | Invalid handle");
            return;
        }

        csScriptNames[handle] = mono_class_get_name(mono_object_get_class(obj));

        csScriptsOverridden[0][csScriptNames[handle]] =
            monoAPI.IsOverridden(mono_object_get_class(obj),
                monoAPI.GetAssClass("Team_Popplio", "PopplioMonoScript"), "Setup");

        csScriptsOverridden[1][csScriptNames[handle]] =
            monoAPI.IsOverridden(mono_object_get_class(obj),
                monoAPI.GetAssClass("Team_Popplio", "PopplioMonoScript"), "Init");

        csScriptsOverridden[2][csScriptNames[handle]] =
            monoAPI.IsOverridden(mono_object_get_class(obj),
                monoAPI.GetAssClass("Team_Popplio", "PopplioMonoScript"), "Update");

        csScriptsOverridden[3][csScriptNames[handle]] =
            monoAPI.IsOverridden(mono_object_get_class(obj),
                monoAPI.GetAssClass("Team_Popplio", "PopplioMonoScript"), "FixedUpdate");

        csScriptsOverridden[4][csScriptNames[handle]] =
            monoAPI.IsOverridden(mono_object_get_class(obj),
                monoAPI.GetAssClass("Team_Popplio", "PopplioMonoScript"), "End");

        int id = monoAPI.Get<int>(obj, mono_class_get_field_from_name(mono_object_get_class(obj), "EntID"));
        csEntityLoadedScripts[id] = handle;
        csLoadedScriptsEntityId[handle] = id;
    }

    void LogicSystem::CSAddToLoadedScripts(int id)
    {
        if (id < 0)
        {
            Logger::Error("LogicSystem: CSAddToLoadedScripts | Invalid entity ID");
            return;
        }

        Entity e = reg.GetEntityById(id);

        if (LoadCSScript(e)) CSLoadScriptAtRun(mono_gchandle_get_target(csLoadedScripts.back()));
        else Logger::Error("LogicSystem: CSAddToLoadedScripts | Failed to load script");

        //MonoObject* obj = SetupCSScript(e);
        //if (obj != nullptr && obj != reinterpret_cast<void*>(-1))
        //{
        //    if (LoadCSScript(e)) CSLoadScriptAtRun(csLoadedScripts.back());
        //    else Logger::Error("LogicSystem: CSAddToLoadedScripts | Failed to load script");

        //    //csLoadedScripts.push_back(obj);

        //    //SetLogicComponentRuntimeVars(*i);
        //    //i->GetComponent<LogicComponent>().scriptObject = obj;
        //    //CSLoadScriptAtRun(csLoadedScripts.back());
        //}
        ////if (LoadCSScript(*i)) CSLoadScriptAtRun(csLoadedScripts.back());
        //else
        //{
        //    Logger::Error("LogicSystem: CSAddToLoadedScripts | Failed to load script");
        //}
    }

    void LogicSystem::CSUpdateLogicObjects()
    {
        for (auto i = csLoadedScripts.begin(); i != csLoadedScripts.end(); ++i)
        {
            if (!monoAPI.IsObjValid(*i))
            {
                csScriptsToBeDeleted.push_back(*i);
                continue;
            }
            reg.GetEntityById(csLoadedScriptsEntityId[*i]).GetComponent<LogicComponent>().scriptObject = 
                mono_gchandle_get_target(*i);
        }
    }
}