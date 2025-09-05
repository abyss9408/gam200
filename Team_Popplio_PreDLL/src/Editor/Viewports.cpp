/******************************************************************************/
/*!
\file   GameViewport.cpp
\author Team Popplio
\author Bryan Ang Wei Ze
\co-author Pearl Goh | Ng Jun Heng Shawn
\contribution Bryan - 20% | Pearl Goh - 60% | Shawn - 20%
\par    Course : CSD2401
\par    Section : A
\date   2025/02/06
\brief
    This source file defines methods for handling the game viewport within the 
    Editor class in the Popplio namespace. The methods allow for resizing the 
    viewport and rendering the game view using the ImGui library.
    It also renders a toolbar for imguizmo transformations.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>

#include "Editor.h"

#include "../Graphic/RenderSystem.h"
#include "font awesome/IconsFontAwesome6.h"

#include <ImGuizmo/ImGuizmo.h>

namespace Popplio
{
    void Editor::RenderSimulationControls()
    {
        ImGui::Begin("Simulation Controls");

        // Get the width of the window
        float windowWidth = ImGui::GetWindowWidth();

        // Calculate total width needed for all buttons
        float buttonWidth = 60.0f;  // Width of each button
        float spacing = 10.0f;      // Spacing between buttons
        float totalWidth = (buttonWidth * 3) + (spacing * 2);  // 3 buttons, 2 spaces between them

        // Calculate starting X position to center the buttons
        float startX = (windowWidth - totalWidth) * 0.5f;

        // Set the cursor position to start drawing buttons
        ImGui::SetCursorPosX(startX);

        // Simulation
        if (ImGui::Button(ICON_FA_PLAY))
        {
            if (editorState == State::EDITING) {
                m_lastFrameTime = ImGui::GetTime(); // Initialize timing on play
                m_playheadPosition = 0.0; // Start from beginning
                StartScene();
            }
            else if (editorState == State::PAUSED) 
            {
                m_lastFrameTime = ImGui::GetTime(); // Reset timing on resume
                ResumeScene();
            }
        }

        ImGui::SameLine();

        if (ImGui::Button(ICON_FA_PAUSE))
        {
            if (editorState == State::PLAYING) PauseScene();
        }

        ImGui::SameLine();

        if (ImGui::Button(ICON_FA_STOP))
        {
            if (editorState == State::PAUSED) ResumeScene();
            if (editorState == State::PLAYING || editorState == State::PAUSED) EndScene();
            m_playheadPosition = 0.0;
            m_lastFrameTime = ImGui::GetTime();
        }

        ImGui::SameLine();

        if (editorState == State::EDITING)
        {
            bool enableEditor{ isEditorEnabled };
            if (ImGui::Checkbox("Run within editor", &enableEditor))
                isEditorEnabled = enableEditor;
        }

		ImGui::End();
    }

    void Editor::RenderToolButton(const char* icon, const char* tooltip, GuizmoTool toolType)
    {
        bool isSelected = currentTool == toolType;

        // Style the button
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(BUTTON_SPACING, BUTTON_SPACING));
        ImGui::PushStyleColor(ImGuiCol_Button, 
            isSelected ? ImVec4(0.2f, 0.4f, 0.8f, 1.0f) : ImVec4(0.2f, 0.2f, 0.2f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 
            isSelected ? ImVec4(0.3f, 0.5f, 0.9f, 1.0f) : ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,
            ImVec4(0.15f, 0.35f, 0.75f, 1.0f));

        // Create the button
        if (ImGui::Button(icon, ImVec2(BUTTON_SIZE, BUTTON_SIZE))) {
            currentTool = toolType;
        }

        // Show tooltip on hover
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("%s", tooltip);
        }

        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar(2);
    }

    void Editor::RenderToolBar()
    {
        ImGui::Begin(ICON_FA_TOOLBOX " Gizmo Tools");

        // Get the width of the window
        float windowWidth = ImGui::GetWindowWidth();

        // Calculate total width needed for all buttons
        float buttonWidth = 40.0f;  // Width of each button
        float spacing = 10.0f;      // Spacing between buttons
        float totalWidth = (buttonWidth * 4) + (spacing * 3);  // 4 buttons, 3 spaces between them

        // Calculate starting X position to center the buttons
        float startX = (windowWidth - totalWidth) * 0.5f;

        // Set the cursor position to start drawing buttons
        ImGui::SetCursorPosX(startX);

        RenderToolButton("T", "Translate (W)", GuizmoTool::Translate);
        ImGui::SameLine();
        RenderToolButton("R", "Rotate (E)", GuizmoTool::Rotate);
        ImGui::SameLine();
        RenderToolButton("S", "Scale (R)", GuizmoTool::Scale);
        ImGui::SameLine();
        RenderToolButton("NT", "No Tool (T)", GuizmoTool::None);

        ImGui::End();
    }

    void Editor::RenderSceneViewport()
    {
        ImGui::Begin(ICON_FA_TABLE_CELLS " Scene", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        // Store viewport data
        isViewportHovered = ImGui::IsWindowHovered();

        // Get the desired aspect ratio (e.g., 16:9)
        const float targetAspectRatio = 16.0f / 9.0f;

        // Get the size of the ImGui window content area
        ImVec2 contentSize = ImGui::GetContentRegionAvail();

        // Calculate viewport size while maintaining aspect ratio
        float viewportWidth = contentSize.x;
        float viewportHeight = contentSize.y;
        float currentAspectRatio = viewportWidth / viewportHeight;

        if (currentAspectRatio > targetAspectRatio)
        {
            // Window is too wide, use height as constraint
            viewportWidth = viewportHeight * targetAspectRatio;
        }
        else
        {
            // Window is too tall, use width as constraint
            viewportHeight = viewportWidth / targetAspectRatio;
        }

        // Calculate padding to center the viewport
        float paddingX = (contentSize.x - viewportWidth) * 0.5f;
        float paddingY = (contentSize.y - viewportHeight) * 0.5f;

        // Set cursor position to apply padding
        ImVec2 cursorPos = ImGui::GetCursorPos();
        ImGui::SetCursorPos(ImVec2(cursorPos.x + paddingX, cursorPos.y + paddingY));
        
        // Handle viewport resizing
        if (static_cast<int>(viewportWidth) != sceneViewportWidth ||
            static_cast<int>(viewportHeight) != sceneViewportHeight)
        {
            sceneViewportWidth = static_cast<int>(viewportWidth);
            sceneViewportHeight = static_cast<int>(viewportHeight);
            ResizeFramebufferTexture(sceneRenderTexture, sceneViewportWidth, sceneViewportHeight);
        }

        // Render to scene framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, sceneFramebuffer);
        glViewport(0, 0, sceneViewportWidth, sceneViewportHeight);

        // Clear and render with editor camera
        cameraManager.SetActiveCamera(true);
        reg.GetSystem<RenderSystem>().Update();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Display the texture
        ImGui::Image((void*)(intptr_t)sceneRenderTexture, ImVec2(viewportWidth, viewportHeight), ImVec2(0, 1), ImVec2(1, 0));

        // Handle entity selection and manipulation
        viewportBounds[0] = ImGui::GetItemRectMin();
        viewportBounds[1] = ImGui::GetItemRectMax();

        if (showGameCameraFrustum)
        {
            DrawGameCameraFrustum();
        }

        if (selectedEntity)
        {
            RenderSelectedEntity();
            HandleGuizmo();
        }

        ImGui::End();
    }

	void Editor::RenderGameViewport()
	{
		ImGui::Begin(ICON_FA_GAMEPAD " Game", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        isGameViewportHovered = ImGui::IsWindowHovered();
        
        // Get the size of the ImGui window content area
        float targetAspectRatio = 16.0f / 9.0f;
        ImVec2 contentSize = ImGui::GetContentRegionAvail();

        float viewportWidth = contentSize.x;
        float viewportHeight = contentSize.y;
        float currentAspectRatio = viewportWidth / viewportHeight;

        if (currentAspectRatio > targetAspectRatio)
        {
            viewportWidth = viewportHeight * targetAspectRatio;
        }
        else
        {
            viewportHeight = viewportWidth / targetAspectRatio;
        }

        float paddingX = (contentSize.x - viewportWidth) * 0.5f;
        float paddingY = (contentSize.y - viewportHeight) * 0.5f;

        ImVec2 cursorPos = ImGui::GetCursorPos();
        ImGui::SetCursorPos(ImVec2(cursorPos.x + paddingX, cursorPos.y + paddingY));

        // Handle viewport resizing
        if (static_cast<int>(viewportWidth) != gameViewportWidth ||
            static_cast<int>(viewportHeight) != gameViewportHeight)
        {
            gameViewportWidth = static_cast<int>(viewportWidth);
            gameViewportHeight = static_cast<int>(viewportHeight);
            ResizeFramebufferTexture(gameRenderTexture, gameViewportWidth, gameViewportHeight);
        }

        // Render to game framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, gameFramebuffer);
        glViewport(0, 0, gameViewportWidth, gameViewportHeight);

        cameraManager.SetActiveCamera(false);
        reg.GetSystem<RenderSystem>().Update();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Clear and render with game camera
        // Display the texture
        ImGui::Image((void*)(intptr_t)gameRenderTexture, ImVec2(viewportWidth, viewportHeight), ImVec2(0, 1), ImVec2(1, 0));

        viewportBoundsGame[0] = ImGui::GetItemRectMin();
        viewportBoundsGame[1] = ImGui::GetItemRectMax();

        ImGui::End();
    }
}