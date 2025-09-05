/******************************************************************************/
/*!
\file   MathLib.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
        This is the header file for the core math library of the engine

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

// Add header files from individual library components here
#include "MatVec.h"
#include "Geometry.h"
#include "Trigonometry.h"
#include "GeneralMath.h"
#include "Lerp.h"
#include "Polynomial.h"

#include <cmath>

namespace PopplioMath
{
    constexpr const float NATURAL_LOG_OF_2{ 0.693147181f };
    //constexpr const float M_PI{ 3.14159265358979323846f };
    const float M_PI{ acosf(-1) };
    const double M_PI_D{ acos(-1) };
}