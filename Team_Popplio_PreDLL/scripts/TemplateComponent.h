/******************************************************************************/
/*!
\file   TemplateComponent.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
        This is the header file for a Logic System template component
        Enables scripting functionality by separating game from engine code
        Implements C# Mono Framework (TODO)

        Use this as a C++ template for game components

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

// Includes

#pragma once

// ECS // DO NOT REMOVE //
#include "../src/Script/LogicData.h"
// ECS // DO NOT REMOVE //

using namespace Popplio;

namespace BassNKick
{
    struct TemplateComponent : ILogicData
    {
        int data;
    };
}