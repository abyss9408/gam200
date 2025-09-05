/******************************************************************************/
/*!
\file   PrefabManager.cpp
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/06
\brief
	This source file implements the PrefabManager class within the Popplio namespace. 
	The PrefabManager class is responsible for managing prefabs, including registering, 
	retrieving, instantiating, updating, and deleting prefabs. It also handles the 
	association between prefabs and their file paths.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>

#include "PrefabManager.h"
#include "PrefabEvent.h"

#include <filesystem>

namespace Popplio
{
	PrefabManager::PrefabManager(EventBus& eventBus)
		: eventBus(eventBus)
	{

	}

	void PrefabManager::RegisterPrefab(const std::string& name, std::unique_ptr<Prefab> prefab, const std::string& filePath)
	{
		prefabs.insert_or_assign(name, std::move(prefab));
		if (!filePath.empty())
		{
			prefabPaths[name] = filePath;
		}
	}

	
	[[nodiscard]] std::optional<std::reference_wrapper<const Prefab>> PrefabManager::GetPrefab(const std::string& name) const
	{
		auto it = prefabs.find(name);
		if (it != prefabs.end())
		{
			return std::cref(*it->second);
		}

		return std::nullopt;
	}

	[[nodiscard]] std::optional<Entity> PrefabManager::InstantiatePrefab(Registry& registry, const std::string& prefabName)
	{
		if (auto prefabOpt = GetPrefab(prefabName))
		{
			return prefabOpt->get().Instantiate(registry, *this);
		}

		return std::nullopt;
	}

	template <typename... Args>
	void PrefabManager::CreateAndRegisterPrefab(const std::string& name, Args&&... args)
	{
		auto prefab = std::make_unique<Prefab>(name);
		prefab->AddComponent<Args>(std::forward<Args>(args)...);
		RegisterPrefab(name, std::move(prefab));
	}

	const std::unordered_map<std::string, std::unique_ptr<Prefab>>& PrefabManager::GetPrefabs() const noexcept
	{
		return prefabs;
	}

	bool PrefabManager::DeletePrefab(const std::string& name)
	{
		auto it = prefabs.find(name);
		if(it == prefabs.end())
		{
			Logger::Warning("Attempted to delete non-existent prefab: " + name, LogDestination::IMGUI_CONSOLE);
			return false;
		}

		// Get the file path before removing from map
		std::string filePath = GetPrefabFilePath(name);

		// Notify listeners such as PrefabSyncSystem
		eventBus.EmitEvent<PrefabEvent>(name, PrefabEventType::Deleted);

		// Remove from memory
		prefabs.erase(it);
		prefabPaths.erase(name);

		// Delete the file if it exists
		if (!filePath.empty())
		{
			return DeletePrefabFile(filePath);
		}

		Logger::Info("Deleted prefab from manager: " + name, LogDestination::IMGUI_CONSOLE);

		return true;
	}

	bool PrefabManager::DeletePrefabFile(const std::string& prefabPath)
	{
		try
		{
			if (std::filesystem::exists(prefabPath))
			{
				std::filesystem::remove(prefabPath);
				Logger::Info("Deleted prefab file: " + prefabPath, LogDestination::IMGUI_CONSOLE);
				return true;
			}
			return false;
		}
		catch (const std::exception& e)
		{
			Logger::Error("Failed to delete prefab file: " + prefabPath + " Error: " + e.what(), LogDestination::IMGUI_CONSOLE);
			return false;
		}
	}

	std::string PrefabManager::GetPrefabFilePath(const std::string& name) const
	{
		auto it = prefabPaths.find(name);
		if (it != prefabPaths.end())
		{
			return it->second;
		}

		return "";
	}

	void PrefabManager::UpdatePrefab(const std::string& name, std::unique_ptr<Prefab> updatedPrefab, const std::type_index& componentType)
	{
		// Normal update without rename
		prefabs[name] = std::move(updatedPrefab);
		eventBus.EmitEvent<PrefabEvent>(name, PrefabEventType::Modified, componentType);
		Logger::Debug("Updated prefab: " + name, LogDestination::IMGUI_CONSOLE);
	}

	bool PrefabManager::PrefabExists(const std::string& name) const
	{
		auto it = prefabPaths.find(name);
		if (it != prefabPaths.end())
		{
			return true;
		}
		return false;
	}
}