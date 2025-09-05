/******************************************************************************/
/*!
\file   Trigonometry.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
        Source file for trigonometric operations.
        Radian and degree conversions.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h> // PCH

//#define _USE_MATH_DEFINES
//
//#define M_PI 3.14159265358979323846  /* pi */

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
    float DegToRad(float angle) { return angle * (static_cast<float>(M_PI) / 180.f); }

    /*!*****************************************************************************
    \brief
    Converts angle in radians to degrees

    \param[in] angle
    [float const&] angle in radians

    \return
    [float const&] Angle in degrees
    *******************************************************************************/
    float RadToDeg(float angle) { return angle * (180.f / static_cast<float>(M_PI)); }

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
    Vec2f const AngleToVector2D(float angle, float scalar)
    {
        return Vec2f(cosf(angle) * scalar, sinf(angle) * scalar);
    }
}