/******************************************************************************/
/*!
\file   SceneLoadSave.cpp
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/05
\brief
	This file defines several methods within the Editor class in the Popplio namespace. 
	These methods are responsible for managing and rendering the scene management 
	functionalities, such as creating, loading, saving, and handling scenes using 
	the ImGui library.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>
#include "Editor.h"
#include "../Utilities/FileDialog.h"
#include "EntitySelectionEvents.h"

namespace Popplio
{
	void Editor::SaveSceneMenu()
	{
		if (currentScenePath.empty())
		{
			std::string path = FileDialog::SaveFile("Scene Files\0*.scene\0", "scene");
			if (!path.empty())
			{
				SaveScene(path);
				SetLastSavedChange();
			}
		}
		else
		{
			SaveScene(currentScenePath);
			SetLastSavedChange();
		}
	}

	void Editor::SaveSceneAsMenu()
	{
		std::string path = FileDialog::SaveFile("Scene Files\0*.scene\0", "scene");
		if (!path.empty())
		{
			SaveScene(path);
			SetLastSavedChange();
		}
	}

	void Editor::RenderSceneMenu()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene"))
			{
				HandleNewScene();
			}

			if (ImGui::MenuItem("Save Scene"))
			{
				SaveSceneMenu();
			}

			if (ImGui::MenuItem("Save Scene As..."))
			{
				SaveSceneAsMenu();
			}

			if (ImGui::MenuItem("Load Scene"))
			{
				std::string path = FileDialog::OpenFile("Scene Files\0*.scene\0", "scene");
				if (!path.empty())
				{
					LoadScene(path);
				}
			}

			if (ImGui::MenuItem("Reload Scene"))
			{
                LoadScene(currentScenePath);
			}

			ImGui::EndMenu();
		}
	}

	void Editor::HandleNewScene()
	{
		// Ask for confirmation if there are unsaved changes
		if (hasUnsavedChanges)
		{
			ImGui::OpenPopup("New Scene?");
		}
		else
		{
			CreateNewScene();
		}

		if (ImGui::BeginPopupModal("New Scene?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("There are unsaved changes. Are you sure you want to create a new scene?");
			ImGui::Separator();

			if (ImGui::Button("Yes", ImVec2(120, 0)))
			{
				CreateNewScene();
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("No", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}

	void Editor::SaveScene(const std::string& path)
	{
		if (serializer.SaveSceneToFile(path, cameraManager))
		{
			//currentScenePath = path;
			hasUnsavedChanges = false;
			Logger::Info("Scene saved successfully: " + path);
		}
		else
		{
			Logger::Error("Failed to save scene: " + path);
		}
	}

	void Editor::LoadScene(const std::string& path)
	{
		if (serializer.LoadSceneFromFile(path, cameraManager))
		{
			currentScenePath = path;
			hasUnsavedChanges = false;
			selectedEntity.reset();
			eventBus.EmitEvent<EntityDeselectedEvent>();
			mono.ReloadAssembly(); 
			Logger::Info("Scene loaded successfully: " + path);
		}
		else
		{
			Logger::Error("Failed to load scene: " + path);
		}
	}

	void Editor::CreateNewScene()
	{
		// Clear all entities
		auto allEntities = reg.GetAllEntities();
		for (auto entity : allEntities)
		{
			reg.KillEntity(entity);
		}

		reg.Update();

		layerManager.Clear();

		currentScenePath.clear();
		hasUnsavedChanges = false;
		selectedEntity.reset();
		eventBus.EmitEvent<EntityDeselectedEvent>();
		Logger::Info("Created new scene");
	}

	std::string Editor::GetScenesDirectory()
	{
		std::filesystem::path scenesPath = "Assets/Scenes";
		if (!std::filesystem::exists(scenesPath))
		{
			std::filesystem::create_directories(scenesPath);
		}
		return scenesPath.string();
	}

	std::string Editor::GetCurrentSceneName()
	{
        return currentScenePath.substr(currentScenePath.find_last_of("/\\") + 1);
	}
}