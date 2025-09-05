/******************************************************************************/
/*!
\file   OnAppQuit.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the header file for OnAppQuit event

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "../EventBus/EventBus.h"

namespace PopplioUtil
{
    // Event triggered when program is quitting
	struct OnAppQuit : public Popplio::Event
	{
		OnAppQuit() {}
	};
}