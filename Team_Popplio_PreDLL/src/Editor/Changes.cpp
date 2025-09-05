/******************************************************************************/
/*!
\file   Changes.cpp
\author Team Popplio
\author			Ng Jun Heng Shawn
\contribution	Shawn - 100%
\par    Course : CSD2401
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the source file for change detection in the editor
        Detects changes in entities and components during editor mode
        Implements change detection for undo and redo functionality

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>

#include "Changes.h"

#include "Editor.h"

#include "../Logging/Logger.h"

namespace Popplio
{
    void Editor::HandleSceneChange(SceneChangeEvent<std::any>& event)
    {
        undoStack.push_back(event);
        Logger::Debug("Scene change detected");
        if (undoStack.size() > CHANGE_STACK_SIZE)
        {
            Logger::Debug("Undo stack limit reached, removing oldest entry...");
            undoStack.pop_front();
        }

        // Clear the redo stack
        redoStack.clear();
        Logger::Debug("Redo stack flushed");

        hasUnsavedChanges = true;
    }

    void Editor::SetLastSavedChange()
    {
        if (!undoStack.empty())
        {
            lastSavedChange = &undoStack.back();
        }

        hasUnsavedChanges = false;
    }

    void Editor::UnsavedPopup()
    {
        if (!hasUnsavedChanges) return;

        while (hasUnsavedChanges) // loop
        {
            Update();
            Render();

            ImGui::OpenPopup("Unsaved Changes");
            if (ImGui::BeginPopupModal("Unsaved Changes", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("You have unsaved changes. Do you want to save before exiting?");
                ImGui::Separator();
                if (ImGui::Button("Save"))
                {
                    SaveSceneMenu();
                    ImGui::CloseCurrentPopup();
                    hasUnsavedChanges = false;
                }
                ImGui::SameLine();
                if (ImGui::Button("Don't Save"))
                {
                    ImGui::CloseCurrentPopup();
                    hasUnsavedChanges = false;
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel"))
                {
                    ImGui::CloseCurrentPopup();
                    // Do something to get out of this loop
                }
                ImGui::EndPopup();
            }
        }
    }
}