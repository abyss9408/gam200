/******************************************************************************/
/*!
\file   Color.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
        Header file for Color class
        Represents a color in RGBA format
        Can be used with Hex values & HSL values
        Has helper functions for color manipulation

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

namespace Popplio
{
#ifdef _MSC_VER
    // Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif
    struct Color
    {
        // Data

        struct
        {
            float r, g, b, a;
        };

        static float const MAX_INT_VALUE;
        static float const MAX_FLOAT_VALUE;

        // ++++++++++++++++++++++++++++++++++++++++++++ //

        // Constructors

        Color();
        Color(int const& red, int const& green, int const& blue, int const& alpha = 255);
        Color(float const& red, float const& green, float const& blue, float const& alpha = 1.f);
        Color(uint32_t hex);
        ~Color() = default;

        // ++++++++++++++++++++++++++++++++++++++++++++ //

		glm::vec3 ToGLMVec3() const;

        // ++++++++++++++++++++++++++++++++++++++++++++ //

        // Helper Functions

        Color& Zero(); // zero out all values
        Color& One(); // set all values to 1

        Color Blend(Color const& other, float const& factor) const;

        Color operator+(Color const& other) const;
        Color operator-(Color const& other) const;

        Color operator*(float const& scalar) const;
        Color operator*(Color const& other) const;

        uint32_t GetHex() const;
        uint32_t SetHex(uint32_t const& hex);

        float HueToRGB(float const& p, float const& q, float const& t) const;

        void GetHSL(float& h, float& s, float& l) const;
        void SetHSL(float const& h, float const& s, float const& l);

        void GetInt(int& red, int& green, int& blue, int& alpha) const;
        void SetInt(int const& red, int const& green, int const& blue, int const& alpha);

        void Clamp();

        //void Normalize();
        //void Denormalize();

        Color GrayScale() const;

        bool operator==(Color const& other) const;
        bool operator!=(Color const& other) const;

        std::string ToString(int const& option = 0) const;

        // ++++++++++++++++++++++++++++++++++++++++++++ //

        // Common Colors

        static const Color White;
        static const Color Black;
        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Yellow;
        static const Color Cyan;
        static const Color Magenta;
        static const Color Gray;
        static const Color LightGray;
        static const Color DarkGray;
        static const Color Transparent;
    };
#ifdef _MSC_VER
    // Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( default : 4201 )
#endif
}