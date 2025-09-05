/******************************************************************************/
/*!
\file   ScriptDirectoryGUI.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        Source file for implementing the Editor ImGui Script Directory GUI

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>

#include "Editor.h"

#include "../Logging/Logger.h"
#include "../Utilities/FileDialog.h"

//#define CSHARP_PROJECT_PATH "\\Project"				// Release executable of engine without VS for the engine
#define CSHARP_PROJECT_PATH "\\..\\..\\Team_Popplio_Script" // Engine with VS

namespace Popplio
{
	namespace ScriptDir // avoid clash with AssetStoreGUI.cpp
	{
		static std::filesystem::path basePath{ std::filesystem::current_path().string() + CSHARP_PROJECT_PATH };

		static std::string projPath
		{ basePath.string().replace(basePath.string().find_last_of("\\") + 1,
			basePath.string().size() - basePath.string().find_last_of("\\") - 1, "")};

		static std::filesystem::path currentPath;
		static std::vector<std::filesystem::path> directories;
		static std::vector<std::filesystem::path> files;
		static std::string message{};

		std::filesystem::path selectedFile;
		//char pathBuffer[256] = { 0 }; // Buffer for manual path input
	}

	void RefreshDirectory() 
	{
		ScriptDir::directories.clear();
		ScriptDir::files.clear();

		if (!std::filesystem::exists(ScriptDir::currentPath)) ScriptDir::currentPath = ScriptDir::basePath;

		try 
		{
			for (const auto& entry : std::filesystem::directory_iterator(ScriptDir::currentPath))
			{
				if (entry.is_directory()) ScriptDir::directories.push_back(entry.path());
				else if (entry.is_regular_file()) ScriptDir::files.push_back(entry.path());
			}

			// sort directories
			std::sort(ScriptDir::directories.begin(), ScriptDir::directories.end());
			std::sort(ScriptDir::files.begin(), ScriptDir::files.end());
		}
		catch (const std::filesystem::filesystem_error& e) // error
		{
			std::stringstream ss{ "" };
			ss << "Error reading directory: " << e.what();
			Logger::Error(ss.str());
		}
	}

	void Editor::RenderScriptDirectoryGUI()
	{
		ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
		ImVec2 buttonSize = ImVec2(100, 50);

		RefreshDirectory();

		static bool goNextFolder = false;
		if (ImGui::Begin(ICON_FA_CODE " C# Project Scripts"))
		{
			if (ScriptDir::currentPath != ScriptDir::basePath)
			{
				// Go back button
				if (ImGui::Button("Back")) 
				{
					ScriptDir::currentPath = ScriptDir::currentPath.parent_path();
					RefreshDirectory();
					ScriptDir::message = "";
					//strcpy_s(pathBuffer, currentPath.string().c_str());
				}
			}
			// Path input and navigation
			//ImGui::InputText("Current Path", pathBuffer, IM_ARRAYSIZE(pathBuffer));
			ImGui::SameLine();

			if (ImGui::SmallButton("Reload"))
			{
				AssetLoader::ReloadAssets();
				ScriptDir::message = "";
			}

			ImGui::SameLine();
			ImGui::Text(ScriptDir::currentPath.string().replace(0, ScriptDir::projPath.size(), " | ").c_str());

			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.7f, 0.1f, 1.0f));
			if (ImGui::SmallButton("+ Existing Script"))
			{
				std::string filetobesaved{};
				auto orignalCurrPath = ScriptDir::basePath;
				filetobesaved = FileDialog::OpenFile("C# Files (*.cs)\0*.cs\0\0", "cs");

				std::filesystem::path newFile{ filetobesaved };
				std::filesystem::path fileSavingTo(ScriptDir::currentPath.string() + "\\" + newFile.filename().string());

				// copy the file 
				try
				{
					std::filesystem::copy_file(filetobesaved, fileSavingTo);

				}
				catch (std::filesystem::filesystem_error& e)
				{
					Logger::Error("Could not copy script: " + std::string(e.what()));
				}
				// go back to the original directory
				std::filesystem::current_path(orignalCurrPath);
			}
			if (ImGui::IsItemHovered()) ImGui::SetTooltip("Add new script into current directory");

			ImGui::PopStyleColor();

			// Add new script?

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.1f, 0.1f, 1.0f));
			if (ImGui::SmallButton("Delete Script"))
			{
				if (ScriptDir::selectedFile.empty())
					ScriptDir::message = "No script selected for deletion!";
				else
				{
					// only delete scripts
					size_t pos = ScriptDir::selectedFile.string().find(".cs");
					if (pos != std::string::npos && pos == ScriptDir::selectedFile.string().size() - 3)
						std::filesystem::remove(ScriptDir::selectedFile);
					else
					{
						ScriptDir::message = "DON'T DELETE ME!!!!";
						ScriptDir::selectedFile = std::filesystem::path();
					}
				}

			}
			if (ImGui::IsItemHovered()) ImGui::SetTooltip("Delete a script from current directory");

			ImGui::PopStyleColor();

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(240, 0, 0, 255));
			ImGui::Text(ScriptDir::message.c_str());
			ImGui::PopStyleColor();

			// showing of files
			if (ImGui::BeginTable("CustomTable", 1, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInner)) {
				// Iterate through all items
				for (const auto& dir : ScriptDir::directories) {
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);

					// Push a custom color for selectable rows
					ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.4f, 0.8f, 0.6f));
					ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.2f, 0.5f, 0.9f, 0.7f));

					// Create selectable with state tracking
					if (ImGui::Selectable(dir.filename().string().c_str(), ImGuiSelectableFlags_None))
					{
						ScriptDir::currentPath = dir;
						goNextFolder = true;
					}

					ImGui::PopStyleColor(2);
				}

				for (const auto& file : ScriptDir::files)
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					// Push a distinct color for non-selectable rows

					if (ScriptDir::selectedFile == file)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(240.0f / 255.f, 140.0f / 255.f, 230.0f / 255.f, 1.0f));
					}
					else
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					}

					if (ImGui::Selectable(file.filename().string().c_str(), ImGuiSelectableFlags_None))
					{
						if (ScriptDir::selectedFile != file)
						{
							ScriptDir::selectedFile = file;
							ScriptDir::message = "";
						}
						else
						{
							// assign to empty path
							ScriptDir::selectedFile = std::filesystem::path();
						}
					}

					ImGui::PopStyleColor();
				}
			}

			ImGui::EndTable();

		}
		ImGui::End();
		if (goNextFolder)
		{
			ScriptDir::message = "";
			RefreshDirectory();
			//strcpy_s(pathBuffer, currentPath.string().c_str());
			goNextFolder = false;
			ScriptDir::selectedFile = std::filesystem::path();
		}
	}

}