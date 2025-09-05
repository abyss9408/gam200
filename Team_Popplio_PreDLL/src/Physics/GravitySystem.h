/******************************************************************************/
/*!
\file   GravitySystem.h
\author Team Popplio
\author Rio Shannon Yvon Leonardo - 100%
\par    Course : CSD2400/CSD2401
\par    Section : A
\date   2024/10/03
\brief
		This header file contains the declarations of functions implemented 
		in GravitySystem.cpp

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "../Math/Vector2D.h"
#include "../ECS/ECS.h" 
#include "../Physics/RigidBodyComponent.h" 
#include "../Physics/Physics.h"
#include "../Engine/Engine.h"
#include "../Logging/Logger.h"

namespace Popplio
{
	class GravitySystem : public System
	{
	public :
		GravitySystem();
		void Update();

	private:
		void ApplyForce(RigidBodyComponent& rigidBody, float forceX, float forceY);
	};
}
