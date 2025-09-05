/******************************************************************************/
/*!
\file   AssetsDragAndDrop.cpp
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/06
\brief
    This source file defines the methods for handling asset drag and drop functionality within the Popplio namespace.
    It includes functionality for processing dragged assets, handling drag and drop events, and validating asset types.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include "Editor.h"

namespace Popplio
{
    void Editor::drop_callback(GLFWwindow* window, int count, const char** paths)
    {
        Editor* editor = static_cast<Editor*>(glfwGetWindowUserPointer(window));
        if (editor)
        {
            for (int i = 0; i < count; i++)
            {
                editor->ProcessDraggedAsset(paths[i]);
            }
        }
    }

    void Editor::HandleAssetDragDrop()
    {
        if (!ImGui::GetIO().WantCaptureMouse) return;

        // Handle drag and drop from Windows Explorer
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("EXTERNAL_FILE"))
            {
                // Get the file path from the payload
                const wchar_t* path = (const wchar_t*)payload->Data;
                std::filesystem::path sourcePath(path);

                ProcessDraggedAsset(sourcePath);
            }
            ImGui::EndDragDropTarget();
        }
    }

    void Editor::ProcessDraggedAsset(const std::filesystem::path& sourcePath)
    {
        std::string extension = sourcePath.extension().string();
        if (!IsValidAssetType(extension))
        {
            Logger::Warning("Unsupported file type: " + extension);
            return;
        }

        // Determine target directory based on file type
        std::string targetDir;
        if (extension == ".wav")
        {
            targetDir = "Assets/Audio/";
        }
        else if (extension == ".png")
        {
            targetDir = "Assets/Textures/";
        }
        else
        {
            return;
        }

        // Create target directory if it doesn't exist
        std::filesystem::create_directories(targetDir);

        // Generate target path
        std::filesystem::path targetPath = targetDir + sourcePath.filename().string();

        try
        {
            // Copy file to assets directory
            std::filesystem::copy_file(sourcePath, targetPath,
                std::filesystem::copy_options::overwrite_existing);

            // Generate meta file
            Serializer::SerializeMetaFile(targetPath);

            // Load the asset
            Serializer::DeserializeMetaFile(targetPath);

            Logger::Info("Successfully imported asset: " + sourcePath.filename().string());
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            Logger::Error("Failed to import asset: " + std::string(e.what()));
        }
    }

    bool Editor::IsValidAssetType(const std::string& extension)
    {
        return extension == ".wav" || extension == ".png";
    }
}