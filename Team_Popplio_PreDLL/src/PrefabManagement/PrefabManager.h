/******************************************************************************/
/*!
\file   PrefabManager.h
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/06
\brief
	This header file defines the PrefabManager class within the Popplio namespace. 
	The PrefabManager class is responsible for managing prefabs, including 
	registering, retrieving, instantiating, updating, and deleting prefabs. 
	It also handles the association between prefabs and their file paths.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "Prefab.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <optional>
#include "../ECS/ECS.h"
#include "../Logging/Logger.h"
#include "../EventBus/EventBus.h"

// Forward declarations
namespace Popplio
{
	class Prefab;
}

namespace Popplio
{
	/**
  * \class PrefabManager
  * \brief Class responsible for managing prefabs.
  * 
  * The PrefabManager class is responsible for managing prefabs, including
  * registering, retrieving, instantiating, updating, and deleting prefabs.
  * It also handles the association between prefabs and their file paths.
  */
	class PrefabManager
	{
	public:
		/**
   * \brief Constructor for PrefabManager.
   * \param eventBus The event bus to be used by the PrefabManager.
   */
		PrefabManager(EventBus& eventBus);

		/**
   * \brief Registers a prefab with the given name and file path.
   * \param name The name of the prefab.
   * \param prefab The prefab to be registered.
   * \param filePath The file path of the prefab.
   */
		void RegisterPrefab(const std::string& name, std::unique_ptr<Prefab> prefab, const std::string& filePath = "");

		/**
   * \brief Retrieves a prefab with the given name.
   * \param name The name of the prefab to retrieve.
   * \return An optional reference to the retrieved prefab.
   */
		[[nodiscard]] std::optional<std::reference_wrapper<const Prefab>> GetPrefab(const std::string& name) const;

		/**
   * \brief Instantiates a prefab with the given name in the specified registry.
   * \param registry The registry to instantiate the prefab in.
   * \param prefabName The name of the prefab to instantiate.
   * \return An optional entity representing the instantiated prefab.
   */
		[[nodiscard]] std::optional<Entity> InstantiatePrefab(Registry& registry, const std::string& prefabName);

		/**
   * \brief Creates and registers a prefab with the given name and constructor arguments.
   * \tparam Args The types of the constructor arguments.
   * \param name The name of the prefab.
   * \param args The constructor arguments.
   */
		template <typename... Args>
		void CreateAndRegisterPrefab(const std::string& name, Args&&... args);

		/**
   * \brief Retrieves the map of registered prefabs.
   * \return A constant reference to the map of registered prefabs.
   */
		const std::unordered_map<std::string, std::unique_ptr<Prefab>>& GetPrefabs() const noexcept;
		
		/**
   * \brief Deletes a prefab with the given name.
   * \param name The name of the prefab to delete.
   * \return True if the prefab was successfully deleted, false otherwise.
   */
		bool DeletePrefab(const std::string& name);

		/**
   * \brief Deletes the file associated with the given prefab path.
   * \param prefabPath The path of the prefab file to delete.
   * \return True if the file was successfully deleted, false otherwise.
   */
		bool DeletePrefabFile(const std::string& prefabPath);

		/**
   * \brief Retrieves the file path associated with the given prefab name.
   * \param name The name of the prefab.
   * \return The file path associated with the prefab.
   */
		std::string GetPrefabFilePath(const std::string& name) const;

		/**
   * \brief Updates a prefab with the given name and updated prefab.
   * \param name The name of the prefab to update.
   * \param updatedPrefab The updated prefab.
   * \param componentType The type of the component to update.
   */
		void UpdatePrefab(const std::string& name, std::unique_ptr<Prefab> updatedPrefab, const std::type_index& componentType);

        bool PrefabExists(const std::string& name) const;

	private:
		EventBus& eventBus;
		// Map to store prefabs
		std::unordered_map<std::string, std::unique_ptr<Prefab>> prefabs;

		// Map to store prefab file paths
		std::unordered_map<std::string, std::string> prefabPaths;
	};
}