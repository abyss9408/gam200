/******************************************************************************/
/*!
\file   LogicComponent.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the header file for engine Logic System component
        Attaches scripts to game objects

        Enables scripting functionality by separating game from engine code
        Implements C# Mono Framework (TODO)

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contentsv
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <map>
#include <vector>
#include "../Serialization/SerializeTypes.h"
#include "../Mono/MonoAPI.h"

#include "Logic.h" // avoid circular dependency
#include "LogicSystem.h"

namespace Popplio
{
    class LogicSystem;

    enum class ScriptLang
    {
        UNASSIGNED = -1,
        CPP = 0,
        CS = 1
    };

    // Attach to gameobjects (Entity)
    struct LogicComponent
    {
    private:
        //Entity* entity = nullptr; // attached entity todo: work around currently
        //Prefab* prefab = nullptr; // attached prefab todo: work around currently

        //size_t scriptIndex; // do not serialize REMEMBER TO UPDATE WHEN UPDATING SCRIPTNAME

    public:

        // Serialize //

        bool isActive;

        std::string scriptName;

        ScriptLang scriptLang; // -1 = unassigned, 0 = C++, 1 = C#

        // variables //

        // key is var name, value is var value
        std::vector<std::pair<std::string, bool>>           bools;
        std::vector<std::pair<std::string, int>>            ints;
        std::vector<std::pair<std::string, float>>          floats;
        std::vector<std::pair<std::string, double>>         doubles;
        std::vector<std::pair<std::string, char>>           chars;
        std::vector<std::pair<std::string, std::string>>    strings;
        std::vector<std::pair<std::string, int>>            entities;
        std::vector<std::pair<std::string, std::string>>    prefabs; // TODO
        //std::vector<std::pair<std::string, std::string>>    scripts; // TODO
        std::vector<std::pair<std::string, std::pair<int, std::string>>>    components; // TODO

        // TODO: Serialize
        std::vector<std::pair<std::string, std::vector<bool>>>          boolLists;
        std::vector<std::pair<std::string, std::vector<int>>>           intLists;
        std::vector<std::pair<std::string, std::vector<float>>>         floatLists;
        std::vector<std::pair<std::string, std::vector<double>>>        doubleLists;
        std::vector<std::pair<std::string, std::vector<char>>>          charLists;
        std::vector<std::pair<std::string, std::vector<std::string>>>   stringLists;
        std::vector<std::pair<std::string, std::vector<int>>>           entityLists;
        std::vector<std::pair<std::string, std::vector<std::string>>>   prefabLists; // TODO
        //std::vector<std::pair<std::string, std::vector<std::string>>>   scriptLists; // TODO
        std::vector<std::pair<std::string, std::vector<std::pair<int, std::string>>>>   componentLists; // TODO

        // ========================================================================= //

        // Runtime // Do not serialize:

        LogicSystem* logicSystem;   // set by logicsystem at runtime
        int entityID;               // entity id at runtime // set by logicsystem at runtime

        bool currIsActive;          // current active state // set by logicsystem at runtime

        // C#
        MonoObject* scriptObject;   // script object at runtime // set by logicsystem at runtime

        //std::map<std::string, std::pair<std::type_info&, void*>> vars{};

        // ========================================================================= //

        /*
        *   logic component constructor to be used by ECS
        */
        LogicComponent(std::string const& s = "") : 
            isActive(true), scriptName(s) , scriptLang(ScriptLang::UNASSIGNED), 
            logicSystem(nullptr), entityID(-1), currIsActive(true), scriptObject(nullptr) {}

        //LogicComponent() : scriptName(""), vars() {}

        //LogicComponent(std::string const& s,
        //    std::map<std::string, std::pair<std::type_info&, void*>> v) :
        //    scriptName(s), vars(v) {}

        //~LogicComponent() = default;

        // doesnt work due to how ECS works
        //~LogicComponent() // destroy script object at runtime
        //{
        //    if (!IsRuntime()) return;

        //    switch (scriptLang)
        //    {
        //        case ScriptLang::CPP:
        //            break;
        //        case ScriptLang::CS:
        //            if (scriptObject == nullptr) return;
        //            logicSystem->CSOnEntityDestroy(entityID, scriptObject);
        //            break;
        //        case ScriptLang::UNASSIGNED:
        //            break;
        //    }
        //}

        /*
        *   clone variable values
        */
        void Clone(LogicComponent const& other, int id)
        {
            (void)other;
            //isActive = other.isActive;
            //scriptName = other.scriptName;
            //scriptLang = other.scriptLang;


            //bools = other.bools;
            //ints = other.ints;
            //floats = other.floats;
            //doubles = other.doubles;
            //chars = other.chars;
            //strings = other.strings;
            //entities = other.entities; // reference
            //prefabs = other.prefabs; // reference
            //scripts = other.scripts; // reference

            //boolLists = other.boolLists;
            //intLists = other.intLists;
            //floatLists = other.floatLists;
            //doubleLists = other.doubleLists;
            //charLists = other.charLists;
            //stringLists = other.stringLists;
            //entityLists = other.entityLists; // reference
            //prefabLists = other.prefabLists; // reference
            //scriptLists = other.scriptLists; // reference

            //logicSystem = other.logicSystem;
            //currIsActive = other.currIsActive;

            entityID = id;

            //reg.ForceRuntimeUpdate(reg.GetEntityById(id));
        }

        /*
        *   reset script variables
        */
        void ResetScriptVars()
        {
            //isActive = true;
            //scriptName = "";
            //scriptLang = ScriptLang::UNASSIGNED;
            bools.clear();
            ints.clear();
            floats.clear();
            doubles.clear();
            chars.clear();
            strings.clear();
            entities.clear();
            prefabs.clear();
            components.clear();
            //scripts.clear();
            boolLists.clear();
            intLists.clear();
            floatLists.clear();
            doubleLists.clear();
            charLists.clear();
            stringLists.clear();
            entityLists.clear();
            prefabLists.clear();
            componentLists.clear();
            //scriptLists.clear();
        }

        /*
        *   check if script has logic system
        */
        bool IsRuntime() const
        {
            if (logicSystem == nullptr) return false;
            return entityID >= 0;
        }

        // ========================================================================= //

        // Serialization //

        void Serialize(rapidjson::Value& objectValue, rapidjson::Value& componentValue, 
            rapidjson::Document::AllocatorType& allocator, LogicSystem& logic) const
        {
            (void)logic;

            SerializeType("isActive", componentValue, isActive, allocator);

            SerializeType("name", componentValue, scriptName, allocator);

            SerializeType("scriptLang", componentValue, static_cast<int>(scriptLang), allocator);

            //if (scriptLang == ScriptLang::UNASSIGNED)
            //{
            //    objectValue.AddMember("logic", componentValue, allocator);
            //    return;
            //}

            // variables //

            // TODO fix vv // This is required to remove non-existent variables that were once serialized

            //MonoClass* scriptClass = nullptr;
            //void* scriptIter = nullptr;
            //MonoClassField* varField = nullptr;

            // check if component has existent variable
            //for (unsigned int index{}; index < bools.size(); ++index)
            //{
            //    std::vector<std::pair<std::string, bool>>::const_iterator i = bools.begin() + index;
            //    bool doesNotExist = true;
            //    while ((varField = mono_class_get_fields(scriptClass, &scriptIter)) != nullptr)
            //    {
            //        if (logic.monoAPI.IsFieldStatic(varField)) continue; // skip static fields
            //        // Check for the Inspector attribute

            //        std::string fieldName = mono_field_get_name(varField);
            //        if (fieldName == i->first)
            //        {
            //            doesNotExist = false;
            //            if (mono_type_get_type(mono_field_get_type(varField)) != MONO_TYPE_BOOLEAN) bools.erase(i);
            //            break;
            //        }
            //    }
            //    if (doesNotExist) bools.erase(i);
            //}

            SerializeObjType("bools", componentValue, allocator, bools);
            SerializeObjType("ints", componentValue, allocator, ints);
            SerializeObjType("floats", componentValue, allocator, floats);
            SerializeObjType("doubles", componentValue, allocator, doubles);
            SerializeObjType("chars", componentValue, allocator, chars);
            SerializeObjType("strings", componentValue, allocator, strings);
            SerializeObjType("entities", componentValue, allocator, entities);
            SerializeObjType("prefabs", componentValue, allocator, prefabs);
            //SerializeObjType("scripts", componentValue, allocator, scripts);
            SerializeObjType("components", componentValue, allocator, components);

            SerializeObjType("boolLists", componentValue, allocator, boolLists);
            SerializeObjType("intLists", componentValue, allocator, intLists);
            SerializeObjType("floatLists", componentValue, allocator, floatLists);
            SerializeObjType("doubleLists", componentValue, allocator, doubleLists);
            SerializeObjType("charLists", componentValue, allocator, charLists);
            SerializeObjType("stringLists", componentValue, allocator, stringLists);
            SerializeObjType("entityLists", componentValue, allocator, entityLists);
            SerializeObjType("prefabLists", componentValue, allocator, prefabLists);
            //SerializeObjType("scriptLists", componentValue, allocator, scriptLists);
            SerializeObjType("componentLists", componentValue, allocator, componentLists);

            objectValue.AddMember("logic", componentValue, allocator);
        }

        template <typename ObjectType>
        static void Deserialize(rapidjson::Value const& componentVal, ObjectType& object, bool safeMode)
        {
            bool isActive;
            std::string compName;
            ScriptLang lang;

            // variables
            std::vector<std::pair<std::string, bool>>           bools;
            std::vector<std::pair<std::string, int>>            ints;
            std::vector<std::pair<std::string, float>>          floats;
            std::vector<std::pair<std::string, double>>         doubles;
            std::vector<std::pair<std::string, char>>           chars;
            std::vector<std::pair<std::string, std::string>>    strings;
            std::vector<std::pair<std::string, int>>            entities;
            std::vector<std::pair<std::string, std::string>>    prefabs;
            //std::vector<std::pair<std::string, std::string>>    scripts;
            std::vector<std::pair<std::string, std::pair<int, std::string>>>    components; // TODO

            std::vector<std::pair<std::string, std::vector<bool>>>          boolLists;
            std::vector<std::pair<std::string, std::vector<int>>>           intLists;
            std::vector<std::pair<std::string, std::vector<float>>>         floatLists;
            std::vector<std::pair<std::string, std::vector<double>>>        doubleLists;
            std::vector<std::pair<std::string, std::vector<char>>>          charLists;
            std::vector<std::pair<std::string, std::vector<std::string>>>   stringLists;
            std::vector<std::pair<std::string, std::vector<int>>>           entityLists;
            std::vector<std::pair<std::string, std::vector<std::string>>>   prefabLists;
            //std::vector<std::pair<std::string, std::vector<std::string>>>   scriptLists;
            std::vector<std::pair<std::string, std::vector<std::pair<int, std::string>>>>   componentLists; // TODO

            if (safeMode)
            {
                if (!componentVal.IsObject())
                {
                    Logger::Error("Invalid logic component structure");
                    return;
                }

                if (!componentVal.HasMember("isActive"))
                {
                    isActive = true;
                    Logger::Warning("Invalid logic component isActive structure, using defaults");
                }
                else isActive = componentVal["isActive"].GetBool();

                if (!componentVal.HasMember("name"))
                {
                    compName = "";
                    Logger::Warning("Invalid logic component name structure, using defaults");
                }
                else compName = componentVal["name"].GetString();

                if (!componentVal.HasMember("scriptLang"))
                {
                    lang = ScriptLang::UNASSIGNED;
                    Logger::Warning("Invalid logic component scriptLang structure, using defaults");
                }
                else lang = static_cast<ScriptLang>(componentVal["scriptLang"].GetInt());

                // variables

                if (!componentVal.HasMember("bools"))
                {
                    Logger::Warning("Invalid logic component bools structure, using defaults");
                }
                else
                {
                    for (auto& [name, value] : componentVal["bools"].GetObject())
                    {
                        bools.push_back(std::make_pair(name.GetString(), value.GetBool()));
                    }
                }

                if (!componentVal.HasMember("ints"))
                {
                    Logger::Warning("Invalid logic component ints structure, using defaults");
                }
                else
                {
                    for (auto& [name, value] : componentVal["ints"].GetObject())
                    {
                        ints.push_back(std::make_pair(name.GetString(), value.GetInt()));
                    }
                }

                if (!componentVal.HasMember("floats"))
                {
                    Logger::Warning("Invalid logic component floats structure, using defaults");
                }
                else
                {
                    for (auto& [name, value] : componentVal["floats"].GetObject())
                    {
                        floats.push_back(std::make_pair(name.GetString(), value.GetFloat()));
                    }
                }

                if (!componentVal.HasMember("doubles"))
                {
                    Logger::Warning("Invalid logic component doubles structure, using defaults");
                }
                else
                {
                    for (auto& [name, value] : componentVal["doubles"].GetObject())
                    {
                        doubles.push_back(std::make_pair(name.GetString(), value.GetDouble()));
                    }
                }

                if (!componentVal.HasMember("chars"))
                {
                    Logger::Warning("Invalid logic component chars structure, using defaults");
                }
                else
                {
                    for (auto& [name, value] : componentVal["chars"].GetObject())
                    {
                        chars.push_back(std::make_pair(name.GetString(), value.GetString()[0]));
                    }
                }

                if (!componentVal.HasMember("strings"))
                {
                    Logger::Warning("Invalid logic component strings structure, using defaults");
                }
                else
                {
                    for (auto& [name, value] : componentVal["strings"].GetObject())
                    {
                        strings.push_back(std::make_pair(name.GetString(), value.GetString()));
                    }
                }

                if (!componentVal.HasMember("entities"))
                {
                    Logger::Warning("Invalid logic component entities structure, using defaults");
                }
                else
                {
                    for (auto& [name, value] : componentVal["entities"].GetObject())
                    {
                        entities.push_back(std::make_pair(name.GetString(), value.GetInt()));
                    }
                }

                if (!componentVal.HasMember("prefabs"))
                {
                    Logger::Warning("Invalid logic component prefabs structure, using defaults");
                }
                else
                {
                    for (auto& [name, value] : componentVal["prefabs"].GetObject())
                    {
                        prefabs.push_back(std::make_pair(name.GetString(), value.GetString()));
                    }
                }

                //if (!componentVal.HasMember("scripts"))
                //{
                //    Logger::Warning("Invalid logic component scripts structure, using defaults");
                //}
                //else
                //{
                //    for (auto& [name, value] : componentVal["scripts"].GetObject())
                //    {
                //        scripts.push_back(std::make_pair(name.GetString(), value.GetString()));
                //    }
                //}

                if (!componentVal.HasMember("components"))
                {
                    Logger::Warning("Invalid logic component components structure, using defaults");
                }
                else
                {
                    int ent = 0;
                    std::string comp = "";
                    for (auto& [name, value] : componentVal["components"].GetObject())
                    {
                        if (static_cast<std::string>(name.GetString()).find("comp") != std::string::npos)
                        {
                            comp = value.GetString();
                            std::pair<int, std::string> entComp = std::make_pair(ent, comp);
                            components.push_back(std::make_pair(name.GetString(), entComp));
                        }
                        else
                        {
                            ent = value.GetInt();
                        }
                    }
                }

                if (!componentVal.HasMember("boolLists"))
                {
                    Logger::Warning("Invalid logic component boolLists structure, using defaults");
                }
                else
                {
                    for (auto& [name, value] : componentVal["boolLists"].GetObject())
                    {
                        
                        std::vector<bool> list;
                        for (auto& [n, v] : value.GetObject())
                        {
                            list.push_back(v.GetBool());
                        }
                        boolLists.push_back(std::make_pair(name.GetString(), list));
                    }
                }

                if (!componentVal.HasMember("intLists"))
                {
                    Logger::Warning("Invalid logic component intLists structure, using defaults");
                }
                else
                {
                    for (auto& [name, value] : componentVal["intLists"].GetObject())
                    {
                        
                        std::vector<int> list;
                        for (auto& [n, v] : value.GetObject())
                        {
                            list.push_back(v.GetInt());
                        }
                        intLists.push_back(std::make_pair(name.GetString(), list));
                    }
                }

                if (!componentVal.HasMember("floatLists"))
                {
                    Logger::Warning("Invalid logic component floatLists structure, using defaults");
                }
                else
                {
                    for (auto& [name, value] : componentVal["floatLists"].GetObject())
                    {
                        
                        std::vector<float> list;
                        for (auto& [n, v] : value.GetObject())
                        {
                            list.push_back(v.GetFloat());
                        }
                        floatLists.push_back(std::make_pair(name.GetString(), list));
                    }
                }

                if (!componentVal.HasMember("doubleLists"))
                {
                    Logger::Warning("Invalid logic component doubleLists structure, using defaults");
                }
                else
                {
                    for (auto& [name, value] : componentVal["doubleLists"].GetObject())
                    {
                        
                        std::vector<double> list;
                        for (auto& [n, v] : value.GetObject())
                        {
                            list.push_back(v.GetDouble());
                        }
                        doubleLists.push_back(std::make_pair(name.GetString(), list));
                    }
                }

                if (!componentVal.HasMember("charLists"))
                {
                    Logger::Warning("Invalid logic component charLists structure, using defaults");
                }
                else
                {
                    for (auto& [name, value] : componentVal["charLists"].GetObject())
                    {
                        
                        std::vector<char> list;
                        for (auto& [n, v] : value.GetObject())
                        {
                            list.push_back(v.GetString()[0]);
                        }
                        charLists.push_back(std::make_pair(name.GetString(), list));
                    }
                }

                if (!componentVal.HasMember("stringLists"))
                {
                    Logger::Warning("Invalid logic component stringLists structure, using defaults");
                }
                else
                {
                    for (auto& [name, value] : componentVal["stringLists"].GetObject())
                    {
                        
                        std::vector<std::string> list;
                        for (auto& [n, v] : value.GetObject())
                        {
                            list.push_back(v.GetString());
                        }
                        stringLists.push_back(std::make_pair(name.GetString(), list));
                    }
                }

                if (!componentVal.HasMember("entityLists"))
                {
                    Logger::Warning("Invalid logic component entityLists structure, using defaults");
                }
                else
                {
                    for (auto& [name, value] : componentVal["entityLists"].GetObject())
                    {
                        
                        std::vector<int> list;
                        for (auto& [n, v] : value.GetObject())
                        {
                            list.push_back(v.GetInt());
                        }
                        entityLists.push_back(std::make_pair(name.GetString(), list));
                    }
                }

                if (!componentVal.HasMember("prefabLists"))
                {
                    Logger::Warning("Invalid logic component prefabLists structure, using defaults");
                }
                else
                {
                    for (auto& [name, value] : componentVal["prefabLists"].GetObject())
                    {

                        std::vector<std::string> list;
                        for (auto& [n, v] : value.GetObject())
                        {
                            list.push_back(v.GetString());
                        }
                        prefabLists.push_back(std::make_pair(name.GetString(), list));
                    }
                }

                //if (!componentVal.HasMember("scriptLists"))
                //{
                //    Logger::Warning("Invalid logic component scriptLists structure, using defaults");
                //}
                //else
                //{
                //    for (auto& [name, value] : componentVal["scriptLists"].GetObject())
                //    {
                //        
                //        std::vector<std::string> list;
                //        for (auto& [n, v] : value.GetObject())
                //        {
                //            list.push_back(v.GetString());
                //        }
                //        scriptLists.push_back(std::make_pair(name.GetString(), list));
                //    }
                //}

                if (!componentVal.HasMember("componentLists"))
                {
                    Logger::Warning("Invalid logic component componentLists structure, using defaults");
                }
                else
                {
                    for (auto& [name, value] : componentVal["componentLists"].GetObject())
                    {
                        std::vector<std::pair<int, std::string>> list;
                        int ent = 0;
                        std::string comp = "";
                        for (auto& [n, v] : value.GetObject())
                        {
                            if (static_cast<std::string>(n.GetString()).find("ent") != std::string::npos)
                            {
                                ent = v.GetInt();
                            }
                            else
                            {
                                comp = v.GetString();
                                list.push_back(std::make_pair(ent, comp));
                            }
                        }
                        componentLists.push_back(std::make_pair(name.GetString(), list));
                    }
                }
            }
            else
            {
                isActive = componentVal["isActive"].GetBool();
                compName = componentVal["name"].GetString();
                lang = static_cast<ScriptLang>(componentVal["scriptLang"].GetInt());

                for (auto& [name, value] : componentVal["bools"].GetObject())
                {
                    bools.push_back(std::make_pair(name.GetString(), value.GetBool()));
                }

                for (auto& [name, value] : componentVal["ints"].GetObject())
                {
                    ints.push_back(std::make_pair(name.GetString(), value.GetInt()));
                }

                for (auto& [name, value] : componentVal["floats"].GetObject())
                {
                    floats.push_back(std::make_pair(name.GetString(), value.GetFloat()));
                }

                for (auto& [name, value] : componentVal["doubles"].GetObject())
                {
                    doubles.push_back(std::make_pair(name.GetString(), value.GetDouble()));
                }

                for (auto& [name, value] : componentVal["chars"].GetObject())
                {
                    chars.push_back(std::make_pair(name.GetString(), value.GetString()[0]));
                }

                for (auto& [name, value] : componentVal["strings"].GetObject())
                {
                    strings.push_back(std::make_pair(name.GetString(), value.GetString()));
                }

                for (auto& [name, value] : componentVal["entities"].GetObject())
                {
                    entities.push_back(std::make_pair(name.GetString(), value.GetInt()));
                }

                for (auto& [name, value] : componentVal["prefabs"].GetObject())
                {
                    prefabs.push_back(std::make_pair(name.GetString(), value.GetString()));
                }

                //for (auto& [name, value] : componentVal["scripts"].GetObject())
                //{
                //    scripts.push_back(std::make_pair(name.GetString(), value.GetString()));
                //}

                {
                    int ent = 0;
                    std::string comp = "";
                    for (auto& [name, value] : componentVal["components"].GetObject())
                    {
                        if (static_cast<std::string>(name.GetString()).find("comp") != std::string::npos)
                        {
                            comp = value.GetString();
                            std::pair<int, std::string> entComp = std::make_pair(ent, comp);
                            components.push_back(std::make_pair(name.GetString(), entComp));
                        }
                        else
                        {
                            ent = value.GetInt();
                        }
                    }
                }

                for (auto& [name, value] : componentVal["boolLists"].GetObject())
                {
                    
                    std::vector<bool> list{};
                    for (auto& [n, v] : value.GetObject())
                        list.push_back(v.GetBool());
                    boolLists.push_back(std::make_pair(name.GetString(), list));
                }

                for (auto& [name, value] : componentVal["intLists"].GetObject())
                {
                    
                    std::vector<int> list{};
                    for (auto& [n, v] : value.GetObject())
                        list.push_back(v.GetInt());
                    intLists.push_back(std::make_pair(name.GetString(), list));
                }

                for (auto& [name, value] : componentVal["floatLists"].GetObject())
                {
                    
                    std::vector<float> list{};
                    for (auto& [n, v] : value.GetObject())
                        list.push_back(v.GetFloat());
                    floatLists.push_back(std::make_pair(name.GetString(), list));
                }

                for (auto& [name, value] : componentVal["doubleLists"].GetObject())
                {
                    
                    std::vector<double> list{};
                    for (auto& [n, v] : value.GetObject())
                        list.push_back(v.GetDouble());
                    doubleLists.push_back(std::make_pair(name.GetString(), list));
                }

                for (auto& [name, value] : componentVal["charLists"].GetObject())
                {
                    
                    std::vector<char> list{};
                    for (auto& [n, v] : value.GetObject())
                        list.push_back(v.GetString()[0]);
                    charLists.push_back(std::make_pair(name.GetString(), list));
                }

                for (auto& [name, value] : componentVal["stringLists"].GetObject())
                {
                    
                    std::vector<std::string> list{};
                    for (auto& [n, v] : value.GetObject())
                        list.push_back(v.GetString());
                    stringLists.push_back(std::make_pair(name.GetString(), list));
                }

                for (auto& [name, value] : componentVal["entityLists"].GetObject())
                {
                    
                    std::vector<int> list{};
                    for (auto& [n, v] : value.GetObject())
                        list.push_back(v.GetInt());
                    entityLists.push_back(std::make_pair(name.GetString(), list));
                }

                for (auto& [name, value] : componentVal["prefabLists"].GetObject())
                {

                    std::vector<std::string> list{};
                    for (auto& [n, v] : value.GetObject())
                        list.push_back(v.GetString());
                    prefabLists.push_back(std::make_pair(name.GetString(), list));
                }

                //for (auto& [name, value] : componentVal["scriptLists"].GetObject())
                //{
                //    
                //    std::vector<std::string> list{};
                //    for (auto& [n, v] : value.GetObject())
                //        list.push_back(v.GetString());
                //    scriptLists.push_back(std::make_pair(name.GetString(), list));
                //}

                for (auto& [name, value] : componentVal["componentLists"].GetObject())
                {
                    std::vector<std::pair<int, std::string>> list;
                    int ent = 0;
                    std::string comp = "";
                    for (auto& [n, v] : value.GetObject())
                    {
                        if (static_cast<std::string>(n.GetString()).find("ent") != std::string::npos)
                        {
                            ent = v.GetInt();
                        }
                        else
                        {
                            comp = v.GetString();
                            list.push_back(std::make_pair(ent, comp));
                        }
                    }
                    componentLists.push_back(std::make_pair(name.GetString(), list));
                }
            }

            object.AddComponent<LogicComponent>(compName);
            object.GetComponent<LogicComponent>().isActive = isActive;
            object.GetComponent<LogicComponent>().scriptLang = lang;

            object.GetComponent<LogicComponent>().bools = bools;
            object.GetComponent<LogicComponent>().ints = ints;
            object.GetComponent<LogicComponent>().floats = floats;
            object.GetComponent<LogicComponent>().doubles = doubles;
            object.GetComponent<LogicComponent>().chars = chars;
            object.GetComponent<LogicComponent>().strings = strings;
            object.GetComponent<LogicComponent>().entities = entities;
            object.GetComponent<LogicComponent>().prefabs = prefabs;
            //object.GetComponent<LogicComponent>().scripts = scripts;
            object.GetComponent<LogicComponent>().components = components;

            object.GetComponent<LogicComponent>().boolLists = boolLists;
            object.GetComponent<LogicComponent>().intLists = intLists;
            object.GetComponent<LogicComponent>().floatLists = floatLists;
            object.GetComponent<LogicComponent>().doubleLists = doubleLists;
            object.GetComponent<LogicComponent>().charLists = charLists;
            object.GetComponent<LogicComponent>().stringLists = stringLists;
            object.GetComponent<LogicComponent>().entityLists = entityLists;
            object.GetComponent<LogicComponent>().prefabLists = prefabLists;
            //object.GetComponent<LogicComponent>().scriptLists = scriptLists;
            object.GetComponent<LogicComponent>().componentLists = componentLists;
        }
    };

    //struct LogicComponent
    //{
    //private:
    //    Entity* entity; // attached entity todo: work around currently
    //    size_t scriptIndex; // do not serialize

    //public:
    //    std::string scriptName;

    //    LogicComponent() : entity(nullptr), scriptIndex(0), scriptName("") {}
    //    LogicComponent(size_t const& i) : entity(nullptr), scriptIndex(i), scriptName("") {} // todo: get script name
    //    LogicComponent(std::string const& s) : entity(nullptr), scriptIndex(0), scriptName(s) {} // todo: get script index
    //    ~LogicComponent() { entity = nullptr; }

    //    size_t const& GetIndex() const { return scriptIndex; }

    //    Entity& GetEntity() { return *entity; }
    //    Entity const& GetEntity() const { return *entity; }

    //    void UpdateEntity(Entity* e) { entity = e; } // workaround
    //};
}