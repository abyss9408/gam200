/******************************************************************************/
/*!
\file		AssetStoreGUI.cpp
\author		Team Popplio
\author		Val Tay Yun Ying (100%)
\co-author
\par		Course : CSD2400 / CSD2401
\par		Section : A
\date		2024/11/30
\brief
		This source file renders the asset store to a window within the editor

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include "Editor.h"
#include "../AssetStore/AssetStore.h"
#include "../AssetStore/AssetLoader.h"
#include "../Utilities/FileDialog.h"

namespace Popplio
{
	static std::filesystem::path currentPath;
	static std::vector<std::filesystem::path> directories;
	static std::vector<std::filesystem::path> files;
	static std::string message{};
	static bool goNextFolder = false;
	std::filesystem::path selectedFile;

	// Constants for UI layout
	constexpr float BUTTON_SIZE = 80.0f;
	constexpr float PADDING = 10.0f;

	// Helper function to get icon based on file type
	static const char* GetFileTypeIcon(const std::string& extension) {
		if (extension == ".wav")
			return ICON_FA_MUSIC;
		if (extension == ".vert" || extension == ".frag")
			return ICON_FA_CODE;
		if (extension == ".ttf")
			return ICON_FA_FONT;
		if (extension == ".cpp")
			return ICON_FA_FILE_CODE;
		if (extension == ".h")
			return ICON_FA_H;
		if (extension == AssetLoader::_metaExtension)
			return ICON_FA_GEAR;
		if (extension == ".png")
			return ICON_FA_IMAGE;
		return ICON_FA_FILE;
	}

	void RefreshDirectoryContents() {
		directories.clear();
		files.clear();

		if (!std::filesystem::exists(currentPath))
		{
			currentPath = std::filesystem::current_path();
		}

		try {
			for (const auto& entry : std::filesystem::directory_iterator(currentPath)) {

				if (entry.is_directory()) {
					directories.push_back(entry.path());
				}
				else if (entry.is_regular_file()) {
					files.push_back(entry.path());
				}
			}

			// Sort directories and files alphabetically
			std::sort(directories.begin(), directories.end());
			std::sort(files.begin(), files.end());
		}
		catch (const std::filesystem::filesystem_error& e) {
			// Handle potential errors (e.g., permission issues)
			std::cerr << "Error reading directory: " << e.what() << std::endl;
		}
	}

	void RenderItemButton(const std::filesystem::path& path, bool isDirectory, bool isSelected, float buttonCenterX) {
		// Prepare for item rendering
		ImGui::PushID(path.string().c_str());

		// Set up styling based on type and selection
		if (isDirectory) {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
		}
		else if (isSelected) {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.8f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.5f, 0.9f, 0.5f));
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
		}

		// Center the button
		ImGui::SetCursorPosX(buttonCenterX);

		// Render the appropriate button type
		if (isDirectory) {
			if (ImGui::Button(ICON_FA_FOLDER, ImVec2(BUTTON_SIZE, BUTTON_SIZE))) {
				currentPath = path;
				goNextFolder = true;
				message = "";
			}
		}
		else {
			bool isTexture = (path.extension() == ".png");
			if (isTexture) {
				std::string assetName = Serializer::GenerateAssetName(path.string());
				unsigned int textureID = AssetStore::GetTexture(assetName);
				if (ImGui::ImageButton(
					(std::string("##") + path.filename().string()).c_str(),
					(void*)(intptr_t)textureID,
					ImVec2(BUTTON_SIZE, BUTTON_SIZE),
					ImVec2(0, 1),
					ImVec2(1, 0))) {
					selectedFile = (selectedFile != path) ? path : std::filesystem::path();
				}
			}
			else {
				if (ImGui::Button(GetFileTypeIcon(path.extension().string()), ImVec2(BUTTON_SIZE, BUTTON_SIZE))) {
					selectedFile = (selectedFile != path) ? path : std::filesystem::path();
				}
			}
		}

		// Render filename with wrapping
		std::string filename = path.filename().string();
		float textWidth = ImGui::CalcTextSize(filename.c_str()).x;
		float textStartX = ImGui::GetCursorPosX();

		if (textWidth > BUTTON_SIZE) {
			textStartX = ImGui::GetCursorPosX();
		}

		ImGui::SetCursorPosX(textStartX);
		ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + BUTTON_SIZE);
		ImGui::TextWrapped("%s", filename.c_str());
		ImGui::PopTextWrapPos();

		// Clean up styling
		ImGui::PopStyleColor(2);
		ImGui::PopID();
	}

	void Editor::RenderAssetStoreGUI() {
		ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

		RefreshDirectoryContents();

		// Ensure directory contents are refreshed before rendering
		if (goNextFolder) {
			RefreshDirectoryContents();
			message = "";
			selectedFile = std::filesystem::path();
			goNextFolder = false;
		}

		if (ImGui::Begin(ICON_FA_FOLDER " Asset Browser")) {
			HandleAssetDragDrop();

			// Toolbar
			if (currentPath != std::filesystem::current_path()) {
				if (ImGui::Button(ICON_FA_ARROW_LEFT " Back", ImVec2(80, 25))) {
					currentPath = currentPath.parent_path();
					RefreshDirectoryContents();
					message = "";
					selectedFile = std::filesystem::path();
				}
				ImGui::SameLine();
			}

			if (ImGui::Button(ICON_FA_ROTATE " Reload", ImVec2(80, 25))) {
				AssetLoader::ReloadAssets();
				message = "";
			}

			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.7f, 0.1f, 1.0f));
			if (ImGui::Button(ICON_FA_CIRCLE_PLUS " New", ImVec2(80, 25))) {
				std::string filetobesaved{};
				if (currentPath.filename().string() == "Audio")
				{
					auto orignalCurrPath = std::filesystem::current_path();
					filetobesaved = FileDialog::OpenFile("Wav Audio Files (*.wav)\0*.wav\0All Files\0*.*\0\0", "wav");

					std::filesystem::path newFile{ filetobesaved };
					std::filesystem::path fileSavingTo(currentPath.string() + "\\" + newFile.filename().string());

					// copy the file 
					try
					{
						std::filesystem::copy_file(filetobesaved, fileSavingTo);

					}
					catch (std::filesystem::filesystem_error& e)
					{
						Logger::Error("Could not copy file: " + std::string(e.what()));
					}
					// go back to the original directory
					std::filesystem::current_path(orignalCurrPath);

				}
				else if (currentPath.filename().string() == "Textures")
				{
					auto orignalCurrPath = std::filesystem::current_path();
					filetobesaved = FileDialog::OpenFile("Image Files (*.png)\0*.png\0All Files\0*.*\0\0", "png");

					std::filesystem::path newFile{ filetobesaved };
					std::filesystem::path fileSavingTo(currentPath.string() + "\\" + newFile.filename().string());

					// copy the file 
					try
					{
						std::filesystem::copy_file(filetobesaved, fileSavingTo);

					}
					catch (std::filesystem::filesystem_error& e)
					{
						Logger::Error("Could not copy file: " + std::string(e.what()));
					}
					// go back to the original directory
					std::filesystem::current_path(orignalCurrPath);

				}
				else
				{
					message = "Cannot add new asset here!";
				}
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Add new asset into current directory");
			}
			ImGui::PopStyleColor();

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.1f, 0.1f, 1.0f));
			if (ImGui::Button(ICON_FA_TRASH " Delete", ImVec2(80, 25))) {
				if (selectedFile.empty()) {
					message = "No file selected for deletion!";
				}
				else if (AssetLoader::IsAsset(selectedFile.string()) ||
					selectedFile.extension().string() == AssetLoader::_metaExtension) {
					std::filesystem::remove(selectedFile);
					selectedFile = std::filesystem::path();
				}
				else {
					message = "Cannot delete this file type!";
				}
			}

			// Current path display
			ImGui::SameLine();
			ImGui::Text(ICON_FA_FOLDER_OPEN " %s", currentPath.string().c_str());

			// Error/status message
			if (!message.empty()) {
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
				ImGui::TextWrapped("%s", message.c_str());
				ImGui::PopStyleColor();
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Delete an asset from current directory");
			}
			ImGui::PopStyleColor();

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(240, 0, 0, 255));
			ImGui::Text(message.c_str());
			ImGui::PopStyleColor();

			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
			if (ImGui::Button(ICON_FA_ERASER " Total Wipe", ImVec2(100, 25)))
			{
				AssetLoader::TotalWipe();
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("WARNING!!! By clicking this, you agree to nuke ALL meta files!!! ALL YOUR SETTINGS MAY BE GONE!!! BE WARNED!!!");
			}
			ImGui::PopStyleColor();

			// Calculate grid layout
			int buttonsPerRow = 15;
			if (buttonsPerRow < 1) buttonsPerRow = 1;

			if (ImGui::BeginTable("##AssetTable", 1, ImGuiTableFlags_None)) {
				int currentColumn = 0;

				// Render directories
				for (const auto& dir : directories) {
					if (currentColumn == 0) {
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
					}

					if (currentColumn > 0) {
						ImGui::SameLine(0, PADDING);
					}

					ImGui::BeginGroup();
					float buttonCenterX = ImGui::GetCursorPosX();
					RenderItemButton(dir, true, false, buttonCenterX);
					ImGui::EndGroup();

					if (++currentColumn >= buttonsPerRow) {
						currentColumn = 0;
					}
				}

				// Render files
				for (const auto& file : files) {

					if (file.extension() == ".cyndaquil")
					{
						continue;
					}

					if (currentColumn == 0) {
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
					}

					if (currentColumn > 0) {
						ImGui::SameLine(0, PADDING);
					}

					ImGui::BeginGroup();
					float buttonCenterX = ImGui::GetCursorPosX();
					RenderItemButton(file, false, selectedFile == file, buttonCenterX);
					ImGui::EndGroup();

					if (++currentColumn >= buttonsPerRow) {
						currentColumn = 0;
					}
				}

				ImGui::EndTable();
			}
		}
		ImGui::End();
	}
}