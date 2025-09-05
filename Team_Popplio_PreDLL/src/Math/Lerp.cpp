/******************************************************************************/
/*!
\file   Lerp.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
        Source file for linear and related interpolation

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h> // PCH

#include <stdlib.h>

// Warning disabled as this is just a library, there will be unused code
#pragma warning(disable: 4505) // C4505: Unreferenced local function has been removed

//#define _USE_MATH_DEFINES

//#define constexpr NATURAL_LOG_OF_2  0.693147181f            // Ln(2) 

//#define constexpr M_PI              3.14159265358979323846  // PI

namespace PopplioMath
{
    static float const Linear(float const& start, float const& end, float const& value)
    {
        return (1.f - value) * start + value * end;
    }


    static float const EaseInQuad(float const& start, float const& end, float const& value)
    {
        return (end - start) * value * value + start;
    }

    static float const EaseOutQuad(float const& start, float const& end, float const& value)
    {
        return -(end - start) * value * (value - 2) + start;
    }

    static float const EaseInOutQuad(float const& start, float const& end, float const& value)
    {
        float v = value / .5f;

        if (v < 1) return (end - start) * 0.5f * v * v + start;

        v--;

        return -(end - start) * 0.5f * (v * (v - 2) - 1) + start;
    }

    static float const EaseInCubic(float const& start, float const& end, float const& value)
    {
        return (end - start) * value * value * value + start;
    }

    static float const EaseOutCubic(float const& start, float const& end, float const& value)
    {
        float v = value - 1.f;
        return (end - start) * (v * v * v + 1) + start;
    }

    static float const EaseInOutCubic(float const& start, float const& end, float const& value)
    {
        float v = value / .5f;

        if (v < 1) return (end - start) * 0.5f * v * v * v + start;
        v -= 2;
        return (end - start) * 0.5f * (v * v * v + 2) + start;
    }

    static float const EaseInQuart(float const& start, float const& end, float const& value)
    {
        return (end - start) * value * value * value * value + start;
    }

    static float const EaseOutQuart(float const& start, float const& end, float const& value)
    {
        float v = value - 1;
        return -(end - start) * (v * v * v * v - 1) + start;
    }

    static float const EaseInOutQuart(float const& start, float const& end, float const& value)
    {
        float v = value / .5f;
        if (v < 1) return (end - start) * 0.5f * v * v * v * v + start;
        v -= 2;
        return -(end - start) * 0.5f * (v * v * v * v - 2) + start;
    }

    static float const EaseInQuint(float const& start, float const& end, float const& value)
    {
        return (end - start) * value * value * value * value * value + start;
    }

    static float const EaseOutQuint(float const& start, float const& end, float const& value)
    {
        float v = value - 1;
        return (end - start) * (v * v * v * v * v + 1) + start;
    }

    static float const EaseInOutQuint(float const& start, float const& end, float const& value)
    {
        float v = value / .5f;

        if (v < 1) return (end - start) * 0.5f * v * v * v * v * v + start;
        v -= 2;
        return (end - start) * 0.5f * (v * v * v * v * v + 2) + start;
    }

    static float const EaseInSine(float const& start, float const& end, float const& value)
    {
        return -(end - start) * cosf(value * (M_PI * 0.5f)) + end + start;
    }

    static float const EaseOutSine(float const& start, float const& end, float const& value)
    {
        return (end - start) * sinf(value * (M_PI * 0.5f)) + start;
    }

    static float const EaseInOutSine(float const& start, float const& end, float const& value)
    {
        return -(end - start) * 0.5f * (cosf(M_PI * value) - 1) + start;
    }

    static float const EaseInExpo(float const& start, float const& end, float const& value)
    {
        return (end - start) * powf(2, 10 * (value - 1)) + start;
    }

    static float const EaseOutExpo(float const& start, float const& end, float const& value)
    {
        return (end - start) * (-powf(2, -10 * value) + 1) + start;
    }

    static float const EaseInOutExpo(float const& start, float const& end, float const& value)
    {
        float v = value / .5f;
        if (v < 1) return (end - start) * 0.5f * powf(2, 10 * (v - 1)) + start;
        v--;
        return (end - start) * 0.5f * (-powf(2, -10 * v) + 2) + start;
    }

    static float const EaseInCirc(float const& start, float const& end, float const& value)
    {
        return -(end - start) * (sqrtf(1 - value * value) - 1) + start;
    }

    static float const EaseOutCirc(float const& start, float const& end, float const& value)
    {
        float v = value - 1;
        return (end - start) * sqrtf(1 - v * v) + start;
    }

    static float const EaseInOutCirc(float const& start, float const& end, float const& value)
    {
        float v = value / .5f;

        if (v < 1) return -(end - start) * 0.5f * (sqrtf(1 - v * v) - 1) + start;
        v -= 2;
        return (end - start) * 0.5f * (sqrtf(1 - v * v) + 1) + start;
    }

    static float const EaseOutBounce(float const& start, float const& end, float const& value)
    {
        float v = value / 1.f;

        if (v < (1 / 2.75f))
        {
            return (end - start) * (7.5625f * v * v) + start;
        }
        else if (v < (2 / 2.75f))
        {
            v -= (1.5f / 2.75f);
            return (end - start) * (7.5625f * (v)*v + .75f) + start;
        }
        else if (v < (2.5 / 2.75))
        {
            v -= (2.25f / 2.75f);
            return (end - start) * (7.5625f * (v)*v + .9375f) + start;
        }
        else
        {
            v -= (2.625f / 2.75f);
            return (end - start) * (7.5625f * (v)*v + .984375f) + start;
        }
    }

    static float const EaseInBounce(float const& start, float const& end, float const& value)
    {
        float d = 1.f;
        return (end - start) - EaseOutBounce(0, (end - start), d - value) + start;
    }



    static float const EaseInOutBounce(float const& start, float const& end, float const& value)
    {
        float d = 1.f;
        if (value < d * 0.5f) return EaseInBounce(0, (end - start), value * 2) * 0.5f + start;
        else return EaseOutBounce(0, (end - start), value * 2 - d) * 0.5f + (end - start) * 0.5f + start;
    }

    static float const EaseInBack(float const& start, float const& end, float const& value)
    {
        float v = value / 1;
        float s = 1.70158f;
        return (end - start) * (v)*v * ((s + 1) * v - s) + start;
    }

    static float const EaseOutBack(float const& start, float const& end, float const& value)
    {
        float s = 1.70158f;

        float v = (value)-1;
        return (end - start) * ((v)*v * ((s + 1) * v + s) + 1) + start;
    }

    static float const EaseInOutBack(float const& start, float const& end, float const& value)
    {
        float s = 1.70158f;

        float v = value / .5f;
        if ((v) < 1)
        {
            s *= (1.525f);
            return (end - start) * 0.5f * (v * v * (((s)+1) * v - s)) + start;
        }
        v -= 2;
        s *= (1.525f);
        return (end - start) * 0.5f * ((v)*v * (((s)+1) * v + s) + 2) + start;
    }

    static float const EaseInElastic(float const& start, float const& end, float const& value)
    {
        float d = 1.f;
        float p = d * .3f;
        float s;
        float a = 0;

        if (value == 0) return start;

        float v = value;

        if ((v /= d) == 1) return start + (end - start);

        if (a == 0.f || a < fabs(end - start))
        {
            a = (end - start);
            s = p / 4;
        }
        else
        {
            s = p / (2 * M_PI) * asinf((end - start) / a);
        }

        return -(a * powf(2, 10 * (v -= 1)) * sinf((v * d - s) * (2 * M_PI) / p)) + start;
    }

    static float const EaseOutElastic(float const& start, float const& end, float const& value)
    {
        float d = 1.f;
        float p = d * .3f;
        float s;
        float a = 0;

        if (value == 0) return start;
        float v = value;
        if ((v /= d) == 1) return start + (end - start);

        if (a == 0.f || a < fabs((end - start)))
        {
            a = (end - start);
            s = p * 0.25f;
        }
        else
        {
            s = p / (2 * M_PI) * asinf((end - start) / a);
        }

        return (a * powf(2, -10 * v) * sinf((v * d - s) * (2 * M_PI) / p) + (end - start) + start);
    }

    static float const EaseInOutElastic(float const& start, float const& end, float const& value)
    {
        float d = 1.f;
        float p = d * .3f;
        float s;
        float a = 0;

        if (value == 0) return start;
        float v = value;
        if ((v /= d * 0.5f) == 2) return start + (end - start);

        if (a == 0.f || a < fabs(end - start))
        {
            a = (end - start);
            s = p / 4;
        }
        else
        {
            s = p / (2 * M_PI) * asinf((end - start) / a);
        }

        if (v < 1) return -0.5f * (a * powf(2, 10 * (v -= 1)) * sinf((v * d - s) * (2 * M_PI) / p)) + start;
        return a * powf(2, -10 * (v -= 1)) * sinf((v * d - s) * (2 * M_PI) / p) * 0.5f + (end - start) + start;
    }
}