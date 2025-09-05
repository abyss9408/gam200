/******************************************************************************/
/*!
\file   CloneEntityEvent.h
\author Team Popplio
\author Bryan
\contribution Bryan - 85%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
		This is the header file for CloneEntityEvent

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include "../EventBus/Event.h"

namespace Popplio
{
	class CloneEntityEvent : public Event
	{
	public:
		int entityId;
		int cloneToId;
		
		CloneEntityEvent(int id, int toId = -1) : entityId(id), cloneToId(toId) {}
	};
}