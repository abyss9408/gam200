/******************************************************************************/
/*!
\file   EditorCamera.cpp
\author Team Popplio
\author Bryan Ang Wei Ze
\co-author Pearl Goh
\contribution Bryan - 10%
\contribution Pearl Goh- 90%
\par    Course : CSD2401
\par    Section : A
\date   2024/11/07
\brief
	This source file defines the EditorCamera class within the Popplio namespace. 
	The EditorCamera class is responsible for managing the editor camera's position, 
	zoom level, and the view and projection matrices used in the editor.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include "EditorCamera.h"

namespace Popplio
{
	EditorCamera::EditorCamera(int width, int height)
		: position(0.0f, 0.0f), zoom(360.0f), aspectRatio(width / static_cast<float>(height)), viewportWidth(width), viewportHeight(height)
	{
		UpdateViewMatrix();
		UpdateProjectionMatrix();
	}

	void EditorCamera::SetPosition(const PopplioMath::Vec2f& offset)
	{
		position = offset; //new position
		UpdateViewMatrix();
	}

	void EditorCamera::SetZoom(float zoomOffset)
	{
		zoom = zoomOffset;
		//if (zoom < 1.0f) zoom = 1.0f; // Prevent zooming too far in
		//if (zoom > 45.0f) zoom = 45.0f; // Prevent zooming too far out
		UpdateProjectionMatrix();
	}

	PopplioMath::Vec2f EditorCamera::GetPosition() const
	{
		return position;
	}

	float EditorCamera::GetZoom() const
	{
		return zoom;
	}

	PopplioMath::M3x3f EditorCamera::GetViewMatrix() const
	{
		return viewMatrix;
	}

	PopplioMath::M3x3f EditorCamera::GetProjectionMatrix() const
	{
		return projectionMatrix;
	}

	void EditorCamera::UpdateViewMatrix()
	{
		viewMatrix.Identity();

		// Set the translation to negate the EditorCamera's position
		viewMatrix.Translate(-position.x, -position.y);

	}

	void EditorCamera::UpdateProjectionMatrix()
	{
		// Calculate projection matrix based on aspect ratio and zoom
		float left = -aspectRatio * zoom;
		float right = aspectRatio * zoom;
		float bottom = -zoom;
		float top = zoom;

		// Here you can define your orthographic projection matrix manually
		projectionMatrix[0][0] = 2.0f / (right - left);
		projectionMatrix[1][1] = 2.0f / (top - bottom);
		projectionMatrix[0][2] = -(right + left) / (right - left);
		projectionMatrix[1][2] = -(top + bottom) / (top - bottom);
		projectionMatrix[2][2] = 1.0f;
	}
}