/******************************************************************************/
/*!
\file   CloneSystem.h
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/06
\brief
    This header file defines the CloneSystem class within the Popplio namespace. 
    The CloneSystem is responsible for cloning entities in a game engine. It inherits 
    from the System class and provides functionality to subscribe to events and handle 
    the cloning of entities.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Cloning/CloneEntityEvent.h"
#include "../Logging/Logger.h"
#include <random>
#include <ctime>

namespace Popplio
{
    /*!
    \class CloneSystem
    \brief Represents the entity cloning system of the engine.

    This class is responsible for cloning entities in the game engine.
    It inherits from the System class and provides functionality to
    subscribe to events and handle the cloning of entities.

    \author Team Popplio
    \author Bryan Ang Wei Ze
    \date 2024/10/02
    */
    class CloneSystem : public System
    {
    public:
        /*!
        \brief Constructs a CloneSystem object.

        \param reg The registry of the game engine.
        */
        CloneSystem(Registry& reg, LayerManager& lay);

        /*!
        \brief Subscribes to relevant events.

        \param eventBus The event bus of the game engine.
        */
        void SubscribeToEvents(EventBus& eventBus);

        /*!
        \brief Clones components between entities

        \param originalEntity | source

        \param newEntity | destination
        */
        void CloneComponents(Entity& originalEntity, Entity& newEntity);

        int CloneEntity(CloneEntityEvent& event); // might not work

        /*!
        \brief Handles the CloneEntityEvent.

        \param event The CloneEntityEvent to handle.
        */
        void OnCloneEntity(CloneEntityEvent& event);

        /*!
        \brief Registers a component cloner for a specific component type.

        \tparam T The component type to register the cloner for.
        */
        template<typename T>
        void RegisterComponentCloner();
        template<>
        void RegisterComponentCloner<LogicComponent>();
        //template <>
        //void RegisterComponentCloner<ParentComponent>();

    private:
        Registry& registry;
        LayerManager& layerManager;
        std::unordered_map<std::type_index, std::function<void(Entity&, Entity&)>> componentCloners;

        static int cloneCounter;

        void CopyEntityInfo(Entity originalEntity, Entity newEntity);
		void HandleHierarchyCloning(Entity originalEntity, Entity newEntity);
        //void CloneComponentsSecondPass(Entity & originalEntity, Entity & newEntity);
        //void CloneComponentsThirdPass(Entity& originalEntity, Entity& newEntity);
        void HandleLogicCloning(Entity originalEntity, Entity newEntity, Entity rootOriginalEntity, Entity rootNewEntity);
    };

    template <typename T>
    void CloneSystem::RegisterComponentCloner()
    {
        componentCloners[std::type_index(typeid(T))] = [this](Entity& original, Entity& clone)
        {
            if (original.HasComponent<T>())
            {
                T& clonedComponent = original.GetComponent<T>();
                clone.AddComponent<T>(clonedComponent);
            }
        };
    }
}