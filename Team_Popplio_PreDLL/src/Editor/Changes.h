/******************************************************************************/
/*!
\file   Changes.h
\author Team Popplio
\author			Ng Jun Heng Shawn
\contribution	Shawn - 100%
\par    Course : CSD2401
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the header file for change detection in the editor
        Detects changes in entities and components during editor mode
        Implements change detection for undo and redo functionality

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "../EventBus/Event.h"
#include "../ECS/ECS.h"

#include "../Utilities/Any.h"

#include <functional>

#define CHANGE_STACK_SIZE 20

namespace Popplio
{
    enum ChangeAction : uint8_t
    {
        ADD_COMPONENT = 0,
        MODIFY_COMPONENT,
        REMOVE_COMPONENT,
        ADD_ENTITY,
        MODIFY_ENTITY,
        REMOVE_ENTITY
    };

    //template <typename T>
    //struct SceneChangeType
    //{
    //    using type = typename T;

    //    std::any value;
    //    SceneChangeType(T const& v) : value(v) {}
    //    Get() { return PopplioUtil::AnyTo<T>(value); }
    //};

    // Scene Change event emitted by components and entities when they are edited, added, or removed from the scene
    template <typename E>
    class SceneChangeEvent : public Event
    {
        public:
        // entities the change is tied to 
        // (use the newState / oldState entity if the change is an entity instead of a component)
        std::vector<Entity> entities; 

        // entity(s) or component(s) that was changed
        // use same as old state if new state doesnt exist
        std::vector<E> newState; 

        // entity(s) or component(s) that was changed 
        // use same as new state if old state doesnt exist
        std::vector<E> oldState; 

        std::vector<ChangeAction> changes; // 0 = added to scene / 1 = already exists / 2 = removed from scene

        std::vector<std::string> typeName;

        SceneChangeEvent(Entity e, E newS, E oldS, ChangeAction const& c, 
            std::string typeId = static_cast<std::string>(typeid(std::any).name())) :
            entities(), newState(), oldState(), changes(), typeName()
        { 
            entities.push_back(e); 
            newState.push_back(newS);
            oldState.push_back(oldS);
            changes.push_back(c);
            typeName.push_back(typeId);
        }
        SceneChangeEvent(std::vector<Entity> e, std::vector<E> newS, 
            std::vector<E> oldS, std::vector<ChangeAction> const& c, 
            std::vector<std::string> typeId = std::vector<std::string>()) :
            entities(e), newState(newS), oldState(oldS), changes(c), typeName() 
        {
            if (typeId.empty()) typeName = 
                std::vector<std::string>(typeId.size(), static_cast<std::string>(typeid(std::any).name()));
            else typeName = typeId;
        }

        //SceneChangeEvent(SceneChangeEvent<E>& other) = delete;
        //~SceneChangeEvent() { delete entity; }
    };
}