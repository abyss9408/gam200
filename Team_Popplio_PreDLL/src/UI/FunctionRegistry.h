/******************************************************************************/
/*!
\file   FunctionRegistry.h
\author Team Popplio
\author Hafawati Binte Mohd Ali (100%)
\par    Course : CSD2450
\par    Section : A
\date   2025/02/06 09:50:22 AM (Last Edit)
\brief
This header file defines the FunctionRegistry class, which provides a
centralized registry for function callbacks. It allows for dynamic function
registration, retrieval, and execution within the engine's UI and scripting systems.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include <pch.h>
#include <unordered_map>
#include <functional>
#include <vector>
#include <string>
#include <mutex>

namespace Popplio
{

    /*!*************************************************************************
    \class FunctionRegistry
    \brief A singleton class that manages function callbacks.

    The FunctionRegistry class provides a mechanism to register, retrieve, and
    execute function callbacks by name. This allows dynamic function binding
    for UI interactions, scripting, and other event-driven operations.
    *************************************************************************/
    class FunctionRegistry
    {
    public:

        /*!*************************************************************************
        \brief Retrieves the singleton instance of the FunctionRegistry.

        \return Reference to the FunctionRegistry instance.
        *************************************************************************/
        static FunctionRegistry& GetInstance()
        {
            static FunctionRegistry instance;
            return instance;
        }

        /*!*************************************************************************
        \brief Registers a function callback with a specified name.

        This function allows storing a function pointer in the registry, enabling
        it to be called dynamically when needed.

        \param[in] name The unique identifier for the function.
        \param[in] func The function callback to register.
        *************************************************************************/
        void RegisterFunction(const std::string& name, std::function<void()> func)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            functionMap[name] = func;
        }

        /*!*************************************************************************
        \brief Checks if a function with the given name exists in the registry.

        \param[in] name The function name to check.
        \return True if the function exists, false otherwise.
        *************************************************************************/
        bool HasFunction(const std::string& name) const
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return functionMap.find(name) != functionMap.end();
        }

        /*!*************************************************************************
        \brief Retrieves a registered function by name.

        \param[in] name The function name to retrieve.
        \return The function callback if found, otherwise an empty function.
        *************************************************************************/
        std::function<void()> GetFunction(const std::string& name) const
        {
            std::lock_guard<std::mutex> lock(mutex_);
            auto it = functionMap.find(name);
            if (it != functionMap.end())
            {
                return it->second;
            }
            return std::function<void()>(); // Return an empty function instead of nullptr
        }

        /*!*************************************************************************
        \brief Retrieves all registered function names.

        \return A vector containing the names of all registered functions.
        *************************************************************************/
        std::vector<std::string> GetAllFunctionNames() const
        {
            std::lock_guard<std::mutex> lock(mutex_);
            std::vector<std::string> names;
            for (const auto& pair : functionMap)
            {
                names.push_back(pair.first);
            }
            return names;
        }

    private:
        std::unordered_map<std::string, std::function<void()>> functionMap; /*!< Stores function name-to-callback mappings. */
        mutable std::mutex mutex_; /*!< Ensures thread safety for function registration and retrieval. */

        /*!*************************************************************************
        \brief Private constructor to enforce the singleton pattern.
        *************************************************************************/
        FunctionRegistry() = default;

        /*!*************************************************************************
        \brief Deleted copy constructor to prevent copying of the singleton instance.
        *************************************************************************/
        FunctionRegistry(const FunctionRegistry&) = delete;

        /*!*************************************************************************
        \brief Deleted assignment operator to prevent reassignment of the singleton instance.
        *************************************************************************/
        FunctionRegistry& operator=(const FunctionRegistry&) = delete;
    };

} // namespace Popplio
