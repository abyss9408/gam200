/******************************************************************************/
/*!
\file   MonoAPITemplates.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the template file for C# Script API (using mono)

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

// MonoAPI.h

namespace PopplioScriptAPI
{
    // Templated Function Definitions //

    // Get Set

    template <typename T>
    T MonoAPI::Get(MonoObject* obj, MonoClassField* field)
    {
        if (field == nullptr) throw std::runtime_error("Field is null");

        //void* val{nullptr};

        //mono_field_get_value(obj, field, &val);
        //return *static_cast<T*>(val);

        try
        {
            T val{};

            mono_field_get_value(obj, field, &val);

            return val;
        }
        catch (std::exception const& e)
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Failed to get field");
            std::stringstream ss;
            ss << "PopplioScriptAPI::MonoAPI | " << e.what();
            Popplio::Logger::Error(ss.str());
        }
    }

    // +++++++++++++++ //

    template <typename T>
    T MonoAPI::Set(MonoObject* obj, MonoClassField* field, T const& val)
    {
        if (field == nullptr) throw std::runtime_error("Field is null");

        mono_field_set_value(obj, field, static_cast<void*>(const_cast<T*>(&val)));
        return val;
    }

    // +++++++++++++++ //

    template <typename T>
    T MonoAPI::GetProp(MonoObject* obj, MonoProperty* prop)
    {
        if (prop == nullptr) throw std::runtime_error("Property is null");

        MonoObject* valObj{ mono_property_get_value(prop, obj, nullptr, nullptr) }; // maybe error

        T val{};

        val = *static_cast<T*>(mono_object_unbox(valObj));

        return val;
    }

    template <typename T>
    void MonoAPI::SetProp(MonoObject* obj, MonoProperty* prop, T const& val)
    {
        if (prop == nullptr) throw std::runtime_error("Property is null");

        void* data[] = { static_cast<void*>(const_cast<T*>(&val)) };
        mono_property_set_value(prop, obj, data, nullptr);

        //return GetProp<T>(obj, prop);
    }

    // +++++++++++++++ //

    template <typename T>
    T* MonoAPI::GetPtr(MonoObject* obj, MonoClass* classObj, char const* field)
    {
        return static_cast<T*>(static_cast<void*>(Get(obj, mono_class_get_field_from_name(classObj, field))));
    }

    // +++++++++++++++ //

    template <typename T>
    void MonoAPI::SetArray(MonoObject* obj, MonoClassField* field, std::vector<T> arr)
    {
        if (field == nullptr) throw std::runtime_error("Field is null");

        try
        {
            mono_field_set_value(obj, field, static_cast<void*>(ArrayToMono1D<T>(arr, &GetAppDomain())));
        }
        catch (std::exception const& e)
        {
            std::stringstream ss;
            ss << "PopplioScriptAPI::MonoAPI | SetMonoArray<T> | Failed to set field | " << e.what();
            Popplio::Logger::Error(ss.str());
        }
    }

    template <typename T>
    void MonoAPI::SetList(MonoObject* obj, MonoClassField* field, std::vector<T> arr)
    {
        if (field == nullptr) throw std::runtime_error("Field is null");

        try
        {
            // get class from field
            MonoClass* fieldClass = mono_object_get_class(mono_field_get_value_object(&GetAppDomain(), field, obj));
            if (fieldClass == nullptr) throw std::runtime_error("Field class is null");
            // get object from field
            MonoObject* fieldObj = mono_field_get_value_object(&GetAppDomain(), field, obj);
            if (fieldObj == nullptr) throw std::runtime_error("Field object is null");
            //MonoClassField* itemsField = mono_class_get_field_from_name(fieldClass, "_items");
            //if (itemsField == nullptr) throw std::runtime_error("Items field is null");
            //MonoArray* itemsArr = nullptr;
            //mono_field_get_value(fieldObj, itemsField, &itemsArr);
            //if (itemsArr == nullptr) throw std::runtime_error("Items array is null");
            //MonoArray* value = ArrayToMono1D<T>(arr, &GetDomain());
            //if (value == nullptr) throw std::runtime_error("Value array is null");

            for (unsigned int i{}; i < arr.size(); i++)
            {
                std::vector<void*> params{};
                CallMethod(fieldObj, "set_Item", params, i, arr[i]);
            }
        }
        catch (std::exception const& e)
        {
            std::stringstream ss;
            ss << "PopplioScriptAPI::MonoAPI | SetMonoArray<T> | Failed to set field | " << e.what();
            Popplio::Logger::Error(ss.str());
        }
    }

    // +++++++++++++++++++++++++++++++++++++ //

    // Invoke

    template <typename T>
    T MonoAPI::CallThunk(MonoClass* classObj, char const* methodName, char const* signature) // TODO FIX
    {
        try
        {
            typedef T(*ThunkType)();
            ThunkType thunk = reinterpret_cast<ThunkType>(GetThunk(classObj, methodName, signature));

            return thunk();
        }
        catch (std::exception const& e)
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Failed to call thunk");
            std::stringstream ss;
            ss << "PopplioScriptAPI::MonoAPI | " << e.what();
            Popplio::Logger::Error(ss.str());
        }
    }

    // +++++++++++++++++++++++++++++++++++++ //

    // Method

    // MonoObject

    template <typename T>
    T MonoAPI::CallMethod(MonoObject* obj, char const* methodName) // invoke c# method
    {
        if (!IsObjValid(obj)) // check if obj is valid
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | method obj is invalid");
            return T();
        }

        MonoObject* result{ nullptr };

        // get MonoClass pointer from inst
        MonoClass* classObj{ mono_object_get_class(obj) };

        MonoMethod* method = mono_class_get_method_from_name(classObj, methodName, 0); // get method

        if (method == nullptr) // failed
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Failed to get method");
            return *static_cast<T*>(mono_object_unbox(result));
        }

        // call C# method
        MonoObject* except{ nullptr }; // exception

        result = mono_runtime_invoke(method, obj, nullptr, &except);

        if (except != nullptr) // failed
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Method returned exception");

            // handle exception TODO

            return *static_cast<T*>(mono_object_unbox(result));
        }

        return *static_cast<T*>(mono_object_unbox(result));
    }

    // +++++++++++++++++++++++++++++++++++++ //

    template <typename T, typename U>
    T MonoAPI::CallMethod(MonoObject* obj, char const* methodName, std::vector<void*> params, U const& val)
    {
        if (!IsObjValid(obj)) // check if obj is valid
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | method obj is invalid");
            return T();
        }

        // add to param list based on type
        if constexpr (std::is_fundamental_v<U>) // base data types // constexpr to check if type is fundamental at compile time
        {
            params.push_back(static_cast<void*>(const_cast<U*>(&val)));

            //if constexpr (std::is_const_v<U>)
            //{
            //    params.push_back(static_cast<void*>(const_cast<U*>(&val)));
            //}
            //else
            //{
            //    params.push_back(static_cast<void*>(&val));
            //}
        }
        else if constexpr (std::is_same_v<U, char*> || std::is_same_v<U, char const*>) // c_strings
        {
            MonoString* str{ CreateMonoString(appDomain, const_cast<char const*>(val)) };
            params.push_back(static_cast<void*>(str));
        }
        else if constexpr (std::is_same_v<U, MonoObject*>)
        {
            params.push_back(static_cast<void*>(const_cast<MonoObject*>(val)));
        }
        else // structs / classes (objects)
        {
            std::string objName = typeid(val).name();
            std::stringstream ss(objName);

            std::string buffer{};

            std::getline(ss, buffer, "::");
            std::string namespaceName = buffer;
            std::getline(ss, buffer, "::");
            std::string className = buffer;

            MonoClass* classInst{ GetAssClass(namespaceName.c_str(), className.c_str()) };

            MonoObject const* inst = mono_value_box(appDomain, classInst, &val);
            params.push_back(static_cast<void*>(const_cast<MonoObject*>(inst)));

            //if constexpr (std::is_const_v<U>)
            //{
            //    MonoObject const* inst = mono_value_box(appDomain, classInst, &val);
            //    params.push_back(static_cast<void*>(const_cast<MonoObject*>(inst)));
            //}
            //else
            //{
            //    MonoObject* obj = mono_value_box(appDomain, classInst, &val);
            //    params.push_back(static_cast<void*>(obj));
            //}
        }

        MonoObject* result{ nullptr };

        MonoClass* classInst{ mono_object_get_class(obj) }; // get class instance

        MonoMethod* method = mono_class_get_method_from_name(classInst, methodName, static_cast<int>(params.size())); // get class method

        if (method == nullptr)
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Failed to get method");
            return *static_cast<T*>(mono_object_unbox(result));
        }

        MonoObject* except{ nullptr }; // exception

        result = mono_runtime_invoke(method, obj, params.data(), &except);

        if (except != nullptr) // failed
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Method returned exception");

            // handle exception TODO

            return *static_cast<T*>(mono_object_unbox(result));
        }

        return *static_cast<T*>(mono_object_unbox(result));
    }

    template <typename T, typename U, typename... Args>
    T MonoAPI::CallMethod(MonoObject* obj, char const* methodName, std::vector<void*> params, U const& val, Args... args)
    {
        if (!IsObjValid(obj)) // check if obj is valid
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | method obj is invalid");
            return;
        }

        // add to param list based on type
        if constexpr (std::is_fundamental_v<U>) // base data types
        {
            params.push_back(static_cast<void*>(const_cast<U*>(&val)));

            //if constexpr (std::is_const_v<U>)
            //{
            //    params.push_back(static_cast<void*>(const_cast<U*>(&val)));
            //}
            //else
            //{
            //    params.push_back(static_cast<void*>(&val));
            //}
        }
        else if constexpr (std::is_same_v<U, char*> || std::is_same_v<U, char const*>) // c_strings
        {
            MonoString* str{ CreateMonoString(appDomain, const_cast<char const*>(val)) };
            params.push_back(static_cast<void*>(str));
        }
        else if constexpr (std::is_same_v<U, MonoObject*>)
        {
            params.push_back(static_cast<void*>(const_cast<MonoObject*>(val)));
        }
        else // structs / classes (objects)
        {
            std::string objName = typeid(val).name();
            std::stringstream ss(objName);

            std::string buffer{};

            std::getline(ss, buffer, "::");
            std::string namespaceName = buffer;
            std::getline(ss, buffer, "::");
            std::string className = buffer;

            MonoClass* classInst{ GetAssClass(namespaceName.c_str(), className.c_str()) };

            MonoObject const* inst{ mono_value_box(appDomain, classInst, &val) };
            params.push_back(static_cast<void*>(const_cast<MonoObject*>(inst)));

            //if constexpr (std::is_const_v<U>)
            //{
            //    MonoObject const* inst = mono_value_box(appDomain, classInst, &val);
            //    params.push_back(static_cast<void*>(const_cast<MonoObject*>(inst)));
            //}
            //else
            //{
            //    MonoObject* obj = mono_value_box(appDomain, classInst, &val);
            //    params.push_back(static_cast<void*>(obj));
            //}
        }

        return CallMethod<T>(obj, methodName, params, args...);
    }

    // +++++++++++++++++++++++++++++++++++++ //

    template <typename T>
    void MonoAPI::CallMethod(MonoObject* obj, char const* methodName, std::vector<void*> params, T const& val)
    {
        if (!IsObjValid(obj)) // check if obj is valid
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | method obj is invalid");
            return;
        }

        // add to param list based on type
        if constexpr (std::is_fundamental_v<T>) // base data types
        {
            params.push_back(static_cast<void*>(const_cast<T*>(&val)));

            //if constexpr (std::is_const_v<T>)
            //{
            //    params.push_back(static_cast<void*>(const_cast<T*>(&val)));
            //}
            //else
            //{
            //    params.push_back(static_cast<void*>(&val));
            //}
        }
        else if constexpr (std::is_same_v<T, char*> || std::is_same_v<T, char const*>) // c_strings
        {
            MonoString* str{ CreateMonoString(appDomain, const_cast<char const*>(val)) };
            params.push_back(static_cast<void*>(str));
        }
        else if constexpr (std::is_same_v<T, MonoObject*>)
        {
            params.push_back(static_cast<void*>(const_cast<MonoObject*>(val)));
        }
        else // structs / classes (objects)
        {
            std::string objName = typeid(val).name();

            //bool isCppType = false;

            //std::stringstream tmp(objName);
            //std::string tmpName{};
            //std::getline(ss, tmpName, ' ')

            //if (tmpName == "class")
            //{
            //    isCppType = true;
            //}

            std::stringstream ss(objName);

            // has to be different (dunno why, maybe due to includes or the c# mono)
            std::string namespaceName{};
            std::getline(ss, namespaceName, ' ');
            std::getline(ss, namespaceName, ':');
            std::string className{};
            std::getline(ss, className, ':');
            std::getline(ss, className, ':');

            //std::getline(ss, buffer, "::");
            //std::string namespaceName = buffer;
            //std::getline(ss, buffer, "::");
            //std::string className = buffer;

            if (IsCppName(namespaceName)) // for now push straight into params if it is a cpp object
            {
                params.push_back(static_cast<void*>(const_cast<T*>(&val)));
            }
            else
            {
                MonoClass* classInst{ nullptr };

                //if (IsCppName(namespaceName))
                //{
                //    std::string tmp = namespaceName;
                //    namespaceName = POPPLIO_CORE;
                //    namespaceName += "."; 
                //    namespaceName += tmp;
                //}

                classInst = GetAssClass(namespaceName.c_str(), className.c_str());

                std::string check = mono_class_get_name(classInst);

                MonoObject* box{ mono_value_box(appDomain, classInst, static_cast<void*>(const_cast<T*>(&val))) };

                MonoObject const* inst{ box }; // has to be different (dunno why, maybe due to includes or the c# mono)
                params.push_back(static_cast<void*>(const_cast<MonoObject*>(inst)));

                //if constexpr (std::is_const_v<T>)
                //{
                //    MonoObject const* inst = mono_value_box(appDomain, classInst, &val);
                //    params.push_back(static_cast<void*>(const_cast<MonoObject*>(inst)));
                //}
                //else
                //{
                //    MonoObject* obj = mono_value_box(appDomain, classInst, &val);
                //    params.push_back(static_cast<void*>(obj));
                //}
            }
        }

        MonoObject* result{ nullptr };

        MonoClass* classInst{ mono_object_get_class(obj) }; // get class instance

        MonoMethod* method = mono_class_get_method_from_name(classInst, methodName, static_cast<int>(params.size())); // get class method

        if (method == nullptr)
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Failed to get method");
            return;
        }

        MonoObject* except{ nullptr }; // exception

        result = mono_runtime_invoke(method, obj, params.data(), &except);

        if (except != nullptr) // failed
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Method returned exception");

            // handle exception TODO

            return;
        }

        return;
    }

    template <typename T, typename... Args>
    void MonoAPI::CallMethod(MonoObject* obj, char const* methodName, std::vector<void*> params, T const& val, Args... args)
    {
        if (!IsObjValid(obj)) // check if obj is valid
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | method obj is invalid");
            return;
        }

        // add to param list based on type
        if constexpr (std::is_fundamental_v<T>) // base data types
        {
            params.push_back(static_cast<void*>(const_cast<T*>(&val)));

            //if constexpr (std::is_const_v<T>)
            //{
            //    params.push_back(static_cast<void*>(const_cast<T*>(&val)));
            //}
            //else
            //{
            //    params.push_back(static_cast<void*>(&val));
            //}
        }
        else if constexpr (std::is_same_v<T, char*> || std::is_same_v<T, char const*>) // c_strings
        {
            MonoString* str{ CreateMonoString(appDomain, const_cast<char const*>(val)) };
            params.push_back(static_cast<void*>(str));
        }
        else if constexpr (std::is_same_v<T, MonoObject*>)
        {
            params.push_back(static_cast<void*>(const_cast<MonoObject*>(val)));
        }
        else // structs / classes (objects)
        {
            std::string objName = typeid(val).name();
            std::stringstream ss(objName);

            std::string buffer{};

            std::getline(ss, buffer, "::");
            std::string namespaceName = buffer;
            std::getline(ss, buffer, "::");
            std::string className = buffer;

            MonoClass* classInst{ GetAssClass(namespaceName.c_str(), className.c_str()) };

            MonoObject const* inst{ mono_value_box(appDomain, classInst, &val) };
            params.push_back(static_cast<void*>(const_cast<MonoObject*>(inst)));

            //if constexpr (std::is_const_v<T>)
            //{
            //    MonoObject const* inst = mono_value_box(appDomain, classInst, &val);
            //    params.push_back(static_cast<void*>(const_cast<MonoObject*>(inst)));
            //}
            //else
            //{
            //    MonoObject* obj = mono_value_box(appDomain, classInst, &val);
            //    params.push_back(static_cast<void*>(obj));
            //}
        }

        return CallMethod(obj, methodName, params, args...);
    }

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

    // MonoClass

    template <typename T>
    T MonoAPI::CallMethod(MonoClass* obj, char const* methodName) // invoke c# method
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
            return T();
            //return *static_cast<T*>(mono_object_unbox(result));
        }

        // call C# method
        MonoObject* except{ nullptr }; // exception

        result = mono_runtime_invoke(method, nullptr, nullptr, &except);

        if (except != nullptr) // failed
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Method returned exception");

            // handle exception TODO

            return T();
            //return *static_cast<T*>(mono_object_unbox(result));
        }

        return *static_cast<T*>(mono_object_unbox(result));
    }

    // +++++++++++++++++++++++++++++++++++++ //

    template <typename T, typename U>
    T MonoAPI::CallMethod(MonoClass* obj, char const* methodName, std::vector<void*> params, U const& val)
    {
        //if (!IsObjValid(obj)) // check if obj is valid
        //{
        //    Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | method obj is invalid");
        //    return;
        //}

        // add to param list based on type
        if constexpr (std::is_fundamental_v<U>) // base data types // constexpr to check if type is fundamental at compile time
        {
            params.push_back(static_cast<void*>(const_cast<U*>(&val)));

            //if constexpr (std::is_const_v<U>)
            //{
            //    params.push_back(static_cast<void*>(const_cast<U*>(&val)));
            //}
            //else
            //{
            //    params.push_back(static_cast<void*>(&val));
            //}
        }
        else if constexpr (std::is_same_v<U, char*> || std::is_same_v<U, char const*>) // c_strings
        {
            MonoString* str{ CreateMonoString(appDomain, const_cast<char const*>(val)) };
            params.push_back(static_cast<void*>(str));
        }
        else if constexpr (std::is_same_v<U, MonoObject*>)
        {
            params.push_back(static_cast<void*>(const_cast<MonoObject*>(val)));
        }
        else // structs / classes (objects)
        {
            std::string objName = typeid(val).name();
            std::stringstream ss(objName);

            std::string buffer{};

            std::getline(ss, buffer, "::");
            std::string namespaceName = buffer;
            std::getline(ss, buffer, "::");
            std::string className = buffer;

            MonoClass* classInst{ GetAssClass(namespaceName.c_str(), className.c_str()) };

            MonoObject const* inst = mono_value_box(appDomain, classInst, &val);
            params.push_back(static_cast<void*>(const_cast<MonoObject*>(inst)));

            //if constexpr (std::is_const_v<U>)
            //{
            //    MonoObject const* inst = mono_value_box(appDomain, classInst, &val);
            //    params.push_back(static_cast<void*>(const_cast<MonoObject*>(inst)));
            //}
            //else
            //{
            //    MonoObject* obj = mono_value_box(appDomain, classInst, &val);
            //    params.push_back(static_cast<void*>(obj));
            //}
        }

        MonoObject* result{ nullptr };

        MonoMethod* method = mono_class_get_method_from_name(obj, methodName, static_cast<int>(params.size())); // get class method

        if (method == nullptr)
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Failed to get method");
            return T();
            //return *static_cast<T*>(mono_object_unbox(result));
        }

        MonoObject* except{ nullptr }; // exception

        result = mono_runtime_invoke(method, nullptr, params.data(), &except);

        if (except != nullptr) // failed
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Method returned exception");

            // handle exception TODO

            return T();
            //return *static_cast<T*>(mono_object_unbox(result));
        }

        return *static_cast<T*>(mono_object_unbox(result));
    }

    template <typename U>
    MonoObject* MonoAPI::CallMethodObj(MonoClass* obj, char const* methodName, std::vector<void*> params, U const& val)
    {
        U temp = val;
        // add to param list based on type
        if constexpr (std::is_fundamental_v<U>) // base data types // constexpr to check if type is fundamental at compile time
        {
            params.push_back(static_cast<void*>(&temp));
        }
        else if constexpr (std::is_same_v<U, char*> || std::is_same_v<U, char const*>) // c_strings
        {
            MonoString* str{ CreateMonoString(appDomain, const_cast<char const*>(val)) };
            params.push_back(static_cast<void*>(str));
        }
        else if constexpr (std::is_same_v<U, MonoObject*>)
        {
            params.push_back(static_cast<void*>(const_cast<MonoObject*>(val)));
        }
        else if constexpr (std::is_same_v<U, MonoArray*>)
        {
            params.push_back(static_cast<void*>(const_cast<MonoArray*>(val)));
        }
        else // structs / classes (objects)
        {
            std::string objName = typeid(val).name();
            std::stringstream ss(objName);

            std::string buffer{};

            //std::getline(ss, buffer, "::");
            //std::string namespaceName = buffer;
            //std::getline(ss, buffer, "::");
            //std::string className = buffer;
            std::string namespaceName{};
            std::getline(ss, namespaceName, ' ');
            std::getline(ss, namespaceName, ':');
            std::string className{};
            std::getline(ss, className, ':');
            std::getline(ss, className, ':');

            MonoClass* classInst{ GetAssClass(namespaceName.c_str(), className.c_str()) };

            MonoObject const* inst = mono_value_box(appDomain, classInst, const_cast<U*>(&val));
            params.push_back(static_cast<void*>(const_cast<MonoObject*>(inst)));
        }

        MonoObject* result{ nullptr };

        MonoMethod* method = mono_class_get_method_from_name(obj, methodName, static_cast<int>(params.size())); // get class method

        if (method == nullptr)
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Failed to get method");
            return nullptr;
            //return *static_cast<T*>(mono_object_unbox(result));
        }

        MonoObject* except{ nullptr }; // exception

        result = mono_runtime_invoke(method, nullptr, params.data(), &except);

        if (except != nullptr) // failed
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Method returned exception");

            Popplio::Logger::Error(GetExceptionMessage(except));

            return nullptr;
            //return *static_cast<T*>(mono_object_unbox(result));
        }

        return result;
    }

    template <typename T, typename U, typename... Args>
    T MonoAPI::CallMethod(MonoClass* obj, char const* methodName, std::vector<void*> params, U const& val, Args... args)
    {
        //if (!IsObjValid(obj)) // check if obj is valid
        //{
        //    Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | method obj is invalid");
        //    return;
        //}

        // add to param list based on type
        if constexpr (std::is_fundamental_v<U>) // base data types
        {
            params.push_back(static_cast<void*>(const_cast<U*>(&val)));

            //if constexpr (std::is_const_v<U>)
            //{
            //    params.push_back(static_cast<void*>(const_cast<U*>(&val)));
            //}
            //else
            //{
            //    params.push_back(static_cast<void*>(&val));
            //}
        }
        else if constexpr (std::is_same_v<U, char*> || std::is_same_v<U, char const*>) // c_strings
        {
            MonoString* str{ CreateMonoString(appDomain, const_cast<char const*>(val)) };
            params.push_back(static_cast<void*>(str));
        }
        else if constexpr (std::is_same_v<U, MonoObject*>)
        {
            params.push_back(static_cast<void*>(const_cast<MonoObject*>(val)));
        }
        else // structs / classes (objects)
        {
            std::string objName = typeid(val).name();
            std::stringstream ss(objName);

            std::string buffer{};

            std::getline(ss, buffer, "::");
            std::string namespaceName = buffer;
            std::getline(ss, buffer, "::");
            std::string className = buffer;

            MonoClass* classInst{ GetAssClass(namespaceName.c_str(), className.c_str()) };

            MonoObject const* inst{ mono_value_box(appDomain, classInst, &val) };
            params.push_back(static_cast<void*>(const_cast<MonoObject*>(inst)));

            //if constexpr (std::is_const_v<U>)
            //{
            //    MonoObject const* inst = mono_value_box(appDomain, classInst, &val);
            //    params.push_back(static_cast<void*>(const_cast<MonoObject*>(inst)));
            //}
            //else
            //{
            //    MonoObject* obj = mono_value_box(appDomain, classInst, &val);
            //    params.push_back(static_cast<void*>(obj));
            //}
        }

        return CallMethod<T>(obj, methodName, params, args...);
    }

    template <typename U, typename... Args>
    MonoObject* MonoAPI::CallMethodObj(MonoClass* obj, char const* methodName, std::vector<void*> params, U const& val, Args... args)
    {
        U temp = val;
        // add to param list based on type
        if constexpr (std::is_fundamental_v<U>) // base data types // constexpr to check if type is fundamental at compile time
        {
            params.push_back(static_cast<void*>(&temp));
        }
        else if constexpr (std::is_same_v<U, char*> || std::is_same_v<U, char const*>) // c_strings
        {
            MonoString* str{ CreateMonoString(appDomain, const_cast<char const*>(val)) };
            params.push_back(static_cast<void*>(str));
        }
        else if constexpr (std::is_same_v<U, MonoObject*>)
        {
            params.push_back(static_cast<void*>(const_cast<MonoObject*>(val)));
        }
        else if constexpr (std::is_same_v<U, MonoArray*>)
        {
            params.push_back(static_cast<void*>(const_cast<MonoArray*>(val)));
        }
        else // structs / classes (objects)
        {
            std::string objName = typeid(val).name();
            std::stringstream ss(objName);

            std::string buffer{};

            //std::getline(ss, buffer, "::");
            //std::string namespaceName = buffer;
            //std::getline(ss, buffer, "::");
            //std::string className = buffer;
            std::string namespaceName{};
            std::getline(ss, namespaceName, ' ');
            std::getline(ss, namespaceName, ':');
            std::string className{};
            std::getline(ss, className, ':');
            std::getline(ss, className, ':');

            MonoClass* classInst{ GetAssClass(namespaceName.c_str(), className.c_str()) };

            MonoObject const* inst = mono_value_box(appDomain, classInst, const_cast<U*>(&val));
            params.push_back(static_cast<void*>(const_cast<MonoObject*>(inst)));
        }

        return CallMethodObj(obj, methodName, params, args...);
    }

    // +++++++++++++++++++++++++++++++++++++ //

    template <typename T>
    void MonoAPI::CallMethod(MonoClass* obj, char const* methodName, std::vector<void*> params, T const& val)
    {
        //if (!IsObjValid(obj)) // check if obj is valid
        //{
        //    Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | method obj is invalid");
        //    return;
        //}

        // add to param list based on type
        if constexpr (std::is_fundamental_v<T>) // base data types
        {
            params.push_back(static_cast<void*>(const_cast<T*>(&val)));

            //if constexpr (std::is_const_v<T>)
            //{
            //    params.push_back(static_cast<void*>(const_cast<T*>(&val)));
            //}
            //else
            //{
            //    params.push_back(static_cast<void*>(&val));
            //}
        }
        else if constexpr (std::is_same_v<T, char*> || std::is_same_v<T, char const*>) // c_strings
        {
            MonoString* str{ CreateMonoString(appDomain, const_cast<char const*>(val)) };
            params.push_back(static_cast<void*>(str));
        }
        else if constexpr (std::is_same_v<T, MonoObject*>)
        {
            params.push_back(static_cast<void*>(const_cast<MonoObject*>(val)));
        }
        else // structs / classes (objects)
        {
            std::string objName = typeid(val).name();

            //bool isCppType = false;

            //std::stringstream tmp(objName);
            //std::string tmpName{};
            //std::getline(ss, tmpName, ' ')

            //if (tmpName == "class")
            //{
            //    isCppType = true;
            //}

            std::stringstream ss(objName);

            // has to be different (dunno why, maybe due to includes or the c# mono)
            std::string namespaceName{};
            std::getline(ss, namespaceName, ' ');
            std::getline(ss, namespaceName, ':');
            std::string className{};
            std::getline(ss, className, ':');
            std::getline(ss, className, ':');

            //std::getline(ss, buffer, "::");
            //std::string namespaceName = buffer;
            //std::getline(ss, buffer, "::");
            //std::string className = buffer;

            if (IsCppName(namespaceName)) // for now push straight into params if it is a cpp object
            {
                params.push_back(static_cast<void*>(const_cast<T*>(&val)));
            }
            else
            {
                MonoClass* classInst{ nullptr };

                //if (IsCppName(namespaceName))
                //{
                //    std::string tmp = namespaceName;
                //    namespaceName = POPPLIO_CORE;
                //    namespaceName += "."; 
                //    namespaceName += tmp;
                //}

                classInst = GetAssClass(namespaceName.c_str(), className.c_str());

                std::string check = mono_class_get_name(classInst);

                MonoObject* box{ mono_value_box(appDomain, classInst, static_cast<void*>(const_cast<T*>(&val))) };

                MonoObject const* inst{ box }; // has to be different (dunno why, maybe due to includes or the c# mono)
                params.push_back(static_cast<void*>(const_cast<MonoObject*>(inst)));

                //if constexpr (std::is_const_v<T>)
                //{
                //    MonoObject const* inst = mono_value_box(appDomain, classInst, &val);
                //    params.push_back(static_cast<void*>(const_cast<MonoObject*>(inst)));
                //}
                //else
                //{
                //    MonoObject* obj = mono_value_box(appDomain, classInst, &val);
                //    params.push_back(static_cast<void*>(obj));
                //}
            }
        }

        MonoObject* result{ nullptr };

        MonoMethod* method = mono_class_get_method_from_name(obj, methodName, static_cast<int>(params.size())); // get class method

        if (method == nullptr)
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Failed to get method");
            return;
        }

        MonoObject* except{ nullptr }; // exception

        result = mono_runtime_invoke(method, nullptr, params.data(), &except);

        if (except != nullptr) // failed
        {
            Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | Method returned exception");

            // handle exception TODO

            return;
        }

        return;
    }

    template <typename T, typename... Args>
    void MonoAPI::CallMethod(MonoClass* obj, char const* methodName, std::vector<void*> params, T const& val, Args... args)
    {
        //if (!IsObjValid(obj)) // check if obj is valid
        //{
        //    Popplio::Logger::Error("PopplioScriptAPI::MonoAPI | method obj is invalid");
        //    return;
        //}

        // add to param list based on type
        if constexpr (std::is_fundamental_v<T>) // base data types
        {
            params.push_back(static_cast<void*>(const_cast<T*>(&val)));

            //if constexpr (std::is_const_v<T>)
            //{
            //    params.push_back(static_cast<void*>(const_cast<T*>(&val)));
            //}
            //else
            //{
            //    params.push_back(static_cast<void*>(&val));
            //}
        }
        else if constexpr (std::is_same_v<T, char*> || std::is_same_v<T, char const*>) // c_strings
        {
            MonoString* str{ CreateMonoString(appDomain, const_cast<char const*>(val)) };
            params.push_back(static_cast<void*>(str));
        }
        else if constexpr (std::is_same_v<T, MonoObject*>)
        {
            params.push_back(static_cast<void*>(const_cast<MonoObject*>(val)));
        }
        else // structs / classes (objects)
        {
            std::string objName = typeid(val).name();
            std::stringstream ss(objName);

            std::string buffer{};

            std::getline(ss, buffer, "::");
            std::string namespaceName = buffer;
            std::getline(ss, buffer, "::");
            std::string className = buffer;

            MonoClass* classInst{ GetAssClass(namespaceName.c_str(), className.c_str()) };

            MonoObject const* inst{ mono_value_box(appDomain, classInst, &val) };
            params.push_back(static_cast<void*>(const_cast<MonoObject*>(inst)));

            //if constexpr (std::is_const_v<T>)
            //{
            //    MonoObject const* inst = mono_value_box(appDomain, classInst, &val);
            //    params.push_back(static_cast<void*>(const_cast<MonoObject*>(inst)));
            //}
            //else
            //{
            //    MonoObject* obj = mono_value_box(appDomain, classInst, &val);
            //    params.push_back(static_cast<void*>(obj));
            //}
        }

        return CallMethod(obj, methodName, params, args...);
    }

    // ++++++++++ //

    // Conversion

    template <typename T>
    T MonoAPI::ToType(MonoObject* obj)
    {
        return *static_cast<T*>(mono_object_unbox(obj));
    }
}