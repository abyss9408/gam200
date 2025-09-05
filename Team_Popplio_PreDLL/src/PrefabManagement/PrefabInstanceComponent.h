/******************************************************************************/
/*!
\file   PrefabInstanceComponent.h
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/05
\brief
	This header file defines the PrefabInstanceComponent structure within the 
	Popplio namespace. The PrefabInstanceComponent is used to represent an instance 
	of a prefab in the game, including information about the prefab's name and 
	whether it should be synchronized with the original prefab.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include <string>

namespace Popplio
{
	struct PrefabInstanceComponent
	{
		std::string prefabName;
		bool syncWithPrefab;

		PrefabInstanceComponent(const std::string& prefabName = "", bool sync = true)
			: prefabName(prefabName), syncWithPrefab(sync) {}
	};
}