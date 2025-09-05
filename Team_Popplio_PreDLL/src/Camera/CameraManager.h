/******************************************************************************/
/*!
\file           CameraManager.h
\author         Team Popplio
\author         Bryan Ang Wei Ze
\contribution   Bryan - 100%
\par            Course : CSD2401
\par            Section : A
\date           2024/11/30
\brief
	This file contains the declaration of the CameraManager class within the
	Popplio namespace. The CameraManager class is responsible for managing
	both the editor and game cameras, allowing for switching between them
	and updating their states.

	The CameraManager class includes methods for setting the active camera,
	updating the camera states, and accessing the view and projection matrices
	of the currently active camera.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include "../ECS/ECS.h"
#ifndef IMGUI_DISABLE
#include "EditorCamera.h"
#endif
#include "GameCamera.h"

namespace Popplio
{
	class CameraManager
	{
	public:
        /**
         * \brief Constructor for the CameraManager class.
         * \param width The width of the viewport.
         * \param height The height of the viewport.
         */
        CameraManager(int width, int height);

        /**
         * \brief Updates the state of the active camera.
         * \param deltaTime The time elapsed since the last update.
         */
        void Update(float deltaTime);

        /**
         * \brief Sets the active camera to either the editor camera or the game camera.
         * \param useEditorCamera If true, the editor camera is set as active; otherwise, the game camera is set as active.
         */
        void SetActiveCamera(bool useEditorCamera);

        /**
         * \brief Checks if the editor camera is currently active.
         * \return True if the editor camera is active, false otherwise.
         */
        bool IsUsingEditorCamera() const;
#ifndef IMGUI_DISABLE
        /**
         * \brief Provides access to the editor camera.
         * \return A reference to the editor camera.
         */
        EditorCamera& GetEditorCamera();

        /**
         * \brief Provides read-only access to the editor camera.
         * \return A constant reference to the editor camera.
         */
        const EditorCamera& GetEditorCamera() const;
#endif
        /**
         * \brief Provides access to the game camera.
         * \return A reference to the game camera.
         */
        GameCamera& GetGameCamera();

        /**
         * \brief Provides read-only access to the game camera.
         * \return A constant reference to the game camera.
         */
        const GameCamera& GetGameCamera() const;

        /**
         * \brief Gets the view matrix of the currently active camera.
         * \return The view matrix of the active camera.
         */
        PopplioMath::M3x3f GetViewMatrix() const;

        /**
         * \brief Gets the projection matrix of the currently active camera.
         * \return The projection matrix of the active camera.
         */
        PopplioMath::M3x3f GetProjectionMatrix() const;

	private:
#ifndef IMGUI_DISABLE
		EditorCamera editorCamera;
#endif
		GameCamera gameCamera;
		bool useEditorCamera;
	};
}