/******************************************************************************/
/*!
\file   CPPScripts.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the template file for C++ scripts to be loaded into the engine
        Function templates for registering and creating scripts
        TODO : Try to not hard code the script registration like here

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

namespace Popplio
{
    //template <class T>
    //static std::shared_ptr<ILogicScript> ScriptRegistry::CPPScriptFactory(Entity* e)
    //{
    //    return std::make_shared<T>(e);
    //}

    template <class T>
    std::shared_ptr<T> ScriptFactory::Create(Entity* e)
    {
        return std::make_shared<T>(e);
    }

    // Create a script type
    template <class T>
    std::shared_ptr<T> ScriptRegistry::Create(std::string const& scriptName, Entity* e) const
    {
        auto it = std::find(scripts.begin(), scripts.end(), scriptName);

        if (it != scripts.end())
        {
            return ScriptFactory::Create<T>(e);
        }

        Logger::Error("LogicSystem: Failed to create script " + scriptName + " in ScriptRegistry", LogDestination::IMGUI_CONSOLE);
        throw std::runtime_error("LogicSystem: Failed to create script " + scriptName + " in ScriptRegistry");
    }

    // Register data to registry
    template<typename T>
    void DataRegistry::RegisterData(std::string const& dataName)
    {
        factories[dataName] = []() -> std::shared_ptr<ILogicData>
            { return std::make_shared<T>(); };
    }

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

    template <class T>
    std::shared_ptr<T> LogicSystem::InstScript(std::string const& scriptType, Entity* e)
    {
        std::shared_ptr<T> tmp = scriptReg.Create<T>(scriptType, e);
        if (!tmp)
        {
            std::stringstream ss{ "" };
            ss << "LogicSystem: Failed to instantiate script " << scriptType << " for entity " << e->GetId();
            Logger::Error(ss.str());
            return nullptr;
        }
        instScripts.emplace(e->GetId(), tmp);
        return tmp;
    }

    template <class T>
    void LogicSystem::LoadScript(std::string const& scriptType)
    {
        scriptReg.RegisterScript(scriptType);
        std::shared_ptr<T> tmp = ScriptFactory::Create<T>(nullptr);
        if (!tmp)
        {
            Logger::Error("LogicSystem: Failed to load script" + scriptType, LogDestination::IMGUI_CONSOLE);
            return;
        }
        AssetStore::StoreScriptAsset(scriptType, tmp);
        scripts.emplace(scriptType, tmp);
    }

    //template <class T>
    //std::shared_ptr<T> LogicSystem::InstScript(std::string const& scriptType, Entity* e)
    //{
    //    std::shared_ptr<ILogicScript> tmp = scriptReg.Create(scriptType, e);
    //    if (!tmp)
    //    {
    //        std::stringstream ss{ "" };
    //        ss << "LogicSystem: Failed to instantiate script " << scriptType << " for entity " << e->GetId();
    //        Logger::Error(ss.str());
    //        return;
    //    }
    //    instScripts.emplace(e->GetId(), tmp);
    //    return tmp;
    //}
}
