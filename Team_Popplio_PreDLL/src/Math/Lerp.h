/******************************************************************************/
/*!
\file   Lerp.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
        Header file for linear and related interpolation

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

namespace PopplioMath
{
    static float const Linear(float const& start, float const& end, float const& value);


    static float const EaseInQuad(float const& start, float const& end, float const& value);

    static float const EaseOutQuad(float const& start, float const& end, float const& value);

    static float const EaseInOutQuad(float const& start, float const& end, float const& value);

    static float const EaseInCubic(float const& start, float const& end, float const& value);

    static float const EaseOutCubic(float const& start, float const& end, float const& value);

    static float const EaseInOutCubic(float const& start, float const& end, float const& value);

    static float const EaseInQuart(float const& start, float const& end, float const& value);

    static float const EaseOutQuart(float const& start, float const& end, float const& value);

    static float const EaseInOutQuart(float const& start, float const& end, float const& value);

    static float const EaseInQuint(float const& start, float const& end, float const& value);

    static float const EaseOutQuint(float const& start, float const& end, float const& value);

    static float const EaseInOutQuint(float const& start, float const& end, float const& value);

    static float const EaseInSine(float const& start, float const& end, float const& value);

    static float const EaseOutSine(float const& start, float const& end, float const& value);

    static float const EaseInOutSine(float const& start, float const& end, float const& value);

    static float const EaseInExpo(float const& start, float const& end, float const& value);

    static float const EaseOutExpo(float const& start, float const& end, float const& value);

    static float const EaseInOutExpo(float const& start, float const& end, float const& value);

    static float const EaseInCirc(float const& start, float const& end, float const& value);

    static float const EaseOutCirc(float const& start, float const& end, float const& value);

    static float const EaseInOutCirc(float const& start, float const& end, float const& value);

    static float const EaseOutBounce(float const& start, float const& end, float const& value);

    static float const EaseInBounce(float const& start, float const& end, float const& value);


    static float const EaseInOutBounce(float const& start, float const& end, float const& value);

    static float const EaseInBack(float const& start, float const& end, float const& value);

    static float const EaseOutBack(float const& start, float const& end, float const& value);

    static float const EaseInOutBack(float const& start, float const& end, float const& value);

    static float const EaseInElastic(float const& start, float const& end, float const& value);

    static float const EaseOutElastic(float const& start, float const& end, float const& value);

    static float const EaseInOutElastic(float const& start, float const& end, float const& value);
}