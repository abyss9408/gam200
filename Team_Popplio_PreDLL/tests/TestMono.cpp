/******************************************************************************/
/*!
\file   TestMono.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the source file for mono tests

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>

#include "TestMono.h"

#include <iostream>

namespace PopplioTest
{
    void TestMonoAPI(MonoAPI& mono)
    {
        // invoke test ------------------------ //
        MonoObject* obj{ mono.InstClass("BassNKick", "Test") };
        
        mono.PrintMemoryStats("1");

        mono.CallMethod(obj, "PrintFloatVar");

        mono.PrintMemoryStats("2");

        std::stringstream ss;
        float f = mono.CallMethod<float>(obj, "ReturnFloatVar");

        mono.PrintMemoryStats("3");

        ss << f;
        std::cout << ss.str() << std::endl;;

        std::vector<void*> params{};

        mono.CallMethod(obj, "IncrementFloatVar", params, 5.0f);
        
        mono.PrintMemoryStats("4");
        
        mono.CallMethod(obj, "PrintFloatVar");

        mono.PrintMemoryStats("5");

        params.clear();

        std::cout << mono.CallMethod<int, int>(obj, "IncrementInt", params, 3) << std::endl;

        mono.PrintMemoryStats("6");

        // invoke test ------------------------ //

        // get set test ------------------------ //
        //MonoObject* obj = mono.InstClass("BassNKick", "Test");
        MonoClass* classObj{ mono_object_get_class(obj) };

        mono.PrintMemoryStats("7");

        MonoClassField* field = mono_class_get_field_from_name(classObj, "MyPublicFloatVar");

        mono.PrintMemoryStats("8");

        uint8_t fieldAccess = mono.GetFieldAccess(field);

        mono.PrintMemoryStats("9");

        if (fieldAccess & static_cast<uint8_t>(MonoAPI::Access::Public))
        {
            // can safely access this field
            float val{mono.Get<float>(obj, field)};

            mono.PrintMemoryStats("10");

            val += 10.f;

            mono.Set(obj, field, val);

            mono.PrintMemoryStats("11");
        }

        // get ref to private
        field = mono_class_get_field_from_name(classObj, "MyPrivateFloatVar");

        mono.PrintMemoryStats("12");

        fieldAccess = mono.GetFieldAccess(field);

        mono.PrintMemoryStats("13");

        if (fieldAccess & static_cast<uint8_t>(MonoAPI::Access::Private))
        {
            // shouldn't access this field

        }

        MonoProperty* prop = mono_class_get_property_from_name(classObj, "Name");

        mono.PrintMemoryStats("14");

        std::string str = mono.GetStr(obj, prop);
        std::cout << str << std::endl;

        mono.PrintMemoryStats("15");

        str += " is the best!";

        field = mono_class_get_field_from_name(classObj, "MyPublicFloatVar");

        mono.PrintMemoryStats("16");

        std::cout << mono.Get<float>(obj, field) << std::endl;

        mono.PrintMemoryStats("17");

        mono.SetStr(obj, prop, str);

        mono.PrintMemoryStats("18");

        std::cout << mono.GetStr(obj, prop) << std::endl;

        std::cout << mono.Get<float>(obj, field) << std::endl;

        prop = mono_class_get_property_from_name(classObj, "MyPublicFloatProperty");

        std::cout << mono.GetProp<float>(obj, prop) << std::endl;

        mono.SetProp(obj, prop, 100.f);

        std::cout << mono.GetProp<float>(obj, prop) << std::endl;

        // get set test ------------------------ //

        //MonoObject* inst{ mono.InstClass("BassNKick", mono_class_get_name(classObj)) };
        //mono.CallMethod(obj, "Do");

        //mono.CallMethod(obj, "Setup");

        // thunk test ------------------------ //
        mono.CallThunk(classObj, "PrintThunk", "()V"); // TODO FIX
        // thunk test ------------------------ //
    }
}