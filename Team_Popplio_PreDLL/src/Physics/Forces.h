/******************************************************************************/
/*!
\file   Forces.h
\author Team Popplio
\author Rio Shannon Yvon Leonardo [100%]
\par    Course : CSD2451/CSD2450
\par    Section : A
\date   26/01/2025
\brief
		This is the header file for the Forces and ForcesManager class.
		This file helps to manage different type of forces applied to objects
		in a physics system.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "../Math/Vector2D.h"
#include "../Physics/Forces.h"
#include <vector> //needed for dynamic vectors

namespace Popplio {

	//enumeration of different types of forces that can be applied to objects
	enum class ForceType
	{
		MOVE_LEFT,
		DRAG,
		IMPULSE,
		CUSTOM
	};

	class Force 
	{
		public:

			PopplioMath::Vec2f direction;	//direction of the force
			ForceType type;					//type of Force based on ForceType
			float magnitude;				//magnitude of the Force
			float lifetime;					//duration of the force
			float age;						//current age of the force
			bool isActive;					//whether force is currently active

			/*!*****************************************************************************
 			\brief
				Constructor for Force clas
 			\param[in] direction
				Direction of the force
 			\param[in] type
				Type of force
 			\param[in] magnitude
				Magnitude of the force
			\param[in] lifetime
				Duration of the force
			*******************************************************************************/

			Force(PopplioMath::Vec2f direction, ForceType type, float magnitude,
				float lifetime = 0.0f);
			
			/*!*****************************************************************************
 			\brief
				Updates the age of the force and deactivates if lifetime is exceeded
 			\param[in] deltaTime
				Time elapsed since last update
			*******************************************************************************/
			void UpdateAge(float deltaTime);

			/*!*****************************************************************************
 			\brief
				Sets the active state of the force
 			\param[in] active
				Boolean to indicate if force is active
			*******************************************************************************/
			void SetActive(bool active);

			/*!*****************************************************************************
 			\brief
				Checks if the force expired based on its lifetime
 			\return 
				Returns true if the force is expired, else false

			*******************************************************************************/
			//checks if Force expired based on its lifetime
			bool IsExpired() const;
	};

	class ForceManager
	{
		public:
			
			/*!*****************************************************************************
			\brief
				Adds a new force to the system
			\param[in] force
				The force object to be added
			*******************************************************************************/
			void AddForce(const Force& force);

			/*!*****************************************************************************
			\brief
				Updates all forces in the system
			\param[in]
				Time elapsed since the last update
			*******************************************************************************/
			void UpdateForces(float deltaTime);

			/*!*****************************************************************************
			\brief
				Activates a force of a specified type
			\param[in] type
				Type of force to be activated
			*******************************************************************************/
			void ActivateForce(ForceType type);

			/*!*****************************************************************************
			\brief
				Deactivates a force of a specified type
			\param[in] type
				Type of force to deactivate
			*******************************************************************************/
			void DeactivateForce(ForceType type);

			/*!*****************************************************************************
			\brief
				Removes a force of the specified type
			\return
				Type of force to remove
			*******************************************************************************/
			void RemoveForce(ForceType type);

			/*!*****************************************************************************
			\brief
				Calculates the total resultant force from all active forces
			\return
				A vector representing the resultant force
			*******************************************************************************/
			PopplioMath::Vec2f GetResultantForce() const;

			/*!*****************************************************************************
			\brief
				Removes all existing forces from the system
			*******************************************************************************/
			void Clear();

		private:

			std::vector<Force> forces; //a dynamical array to store all the forces

	};
}