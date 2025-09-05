/******************************************************************************/
/*!
\file   Forces.cpp
\author Team Popplio
\author Rio Shannon Yvon Leonardo [100%]
\par    Course : CSD2451/CSD2450
\par    Section : A
\date   26/01/2025
\brief
		This file contains the implementation of the Force system that
		is defined in Forces.h. It contains logic to handle the forces
		such as including, adding, updating , activating, deactivating
		and computing resultant force from all active forces.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>
#include "../Physics/Forces.h"
#include "../Logging/Logger.h"

namespace Popplio
{
	Force::Force(PopplioMath::Vec2f direction, ForceType type, float magnitude,
		float lifetime) :
		direction(direction),
		type(type),
		magnitude(magnitude),
		lifetime(lifetime),
		age(0.0f),
		isActive(false)
	{
	}

	void Force::UpdateAge(float deltaTime) {
		//only updates if force is active and has a finite lifetime
		if (isActive && lifetime > 0.0f) {
			age += deltaTime;
			//deactivate the force when lifetime is exceeded
			if (age >= lifetime) {
				isActive = false;
				age = 0.0f;
			}
		}
	}

	void Force::SetActive(bool active) {
		isActive = active;
		//reset age when activating force
		if (active) {
			age = 0.0f;
		}
	}

	bool Force::IsExpired() const {
		return lifetime > 0.0f && age >= lifetime;
	}

	void ForceManager::AddForce(const Force& force)
	{
		forces.push_back(force);
	}

	void ForceManager::UpdateForces(float deltaTime)
	{
		//update age of each force
		for (auto& force : forces)
		{
			force.UpdateAge(deltaTime);
		}
	}

	void ForceManager::ActivateForce(ForceType type)
	{
		//find and activate the first force of the specified type
		for (auto& force : forces) 
		{
			if (force.type == type) {
				force.SetActive(true);
				break;
			}
		}
	}

	void ForceManager::DeactivateForce(ForceType type)
	{
		//find and deactivate the first force of the specified type
		for (auto& force : forces)
		{
			if (force.type == type) {
				force.SetActive(false);
				break;
			}
		}
	}

	void ForceManager::RemoveForce(ForceType type)
	{
		forces.erase(
			std::remove_if(forces.begin(), forces.end(),
				[type](const Force& force) {
					return force.type == type;
				}),
			forces.end()
		);
	}

	PopplioMath::Vec2f ForceManager::GetResultantForce() const {
		PopplioMath::Vec2f resultant(0.0f, 0.0f);
		//sum up all of the active forces
		for (const auto& force : forces)
		{
			if (force.isActive) {
				resultant.x += force.direction.x * force.magnitude;
				resultant.y += force.direction.y * force.magnitude;
			}
		}
		return resultant;
	}

	void ForceManager::Clear() {
		forces.clear();
	}
}