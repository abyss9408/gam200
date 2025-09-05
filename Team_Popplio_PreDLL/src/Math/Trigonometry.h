/******************************************************************************/
/*!
\file   Trigonometry.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
        Header file for trigonometric operations.
        Radian and degree conversions.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "Vector2D.h"

namespace PopplioMath
{
    /*!*****************************************************************************
    \brief
    Converts angle in degrees to radians

    \param[in] angle
    [float const&] angle in degrees

    \return
    [float const&] Angle in radians
    *******************************************************************************/
    float DegToRad(float angle);

    /*!*****************************************************************************
    \brief
    Converts angle in radians to degrees

    \param[in] angle
    [float const&] angle in radians

    \return
    [float const&] Angle in degrees
    *******************************************************************************/
    float RadToDeg(float angle);

    /*!*****************************************************************************
    \brief
    Compute resulting 2D vector from given angle and scalar

    \param[in] angle
    [float const&] angle in radians from origin of resulting vector

    \param[in] scalar
    [float const&] scalar value of vector

    \return
    [Vector2D const&] Vector2D result
    *******************************************************************************/
    Vec2f const AngleToVector2D(float angle, float scalar);
}