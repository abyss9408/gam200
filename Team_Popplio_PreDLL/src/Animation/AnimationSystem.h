/******************************************************************************/
/*!
\file   AnimationSystem.h
\author Team Popplio
\author Hafawati Binte Mohd Ali (100%)
\par    Course : CSD2400
\par    Section : A
\date   2025/01/10
\brief
This header file declares the AnimationSystem class, which is used for handling
animations in the game engine. It includes functions for updating animations,
subscribing to events, and handling key presses.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "AnimationComponent.h"
#include <GLFW/glfw3.h>

namespace Popplio
{
	class AnimationSystem : public System
	{
	public:
		/*!*****************************************************************************
		\brief
		Constructor for the AnimationSystem class.

		*******************************************************************************/
		AnimationSystem();

		/*!*****************************************************************************
		\brief
		Subscribes to key press and release events.

		\param[in] eventBus
		[const std::unique_ptr<EventBus>&] Pointer to the event bus.

		*******************************************************************************/
		//void SubscribeToEvents(const std::unique_ptr<EventBus>& eventBus);

		/*!*************************************************************************
	    \brief Updates the animations for all entities with an AnimationComponent.

	    \param[in] deltaTime
		  The time elapsed since the last update.
		*************************************************************************/
		void Update(float deltaTime);

		/*!*************************************************************************
	    \brief Stops the animations for all entities with an AnimationComponent.
		*************************************************************************/
		void StopAll();

		/*!*************************************************************************
		\brief Starts the animations for all entities with an AnimationComponent.
		*************************************************************************/
		void StartAll();

		/*!*************************************************************************
		\brief Pauses the animations for all entities with an AnimationComponent.
		*************************************************************************/
		void PauseAll();

		/*!*************************************************************************
		\brief Resumes the animations for all entities that were paused with an AnimationComponent.
		*************************************************************************/
		void ResumeAll();

		//bool SetIsMoving(bool moving) { return isMoving = moving; }
		//bool SetIsAttacking(bool attacking) { return isAttacking = attacking; }
		//bool SetIdle(bool isIdle) { return this->idle = isIdle; }

	private:

		//bool isMoving = false;
		//bool isAttacking = false;
		//bool idle = true;
		/*!*****************************************************************************
		\brief
		Handles key press events.

		\param[in] event
		[KeyPressedEvent&] Reference to the key pressed event.

		*******************************************************************************/
		//void OnKeyPressed(KeyPressedEvent& event);
	};
}