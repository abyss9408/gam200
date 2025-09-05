/******************************************************************************/
/*!
\file   GameCamera.cpp
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401
\par    Section : A
\date   2024/11/30
\brief
	This source file defines the GameCamera class within the Popplio namespace.
    The GameCamera class is responsible for managing the game camera's position,
    zoom level, rotation, and the view and projection matrices used in the game.

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
#include <pch.h>
#include "GameCamera.h"
#include "../Engine/Engine.h"
#include <random>

namespace Popplio
{
	GameCamera::GameCamera(int width, int height)
		: position(0.0f, 0.0f), zoom(360.0f), aspectRatio(width / static_cast<float>(height)), targetEntity(), followSpeed(5.0f)
	{
		UpdateViewMatrix();
		UpdateProjectionMatrix();
	}

	PopplioMath::Vec2f GameCamera::GetPosition() const
	{
		return position;
	}

	float GameCamera::GetZoom() const
	{
		return zoom;
	}

	float GameCamera::GetAspectRatio() const
	{
		return aspectRatio;
	}

	void GameCamera::SetPosition(const PopplioMath::Vec2f& pos)
	{
		position = pos;
		UpdateViewMatrix();
		UpdateProjectionMatrix();
	}

	void GameCamera::SetTarget(std::weak_ptr<Entity> target)
	{
		targetEntity = target;
		UpdateViewMatrix();
		UpdateProjectionMatrix();
	}

	void GameCamera::ClearTarget()
	{
		targetEntity.reset();
	}

	bool GameCamera::HasTarget() const
	{
		return !targetEntity.expired();
	}

	void GameCamera::SetZoom(float newZoom)
	{
		zoom = std::max(0.1f, newZoom);
		UpdateViewMatrix();
		UpdateProjectionMatrix();
	}

	void GameCamera::Update(float dt)
	{
		UpdateTargetTracking(dt);
	}

	void GameCamera::UpdateTargetTracking(float dt)
	{
		if (auto target = targetEntity.lock())
		{
			if (target->HasComponent<TransformComponent>())
			{
				auto& transform = target->GetComponent<TransformComponent>();
				PopplioMath::Vec2f targetPos = transform.position;
				PopplioMath::Vec2f diff = targetPos - position;
				position += diff * followSpeed * dt;
			}
		}
	}

	PopplioMath::M3x3f GameCamera::GetViewMatrix() const
	{
		return viewMatrix;
	}

	PopplioMath::M3x3f GameCamera::GetProjectionMatrix() const
	{
		return projectionMatrix;
	}

	void GameCamera::UpdateProjectionMatrix()
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

	void GameCamera::UpdateViewMatrix()
	{
		// Build view matrix
		viewMatrix.Identity();
		viewMatrix.Translate(-position.x, -position.y);
	}

	PopplioMath::Vec2f GameCamera::ScreenToWorld(const PopplioMath::Vec2f& screenPos) const
	{
		// Convert screen coordinates (0 to width/height) to normalized device coordinates (-1 to 1)
		PopplioMath::Vec2f ndc(
			(screenPos.x / Engine::Config::windowWidth) * 2.f - 1.f,
			1.f - (screenPos.y / Engine::Config::windowHeight) * 2.f
		);

		// Create inverse matrices
		PopplioMath::M3x3f projectionInverse = GetProjectionMatrix();
		projectionInverse.Inverse();
		PopplioMath::M3x3f viewInverse = GetViewMatrix();
		viewInverse.Inverse();

		// Transform to world space
		PopplioMath::Vec2f worldPos = (viewInverse * (projectionInverse * PopplioMath::Vec2f(ndc.x, ndc.y)));
		return worldPos;
	}

	PopplioMath::Vec2f GameCamera::WorldToScreen(const PopplioMath::Vec2f& worldPos) const
	{
		// Transform to ndc space
		PopplioMath::Vec2f clipPos = GetProjectionMatrix() * (GetViewMatrix() * worldPos);

		// Convert to screen space
		return PopplioMath::Vec2f(
			(clipPos.x + 1.f) * 0.5f * Engine::Config::windowWidth,
			(1.f - clipPos.y) * 0.5f * Engine::Config::windowHeight
		);
	}
}