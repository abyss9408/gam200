/******************************************************************************/
/*!
\file   MonoAPI.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the header file for C# Script API (using mono)

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

// includes

#pragma once

#include "../ECS/ECS.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
//#include <mono/metadata/attrdefs.h>
//#include <mono/metadata/reflection.h>
//#include <mono/eglib/glib.h>
//#include <glib-2.0/glib.h>

#include "../Utilities/Constants.h"
#include "MonoUtilities.h"

#include <cstdarg>
#include <type_traits>
#include <filesystem>

// +++++++++++++++++++++++++++++++++++++ //

// macros

#define POPPLIO_ASSEMBLY_DIR        "mono/lib"                  // mono assembly relative directory
#define POPPLIO_ASSEMBLY_PATH       "Team_Popplio_Script.dll"   // C# scripts DLL path

#define POPPLIO_DOMAIN_NAME         "PopplioRuntime"            // mono domain name
#define POPPLIO_APP_DOMAIN_NAME     "PopplioScriptRuntime"      // mono app domain name

#define POPPLIO_PROJECT_NAMESPACE   "BassNKick"                 // C# project base namespace
#define POPPLIO_PROJECT_BASECLASS   "PopplioMonoScript"         // C# project base script class

#define POPPLIO_CORE                "Team_Popplio"              // C++ core namespace

#define POPPLIO_CORE_NAMESPACE      "Popplio"                   // engine core namespace
#define POPPLIO_API_NAMESPACE       "PopplioScriptAPI"          // engine API namespace
#define POPPLIO_MATH_NAMESPACE      "PopplioMath"               // engine math namespace
#define POPPLIO_UTIL_NAMESPACE      "PopplioUtil"               // engine util namespace

#define POPPLIO_ASSEMBLY_RELOAD_INTERVAL 10                     // hot reload interval in seconds

// +++++++++++++++++++++++++++++++++++++ //

namespace PopplioScriptAPI
{
    class MonoAPI
    {
        // data //

        // assembly info
        MonoDomain* domain;
        MonoDomain* appDomain;

        char appDomainName[POPPLIO_MAX_PATH];

        MonoAssembly* assembly;
        MonoImage* image;

        // +++++++++ //

        // garbage collection

        std::vector<uint32_t> gcHandles;  // track gc handles
        std::vector<uint32_t> gcLifeHandles;  // track gc handles that exist for runtime's whole lifetime

        //std::unordered_map<MonoObject*, uint32_t> gcHandles;  // track gc handles
        //std::unordered_map<MonoObject*, uint32_t> gcLifeHandles;  // track gc handles that exist for runtime's whole lifetime

        //std::unique_ptr<MonoDomain> domain;
        //std::unique_ptr<MonoDomain> appDomain;

        // +++++++++ //

        // loaded classes

        std::vector<MonoClass*> loadedClasses;
        std::vector<MonoClass*> loadedEngineClasses;
        std::vector<MonoClass*> loadedUserClasses;

        std::vector<std::pair<std::string, std::string>> loadedClassNames;
        std::vector<std::pair<std::string, std::string>> loadedEngineClassNames;
        std::vector<std::pair<std::string, std::string>> loadedUserClassNames;

        // +++++++++ //

        // registry
        Popplio::Registry& registry;

        // +++++++++++++++++++++++++++++++++++++ //

        public:

        // assembly info

        std::string assemblyPath;
        unsigned observeInterval;

        // +++++++++ //

        // hot reload

        bool hotReloadEnabled;
        std::filesystem::file_time_type lastModified; // last modified time of assembly (C# project)

        // +++++++++ //

        // access modifiers

        enum class Access : uint8_t
        {
            None = 0,
            Private = (1 << 0),
            Internal = (1 << 1),
            Protected = (1 << 2),
            Public = (1 << 3)
        };

        // +++++++++++++++++++++++++++++++++++++ //

        // Functions

        // Field / Property Access

        /*
        *   get C# field static status
        */
        bool IsFieldStatic(MonoClassField* field);
        /*
        *   get C# field accessibility
        */
        uint8_t GetFieldAccess(MonoClassField* field);
        /*
        *   get C# property accessibility
        */
        uint8_t GetPropertyAccess(MonoProperty* prop); 

        // set up

        MonoAPI() = delete;

        /*
        *   MonoAPI arg constructor
        */
        MonoAPI(Popplio::Registry& reg);

        MonoAPI(MonoAPI const&) = delete;

        MonoAPI& operator=(MonoAPI const&) = delete;

        /*
        *   MonoAPI destructor
        */
        ~MonoAPI();

        /*
        *   start mono
        */
        void StartMono();
        /*
        *   Unload mono runtime
        */
        void UnloadRuntime();
        /*
        *   shut down mono
        */
        void ShutdownMono();

        // getters

        MonoDomain& GetDomain() { return *domain; }
        MonoDomain& GetAppDomain() { return *appDomain; }
        MonoAssembly& GetAssembly() { return *assembly; }
        MonoImage& GetImage() { return *image; }
        std::vector<MonoClass*> const& GetLoadedClasses() const { return loadedClasses; }
        std::vector<MonoClass*> const& GetLoadedUserClasses() const { return loadedUserClasses; }
        std::vector<MonoClass*> const& GetLoadedEngineClasses() const { return loadedEngineClasses; }
        std::vector<std::pair<std::string, std::string>> const& GetLoadedClassNames() const { return loadedClassNames; }
        std::vector<std::pair<std::string, std::string>> const& GetLoadedUserClassNames() const { return loadedUserClassNames; }
        std::vector<std::pair<std::string, std::string>> const& GetLoadedEngineClassNames() const { return loadedEngineClassNames; }

        //std::unordered_map<MonoObject*, uint32_t>& GetHandles() { return gcHandles; }
        std::vector<uint32_t>& GetHandles() { return gcHandles; }
        std::vector<uint32_t>& GetLifeHandles() { return gcLifeHandles; }

        // +++++++++++++++++++++++++++++++++++++ //

        // load assembly

        /*
        *   convert file to byte stream
        */
        char* GetFileAsBytes(std::string const& path, uint32_t* outSize);
        /*
        *   load c# assembly
        */
        MonoAssembly* LoadCSAssembly(std::string const& path);


        // +++++++++++++++++++++++++++++++++++++ //

        // interface

        /*
        *   get class using namespace and class name
        */
        MonoClass* GetAssClass(char const* namespaceName, char const* className);
        /*
        *   instantiate class object using namespace and class name
        */
        MonoObject* InstClass(char const* namespaceName, char const* className);
        /*
        *   instantiate class object for lifetime of runtime using namespace and class name
        */
        MonoObject* InstLifeClass(char const* namespaceName, char const* className);

        // ++++++++++ //

        // Get Field / Property

        /*
        *   get c# field
        */
        MonoClassField* GetField(MonoClass* classObj, char const* fieldName) const
        { return mono_class_get_field_from_name(classObj, fieldName); }
        /*
        *   get c# property
        */
        MonoProperty* GetProperty(MonoClass* classObj, char const* fieldName) const
        { return mono_class_get_property_from_name(classObj, fieldName); }

        // Get Set

        /*
        *   get value from c# field
        */
        template <typename T>
        T Get(MonoObject* obj, MonoClassField* field);
        template <>
        char Get<char>(MonoObject* obj, MonoClassField* field);
        template <>
        std::string Get<std::string>(MonoObject* obj, MonoClassField* field);
        //T Get(MonoObject* obj, MonoClassField* field) const;
        //template <>
        /*
        *   get (MonoObject*) value from c# field
        */
        MonoObject* Get(MonoObject* obj, MonoClassField* field);
        /*
        *   set c# field value
        */
        template <typename T>
        T Set(MonoObject* obj, MonoClassField* field, T const& val);
        template <>
        char Set<char>(MonoObject* obj, MonoClassField* field, char const& val);
        template <>
        std::string Set<std::string>(MonoObject* obj, MonoClassField* field, std::string const& val);

        /*
        *   get (string) value from c# property
        */
        std::string GetStr(MonoObject* obj, MonoProperty* prop);
        /*
        *   set c# property (string) value
        */
        std::string SetStr(MonoObject* obj, MonoProperty* prop, std::string const& val);

        /*
        *   get value c# property
        */
        template <typename T>
        T GetProp(MonoObject* obj, MonoProperty* prop);
        template <>
        std::string GetProp<std::string>(MonoObject* obj, MonoProperty* prop);
        //T GetProp(MonoObject* obj, MonoProperty* prop) const;
        /*
        *   set c# property value
        */
        template <typename T>
        void SetProp(MonoObject* obj, MonoProperty* prop, T const& val);

        /*
        *   set c# property value
        */
        void SetProp(MonoObject* obj, MonoProperty* prop);

        /*
        *   get c# field pointer value
        */
        template <typename T>
        T* GetPtr(MonoObject* obj, MonoClass* classObj, char const* field);

        /*
        *   get pointer to mono thunk
        *   TODO FIX
        */
        void* GetThunk(MonoClass* classObj, char const* methodName, char const* signature);

        /*
        *   get mono array from a mono object
        */
        MonoArray* GetArray(MonoObject* obj);

        /*
        *   get mono array from a mono class
        */
        //MonoArray* GetMonoArray(MonoClass* classObj);

        /*
        *   set mono array in a field
        */
        template <typename T>
        void SetArray(MonoObject* obj, MonoClassField* field, std::vector<T> arr);

        /*
        *   set List<T> in a field
        */
        template <typename T>
        void SetList(MonoObject* obj, MonoClassField* field, std::vector<T> arr);

        // ++++++++++ //

        // Call Method | Thunk

        // TODO add param variants

        /*
        *   call method without return value as thunk
        *   TODO FIX
        */
        void CallThunk(MonoClass* classObj, char const* methodName, char const* signature);

        /*
        *   call method with return value as thunk
        *   TODO FIX
        */
        template <typename T>
        T CallThunk(MonoClass* classObj, char const* methodName, char const* signature);
        
        // ++++++++++ //

        // Method | MonoObject

        // Call Method | No Params

        /*
        *   call method with return value
        *   TODO FIX
        */
        template <typename T>
        T CallMethod(MonoObject* obj, char const* methodName);

        /*
        *   call without return value method
        *   TODO FIX
        */
        void CallMethod(MonoObject* obj, char const* methodName);

        // ++++++++++ //

        // Call Method | Params

        /*
        *   call method with return value using parameters
        *   TODO FIX
        */
        template <typename T, typename U>
        T CallMethod(MonoObject* obj, char const* methodName, std::vector<void*> params, U const& val);
        /*
        *   call method with return value using parameters
        *   TODO FIX
        */
        template <typename T, typename U, typename... Args>
        T CallMethod(MonoObject* obj, char const* methodName, std::vector<void*> params, U const& val, Args... args);

        /*
        *   call method without return value using parameters
        *   TODO FIX
        */
        template <typename T>
        void CallMethod(MonoObject* obj, char const* methodName, std::vector<void*> params, T const& val);
        /*
        *   call method without return value using parameters
        *   TODO FIX
        */
        template <typename T, typename... Args>
        void CallMethod(MonoObject* obj, char const* methodName, std::vector<void*> params, T const& val, Args... args);

        // ++++++++++ //

        // Method | MonoClass

        // Call Method | No Params

        /*
        *   call method with return value
        *   TODO FIX
        */
        template <typename T>
        T CallMethod(MonoClass* obj, char const* methodName);
        //template <>
        MonoObject* CallMethodObj(MonoClass* obj, char const* methodName);

        /*
        *   call without return value method
        *   TODO FIX
        */
        void CallMethod(MonoClass* obj, char const* methodName);

        // ++++++++++ //

        // Call Method | Params

        /*
        *   call method with return value using parameters
        *   TODO FIX
        */
        template <typename T, typename U>
        T CallMethod(MonoClass* obj, char const* methodName, std::vector<void*> params, U const& val);
        //template <MonoObject*, typename U>
        template <typename U>
        MonoObject* CallMethodObj(MonoClass* obj, char const* methodName, std::vector<void*> params, U const& val);
        /*
        *   call method with return value using parameters
        *   TODO FIX
        */
        template <typename T, typename U, typename... Args>
        T CallMethod(MonoClass* obj, char const* methodName, std::vector<void*> params, U const& val, Args... args);
        template <typename U, typename... Args>
        MonoObject* CallMethodObj(MonoClass* obj, char const* methodName, std::vector<void*> params, U const& val, Args... args);

        /*
        *   call method without return value using parameters
        *   TODO FIX
        */
        template <typename T>
        void CallMethod(MonoClass* obj, char const* methodName, std::vector<void*> params, T const& val);
        /*
        *   call method without return value using parameters
        *   TODO FIX
        */
        template <typename T, typename... Args>
        void CallMethod(MonoClass* obj, char const* methodName, std::vector<void*> params, T const& val, Args... args);

        // ++++++++++ //

        // Debug

        /*
        *   log mono error if exists
        */
        bool CheckMonoError(MonoError& err);

        /*
        *   log memory information
        */
        void PrintMemoryStats(std::string const& label);

        /*
        *   log loaded assembly types
        */
        void PrintAssemblyTypes(bool const& onlyUserTypes = false, bool const& onlyEngineTypes = false);

        /*
        *   get message from MonoObject exception
        */
        std::string GetExceptionMessage(MonoObject* exceptionObject);

        /*
        *   get detailed message from MonoObject exception
        */
        std::string GetDetailedExceptionMessage(MonoObject* exceptionObject);

        /*
        *   object callback
        */
        static mono_bool ObjectCallback(MonoObject* obj, MonoClass* c, uintptr_t size,
            uintptr_t numRefs, MonoObject** refs, unsigned long long* offsets, void* userData);

        // ++++++++++ //

        // Garbage Collection

        /*
        *   track object for garbage collection
        */
        MonoObject* AddGC(MonoObject* obj, bool const& lifeGC = false);
        /*
        *   garbage collect object
        */
        void RemoveGC(uint32_t handle, bool const& lifeGC = false);
        void RemoveGC(MonoObject* obj, bool const& lifeGC = false);
        /*
        *   check if object is valid
        */
        bool IsObjValid(MonoObject* obj, bool const& lifeGC = false);
        bool IsObjValid(uint32_t handle);
        bool IsObjValid(MonoGCHandle const& handle, bool const& lifeGC = false);
        /*
        *   clear invalidated objects in gc
        */
        void ClearInvalidGC();
        /*
        *   clear all objects in gc
        */
        void ClearAllGC();

        /*
        *   instantiate new object as nullptr and track for garbage collection
        */
        //MonoObject*     GCObj       (bool const& lifeGC = false);

        /*
        *   track existing object for garbage collection
        */
        MonoObject*     GCObj       (MonoObject* obj, bool const& lifeGC = false);
        /*
        *   track existing c++ string for garbage collection
        */
        MonoString*     GCObjStr    (const char* str); //, bool const& lifeGC = false);
        /*
        *   track existing mono string for garbage collection
        */
        MonoString*     GCObjStr    (MonoString* str, bool const& lifeGC);
        /*
        *   track existing mono class for garbage collection
        */
        MonoClass*      GCObjClass  (MonoClass* obj, bool const& lifeGC = false);
        /*
        *   track existing mono method for garbage collection
        */
        MonoMethod*     GCObjMethod (MonoMethod* obj, bool const& lifeGC = false);
        /*
        *   track existing mono c# field for garbage collection
        */
        MonoClassField* GCObjField  (MonoClass* classObj, const char* fieldName, bool const& lifeGC = false);
        /*
        *   track existing c# property for garbage collection
        */
        MonoProperty*   GCObjProp   (MonoClass* classObj, const char* propName, bool const& lifeGC = false);
        /*
        *   clean up stale GC handles
        */
        void CleanupStaleHandles();
        /*
        *   check current memory and GC status
        */
        void CheckMemoryStatus();
        /*
        *   free mono object
        */
        static void MonoFree(void* obj);
        /*
        *   get gc handle (DANGEROUS)
        */
        uint32_t GetGC(MonoObject* obj, bool const& lifeGC);

        /*
        *   Force Garbage Collection (GC)
        */
        void ForceGC(bool const& immediate = true);
        void ForceGC(int gen);

        // ++++++++++ //

        // Threads

        void DetachNonEssentialThreads();

        // ++++++++++ //

        // Conversion

        /*
        *   convert mono c# string to c++ std::string
        */
        std::string ToStdString(MonoString* str);

        /*
        *   convert c++ std::string to mono c# string
        */
        MonoString* ToMonoString(std::string const& str);

        /*
        *   convert MonoObject* to c++ type
        */
        template <typename T>
        T ToType(MonoObject* obj);

        // ++++++++++ //

        // Helper

        /*
        *   get all classes with same base class in MonoAPI
        */
        std::vector<MonoClass*> GetClassWithSameBase(const char* baseClassN, const char* namespaceN);
        
        /*
        *   check if name is C++ namespace
        */
        bool IsCppName(std::string const& name);

        /*
        *   separate namespace and class name (Namespace::Class)
        */
        std::pair<std::string, std::string> SplitName(std::string const& name);

        /*
        *   check if there is a loaded user class of the same name
        */
        bool IsLoadedUserClass(std::string const& name);

        /*
        *   check if class has overridden given method
        */
        bool IsOverridden(MonoClass* derived, MonoClass* base, std::string const& method);
        
        MonoString* CreateMonoString(MonoDomain* app, char const* val);

        // ++++++++++ //

        // Hot Reload

        /*
        *   unload mono assembly
        */
        void UnloadAssembly();

        /*
        *   reload the mono assembly
        */
        void ReloadAssembly();

        /*
        *   starts observing for changes in C# script code
        */
        void StartObserving();
    };
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

#include "MonoAPITemplates.h"