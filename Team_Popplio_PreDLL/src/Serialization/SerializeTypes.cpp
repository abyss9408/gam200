/******************************************************************************/
/*!
\file   SerializeTypes.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C++ source file for serializing & deserializing spectific Types

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>

#include "SerializeTypes.h"

#include "../Utilities/Constants.h"

namespace Popplio
{
    template <>
    void SerializeType<bool>(std::string const& name, rapidjson::Value& value, bool const& field, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value val;
        val.SetBool(field);
        rapidjson::Value str(name.c_str(), allocator);
        value.AddMember(str, val, allocator);
    }
    template <>
    void SerializeType<std::string>(std::string const& name, rapidjson::Value& value, std::string const& field, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value val;
        val.SetString(field.c_str(), static_cast<rapidjson::SizeType>(field.length()), allocator);
        rapidjson::Value str(name.c_str(), allocator);
        value.AddMember(str, val, allocator);
    }
    template <>
    void SerializeType<int>(std::string const& name, rapidjson::Value& value, int const& field, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value val;
        val.SetInt(field);
        rapidjson::Value str(name.c_str(), allocator);
        value.AddMember(str, val, allocator);
    }
    template <>
    void SerializeType<float>(std::string const& name, rapidjson::Value& value, float const& field, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value val;
        val.SetFloat(field);
        rapidjson::Value str(name.c_str(), allocator);
        value.AddMember(str, val, allocator);
    }
    template <>
    void SerializeType<double>(std::string const& name, rapidjson::Value& value, double const& field, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value val;
        val.SetDouble(field);
        rapidjson::Value str(name.c_str(), allocator);
        value.AddMember(str, val, allocator);
    }
    template <>
    void SerializeType<char>(std::string const& name, rapidjson::Value& value, char const& field, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value val;
        val.SetString(&field, static_cast<rapidjson::SizeType>(1), allocator);
        rapidjson::Value str(name.c_str(), allocator);
        value.AddMember(str, val, allocator);
    }
    template <>
    void SerializeType<unsigned int>(std::string const& name, rapidjson::Value& value, unsigned int const& field, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value val;
        val.SetUint(field);
        rapidjson::Value str(name.c_str(), allocator);
        value.AddMember(str, val, allocator);
    }
}