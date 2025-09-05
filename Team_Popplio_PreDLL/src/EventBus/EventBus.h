/******************************************************************************/
/*!
\file   EventBus.h
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/10/02
\brief
        This is the header file for the Events System of the engine

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include "Event.h"
#include "../Logging/Logger.h"
#include <map>
#include <typeindex>
#include <memory>
#include <list>
#include <functional>

namespace Popplio
{
	
    /**
     * @brief Interface for event callback classes.
     * 
     * This interface defines the contract for event callback classes.
     * Any class implementing this interface must provide an implementation
     * for the Call() method, which is responsible for invoking the event callback function.
     */
    class IEventCallback
    {
    public:
        /**
         * @brief Destructor for IEventCallback.
         */
        virtual ~IEventCallback() = default;

        /**
         * @brief Call the event callback function.
         * 
         * @param e The event object.
         */
        virtual void Call(Event& e) = 0;
    };

    /**
     * @brief Represents a callback for a member function event.
     * 
     * This class is responsible for invoking a member function callback when an event is triggered.
     * It inherits from the IEventCallback interface and provides an implementation for the Call() method.
     * 
     * @tparam TOwner The owner class type.
     * @tparam TEvent The event type.
     */
    template <typename TOwner, typename TEvent>
    class MemberFunctionEventCallback : public IEventCallback
    {
    public:
        using CallbackFunction = void (TOwner::*)(TEvent&); /**< The type of the member function callback. */
        /**
         * @brief Constructor for MemberFunctionEventCallback.
         * 
         * @param owner The owner instance.
         * @param callback The member function callback.
         */
        MemberFunctionEventCallback(TOwner* owner, CallbackFunction callback)
            : ownerInstance(owner), callbackFunction(callback) {}

        /**
         * @brief Call the member function callback.
         * 
         * @param e The event object.
         */
        void Call(Event& e) override
        {
            std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(e));
        }

    private:
        
        TOwner* ownerInstance; /**< The owner instance. */
        CallbackFunction callbackFunction; /**< The member function callback. */
    };

    /**
     * @brief Represents a callback for a function event.
     * 
     * This class is responsible for invoking a function callback when an event is triggered.
     * It inherits from the IEventCallback interface and provides an implementation for the Call() method.
     * 
     * @tparam TEvent The event type.
     */
    template <typename TEvent>
    class FunctionEventCallback : public IEventCallback
    {
    public:
        /**
         * @brief Constructor for FunctionEventCallback.
         * 
         * @param callback The function callback.
         */
        FunctionEventCallback(std::function<void(TEvent&)> callback)
            : callbackFunction(callback) {}

        /**
         * @brief Call the function callback.
         * 
         * @param e The event object.
         */
        void Call(Event& e) override
        {
            callbackFunction(static_cast<TEvent&>(e));
        }

    private:
        std::function<void(TEvent&)> callbackFunction; /**< The function callback. */
    };

    class EventBus
    {
    public:
        /**
         * @brief Resets the event bus by clearing all subscribers.
         */
        void Reset()
        {
            subscribers.clear();
        }

        /**
         * @brief Subscribes an owner instance to an event with a member function callback.
         * 
         * @tparam TEvent The event type.
         * @tparam TOwner The owner class type.
         * @param ownerInstance The owner instance.
         * @param callbackFunction The member function callback.
         */
        template <typename TEvent, typename TOwner>
        void SubscribeToEvent(TOwner* ownerInstance, void (TOwner::* callbackFunction)(TEvent&))
        {
            subscribers[typeid(TEvent)].push_back(
                std::make_shared<MemberFunctionEventCallback<TOwner, TEvent>>
                (ownerInstance, callbackFunction)
            );
        }

        /**
         * @brief Subscribes a function callback to an event.
         * 
         * @tparam TEvent The event type.
         * @param callback The function callback.
         */
        template <typename TEvent>
        void SubscribeToEvent(std::function<void(TEvent&)> callback)
        {
            subscribers[typeid(TEvent)].push_back(
                std::make_shared<FunctionEventCallback<TEvent>>(callback)
            );
        }

        /**
         * @brief Emits an event by invoking all subscribed event handlers.
         * 
         * @tparam TEvent The event type.
         * @tparam TArgs The argument types.
         * @param args The arguments to construct the event object.
         */
        template <typename TEvent, typename ...TArgs>
        void EmitEvent(TArgs&& ...args)
        {
            auto handlers = subscribers[typeid(TEvent)];
            if (!handlers.empty())
            {
                TEvent event(std::forward<TArgs>(args)...);
                for (auto& handler : handlers)
                {
                    handler->Call(event);
                }
            }
        }

        /**
         * @brief Unsubscribes an owner instance to an event with a member function callback.
         *
         * @tparam TEvent The event type.
         * @tparam TOwner The owner class type.
         * @param ownerInstance The owner instance.
         * @param callbackFunction The member function callback.
         */
        template <typename TEvent, typename TOwner>
        void UnsubscribeToEvent(TOwner* ownerInstance, void (TOwner::* callbackFunction)(TEvent&)) // TODO IMPLEMENT
        {
            //subscribers[typeid(TEvent)].erase(
            //    MemberFunctionEventCallback<TOwner, TEvent>
            //    (ownerInstance, callbackFunction)
            //);
        }

        /**
         * @brief Unsubscribes a function callback to an event.
         *
         * @tparam TEvent The event type.
         * @param callback The function callback.
         */
        template <typename TEvent>
        void UnsubscribeToEvent(std::function<void(TEvent&)> callback) // TODO IMPLEMENT
        {
            //subscribers[typeid(TEvent)].erase(
            //    FunctionEventCallback<TEvent>(callback)
            //);
        }

    private:
        std::map<std::type_index, std::list<std::shared_ptr<IEventCallback>>> subscribers; /**< The map of event subscribers. */
    };
}