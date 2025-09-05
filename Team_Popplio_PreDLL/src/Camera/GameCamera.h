/******************************************************************************/
/*!
\file           GameCamera.h
\author         Team Popplio
\author         Bryan Ang Wei Ze
\contribution   Bryan - 100%
\par            Course : CSD2401
\par            Section : A
\date           2024/11/30
\brief
    This file contains the declaration of the GameCamera class within the Popplio
    namespace. The GameCamera class is responsible for managing the game camera's
    position, zoom level, rotation, and the view and projection matrices used in
    the game.

    The GameCamera class includes methods for setting and getting the camera's
    position, zoom, and rotation, as well as methods for handling camera shake,
    transitions, and target tracking. It also provides methods for converting
    between screen and world coordinates.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include "../Math/MatVec.h"
#include "../ECS/ECS.h"
#include <functional>

namespace Popplio
{
	class GameCamera
    {
    public:
		GameCamera(int width, int height);

        /**
        * @brief Sets the position of the camera.
        * 
        * @param pos The new position of the camera as a 2D vector.
        */
        void SetPosition(const PopplioMath::Vec2f& pos);

        /**
        * @brief Sets the target entity for the camera to follow.
        * 
        * @param target A weak pointer to the target entity.
        */
        void SetTarget(std::weak_ptr<Entity> target);

        /**
        * @brief Clears the current target entity.
        */
        void ClearTarget();

        /**
        * @brief Checks if the camera has a target entity.
        * 
        * @return True if the camera has a target entity, false otherwise.
        */
        bool HasTarget() const;

        /**
        * @brief Sets the zoom level of the camera.
        * 
        * @param newZoom The new zoom level.
        */
        void SetZoom(float newZoom);

        /**
        * @brief Gets the current position of the camera.
        * 
        * @return The current position of the camera as a 2D vector.
        */
        PopplioMath::Vec2f GetPosition() const;

        /**
        * @brief Gets the current zoom level of the camera.
        * 
        * @return The current zoom level.
        */
        float GetZoom() const;

        float GetAspectRatio() const;

        /**
        * @brief Updates the camera state based on the elapsed time.
        * 
        * @param deltaTime The time elapsed since the last update.
        */
        void Update(float deltaTime);

        /**
        * @brief Gets the view matrix for rendering.
        * 
        * @return The view matrix.
        */
        PopplioMath::M3x3f GetViewMatrix() const;

        /**
        * @brief Gets the projection matrix for rendering.
        * 
        * @return The projection matrix.
        */
        PopplioMath::M3x3f GetProjectionMatrix() const;

        /**
        * @brief Converts screen coordinates to world coordinates.
        * 
        * @param screenPos The position in screen coordinates.
        * @return The position in world coordinates.
        */
        PopplioMath::Vec2f ScreenToWorld(const PopplioMath::Vec2f& screenPos) const;

        /**
        * @brief Converts world coordinates to screen coordinates.
        * 
        * @param worldPos The position in world coordinates.
        * @return The position in screen coordinates.
        */
        PopplioMath::Vec2f WorldToScreen(const PopplioMath::Vec2f& worldPos) const;

    private:
		// Camera properties
		PopplioMath::Vec2f position;
		float zoom;
		float aspectRatio;
		std::weak_ptr<Entity> targetEntity;
		float followSpeed;
        PopplioMath::M3x3f viewMatrix; ///< The view matrix of the camera.
        PopplioMath::M3x3f projectionMatrix; ///< The projection matrix of the camera.

        void UpdateViewMatrix();
        void UpdateProjectionMatrix();
        void UpdateTargetTracking(float dt);
    };
}