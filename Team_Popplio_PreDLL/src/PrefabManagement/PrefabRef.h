/******************************************************************************/ 
/*! 
\file   PrefabRef.h 
\author Team Popplio 
\author Bryan Ang Wei Ze 
\contribution Bryan - 100% 
\par    Course : CSD2401 / UXGD2400 / DAA2402 
\par    Section : A 
\date   2025/02/06 
\brief
This file contains the definition of the PrefabRef class, which is used to reference prefabs in the game engine. 
The PrefabRef class includes properties for the prefab path, local position, local scale, and local rotation.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents 
without the prior written consent of DigiPen Institute of 
Technology is prohibited. 
*/ 
/******************************************************************************/
#pragma once
#include <string>
#include "../Math/Vector2D.h"

namespace Popplio
{
	class PrefabRef
	{
	public:
		PrefabRef(const std::string& prefabPath = "", const PopplioMath::Vec2f& pos = { 0, 0 },
			const PopplioMath::Vec2f& scl = { 1, 1 }, double rot = 0)
			: prefabPath(prefabPath), localPosition(pos), localScale(scl), localRotation(rot)
		{

		}
		std::string prefabPath;
		PopplioMath::Vec2f localPosition;
		PopplioMath::Vec2f localScale;
		double localRotation;
	};
}