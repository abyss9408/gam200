/******************************************************************************/
/*!
\file           CameraManager.cpp
\author         Team Popplio
\author         Bryan Ang Wei Ze
\contribution   Bryan - 100%
\par            Course : CSD2401
\par            Section : A
\date           2024/11/30
\brief
	This source file contains the implementation of the CameraManager class
	within the Popplio namespace. The CameraManager class is responsible for
	managing both the editor and game cameras, allowing for switching between
	them and updating their states.

	The CameraManager class includes methods for setting the active camera,
	updating the camera states, and accessing the view and projection matrices
	of the currently active camera.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include "../Engine/Engine.h"
#include "CameraManager.h"

namespace Popplio
{
#ifndef IMGUI_DISABLE
	CameraManager::CameraManager(int width, int height)
		: editorCamera(width, height), gameCamera(width, height), useEditorCamera(true)
	{
	}
#else
	CameraManager::CameraManager(int width, int height)
		: gameCamera(width, height), useEditorCamera(false)
	{
	}
#endif // !IMGUI_DISABLE

	void CameraManager::Update(float deltaTime)
	{
		if (!useEditorCamera)
		{
			gameCamera.Update(deltaTime);
		}
	}

	void CameraManager::SetActiveCamera(bool setUseEditorCamera)
	{
		useEditorCamera = setUseEditorCamera;
	}

	bool CameraManager::IsUsingEditorCamera() const
	{
		return useEditorCamera;
	}

#ifndef IMGUI_DISABLE
	EditorCamera& CameraManager::GetEditorCamera()
	{
		return editorCamera;
	}

	const EditorCamera& CameraManager::GetEditorCamera() const
	{
		return editorCamera;
	}
#endif

	GameCamera& CameraManager::GetGameCamera()
	{
		return gameCamera;
	}

	const GameCamera& CameraManager::GetGameCamera() const
	{
		return gameCamera;
	}

	PopplioMath::M3x3f CameraManager::GetViewMatrix() const
	{
#ifndef IMGUI_DISABLE
		return useEditorCamera ? editorCamera.GetViewMatrix() : gameCamera.GetViewMatrix();
#else
        return gameCamera.GetViewMatrix();
#endif
	}

	PopplioMath::M3x3f CameraManager::GetProjectionMatrix() const
	{
#ifndef IMGUI_DISABLE
		return useEditorCamera ? editorCamera.GetProjectionMatrix() : gameCamera.GetProjectionMatrix();
#else
        return gameCamera.GetProjectionMatrix();
#endif
	}
}