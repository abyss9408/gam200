
/******************************************************************************/
/*!
\file       TextComponent.cpp
\author     Team Popplio
\author     Hafawati Binte Mohd Ali (100%)
\par        Course     : CSD2450
\par        Section    : A
\date       2025/04/06
\brief
    This source file implements the functionality for the TextComponent class
    in the Popplio engine. It defines how text width and height are calculated
    using font metrics, and handles font loading and fallback behavior in case
    of missing fonts or sizes.

    The class interacts with the AssetStore to fetch font resources, calculates
    character dimensions for width and height metrics, and supports alpha and
    color properties used for text rendering.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>
#include "GraphicComponent.h"   

namespace Popplio
{
    TextComponent::TextComponent(
        const std::string& t,
        const std::string& font,
        unsigned int size,
        float al,
        glm::vec3 col
    )
        : isActive(true), text(t), fontName(font), fontSize(size), alpha(al), color(col)
    {
    }

    float TextComponent::GetWidth() const
    {
        Font* font = AssetStore::GetFont(fontName);
        if (!font)
        {
            Logger::Warning("Font not found: " + fontName);
            return 0.0f;
        }

        if (!font->IsSizeLoaded(fontSize))
        {
            font->LoadSize(fontSize);
        }

        float totalWidth = 0.0f;
        for (char c : text)
        {
            const Character& ch = font->GetCharacter(c, fontSize);
            totalWidth += static_cast<float>(ch.Advance >> 6);
        }
        return totalWidth;
    }

    float TextComponent::GetHeight() const
    {
        Font* font = AssetStore::GetFont(fontName);
        if (!font)
        {
            Logger::Warning("Font not found: " + fontName);
            return 0.0f;
        }

        if (!font->IsSizeLoaded(fontSize))
        {
            font->LoadSize(fontSize);
        }

        float maxAscent = 0.0f;
        float maxDescent = 0.0f;

        for (char c : text)
        {
            const Character& ch = font->GetCharacter(c, fontSize);
            maxAscent = std::max(maxAscent, static_cast<float>(ch.Bearing.y));
            float descent = static_cast<float>(ch.Size.y) - static_cast<float>(ch.Bearing.y);
            maxDescent = std::max(maxDescent, descent);
        }

        return maxAscent + maxDescent;
    }

}
