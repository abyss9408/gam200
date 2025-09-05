/******************************************************************************/
/*!
\file   Prefab.h
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/14
\brief
	This header file defines the Prefab class within the Popplio namespace.
	A Prefab is a template for creating entities with predefined components in a game engine.
	The class provides methods to instantiate these prefabs into a registry, validate their state,
	and manage their components.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <typeindex>
#include <any>
#include "../ECS/ECS.h"
#include "../Transformation/TransformComponent.h"
#include "../Script/ActiveComponent.h"
#include "PrefabRef.h"

// Forward declarations
namespace Popplio
{
	class PrefabManager;
}

namespace Popplio
{
	/**
  * @brief Represents a Prefab object that can be used to instantiate entities with predefined components.
  */
	class Prefab
	{
	public:
		/**
   * @brief Constructs a Prefab object with the specified name.
   * @param name The name of the Prefab.
   */
		explicit Prefab(const std::string& name);

		/**
   * @brief Adds a component of type T to the Prefab with the specified arguments.
   * @tparam T The type of the component to add.
   * @tparam Args The types of the arguments to pass to the component constructor.
   * @param args The arguments to pass to the component constructor.
   */
		template <typename T, typename... Args>
		void AddComponent(Args&&... args);

		/**
   * @brief Removes the component of type T from the Prefab.
   * @tparam T The type of the component to remove.
   */
		template <typename T>
		void RemoveComponent();

		/**
   * @brief Checks if the Prefab has a component of type T.
   * @tparam T The type of the component to check.
   * @return True if the Prefab has a component of type T, false otherwise.
   */
		template <typename T>
		bool HasComponent() const;

		/**
   * @brief Gets the component of type T from the Prefab.
   * @tparam T The type of the component to get.
   * @return A reference to the component of type T.
   * @throws std::runtime_error if the component of type T is not found in the Prefab.
   */
		template <typename T>
		T& GetComponent();

		/**
   * @brief Gets the component of type T from the Prefab.
   * @tparam T The type of the component to get.
   * @return A const reference to the component of type T.
   * @throws std::runtime_error if the component of type T is not found in the Prefab.
   */
		template <typename T>
		const T& GetComponent() const;

		/**
   * @brief Instantiates an entity with the components defined in the Prefab.
   * @param registry The ECS registry to create the entity in.
   * @return The instantiated entity.
   */
		[[nodiscard]] Entity InstantiateBase(Registry& registry) const;

		void InstantiateChildPrefabs(Entity& parentEntity, const std::vector<PrefabRef>& childRefs, Registry& registry, PrefabManager& prefabManager) const;

		// Handles nested prefabs
		[[nodiscard]] Entity Instantiate(Registry& registry, PrefabManager& prefabManager) const;

		/**
   * @brief Gets the name of the Prefab.
   * @return The name of the Prefab.
   */
		[[nodiscard]] const std::string& GetName() const noexcept;

		/**
   * @brief Checks if the Prefab is valid.
   * @return True if the Prefab is valid, false otherwise.
   */
		[[nodiscard]] bool IsValid() const noexcept;

		void AddChildPrefab(const PrefabRef& prefabRef);
		void RemoveChildPrefab(const std::string& prefabPath);
		[[nodiscard]] const std::vector<PrefabRef>& GetChildPrefabs() const;

	private:
		std::string name; /**< The name of the Prefab. */
		bool isValid; /**< Flag indicating if the Prefab is valid. */
		std::unordered_map<std::type_index, std::function<void(Entity&)>> componentFactories; /**< Map of component factories. */
		std::unordered_map<std::type_index, std::any> componentData; /**< Map of component data. */
		std::vector<PrefabRef> childPrefabs; /**< The children prefabs. */
		/**
   * @brief Helper function to validate the state of the Prefab.
   */
		void ValidatePrefabState();
	};

	template <typename T>
	void Prefab::RemoveComponent()
	{
		// Special handling for TransformComponent - cannot be removed
		if constexpr (std::is_same_v<T, TransformComponent>)
		{
			Logger::Warning("Cannot remove TransformComponent from prefab as it is required", LogDestination::IMGUI_CONSOLE);
			return;
		}

		// Special handling for ActiveComponent - cannot be removed
		if constexpr (std::is_same_v<T, ActiveComponent>)
		{
			Logger::Warning("Cannot remove ActiveComponent from prefab as it is required", LogDestination::IMGUI_CONSOLE);
			return;
		}

		// Check if component exists
		if (!HasComponent<T>())
		{
			Logger::Warning("Attempted to remove non-existent component type: " + std::string(typeid(T).name()), LogDestination::IMGUI_CONSOLE);
			return;
		}

		auto typeIndex = std::type_index(typeid(T));

		// Remove from component data storage
		componentData.erase(typeIndex);

		// Remove from component factories
		componentFactories.erase(typeIndex);

		ValidatePrefabState();

		Logger::Debug("Removed component type: " + std::string(typeid(T).name()) + " from prefab: " + name, LogDestination::IMGUI_CONSOLE);
	}

	template <typename T, typename... Args>
	void Prefab::AddComponent(Args&&... args)
	{
		// Update components data
		componentData[std::type_index(typeid(T))] = T(std::forward<Args>(args)...);

		// Create factory function for this component
		componentFactories[std::type_index(typeid(T))] =
			[args = std::make_tuple(std::forward<Args>(args)...)](Entity& e) {
			std::apply([&e](const auto&... params) {
				e.AddComponent<T>(params...);
			}, args);
		};

		ValidatePrefabState();

		Logger::Debug("Added component to prefab " + name + ": " + typeid(T).name(), LogDestination::IMGUI_CONSOLE);
	}

	template <typename T>
	bool Prefab::HasComponent() const
	{
		return componentData.contains(std::type_index(typeid(T)));
	}

	template <typename T>
	T& Prefab::GetComponent()
	{
		auto it = componentData.find(std::type_index(typeid(T)));
		if (it == componentData.end())
		{
			throw std::runtime_error("Component not found in prefab: " + 
				std::string(typeid(T).name()));
		}
		return std::any_cast<T&>(it->second);
	}

	template <typename T>
	const T& Prefab::GetComponent() const
	{
		auto it = componentData.find(std::type_index(typeid(T)));
		if (it == componentData.end())
		{
			throw std::runtime_error("Component not found in prefab: " + 
				std::string(typeid(T).name()));
		}
		return std::any_cast<const T&>(it->second);
	}
}