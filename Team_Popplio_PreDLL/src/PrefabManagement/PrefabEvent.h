/******************************************************************************/
/*!
\file   PrefabEvent.h
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/05
\brief
	This header file defines the PrefabEvent structure within the Popplio namespace. 
	The PrefabEvent structure is used to represent events related to prefabs, 
	such as modifications or deletions. It inherits from the Event class and 
	includes information about the prefab and the type of event.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include "../EventBus/Event.h"
#include <string>
#include <typeindex>

namespace Popplio
{
	enum class PrefabEventType
	{
		Modified,
		Deleted,
	};

	struct PrefabEvent : public Event
	{
		std::string prefabName;
		PrefabEventType eventType;
		std::optional<std::type_index> modifiedComponentType;

		PrefabEvent(const std::string& name, PrefabEventType type,
			std::optional<std::type_index> componentType = std::nullopt)
			: prefabName(name), eventType(type), modifiedComponentType(componentType) {}
	};
}