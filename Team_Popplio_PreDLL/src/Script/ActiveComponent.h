/******************************************************************************/
/*!
\file   ActiveComponent.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
        This is the header file for the IsActive Component
        Enables / disables game object

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "../src/ECS/ECS.h"

namespace Popplio
{
    struct ActiveComponent 
    {
        bool isActive;

        ActiveComponent() : isActive(true) {} // default to active
        ActiveComponent(bool const& active) : isActive(active) {}
        ~ActiveComponent() = default;
    };
}
