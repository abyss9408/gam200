/******************************************************************************/
/*!
\file   MonoAPI.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the source file for C# Script API (using mono)

        Enables scripting functionality by separating game from engine code
        Implements C# Mono Framework
        Reference:
        https://nilssondev.com/mono-guide/book/

        Extensive API to use internal calls, get/set fields, properties, call methods, and more
        Links with the main executable as a DLL and communicates with main engine DLL

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>

#include "MonoAPI.h"

#include "../Logging/Logger.h"

//#include "MonoNative.h" 
#include "MonoInternalCall.h" // calls to c# from c++
#include "MonoUtilities.h"

#include <mono/metadata/attrdefs.h> // Accessiblity
#include <mono/metadata/threads.h>
#include <mono/metadata/mono-gc.h>
//#include <mono/metadata/gc.h>
#include <mono/metadata/mono-debug.h>
#include <mono/metadata/object.h>
#include <mono/metadata/debug-helpers.h>
//#include <mono/metadata/debug-mono-symfile.h>
#include <mono/metadata/class.h>
#include <mono/metadata/assembly.h>

#include <iostream>
#include <thread>

//#include <mono/eglib/glib.h>

// Code to call from C# //
#include "../ECS/ECS.h"
// Code to call from C# //

#include "../tests/TestMono.h" // tests

namespace PopplioScriptAPI
{
    // access modifiers

    bool MonoAPI::IsFieldStatic(MonoClassField* field)
    {
        return mono_field_get_flags(field) & MONO_FIELD_ATTR_STATIC;
    }

    uint8_t MonoAPI::GetFieldAccess(MonoClassField* field) // get field access
    {
        uint8_t access = static_cast<uint8_t>(Access::None); // initialize to none
        uint32_t accessFlag = mono_field_get_flags(field) & MONO_FIELD_ATTR_FIELD_ACCESS_MASK; // set access flag

        switch (accessFlag) // sets flag accordingly
        {
            case MONO_FIELD_ATTR_PRIVATE:
            {
                access = static_cast<uint8_t>(Access::Private);
                break;
            }
            case MONO_FIELD_ATTR_FAM_AND_ASSEM:
            {
                access |= static_cast<uint8_t>(Access::Protected);
                access |= static_cast<uint8_t>(Access::Internal);
                break;
            }
            case MONO_FIELD_ATTR_ASSEMBLY:
            {
                access = static_cast<uint8_t>(Access::Internal);
                break;
            }
            case MONO_FIELD_ATTR_FAMILY:
            {
                access = static_cast<uint8_t>(Access::Protected);
                break;
            }
            case MONO_FIELD_ATTR_FAM_OR_ASSEM:
            {
                access |= static_cast<uint8_t>(Access::Private);
                access |= static_cast<uint8_t>(Access::Protected);
                break;
            }
            case MONO_FIELD_ATTR_PUBLIC:
            {
                access = static_cast<uint8_t>(Access::Public);
                break;
            }
        }

        return access;
    }

    uint8_t MonoAPI::GetPropertyAccess(MonoProperty* prop) // get property access
    {
        uint8_t access = static_cast<uint8_t>(Access::None); // initialize to none

        MonoMethod* propGetter = mono_property_get_get_method(prop); // ref to getter

        if (propGetter != nullptr) // getter exists
        {
            uint32_t accessFlag = mono_method_get_flags(propGetter, nullptr) & MONO_METHOD_ATTR_ACCESS_MASK; // set access flag

            switch (accessFlag) // sets flag accordingly
            {
                case MONO_FIELD_ATTR_PRIVATE:
                {
                    access = static_cast<uint8_t>(Access::Private);
                    break;
                }
                case MONO_FIELD_ATTR_FAM_AND_ASSEM:
                {
                    access |= static_cast<uint8_t>(Access::Protected);
                    access |= static_cast<uint8_t>(Access::Internal);
                    break;
                }
                case MONO_FIELD_ATTR_ASSEMBLY:
                {
                    access = static_cast<uint8_t>(Access::Internal);
                    break;
                }
                case MONO_FIELD_ATTR_FAMILY:
                {
                    access = static_cast<uint8_t>(Access::Protected);
                    break;
                }
                case MONO_FIELD_ATTR_FAM_OR_ASSEM:
                {
                    access |= static_cast<uint8_t>(Access::Private);
                    access |= static_cast<uint8_t>(Access::Protected);
                    break;
                }
                case MONO_FIELD_ATTR_PUBLIC:
                {
                    access = static_cast<uint8_t>(Access::Public);
                    break;
                }
            }
        }

        MonoMethod* propSetter = mono_property_get_set_method(prop); // ref to setter

        if (propSetter != nullptr) // setter exists
        {
            uint32_t accessFlag = mono_method_get_flags(propSetter, nullptr) & MONO_METHOD_ATTR_ACCESS_MASK; // set access flag

            if (accessFlag != MONO_FIELD_ATTR_PUBLIC) // public
                access = static_cast<uint8_t>(Access::Private);
        }
        else
        {
            access = static_cast<uint8_t>(Access::Private);
        }

        return access;
    }

    // +++++++++++++++++++++++++++++++++++++ //

    // set up

    MonoAPI::MonoAPI(Popplio::Registry& reg) : appDomainName(POPPLIO_APP_DOMAIN_NAME), domain(nullptr), appDomain(nullptr),
        assembly(nullptr), image(nullptr), gcHandles(), gcLifeHandles(), 
        assemblyPath(POPPLIO_ASSEMBLY_PATH), observeInterval(POPPLIO_ASSEMBLY_RELOAD_INTERVAL), hotReloadEnabled(true), 
        lastModified(std::filesystem::last_write_time(POPPLIO_ASSEMBLY_PATH)),
        loadedClasses(std::vector<MonoClass*>()), loadedEngineClasses(std::vector<MonoClass*>()), 
        loadedUserClasses(std::vector<MonoClass*>()), loadedClassNames(std::vector<std::pair<std::string, std::string>>()),
        loadedEngineClassNames(std::vector<std::pair<std::string, std::string>>()), 
        loadedUserClassNames(std::vector<std::pair<std::string, std::string>>()),
        registry(reg)
    {
        mono_set_assemblies_path(POPPLIO_ASSEMBLY_DIR); // set assembly path

        //mono_set_dirs(POPPLIO_ASSEMBLY_DIR, nullptr);

        //domain = std::unique_ptr<MonoDomain>(mono_jit_init("PopplioRuntime")); // get runtime ptr
        domain = mono_jit_init(POPPLIO_DOMAIN_NAME); // get runtime ptr

        mono_thread_set_main(mono_thread_current());

        StartMono(); // start mono runtime
    }

    void MonoAPI::StartMono()
    {
        if (domain == nullptr)
        {
            std::stringstream ss;
            ss << "PopplioScriptAPI::MonoAPI | Failed to initialize Mono domain";

            Popplio::Logger::Error(ss.str()); // print to logger

            return;
        }

        //auto currPath = std::filesystem::current_path();
        if (!Popplio::Engine::programPath.empty())
            std::filesystem::current_path(std::filesystem::path(Popplio::Engine::programPath));

        //#if defined(DEBUG) | defined(_DEBUG)
        //        mono_debug_domain_create(domain);
        //#endif

                //AddGC(reinterpret_cast<MonoObject*>(domain));

                //strcat_s(appDomainName, sizeof(appDomainName), "PopplioAppDomain");
                //appDomain = std::unique_ptr<MonoDomain>(mono_domain_create_appdomain(appDomainName, nullptr)); // get app domain ptr
                //mono_domain_set(appDomain.get(), true);

                //strcpy_s(appDomainName, sizeof(appDomainName), POPPLIO_APP_DOMAIN_NAME);
                //appDomain = mono_domain_create_appdomain(appDomainName, nullptr); // get app domain ptr
                //PrintMemoryStats();

        //appDomain = domain; // set app domain to root domain

        //PrintMemoryStats();

        appDomain = mono_domain_create_appdomain(appDomainName, nullptr);

        mono_domain_set(appDomain, true);

        //PrintMemoryStats();

        assembly = LoadCSAssembly(POPPLIO_ASSEMBLY_PATH); // load ass path here

        if (assembly == nullptr)
        {
            std::stringstream ss;
            ss << "PopplioScriptAPI::MonoAPI | Failed to initialize Mono assembly";

            Popplio::Logger::Error(ss.str()); // print to logger
            //std::filesystem::current_path(currPath);
            return;
        }

        image = mono_assembly_get_image(assembly);

        // init utilities
        MonoUtilInit(this);

        //mono_jit_exec(domain, assembly, 0, nullptr);

        // get loaded classes //
        MonoTableInfo const* typeTable = mono_image_get_table_info(&GetImage(), MONO_TABLE_TYPEDEF); // get table of types
        int rows = mono_table_info_get_rows(typeTable); // get num of rows

        std::string name{ "" };

        for (int i{ 1 }; i < rows; ++i) // skip <module> so start at 1
        {
            uint32_t cols[MONO_TYPEDEF_SIZE]; // get cols
            mono_metadata_decode_row(typeTable, i, cols, MONO_TYPEDEF_SIZE); // decode row
            char const* typeNamespace = mono_metadata_string_heap(&GetImage(), cols[MONO_TYPEDEF_NAMESPACE]); // get namespace
            char const* typeName = mono_metadata_string_heap(&GetImage(), cols[MONO_TYPEDEF_NAME]); // get name
            MonoClass* classObj{ GetAssClass(typeNamespace, typeName) }; // get class

            name = typeNamespace;

            name = name.substr(0, name.find_first_of('.'));

            if (name == POPPLIO_PROJECT_NAMESPACE)
            {
                loadedUserClasses.push_back(classObj);
                loadedUserClassNames.push_back({ typeNamespace, typeName });
            }
            else if (name == POPPLIO_CORE)
            {
                loadedEngineClasses.push_back(classObj);
                loadedEngineClassNames.push_back({ typeNamespace, typeName });
            }

            loadedClasses.push_back(classObj);
            loadedClassNames.push_back({ typeNamespace, typeName });
        }
        // get loaded classes //

        // Load Native Library
        //MonoObject* obj{ InstLifeClass("Team_Popplio", "PopplioNativeInterop") };
        //CallMethod(obj, "Init");

        // register internal calls
        //SetIntCall(registry);
        RegisterInternalCalls();

        // debug info //
        Popplio::Logger::Info("PopplioScriptAPI::MonoAPI | Mono runtime version:");
        Popplio::Logger::Info(mono_get_runtime_build_info());

        if (Popplio::Engine::Config::verbose) 
            PrintAssemblyTypes(); // set to false to show all loaded assemblies

        PrintMemoryStats("Initialization");
        // debug info //

        // hot reload //
        //StartObserving(this);
        //InstLifeClass("Team_Popplio", "DotNetHotReload"); // TODO FIX

        // test //
        //PopplioTest::TestMonoAPI(*this); 
        // test //

        MonoObject* csBridge{ InstClass("Team_Popplio", "PopplioCS") };
        std::vector<void*> params{};
        std::string str = std::filesystem::current_path().string() + POPPLIO_ASSEMBLY_PATH;
        CallMethod(csBridge, "Init", params, str.c_str());

        //std::filesystem::current_path(currPath);
    }

    void MonoAPI::UnloadRuntime()
    {
        if (!domain) return;

        loadedUserClasses.clear();
        loadedEngineClasses.clear();
        loadedClasses.clear();

        loadedUserClassNames.clear();
        loadedEngineClassNames.clear();
        loadedClassNames.clear();

        // 1. Free all GC handles first
        ClearAllGC();
        //for (auto& [obj, handle] : gcHandles) mono_gchandle_free(handle);
        //gcHandles.clear();
        //for (auto& [obj, handle] : gcLifeHandles) mono_gchandle_free(handle);
        //gcLifeHandles.clear();

        // 2. Clean up the assembly and image
        if (assembly) 
        {
            //MonoImage* image = mono_assembly_get_image(assembly);

            if (image) mono_image_close(image);

            assembly = nullptr;
        }

        // 3. Force aggressive GC
        ForceGC();

        // 4. Detach all threads
        mono_thread_cleanup();
        MonoThread* currentThread = mono_thread_current();
        if (currentThread) mono_thread_detach(currentThread);

        // 5. Wait for pending finalizers
        while (mono_gc_pending_finalizers()) 
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            mono_gc_finalize_notify();
        }

        // 6. One final GC pass
        ForceGC();

        // 7. Cleanup the runtime
        mono_jit_cleanup(domain);
        domain = nullptr;
        appDomain = nullptr;

        // 8. Additional cleanup steps
        //mono_gc_cleanup();
        mono_thread_cleanup();
        //mono_runtime_cleanup(domain);
    }

    void MonoAPI::ShutdownMono()
    {
        try 
        {
            UnloadRuntime();
        }
        catch (const std::exception& e) 
        {
            std::stringstream ss;
            ss << "MonoAPI::ShutdownMono | Exception during shutdown: " << e.what();
            Popplio::Logger::Error(ss.str());
        }
    }

    MonoAPI::~MonoAPI() // exit mono runtime
    {
        PrintMemoryStats("Before Shutdown");
        ShutdownMono();
        PrintMemoryStats("After Shutdown");

        //mono_gc_collect(mono_gc_max_generation());

        //mono_gc_finalize_notify();

        //MonoThread* currentThread = mono_thread_current();
        //if (currentThread) mono_thread_detach(currentThread);

        //if (appDomain)
        //{
        //    //mono_domain_set(nullptr, false);
        //    mono_jit_cleanup(appDomain);
        //    appDomain = nullptr;
        //}
        //if (domain)
        //{
        //    //mono_domain_set(nullptr, false);
        //    mono_jit_cleanup(domain);
        //    domain = nullptr;
        //}

        

        //Popplio::Logger::Info("PopplioScriptAPI::MonoAPI | Shutting down mono...");
        //Popplio::Logger::Info("PopplioScriptAPI::MonoAPI | Detaching threads...");

        // detach threads
        //mono_thread_detach(mono_thread_current());

        // release managed obj refs
        //mono_gchandle_free(gc_handle);

        //Popplio::Logger::Info("PopplioScriptAPI::MonoAPI | Unloading domain...");

        // unload app domain
        //mono_domain_unload(appDomain);

        // unload domain
        //mono_domain_unload(domain);

        //Popplio::Logger::Info("PopplioScriptAPI::MonoAPI | Shutting down runtime...");

        // shutdown runtime
        //mono_jit_cleanup(appDomain);
        //mono_jit_cleanup(domain);
    }

    char* MonoAPI::GetFileAsBytes(std::string const& path, uint32_t* outSize)
    {
        std::ifstream file(path, std::ios::binary | std::ios::ate); // open file

        if (!file) return nullptr; // fail // cant open

        std::streampos end = file.tellg();  // set to end
        file.seekg(0, std::ios::beg);       // set to beginning
        uint32_t size = static_cast<uint32_t>(end - file.tellg()); // get size

        if (size == 0) return nullptr; // fail // file empty

        char* buffer = new char[size]; // create buffer ( remember to free! )

        file.read((char*)buffer, size);
        file.close();

        *outSize = size;
        return buffer;
    }

    MonoAssembly* MonoAPI::LoadCSAssembly(std::string const& path)
    {
        uint32_t fSize = 0;
        char* fData = GetFileAsBytes(path, &fSize); // read bytes into buffer

        MonoImageOpenStatus status;
        MonoImage* loadImage = mono_image_open_from_data_full(fData, fSize, 1, &status, 0); // load ass
        // return nullptr if failed, otherwise returns valid pointer to an image    

        if (status != MONO_IMAGE_OK)
        {
            std::stringstream ss;
            ss << "PopplioScriptAPI::MonoAPI | Mono Image Error: " <<  mono_image_strerror(status);

            Popplio::Logger::Error(ss.str()); // print to logger

            return nullptr;
        }

        MonoAssembly* loadAssembly = mono_assembly_load_from_full(loadImage, path.c_str(), &status, 0);

        delete[] fData; // free allocated file data on heap

        return loadAssembly;
    }

    // +++++++++++++++++++++++++++++++++++++ //

    // interface

    MonoClass* MonoAPI::GetAssClass(char const* namespaceName, char const* className) // get class from assembly
    {
        //MonoImage* image = mono_assembly_get_image(assembly);
        MonoClass* classObj{ mono_class_from_name(image, namespaceName, className) };

        if (classObj == nullptr)
        {
            std::stringstream ss{ "" };

            ss << "PopplioScriptAPI::MonoAPI | Failed to get class: " << namespaceName << "::" << className;

            Popplio::Logger::Debug(ss.str());
            return nullptr;
        }

        return classObj;
    }

    MonoObject* MonoAPI::InstClass(char const* namespaceName, char const* className) // create class instance
    {
        MonoClass* classObj{ GetAssClass(namespaceName, className) }; // ref to class
        if (classObj == nullptr) // failed
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Failed to get class" + 
                static_cast<std::string>(namespaceName) + "::" + static_cast<std::string>(className));
            return nullptr;
        }
        MonoObject* classInst{ mono_object_new(appDomain, classObj) }; // alloc inst of class

        if (classInst == nullptr) // failed
        {
            std::stringstream ss{ "" };

            ss << "PopplioScriptAPI::MonoAPI | Failed to create class instance: " << namespaceName << "::" << className;

            Popplio::Logger::Error(ss.str());
            return nullptr;
        }

        GCObj(classInst); // add to gc

        mono_runtime_object_init(classInst); // call class constructor

        return classInst;
    }

    MonoObject* MonoAPI::InstLifeClass(char const* namespaceName, char const* className)
    {
        MonoClass* classObj{ GetAssClass(namespaceName, className) }; // ref to class
        MonoObject* classInst{ GCObj(mono_object_new(appDomain, classObj), true) }; // alloc inst of class

        if (classInst == nullptr) // failed
        {
            std::stringstream ss{ "" };

            ss << "PopplioScriptAPI::MonoAPI | Failed to create lifetime class instance: " << namespaceName << "::" << className;

            Popplio::Logger::Error(ss.str());
            return nullptr;
        }

        mono_runtime_object_init(classInst); // call class constructor

        return classInst;
    }

    // +++++++++++++++++++++++++++++++++++++ //

    // Get Set

    MonoObject* MonoAPI::Get(MonoObject* obj, MonoClassField* field)
    {
        if (field == nullptr) throw std::runtime_error("Field is null");

        void* val{ nullptr };

        mono_field_get_value(obj, field, &val);
        return static_cast<MonoObject*>(val);
    }

    std::string MonoAPI::GetStr(MonoObject* obj, MonoProperty* prop)
    {
        if (prop == nullptr) return ""; // empty string

        MonoString* str = reinterpret_cast<MonoString*>(mono_property_get_value(prop, obj, nullptr, nullptr));
        return ToStdString(str);
    }

    std::string MonoAPI::SetStr(MonoObject* obj, MonoProperty* prop, std::string const& val)
    {
        if (prop == nullptr) return ""; // empty string

        MonoString* str{ CreateMonoString(appDomain, const_cast<std::string*>(&val)->c_str()) };

        mono_property_set_value(prop, obj, reinterpret_cast<void**>(&str), nullptr);

        return GetStr(obj, prop);
    }

    void* MonoAPI::GetThunk(MonoClass* classObj, char const* methodName, char const* signature = "") // TODO FIX
    {
        static_cast<void>(signature);

        MonoMethod* method{ mono_class_get_method_from_name(classObj, methodName, 0) };

        if (!method) throw std::runtime_error("Failed to find method");

        return mono_method_get_unmanaged_thunk(method);
    }

    void MonoAPI::SetProp(MonoObject* obj, MonoProperty* prop)
    {
        if (prop == nullptr) throw std::runtime_error("Property is null");

        void** data = nullptr;
        mono_property_set_value(prop, obj, data, nullptr);

        //return GetProp<T>(obj, prop);
    }

    MonoArray* MonoAPI::GetArray(MonoObject* obj)
    {
        if (obj == nullptr) throw std::runtime_error("Object is null");

        // Get the MonoClass of the object
        MonoClass* objClass = mono_object_get_class(obj);

        //GetMonoArray(objClass);

        // Check if the object is an array
        if (objClass == nullptr || !(mono_class_is_assignable_from(mono_get_array_class(), objClass)))
            throw std::runtime_error("Object is not an array");

        // Cast the MonoObject to MonoArray
        return (MonoArray*)obj;
    }

    // +++++++++++++++++++++++++++++++++++++ //

    // Invoke

    void MonoAPI::CallThunk(MonoClass* classObj, char const* methodName, char const* signature) // TODO FIX
    {
        try
        {
            typedef int(*ThunkType)();
            ThunkType thunk = (ThunkType)GetThunk(classObj, methodName, signature);

            thunk();

            return;
        }
        catch (std::exception const& e)
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Failed to call thunk");
            std::stringstream ss;
            ss << "PopplioScriptAPI::MonoAPI | " << e.what();
            Popplio::Logger::Error(ss.str());
        }
    }

    void MonoAPI::CallMethod(MonoObject* obj, char const* methodName) // invoke c# method
    {
        if (!IsObjValid(obj)) // check if obj is valid
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | method obj is invalid");
            return; 
        }

        // get MonoClass pointer from inst
        MonoClass* classObj{ mono_object_get_class(obj) };

        //std::string className = mono_class_get_name(classObj);

        MonoMethod* method = nullptr;
        std::string str = "";
        try
        {
            str = mono_class_get_name(classObj);
            method = mono_class_get_method_from_name(classObj, methodName, 0); // get method
        }
        catch (std::exception const& e)
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Failed to get method");
            std::stringstream ss;
            ss << "PopplioScriptAPI::MonoAPI | " << e.what();
            Popplio::Logger::Error(ss.str());
        }
        
        if (method == nullptr) // failed
        {
            std::stringstream ss{ "" };

            ss << "PopplioScriptAPI::MonoAPI | Failed to get method: " << 
                mono_class_get_namespace(classObj) << "::" << mono_class_get_name(classObj) << "." << methodName;

            Popplio::Logger::Error(ss.str());
            return;
        }

        // Check if method is virtual
        //uint32_t flags = mono_method_get_flags(method, NULL);
        //bool isVirtual = (flags & MONO_METHOD_ATTR_VIRTUAL) != 0;

        //if (isVirtual) 
        //{
        //    Popplio::Logger::Debug("Calling virtual method: " + std::string(methodName));
        //}

        // call C# method
        MonoObject* except{ nullptr }; // exception

        std::vector<void*> params{};

        try
        {
            mono_runtime_invoke(method, obj, params.data(), &except); // maybe error

            if (except != nullptr) // failed
            {
                std::stringstream ss{ "" };

                ss << "PopplioScriptAPI::MonoAPI | Method returned exception: " <<
                    mono_class_get_namespace(classObj) << "::" << mono_class_get_name(classObj) << "." << methodName <<
                    " | " << GetExceptionMessage(except);

                Popplio::Logger::Error(ss.str());
                // handle exception TODO

                return;
            }
        }
        catch (std::exception const& e) // catch exception
        {
            std::stringstream ss{ "" };
            ss << "PopplioScriptAPI::MonoAPI | Method failed: " <<
                mono_class_get_namespace(classObj) << "::" << mono_class_get_name(classObj) << "." << methodName <<
                " | " << e.what();
            Popplio::Logger::Error(ss.str());
            return;
        }

        //if (except != nullptr) // failed
        //{
        //    std::stringstream ss{ "" };

        //    ss << "PopplioScriptAPI::MonoAPI | Method returned exception: " <<
        //        mono_class_get_namespace(classObj) << "::" << mono_class_get_name(classObj) << "." << methodName << 
        //        " | " << GetExceptionMessage(except);

        //    Popplio::Logger::Error(ss.str());
        //    // handle exception TODO

        //    return;
        //}

        return;
    }

    void MonoAPI::CallMethod(MonoClass* obj, char const* methodName)
    {
        //if (!IsObjValid(obj)) // check if obj is valid
        //{
        //    Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | method obj is invalid");
        //    return;
        //}

        MonoObject* result{ nullptr };

        MonoMethod* method = mono_class_get_method_from_name(obj, methodName, 0); // get method

        if (method == nullptr) // failed
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Failed to get method");
            return;
        }

        // call C# method
        MonoObject* except{ nullptr }; // exception

        result = mono_runtime_invoke(method, nullptr, nullptr, &except);

        if (except != nullptr) // failed
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Method returned exception");

            // handle exception TODO

            return;
        }
    }

    // ++++++++++ //

    // Wrap Class Funcs

    //static void PopplioLoggerCritical(
    //    const std::string& message, Popplio::LogDestination dest = Popplio::LogDestination::ALL)
    //{
    //    Popplio::Logger::Critical(message, dest);
    //}

    //static void PopplioLoggerDebug(
    //    const std::string& message, Popplio::LogDestination dest = Popplio::LogDestination::ALL)
    //{
    //    Popplio::Logger::Debug(message, dest);
    //}

    //static void PopplioLoggerError(
    //    const std::string& message, Popplio::LogDestination dest = Popplio::LogDestination::ALL)
    //{
    //    Popplio::Logger::Error(message, dest);
    //}

    //static void PopplioLoggerInfo(
    //    const std::string& message, Popplio::LogDestination dest = Popplio::LogDestination::ALL)
    //{
    //    Popplio::Logger::Info(message, dest);
    //}

    // Register Internal Calls

    //void MonoAPI::RegisterInternalCalls()
    //{
    //    mono_add_internal_call("Team_Popplio.Log::Critical", PopplioLog::Critical);
    //    mono_add_internal_call("Team_Popplio.Log::Debug", PopplioLog::Debug);
    //    mono_add_internal_call("Team_Popplio.Log::Error", PopplioLog::Error);
    //    mono_add_internal_call("Team_Popplio.Log::Info", PopplioLog::Info);
    //}

    // ++++++++++ //

    // Debug

    bool MonoAPI::CheckMonoError(MonoError& err)
    {
        bool hasErr = !mono_error_ok(&err); // check if error

        if (hasErr)
        {
            unsigned short errCode = mono_error_get_error_code(&err); // get error code
            const char* errMsg = mono_error_get_message(&err); // get error message

            std::stringstream ss;
            ss << "PopplioScriptAPI::MonoAPI | Mono Error: " << std::to_string(errCode) << " | " << errMsg; // err msg

            Popplio::Logger::Error(ss.str()); // print to logger

            mono_error_cleanup(&err); // clean up error
        }

        return hasErr;
    }

    void MonoAPI::PrintMemoryStats(std::string const& label)
    {
        Popplio::Logger::Info("[" + label + "]");
        Popplio::Logger::Info("Mono Memory Stats:");

        std::stringstream ss;
        ss << "GC Handles: " << gcHandles.size() << " | " << gcLifeHandles.size();
        Popplio::Logger::Info(ss.str());
        ss.str(std::string());
        ss << "Heap Size: " << mono_gc_get_heap_size();
        Popplio::Logger::Info(ss.str());
        ss.str(std::string());
        ss << "Used Size: " << mono_gc_get_used_size();
        Popplio::Logger::Info(ss.str());

        if (!Popplio::Engine::Config::verbose) return;

        Popplio::Logger::Info("= Heap Objects =");
        int flag = 0;
        int count = 0;
        mono_gc_walk_heap(flag, ObjectCallback, &count);
    }

    void MonoAPI::PrintAssemblyTypes(bool const& onlyUserTypes, bool const& onlyEngineTypes) // test
    {
        // get table of types per row, with each col containing info
        MonoTableInfo const* typeDefTable = mono_image_get_table_info(&GetImage(), MONO_TABLE_TYPEDEF);
        int32_t numTypes = mono_table_info_get_rows(typeDefTable);

        std::stringstream ss;

        std::string n;

        for (int32_t i{}; i < numTypes; ++i)
        {
            uint32_t cols[MONO_TYPEDEF_SIZE];

            mono_metadata_decode_row(typeDefTable, i, cols, MONO_TYPEDEF_SIZE);

            char const* nameSpace = mono_metadata_string_heap(&GetImage(), cols[MONO_TYPEDEF_NAMESPACE]);
            char const* name = mono_metadata_string_heap(&GetImage(), cols[MONO_TYPEDEF_NAME]);

            n = nameSpace;

            n = n.substr(0, n.find_first_of('.'));

            if ((onlyUserTypes && n != POPPLIO_PROJECT_NAMESPACE) && (onlyEngineTypes && n != POPPLIO_CORE))
                continue;

            ss.str("");

            ss << "PopplioScriptAPI::MonoAPI | Type loaded: " << nameSpace << "." << name; // ss

            Popplio::Logger::Info(ss.str()); // print to logger
        }
    }

    std::string MonoAPI::GetExceptionMessage(MonoObject* exceptionObject)
    {
        if (exceptionObject == nullptr) return "No exception object";

        MonoClass* exceptionClass = mono_object_get_class(exceptionObject);
        MonoMethod* toStringMethod = mono_class_get_method_from_name(exceptionClass, "ToString", 0);

        if (toStringMethod == nullptr) return "Unable to find ToString method";

        MonoString* monoString = nullptr;
        MonoObject* exceptionResult = nullptr;

        // Invoke the ToString() method on the exception object
        exceptionResult = mono_runtime_invoke(toStringMethod, exceptionObject, nullptr, nullptr);

        if (exceptionResult != nullptr) 
        {
            monoString = (MonoString*)exceptionResult;

            // Convert MonoString to C++ std::string
            char* utf8_message = mono_string_to_utf8(monoString);
            std::string exceptionMessage(utf8_message);

            // Free the allocated memory
            //mono_free(utf8_message);
            MonoAPI::MonoFree(utf8_message);

            return exceptionMessage;
        }

        return "Failed to retrieve exception message";
    }

    std::string MonoAPI::GetDetailedExceptionMessage(MonoObject* exceptionObject)
    {
        if (exceptionObject == nullptr) return "No exception object";

        MonoClass* exceptionClass = mono_object_get_class(exceptionObject);

        // Get Message property
        MonoClassField* messageField = mono_class_get_field_from_name(exceptionClass, "_message");

        if (messageField) 
        {
            MonoString* monoString = nullptr;
            mono_field_get_value(exceptionObject, messageField, &monoString);

            if (monoString) 
            {
                char* utf8_message = mono_string_to_utf8(monoString);
                std::string exceptionMessage(utf8_message);
                //mono_free(utf8_message);
                MonoAPI::MonoFree(utf8_message);
                return exceptionMessage;
            }
        }

        return "Unable to extract exception message";
    }

    mono_bool MonoAPI::ObjectCallback(MonoObject* obj, MonoClass* c, uintptr_t size, 
        uintptr_t numRefs, MonoObject** refs, unsigned long long* offsets, void* userData)
    {
        if (!obj) return 0;

        int* count = static_cast<int*>(userData);

        std::string className = mono_class_get_name(c);
        std::string namespaceName = mono_class_get_namespace(c);

        Popplio::Logger::Info(std::to_string(*count) + " C# Object [" /*+ *reinterpret_cast<int*>(obj)*/ + std::string("]: ") +
            namespaceName + "::" + className + " | size: " + std::to_string(static_cast<unsigned long>(size)) + 
            std::to_string(static_cast<unsigned long>(numRefs)));

        for (uintptr_t i = 0; i < numRefs; i++) 
        {
            if (refs[i] != nullptr) //NULL) 
            {
                MonoClass* ref_class = mono_object_get_class(refs[i]);
                Popplio::Logger::Info("    Ref " + std::to_string(static_cast<unsigned long>(i)) + std::string(": ") +
                    mono_class_get_name(ref_class) + 
                    //std::string(" at ") + std::to_string(*reinterpret_cast<int*>(static_cast<void*>(refs[i]))) +
                    std::string(", off: ") + std::to_string(offsets[i]));
            }
        }

        ++(*count);

        return 1;
    }

    // ++++++++++ //

    // Garbage Collection

    MonoObject* MonoAPI::AddGC(MonoObject* obj, bool const& lifeGC)
    {
        //uint32_t handle = mono_gchandle_new(obj, true);
        uint32_t handle = mono_gchandle_new(obj, false);
        //uint32_t handle = mono_gchandle_new_weakref(obj, false);

        if (handle == 0)
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | GCObj | Failed to add object to GC");
            return nullptr;
        }

        if (lifeGC)
            gcLifeHandles.push_back(handle); //mono_gchandle_new(obj, true); // add to gc handle list
        else
            gcHandles.push_back(handle); //mono_gchandle_new(obj, true); // add to gc handle list

        //if (lifeGC)
        //    gcLifeHandles[obj] = handle; //mono_gchandle_new(obj, true); // add to gc handle list
        //else    
        //    gcHandles[obj] = handle; //mono_gchandle_new(obj, true); // add to gc handle list
        // ^ set pin to true as false will crash after an object gets collected

        return obj;
    }
    void MonoAPI::RemoveGC(uint32_t handle, bool const& lifeGC)
    {
        if (lifeGC)
        {
            auto it = std::find(gcLifeHandles.begin(), gcLifeHandles.end(), handle);
            if (it == gcLifeHandles.end()) return;
            mono_gchandle_free(handle);
            gcLifeHandles.erase(it);
        }
        else
        {
            auto it = std::find(gcHandles.begin(), gcHandles.end(), handle);
            if (it == gcHandles.end()) return;
            mono_gchandle_free(handle);
            gcHandles.erase(it);
        }
    }
    void MonoAPI::RemoveGC(MonoObject* obj, bool const& lifeGC)
    {
        if (lifeGC)
        {
            for (uint32_t handle : gcLifeHandles)
            {
                if (mono_gchandle_get_target(handle) == obj)
                {
                    mono_gchandle_free(handle);
                    gcLifeHandles.erase(std::find(gcLifeHandles.begin(), gcLifeHandles.end(), handle));
                }
            }
            //if (gcLifeHandles.find(obj) == gcLifeHandles.end()) return;
            //mono_gchandle_free(gcLifeHandles[obj]);
            //gcLifeHandles.erase(obj);
        }
        else
        {
            for (uint32_t handle : gcHandles)
            {
                if (mono_gchandle_get_target(handle) == obj)
                {
                    mono_gchandle_free(handle);
                    gcHandles.erase(std::find(gcHandles.begin(), gcHandles.end(), handle));
                }
            }
            //if (gcHandles.find(obj) == gcHandles.end()) return;
            //mono_gchandle_free(gcHandles[obj]);
            //gcHandles.erase(obj);
        }
    }
    bool MonoAPI::IsObjValid(MonoObject* obj, bool const& lifeGC)
    {
        if (obj == nullptr) return false;
        //else return true;

        if (lifeGC)
        {
            for (uint32_t handle : gcLifeHandles)
            {
                if (mono_gchandle_get_target(handle) == obj) return true;
            }
            //GCObj(obj, true);
        }
        else
        {
            for (uint32_t handle : gcHandles)
            {
                if (mono_gchandle_get_target(handle) == obj) return true;
            }
            //GCObj(obj, false);
        }

        //if (lifeGC && gcLifeHandles.find(obj) != gcLifeHandles.end())
        //    return true;
        //if (!lifeGC && gcHandles.find(obj) != gcHandles.end())
        //    return true;
        
        return false;
    }
    bool MonoAPI::IsObjValid(uint32_t handle)
    {
        return mono_gchandle_get_target(handle) != nullptr;
        //if (lifeGC) return mono_gchandle_get_target(gcLifeHandles[obj]) != nullptr;
        //else return mono_gchandle_get_target(gcHandles[obj]) != nullptr;
    }
    bool MonoAPI::IsObjValid(MonoGCHandle const& handle, bool const& lifeGC)
    {
        if (lifeGC) return mono_gchandle_get_target_v2(handle) != nullptr;
        else return mono_gchandle_get_target_v2(handle) != nullptr;
    }
    void MonoAPI::ClearInvalidGC()
    {
        // Capture `this` explicitly to access class methods
        std::erase_if(gcHandles, [this](auto& handle)
        {
            if (!this->IsObjValid(mono_gchandle_get_target(handle)))
            { // Explicitly use `this->`
                mono_gchandle_free(handle);
                return true; // Mark for removal
            }
            return false;
        });

        std::erase_if(gcLifeHandles, [this](auto& handle)
        {
            if (!this->IsObjValid(mono_gchandle_get_target(handle), true))
            {
                mono_gchandle_free(handle);
                return true; // Mark for removal
            }
            return false;
        });
        //std::erase_if(gcHandles, [this](auto& pair) 
        //{
        //    if (!this->IsObjValid(pair.first)) 
        //    { // Explicitly use `this->`
        //        mono_gchandle_free(pair.second);
        //        return true; // Mark for removal
        //    }
        //    return false;
        //});

        //std::erase_if(gcLifeHandles, [this](auto& pair) 
        //{
        //    if (!this->IsObjValid(pair.first, true)) 
        //    {
        //        mono_gchandle_free(pair.second);
        //        return true; // Mark for removal
        //    }
        //    return false;
        //});
    }
    void MonoAPI::ClearAllGC()
    {
        for (uint32_t handle : gcHandles) mono_gchandle_free(handle);
        gcHandles.clear();
        for (uint32_t handle : gcLifeHandles) mono_gchandle_free(handle);
        gcLifeHandles.clear();
        //for (auto& [obj, handle] : gcHandles) mono_gchandle_free(handle);
        //gcHandles.clear();
        //for (auto& [obj, handle] : gcLifeHandles) mono_gchandle_free(handle);
        //gcLifeHandles.clear();
    }


    //MonoObject* MonoAPI::GCObj(bool const& lifeGC)
    //{
    //    MonoObject* tmp{ nullptr };

    //    AddGC(tmp, lifeGC);

    //    return tmp;
    //}

    MonoObject* MonoAPI::GCObj(MonoObject* obj, bool const& lifeGC)
    {
        if (obj == nullptr)
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | GCObj | Object is null");
            return nullptr;
        }

        // Check if already tracked
        if (lifeGC)
        {
            for (uint32_t handle : gcLifeHandles)
            {
                if (mono_gchandle_get_target(handle) == obj)
                    return obj;
            }
        }
        else
        {
            for (uint32_t handle : gcHandles)
            {
                if (mono_gchandle_get_target(handle) == obj)
                    return obj;
            }
        }
        //if (lifeGC && gcLifeHandles.find(obj) != gcLifeHandles.end())
        //    return obj;
        //if (!lifeGC && gcHandles.find(obj) != gcHandles.end())
        //    return obj;

        return AddGC(obj, lifeGC);

        //return obj;
    }

    MonoString* MonoAPI::GCObjStr(const char* str)//, bool const& lifeGC)
    {
        MonoString* tmp { CreateMonoString(appDomain, str) };

        //AddGC(reinterpret_cast<MonoObject*>(tmp), lifeGC);

        return tmp;
    }

    MonoString* MonoAPI::GCObjStr(MonoString* str, bool const& lifeGC)
    {
        AddGC(reinterpret_cast<MonoObject*>(str), lifeGC);

        return str;
    }

    MonoClass* MonoAPI::GCObjClass(MonoClass* obj, bool const& lifeGC)
    {
        AddGC(reinterpret_cast<MonoObject*>(obj), lifeGC);

        return obj;
    }

    MonoMethod* MonoAPI::GCObjMethod(MonoMethod* obj, bool const& lifeGC)
    {
        AddGC(reinterpret_cast<MonoObject*>(obj), lifeGC);

        return obj;
    }

    MonoClassField* MonoAPI::GCObjField(MonoClass* classObj, const char* fieldName, bool const& lifeGC)
    {
        MonoClassField* tmp{ mono_class_get_field_from_name(classObj, fieldName) };

        AddGC(reinterpret_cast<MonoObject*>(tmp), lifeGC);

        return tmp;
    }

    MonoProperty* MonoAPI::GCObjProp(MonoClass* classObj, const char* propName, bool const& lifeGC)
    {
        MonoProperty* tmp{ mono_class_get_property_from_name(classObj, propName) };

        AddGC(reinterpret_cast<MonoObject*>(tmp), lifeGC);

        return tmp;
    }

    void MonoAPI::CleanupStaleHandles()
    {
        std::vector<uint32_t> toRemove{};
        //std::vector<MonoObject*> toRemove{};

        // Check regular handles
        //for (auto& [obj, handle] : gcHandles) 
        for (auto& handle : gcHandles)
        {
            if (!IsObjValid(handle)) toRemove.push_back(handle);
            //if (!IsObjValid(obj, false)) toRemove.push_back(obj);

            //if (!mono_gchandle_get_target(handle)) 
            //{
            //    mono_gchandle_free(handle);
            //    toRemove.push_back(obj);
            //}
        }

        // Remove invalid entries
        for (auto obj : toRemove)
            RemoveGC(obj, false);
            //gcHandles.erase(obj);

        toRemove.clear();

        // Check lifetime handles
        //for (auto& [obj, handle] : gcLifeHandles)
        for (auto& handle : gcLifeHandles)
        {
            if (!IsObjValid(handle)) toRemove.push_back(handle);
            //if (!IsObjValid(obj, true)) toRemove.push_back(obj);
        }

        for (auto obj : toRemove)
            RemoveGC(obj, true);
    }

    void MonoAPI::CheckMemoryStatus()
    {
        if (gcHandles.size() + gcLifeHandles.size() > POPPLIO_GC_HANDLES)
        {
            Popplio::Logger::Critical("Exceeded Mono object handle limit, attempting clear");
            //ForceGC(); // might die
            CleanupStaleHandles();

            // definitely die
            if (gcHandles.size() + gcLifeHandles.size() > POPPLIO_GC_HANDLES)
            {
                Popplio::Logger::Critical("Exceeded maximum Mono object handles");
                //throw std::runtime_error("Mono object limit exceeded");
            }
        }
        else if (gcHandles.size() + gcLifeHandles.size() > POPPLIO_GC_HANDLES_WARNING)
        {
            Popplio::Logger::Critical("Approaching Mono object handle limit");
            Popplio::Logger::Warning("PopplioScriptAPI::MonoAPI | GC Handles: " + 
                std::to_string(gcHandles.size() + gcLifeHandles.size()));
        }
    }

    void MonoAPI::MonoFree(void* obj)
    {
        //g_free(obj);
        if (obj != nullptr) mono_free(obj); //free(obj);
    }

    uint32_t MonoAPI::GetGC(MonoObject* obj, bool const& lifeGC)
    {
        if (lifeGC)
        {
            for (uint32_t handle : gcLifeHandles)
            {
                if (mono_gchandle_get_target(handle) == obj)
                    return handle;
            }
        }
        else
        {
            for (uint32_t handle : gcHandles)
            {
                if (mono_gchandle_get_target(handle) == obj)
                    return handle;
            }
        }

        return 0;

        //return lifeGC ? gcLifeHandles[obj] : gcHandles[obj];
    }

    void MonoAPI::ForceGC(bool const& immediate)
    {
        // Clear all known references
        //clear_global_references(); // Make sure no static/global references exist

        if (!immediate)
        {
            mono_gc_collect(mono_gc_max_generation());
            mono_gc_finalize_notify();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        else
        {
            // Force multiple GC passes
            for (int i = 0; i < 5; ++i)
            {
                mono_gc_collect(mono_gc_max_generation());
                mono_gc_finalize_notify();
                while (mono_gc_pending_finalizers() > 0)
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }

        //mono_gc_references();
        //mono_gc_weak_references();
    }

    void MonoAPI::ForceGC(int gen)
    {
        // gen = -1 for all generations
        for (int i = 0; i < 5; ++i)
        {
            mono_gc_collect(gen);
            mono_gc_finalize_notify();
            while (mono_gc_pending_finalizers() > 0)
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    // ++++++++++ //

    // Threads

    void MonoAPI::DetachNonEssentialThreads()
    {
        
    }

    // ++++++++++ //

    // Conversion

    std::string MonoAPI::ToStdString(MonoString* str)
    {
        if (str == nullptr || mono_string_length(str) == 0) return ""; // empty string

        MonoError err{};

        char* cStr = mono_string_to_utf8_checked(str, &err); // convert to c string

        if (CheckMonoError(err)) return ""; // return empty if error

        std::string result{ cStr };

        //mono_free(cStr); // free c string
        MonoAPI::MonoFree(cStr);

        return result;
    }

    MonoString* MonoAPI::ToMonoString(std::string const& str)
    {
        return CreateMonoString(mono_domain_get(), str.c_str());
    }

    // ++++++++++ //

    // Helper

    std::vector<MonoClass*> MonoAPI::GetClassWithSameBase(const char* baseClassN, const char* namespaceN)
    {
        MonoTableInfo const* typeTable = mono_image_get_table_info(&GetImage(), MONO_TABLE_TYPEDEF); // get table of types
        int rows = mono_table_info_get_rows(typeTable); // get num of rows

        std::vector<MonoClass*> classList{}; // list of classes

        for (int i{ 1 }; i < rows; ++i) // skip <module> so start at 1
        {
            uint32_t cols[MONO_TYPEDEF_SIZE]; // get cols
            mono_metadata_decode_row(typeTable, i, cols, MONO_TYPEDEF_SIZE); // decode row
            char const* typeNamespace = mono_metadata_string_heap(&GetImage(), cols[MONO_TYPEDEF_NAMESPACE]); // get namespace
            char const* typeName = mono_metadata_string_heap(&GetImage(), cols[MONO_TYPEDEF_NAME]); // get name

            if (strcmp(typeNamespace, namespaceN) != 0) // check namespace
                continue;

            MonoClass* classObj{ GetAssClass(typeNamespace, typeName) }; // get class
            if (classObj == nullptr) continue; // skip if null

            MonoClass* parent{ mono_class_get_parent(classObj) }; // get parent
            if (parent == nullptr) continue; // skip if null

            bool inheritsBaseClass{ false };

            while (parent != nullptr) // check if inherits base class
            {
                if (strcmp(mono_class_get_name(parent), baseClassN) == 0) // check if base class
                {
                    inheritsBaseClass = true;
                    break;
                }
                parent = mono_class_get_parent(parent); // get next parent
            }

            if (!inheritsBaseClass) continue; // skip if not base class

            // add to list
            classList.push_back(classObj);
        }

        return classList;
    }

    bool MonoAPI::IsCppName(std::string const& name)
    {
        if (name == POPPLIO_CORE_NAMESPACE) return true;
        if (name == POPPLIO_API_NAMESPACE) return true;
        if (name == POPPLIO_MATH_NAMESPACE) return true;
        if (name == POPPLIO_UTIL_NAMESPACE) return true;

        return false;
    }

    std::pair<std::string, std::string> MonoAPI::SplitName(std::string const& name)
    {
        std::stringstream ss(name);

        std::string namespaceN{};
        std::getline(ss, namespaceN, ':');
        std::string classN{};
        std::getline(ss, classN, ':');
        std::getline(ss, classN, ':');

        return std::make_pair(namespaceN, classN);
    }

    bool MonoAPI::IsLoadedUserClass(std::string const& name)
    {
        for (auto const& n : loadedUserClassNames)
        {
            if (n.first + "." + n.second == name) return true;
        }
        return false;
    }

    bool MonoAPI::IsOverridden(MonoClass* derived, MonoClass* base, std::string const& method)
    {
        if (!base || !derived) 
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | IsOverridden | Base or derived class is null");
            return false;
        }

        if (method == "")
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | IsOverridden | Method name is empty");
            return false;
        }

        MonoMethod* baseMethod = mono_class_get_method_from_name(base, method.c_str(), -1);
        MonoMethod* derivedMethod = mono_class_get_method_from_name(derived, method.c_str(), -1);

        if (!baseMethod || !derivedMethod)
        {
            //Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | IsOverridden | Method not found");
            return false;
        }

        // Compare the method pointers
        return mono_method_get_class(derivedMethod) != base;
    }

    MonoString* MonoAPI::CreateMonoString(MonoDomain* app, char const* val)
    {
        MonoString* str = mono_string_new(app, val);

        AddGC(reinterpret_cast<MonoObject*>(str), false);

        return str;
    }

    // +++++++++++++++++++++++++++++++++++++++++++++ //

    // Function Template Specializations

    template <>
    char MonoAPI::Get<char>(MonoObject* obj, MonoClassField* field)
    {
        if (field == nullptr) throw std::runtime_error("Field is null");

        try
        {
            uint16_t val{};

            mono_field_get_value(obj, field, &val);

            return UInt16ToChar(val); //static_cast<char>(static_cast<wchar_t>(val));
        }
        catch (std::exception const& e)
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Failed to get field");
            std::stringstream ss;
            ss << "PopplioScriptAPI::MonoAPI | " << e.what();
            Popplio::Logger::Error(ss.str());
            return '\0';
        }
    }

    template<>
    std::string MonoAPI::Get<std::string>(MonoObject* obj, MonoClassField* field)
    {
        if (field == nullptr) throw std::runtime_error("Field is null");

        try
        {
            MonoString* str{ nullptr };
            mono_field_get_value(obj, field, &str);
            return ToStdString(str);
        }
        catch (std::exception const& e)
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Failed to get field");
            std::stringstream ss;
            ss << "PopplioScriptAPI::MonoAPI | " << e.what();
            Popplio::Logger::Error(ss.str());
            return std::string();
        }
    }

    template <>
    char MonoAPI::Set<char>(MonoObject* obj, MonoClassField* field, char const& val)
    {
        if (field == nullptr) throw std::runtime_error("Field is null");

        uint16_t c = CharToUInt16(val); //static_cast<uint16_t>(static_cast<wchar_t>(val));

        mono_field_set_value(obj, field, static_cast<void*>(&c));
        return val;
    }

    template <>
    std::string MonoAPI::Set<std::string>(MonoObject* obj, MonoClassField* field, std::string const& val)
    {
        if (field == nullptr) throw std::runtime_error("Field is null");

        mono_field_set_value(obj, field, static_cast<void*>(ToMonoString(const_cast<std::string&>(val))));
        return val;
    }

    template <>
    std::string MonoAPI::GetProp<std::string>(MonoObject* obj, MonoProperty* prop)
    {
        if (prop == nullptr) throw std::runtime_error("Property is null");

        MonoObject* valObj{ mono_property_get_value(prop, obj, nullptr, nullptr) };

        return MonoToString(valObj);
    }

    MonoObject* MonoAPI::CallMethodObj(MonoClass* obj, char const* methodName) // invoke c# method
    {
        MonoObject* result{ nullptr };

        MonoMethod* method = mono_class_get_method_from_name(obj, methodName, 0); // get method

        if (method == nullptr) // failed
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Failed to get method");
            return nullptr;
        }

        // call C# method
        MonoObject* except{ nullptr }; // exception

        result = mono_runtime_invoke(method, nullptr, nullptr, &except);

        if (except != nullptr) // failed
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Method returned exception");

            return nullptr;
        }

        return result;
    }

    //void Initialize()
    //{
    //    // Initialize Mono
    //    mono_jit_init("Popplio");
    //    // Load the assembly
    //    MonoAssembly* assembly = mono_domain_assembly_open(mono_domain_get(), "PopplioScript.dll");
    //    // Check if the assembly is loaded
    //    if (assembly == nullptr)
    //    {
    //        std::cout << "Failed to load assembly" << std::endl;
    //        return;
    //    }
    //    // Get the image of the assembly
    //    MonoImage* image = mono_assembly_get_image(assembly);
    //    // Get the class from the assembly
    //    MonoClass* classInstance = mono_class_from_name(image, "PopplioScript", "PopplioScript");
    //    // Check if the class is loaded
    //    if (classInstance == nullptr)
    //    {
    //        std::cout << "Failed to load class" << std::endl;
    //        return;
    //    }
    //    // Get the method from the class
    //    MonoMethod* method = mono_class_get_method_from_name(classInstance, "Initialize", 0);
    //    // Check if the method is loaded
    //    if (method == nullptr)
    //    {
    //        std::cout << "Failed to load method" << std::endl;
    //        return;
    //    }
    //    // Call the method
    //    mono_runtime_invoke(method, nullptr, nullptr, nullptr);
    //}
}
