/******************************************************************************/
/*!
\file   PersistOnLoadComponent.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the header file for PersistOnLoad Component
        Labels entity to persist on scene load and not be destroyed

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "../Serialization/SerializeTypes.h"

namespace Popplio
{
    struct PersistOnLoadComponent // todo: add to component system
    {
        bool persistOnLoad;

        PersistOnLoadComponent() : persistOnLoad(true) {} // default to active
        PersistOnLoadComponent(bool const& active) : persistOnLoad(active) {}
        ~PersistOnLoadComponent() = default;

        void Serialize(rapidjson::Value& objectValue, rapidjson::Value& componentValue,
            rapidjson::Document::AllocatorType& allocator) const
        {
            SerializeType("persistOnLoad", componentValue, persistOnLoad, allocator);
            objectValue.AddMember("persist", componentValue, allocator);
        }
        template <typename ObjectType>
        static void Deserialize(rapidjson::Value const& componentVal, ObjectType& object, bool safeMode)
        {
            bool persistOnLoad;
            if (safeMode)
            {
                if (!componentVal.IsObject())
                {
                    Logger::Error("Invalid persist on load component structure");
                    return;
                }
                if (!componentVal.HasMember("persistOnLoad"))
                {
                    persistOnLoad = true;
                    Logger::Warning("Invalid persist on load structure, using defaults");
                }
                else persistOnLoad = componentVal["persistOnLoad"].GetBool();
            }
            else
            {
                persistOnLoad = componentVal["persistOnLoad"].GetBool();
            }
            object.AddComponent<PersistOnLoadComponent>(persistOnLoad);
        }
    };
}