/******************************************************************************/
/*!
\file   LogicScript.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
        This is the source file for engine Logic System script interface
        Enables scripting functionality by separating game from engine code
        Able to be attached to entities (gameobjects)
        Implements C# Mono Framework (TODO)

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <pch.h>
#include "LogicScript.h"

namespace Popplio
{
    std::unordered_map<std::string, ILogicScript&> ILogicScript::scripts{};
}