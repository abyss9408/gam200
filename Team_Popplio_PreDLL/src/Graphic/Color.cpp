/******************************************************************************/
/*!
\file   Color.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
        Source file for Color class
        Represents a color in RGBA format
        Can be used with Hex values & HSL values
        Has helper functions for color manipulation

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>

#include "Color.h"

#include <algorithm>

namespace Popplio
{
    float const Color::MAX_INT_VALUE = 255.f;
    float const Color::MAX_FLOAT_VALUE = 1.f;

    Color::Color() : r(1.f), g(1.f), b(1.f), a(1.f) 
    { Clamp(); }

    Color::Color(int const& red, int const& green, int const& blue, int const& alpha) : 
        r(static_cast<float>(red) / MAX_INT_VALUE), g(static_cast<float>(green) / MAX_INT_VALUE), 
        b(static_cast<float>(blue) / MAX_INT_VALUE), a(static_cast<float>(alpha) / MAX_INT_VALUE)
    { Clamp(); }

	Color::Color(float const& red, float const& green, float const& blue, float const& alpha) : 
        r(red), g(green), b(blue), a(alpha) 
    { Clamp(); }

    Color::Color(uint32_t hex) : r(((hex >> 24) & 0xFF) / MAX_INT_VALUE), g(((hex >> 16) & 0xFF) / MAX_INT_VALUE), 
        b(((hex >> 8) & 0xFF) / MAX_INT_VALUE), a((hex & 0xFF) / MAX_INT_VALUE) 
    { Clamp(); }

	glm::vec3 Color::ToGLMVec3() const { return glm::vec3(r, g, b); }

    Color& Color::Zero()
    {
        r = g = b = a = 0.f;
        return *this;
    }
    Color& Color::One()
    {
        r = g = b = a = 1.f;
        return *this;
    }

    Color Color::Blend(Color const& other, float const& factor) const
    {
        return Color(r + (other.r - r) * factor, g + (other.g - g) * factor,
            b + (other.b - b) * factor, a + (other.a - a) * factor);
    }

    Color Color::operator+(Color const& other) const
    {
        return Color(r + other.r, g + other.g, b + other.b, a + other.a);
    }
    Color Color::operator-(Color const& other) const
    {
        return Color(r - other.r, g - other.g, b - other.b, a - other.a);
    }

    Color Color::operator*(float const& scalar) const
    {
        return Color(r * scalar, g * scalar, b * scalar, a * scalar);
    }
    Color Color::operator*(Color const& other) const
    {
        return Color(r * other.r, g * other.g, b * other.b, a * other.a);
    }

    uint32_t Color::GetHex() const
    {
        return (static_cast<uint32_t>(r * MAX_INT_VALUE) << 24) | (static_cast<uint32_t>(g * MAX_INT_VALUE) << 16) |
            (static_cast<uint32_t>(b * MAX_INT_VALUE) << 8) | static_cast<uint32_t>(a * MAX_INT_VALUE);
    }
    uint32_t Color::SetHex(uint32_t const& hex)
    {
        r = ((hex >> 24) & 0xFF) / MAX_INT_VALUE;
        g = ((hex >> 16) & 0xFF) / MAX_INT_VALUE;
        b = ((hex >> 8) & 0xFF) / MAX_INT_VALUE;
        a = (hex & 0xFF) / MAX_INT_VALUE;

        Clamp();

        return hex;
    }

    // https://www.programmingalgorithms.com/algorithm/rgb-to-hsl/cpp/
    // https://stackoverflow.com/questions/2353211/hsl-to-rgb-color-conversion

    float Color::HueToRGB(float const& p, float const& q, float const& t) const
    {
        float t1 = t;

        if (t1 < 0.f) t1 += 1.f;
        if (t1 > 1.f) t1 -= 1.f;
        if (t1 < 1.f / 6.f) return p + (q - p) * 6.f * t1;
        if (t1 < 1.f / 2.f) return q;
        if (t1 < 2.f / 3.f) return p + (q - p) * (2.f / 3.f - t1) * 6.f;

        return p;
    }

    void Color::GetHSL(float& h, float& s, float& l) const
    {
        float max = std::max(r, std::max(g, b));
        float min = std::min(r, std::min(g, b));

        l = (max + min) / 2.f;

        if (max == min)
        {
            h = s = 0.f; // achromatic
        }
        else
        {
            float d = max - min;
            s = ((l > 0.5f) ? (d / (2.f - max - min)) : (d / (max + min)));
            if (max == r)
            {
                h = ((g - b) / 6.f) / d;
            }
            else if (max == g)
            {
                h = (1.f / 3.f) + ((b - r) / 6.f) / d;
            }
            else
            {
                h = (2.f / 3.f) + ((r - g) / 6.f) / d;
            }

            if (h < 0.f) h += 1.f;

            if (h > 1.f) h -= 1.f;
        }
    }

    void Color::SetHSL(float const& h, float const& s, float const& l)
    {
        if (s == 0.f)
        {
            r = g = b = l; // achromatic
        }
        else
        {
            float q = (l < 0.5f) ? (l * (1.f + s)) : (l + s - l * s);
            float p = 2.f * l - q;
            r = HueToRGB(p, q, h + 1.f / 3.f);
            g = HueToRGB(p, q, h);
            b = HueToRGB(p, q, h - 1.f / 3.f);
        }

        Clamp();
    }

    void Color::GetInt(int& red, int& green, int& blue, int& alpha) const
    {
        red = static_cast<int>(r * MAX_INT_VALUE);
        green = static_cast<int>(g * MAX_INT_VALUE);
        blue = static_cast<int>(b * MAX_INT_VALUE);
        alpha = static_cast<int>(a * MAX_INT_VALUE);
    }
    void Color::SetInt(int const& red, int const& green, int const& blue, int const& alpha)
    {
        r = static_cast<float>(red) / MAX_INT_VALUE;
        g = static_cast<float>(green) / MAX_INT_VALUE;
        b = static_cast<float>(blue) / MAX_INT_VALUE;
        a = static_cast<float>(alpha) / MAX_INT_VALUE;

        Clamp();
    }

    void Color::Clamp()
    {
        r = std::clamp(r, 0.f, 1.f);
        g = std::clamp(g, 0.f, 1.f);
        b = std::clamp(b, 0.f, 1.f);
        a = std::clamp(a, 0.f, 1.f);
    }

    // Gray=0.299×R+0.587×G+0.114×B

    Color Color::GrayScale() const
    {
        float gray = 0.299f * r + 0.587f * g + 0.114f * b;
        return Color(gray, gray, gray, a);
    }

    bool Color::operator==(Color const& other) const
    {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }
    bool Color::operator!=(Color const& other) const
    {
        return !(*this == other);
    }

    std::string Color::ToString(int const& option) const
    {
        switch (option)
        {
            case 0: // float RGBA
            return "RGBA: " + std::to_string(r) + ", " + std::to_string(g) + ", " + 
                std::to_string(b) + ", " + std::to_string(a);
            case 1: // int RGBA
            int red, green, blue, alpha;
            GetInt(red, green, blue, alpha);
            return "RGBA: " + std::to_string(red) + ", " + std::to_string(green) + ", " + 
                std::to_string(blue) + ", " + std::to_string(alpha);
            case 2: // HEX | hexadecimal
            return "HEX: " + std::to_string(GetHex());
            case 3: // HSL | Hue, Saturation, Lightness
            float h, s, l;
            GetHSL(h, s, l);
            return "HSL: " + std::to_string(h) + ", " + std::to_string(s) + ", " + std::to_string(l);
            default:
            return "RGBA: " + std::to_string(r) + ", " + std::to_string(g) + ", " +
                std::to_string(b) + ", " + std::to_string(a);
        }
    }

    const Color Color::White =          Color(1.f, 1.f, 1.f, 1.f);
    const Color Color::Black =          Color(0.f, 0.f, 0.f, 1.f);
    const Color Color::Red =            Color(1.f, 0.f, 0.f, 1.f);
    const Color Color::Green =          Color(0.f, 1.f, 0.f, 1.f);
    const Color Color::Blue =           Color(0.f, 0.f, 1.f, 1.f);
    const Color Color::Yellow =         Color(1.f, 1.f, 0.f, 1.f);
    const Color Color::Cyan =           Color(0.f, 1.f, 1.f, 1.f);
    const Color Color::Magenta =        Color(1.f, 0.f, 1.f, 1.f);
    const Color Color::Gray =           Color(0.5f, 0.5f, 0.5f, 1.f);
    const Color Color::LightGray =      Color(0.75f, 0.75f, 0.75f, 1.f);
    const Color Color::DarkGray =       Color(0.25f, 0.25f, 0.25f, 1.f);
    const Color Color::Transparent =    Color(0.f, 0.f, 0.f, 0.f);
}