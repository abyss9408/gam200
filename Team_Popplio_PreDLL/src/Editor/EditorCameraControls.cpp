/******************************************************************************/
/*!
\file   EditorCameraControls.cpp
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/05
\brief
    This source file defines methods for handling camera controls within the Editor 
    class in the Popplio namespace. The methods allow for camera panning and zooming 
    using mouse input, and they ensure that these actions are only performed when the 
    mouse is within the viewport.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>

#include "Editor.h"

#include "../Input/InputSystem.h"

namespace Popplio
{
    bool Editor::IsMouseInViewport() const
    {
        return isViewportHovered;
    }

    PopplioMath::Vec2f Editor::GetMouseViewportPosition() const
    {
        ImVec2 mousePos = ImGui::GetMousePos();

        // Get viewport bounds
        ImVec2 viewportMin = viewportBounds[0];
        ImVec2 viewportMax = viewportBounds[1];

        // Convert mouse position to normalized viewport coordinates (0 to 1)
        float normalizedX = (mousePos.x - viewportMin.x) / (viewportMax.x - viewportMin.x);
        float normalizedY = (mousePos.y - viewportMin.y) / (viewportMax.y - viewportMin.y);

        return PopplioMath::Vec2f(normalizedX, normalizedY);
    }

    void Editor::HandleCameraInput()
    {
        // Early return if mouse is not in viewport
        if (!IsMouseInViewport())
        {
            isDraggingCamera = false;
            return;
        }

        // Get current mouse position and delta
        auto mousePos = InputSystem::GetMouse();
        auto mouseDelta = InputSystem::GetMouseDelta();

        // Middle/Right mouse button camera panning
        if ((InputSystem::IsMouseButtonDown(InputSystem::MouseButton::Right) || 
            InputSystem::IsMouseButtonDown(InputSystem::MouseButton::Middle)))
        {
            if (!isDraggingCamera)
            {
                isDraggingCamera = true;
                lastMousePos = mousePos;
            }
            else
            {
                // Calculate movement based on mouse delta and zoom level
                float zoomFactor = GetCamera().GetZoom();
                PopplioMath::Vec2f deltaPos =
                {
                    (mousePos.x - lastMousePos.x) * zoomFactor * 0.005f,
                    (mousePos.y - lastMousePos.y) * zoomFactor * 0.005f
                };

                auto currentPos = GetCamera().GetPosition();
                GetCamera().SetPosition({
                    currentPos.x - deltaPos.x,
                    currentPos.y + deltaPos.y
                });

                lastMousePos = mousePos;
            }
        }
        else
        {
            isDraggingCamera = false;
        }

        // Mouse wheel zoom
        PopplioMath::Vec2f scroll = InputSystem::GetMouseScroll();
        if (scroll.y != 0)
        {
            float currentZoom = GetCamera().GetZoom();
            float newZoom = currentZoom * (1 - scroll.y * zoomSpeed);
            GetCamera().SetZoom(newZoom);
        }
    }

    EditorCamera& Editor::GetCamera()
	{
		return cameraManager.GetEditorCamera();
	}

    const EditorCamera& Editor::GetCamera() const
    {
        return cameraManager.GetEditorCamera();
    }
}