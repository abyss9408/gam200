
/******************************************************************************/
/*!
\file   InGameCameraEdit.cpp
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/06
\brief
    This source file defines the methods for editing the in-game camera within the Popplio namespace.
    It includes functionality for rendering the camera controller window and drawing the game camera frustum.

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
	void Editor::RenderCameraControllerWindow()
	{
		if (!ImGui::Begin(ICON_FA_VIDEO " Game Camera"))
		{
			ImGui::End();
			return;
		}

        GameCamera& gameCamera = cameraManager.GetGameCamera();

		// Position Controls
        PopplioMath::Vec2f cameraPos = gameCamera.GetPosition();
        if (ImGui::DragFloat2("Position", &cameraPos.x, 1.0f))
        {
            gameCamera.SetPosition(cameraPos);
        }

        // Zoom Controls
        float zoom = gameCamera.GetZoom();
        if (ImGui::DragFloat("Zoom", &zoom, 1.0f, 0.1f, 1000.0f))
        {
            gameCamera.SetZoom(zoom);
        }

        // View Frustum Visualization
        ImGui::Separator();
        ImGui::Checkbox("Show View Frustum", &showGameCameraFrustum);
        

        ImGui::End();
	}

    void Editor::DrawGameCameraFrustum()
    {
        GameCamera& gameCamera = cameraManager.GetGameCamera();

        // Calculate frustum corners in world space
		float aspectRatio = gameCamera.GetAspectRatio();
        float zoom = gameCamera.GetZoom();
        PopplioMath::Vec2f pos = gameCamera.GetPosition();

        // Calculate the four corners of the frustum
        PopplioMath::Vec2f topLeft = pos + PopplioMath::Vec2f(-zoom * aspectRatio, zoom);
        PopplioMath::Vec2f topRight = pos + PopplioMath::Vec2f(zoom * aspectRatio, zoom);
        PopplioMath::Vec2f bottomLeft = pos + PopplioMath::Vec2f(-zoom * aspectRatio, -zoom);
        PopplioMath::Vec2f bottomRight = pos + PopplioMath::Vec2f(zoom * aspectRatio, -zoom);

        // Convert world space positions to screen space
        auto screenTopLeft = WorldToScreenPosition(topLeft);
        auto screenTopRight = WorldToScreenPosition(topRight);
        auto screenBottomLeft = WorldToScreenPosition(bottomLeft);
        auto screenBottomRight = WorldToScreenPosition(bottomRight);

        // Get the ImGui draw list for the current window
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        // Draw the frustum outline
        ImU32 frustumColor = IM_COL32(255, 255, 255, 255);
        drawList->AddQuad(
            ImVec2(screenTopLeft.x, screenTopLeft.y),
            ImVec2(screenTopRight.x, screenTopRight.y),
            ImVec2(screenBottomRight.x, screenBottomRight.y),
            ImVec2(screenBottomLeft.x, screenBottomLeft.y),
            frustumColor, 2.0f
        );

        // Draw camera icon at camera position
        auto screenCameraPos = WorldToScreenPosition(pos);

        // Calculate icon size based on editor camera zoom
		float editorZoom = cameraManager.GetEditorCamera().GetZoom();
        float baseSize = 24.0f; // Base icon size
        float minSize = 16.0f;  // Minimum icon size
        float maxSize = 48.0f;  // Maximum icon size

        // Inverse relationship with zoom - icon gets smaller as we zoom ou
        float iconSize = std::clamp(baseSize * (360.0f / editorZoom), minSize, maxSize);

        ImVec2 iconMin(screenCameraPos.x - iconSize / 2, screenCameraPos.y - iconSize / 2);
        ImVec2 iconMax(screenCameraPos.x + iconSize / 2, screenCameraPos.y + iconSize / 2);

        // Center the icon text
        float textWidth = ImGui::CalcTextSize(ICON_FA_VIDEO).x;
        float textHeight = ImGui::GetFont()->FontSize;

        // Scale the font size based on the icon size
        float scale = iconSize / baseSize;
        textWidth *= scale;
        textHeight *= scale;

        ImVec2 textPos(
            screenCameraPos.x - textWidth * 0.5f,
            screenCameraPos.y - textHeight * 0.5f
        );

        // Draw the camera icon with scaled size
        drawList->AddText(ImGui::GetFont(), iconSize,
            textPos, IM_COL32(255, 255, 255, 255), ICON_FA_VIDEO);
    }
}