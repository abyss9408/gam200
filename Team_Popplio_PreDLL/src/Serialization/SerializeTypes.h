/******************************************************************************/
/*!
\file   SerializeTypes.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C++ header file for serializing & deserializing spectific Types

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <rapidjson/document.h>

namespace Popplio
{
    template <typename T>
    void SerializeType(std::string const& name, rapidjson::Value& value, 
        T const& field, rapidjson::Document::AllocatorType& allocator)
    {
        static_cast<void>(name);
        static_cast<void>(value);
        static_cast<void>(field);
        static_cast<void>(allocator);
        Logger::Warning("Popplio::SerializeType | Type not supported");
    }
    template <>
    void SerializeType<bool>(std::string const& name, rapidjson::Value& value, 
        bool const& field, rapidjson::Document::AllocatorType& allocator);
    template <>
    void SerializeType<std::string>(std::string const& name, rapidjson::Value& value, 
        std::string const& field, rapidjson::Document::AllocatorType& allocator);
    template <>
    void SerializeType<int>(std::string const& name, rapidjson::Value& value, 
        int const& field, rapidjson::Document::AllocatorType& allocator);
    template <>
    void SerializeType<float>(std::string const& name, rapidjson::Value& value, 
        float const& field, rapidjson::Document::AllocatorType& allocator);
    template <>
    void SerializeType<double>(std::string const& name, rapidjson::Value& value, 
        double const& field, rapidjson::Document::AllocatorType& allocator);
    template <>
    void SerializeType<char>(std::string const& name, rapidjson::Value& value, 
        char const& field, rapidjson::Document::AllocatorType& allocator);
    template <>
    void SerializeType<unsigned int>(std::string const& name, rapidjson::Value& value, 
        unsigned int const& field, rapidjson::Document::AllocatorType& allocator);

    namespace
    {
        template <typename T>
        void SerializeObjTypeInternal(std::string const& name, rapidjson::Value& value, rapidjson::Value& objValue,
            rapidjson::Document::AllocatorType& allocator, std::string const& objName, T const& field)
        {
            rapidjson::Value objNameStr(objName.c_str(), allocator);
            objValue.AddMember(objNameStr, field, allocator);
            rapidjson::Value nameStr(name.c_str(), allocator);
            value.AddMember(nameStr, objValue, allocator);
        }
        template <typename T, typename ...Args>
        void SerializeObjTypeInternal(std::string const& name, rapidjson::Value& value, rapidjson::Value& objValue,
            rapidjson::Document::AllocatorType& allocator, std::string const& objName, T const& field, Args... params)
        {
            rapidjson::Value objNameStr(objName.c_str(), allocator);
            objValue.AddMember(objNameStr, field, allocator);
            SerializeObjTypeInternal(name, value, objValue, allocator, params...);
        }
    }
    template <typename ...Args>
    void SerializeObjType(std::string const& name, rapidjson::Value& value, 
        rapidjson::Document::AllocatorType& allocator, Args... params)
    {
        rapidjson::Value obj(rapidjson::kObjectType);
        SerializeObjTypeInternal(name, value, obj, allocator, params...);
    }
    template <typename T>
    void SerializeObjType(std::string const& name, rapidjson::Value& value, 
        rapidjson::Document::AllocatorType& allocator, std::initializer_list<std::string> names, std::initializer_list<T> values)
    {
        //if (names.size() <= 0 || values.size() <= 0) return;
        if (names.size() != values.size())
        {
            Logger::Error("Popplio::SerializeObjType | Names and Values size mismatch");
            return;
        }

        rapidjson::Value obj(rapidjson::kObjectType);
        if (names.size() > 0)
        {
            for (size_t i{}; names.begin() + i != names.end(); ++i)
            {
                SerializeType(*(names.begin() + i), obj, *(values.begin() + i), allocator);
                //rapidjson::Value objNameStr((names.begin() + i)->c_str(), allocator);
                //obj.AddMember(objNameStr, *(values.begin() + i), allocator);
            }
        }
        rapidjson::Value nameStr(name.c_str(), allocator);
        value.AddMember(nameStr, obj, allocator);
    }
    template <typename T>
    void SerializeObjType(std::string const& name, rapidjson::Value& value, 
        rapidjson::Document::AllocatorType& allocator, std::vector<std::string> names, std::vector<T> values)
    {
        //if (names.size() <= 0 || values.size() <= 0) return;
        if (names.size() != values.size())
        {
            Logger::Error("Popplio::SerializeObjType | Names and Values size mismatch");
            return;
        }

        rapidjson::Value obj(rapidjson::kObjectType);
        if (names.size() > 0)
        {
            for (size_t i{}; names.begin() + i != names.end(); ++i)
            {
                SerializeType(*(names.begin() + i), obj, *(values.begin() + i), allocator);
                //rapidjson::Value objNameStr((names.begin() + i)->c_str(), allocator);
                //obj.AddMember(objNameStr, *(values.begin() + i), allocator);
            }
        }
        rapidjson::Value nameStr(name.c_str(), allocator);
        value.AddMember(nameStr, obj, allocator);
    }
    template <typename T>
    void SerializeObjType(std::string const& name, rapidjson::Value& value, 
        rapidjson::Document::AllocatorType& allocator, std::vector<std::pair<std::string, T>> values)
    {
        //if (values.size() <= 0) return;

        rapidjson::Value obj(rapidjson::kObjectType);
        if (values.size() > 0)
        {
            for (std::pair<std::string, T> const& val : values)
            {
                SerializeType(val.first, obj, val.second, allocator);
                //rapidjson::Value objNameStr(val.first.c_str(), allocator);
                //obj.AddMember(objNameStr, val.second, allocator);
            }
        }
        rapidjson::Value nameStr(name.c_str(), allocator);
        value.AddMember(nameStr, obj, allocator);
    }
    template <typename T>
    void SerializeObjType(std::string const& name, rapidjson::Value& value, 
        rapidjson::Document::AllocatorType& allocator, std::vector<std::pair<std::string, std::vector<T>>> values)
    {
        rapidjson::Value obj(rapidjson::kObjectType);
        for (unsigned int i{}; i < values.size(); ++i)
        {
            rapidjson::Value element(rapidjson::kObjectType);
            for (unsigned int j{}; j < values[i].second.size(); ++j)
            {
                SerializeType<T>(std::to_string(j), element, values[i].second[j], allocator);
            }
            rapidjson::Value varStr(values[i].first.c_str(), allocator);
            obj.AddMember(varStr, element, allocator);
        }
        rapidjson::Value nameStr(name.c_str(), allocator);
        value.AddMember(nameStr, obj, allocator);
    }

    template <typename T, typename U>
    void SerializeObjType(std::string const& name, rapidjson::Value& value,
        rapidjson::Document::AllocatorType& allocator, std::vector<std::pair<std::string, std::pair<T, U>>> values)
    {
        //if (values.size() <= 0) return;

        rapidjson::Value obj(rapidjson::kObjectType);
        if (values.size() > 0)
        {
            for (std::pair<std::string, std::pair<T, U>> const& val : values)
            {
                SerializeType(val.first + "_ent", obj, val.second.first, allocator);
                SerializeType(val.first + "_comp", obj, val.second.second, allocator);
                //rapidjson::Value objNameStr(val.first.c_str(), allocator);
                //obj.AddMember(objNameStr, val.second, allocator);
            }
        }
        rapidjson::Value nameStr(name.c_str(), allocator);
        value.AddMember(nameStr, obj, allocator);
    }
    template <typename T, typename U>
    void SerializeObjType(std::string const& name, rapidjson::Value& value,
        rapidjson::Document::AllocatorType& allocator, std::vector<std::pair<std::string, std::vector<std::pair<T, U>>>> values)
    {
        rapidjson::Value obj(rapidjson::kObjectType);
        for (unsigned int i{}; i < values.size(); ++i)
        {
            rapidjson::Value element(rapidjson::kObjectType);
            for (unsigned int j{}; j < values[i].second.size(); ++j)
            {
                SerializeType<T>(std::to_string(j) + "_ent", element, values[i].second[j].first, allocator);
                SerializeType<U>(std::to_string(j) + "_comp", element, values[i].second[j].second, allocator);
            }
            rapidjson::Value varStr(values[i].first.c_str(), allocator);
            obj.AddMember(varStr, element, allocator);
        }
        rapidjson::Value nameStr(name.c_str(), allocator);
        value.AddMember(nameStr, obj, allocator);
    }
}