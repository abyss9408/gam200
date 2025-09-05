/******************************************************************************/
/*!
\file   MonoHotReload.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the source file for mono hot reload functionality

        Enables hot reload compile for C# scripts in the engine with mono

        Extension of MonoAPI in PopplioScriptAPI

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>

#include "MonoAPI.h"

#include "../Logging/Logger.h"

#include <mono/metadata/debug-helpers.h>
#include <filesystem>
#include <thread>
#include <chrono>

namespace PopplioScriptAPI
{
    void MonoAPI::UnloadAssembly()
    {
        Popplio::Logger::Info("PopplioScriptAPI::MonoAPI | Unloading Mono...");

        loadedUserClasses.clear();
        loadedEngineClasses.clear();
        loadedClasses.clear();

        loadedUserClassNames.clear();
        loadedEngineClassNames.clear();
        loadedClassNames.clear();

        // Free all GC handles
        ClearAllGC();
        //for (auto& [obj, handle] : gcHandles) mono_gchandle_free(handle);
        //gcHandles.clear();
        //for (auto& [obj, handle] : gcLifeHandles) mono_gchandle_free(handle);
        //gcLifeHandles.clear();

        ForceGC();

        mono_domain_set(mono_get_root_domain(), false);

        mono_domain_unload(appDomain);
    }

    void MonoAPI::ReloadAssembly()
    {
        try
        {
            Popplio::Logger::Info("PopplioScriptAPI::MonoAPI | Reloading Mono...");

            UnloadAssembly();

            //ShutdownMono();

            StartMono();

            Popplio::Logger::Info("PopplioScriptAPI::MonoAPI | Successfully reloaded Mono!");
        }
        catch (const std::exception & e)
        {
            std::stringstream s{ "" };
            s << "PopplioScriptAPI::MonoAPI | Failed to reload Mono! - " << e.what();

            Popplio::Logger::Error(s.str());
        }
    }

    void MonoAPI::StartObserving()
    {
        std::thread observerThread
        ([&]()
            {
                while (hotReloadEnabled)
                {
                    Popplio::Logger::Debug("PopplioScriptAPI::MonoAPI | Observing assembly for hot reload");
                    auto currModified = std::filesystem::last_write_time(assemblyPath); // get current modified time

                    if (currModified != lastModified) // modified
                    {
                        ReloadAssembly(); // reload assembly
                        lastModified = currModified; // set new modified time
                    }

                    std::this_thread::sleep_for(std::chrono::seconds(observeInterval)); // sleep for ___ seconds
                }
            }
        );

        observerThread.detach(); // detach thread
    }

    //void MonoAPI::ReloadAssembly()
    //{
    //    if (assembly != nullptr) mono_assembly_close(assembly); // unload

    //    assembly = mono_domain_assembly_open(domain, assemblyPath.c_str()); // reload

    //    if (assembly == nullptr) // failed
    //    {
    //        Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Failed to reload assembly");
    //        return;
    //    }

    //    // invoke hot reload method
    //    MonoImage* image = mono_assembly_get_image(assembly); // get image
    //    MonoClass* classObj = mono_class_from_name(image, "Team_Popplio", "MonoHotReload"); // get class
    //    MonoMethod* method = mono_class_get_method_from_name(classObj, "OnHotReload", 0); // get method

    //    if (method == nullptr) // failed
    //    {
    //        Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Failed to get hot reload method");
    //        return;
    //    }

    //    MonoObject* except{ nullptr }; // exception
    //    mono_runtime_invoke(method, nullptr, nullptr, &except); // call method
    //}
}