/******************************************************************************/
/*!
\file   Font.cpp
\author Team Popplio
\author Hafawati Binte Mohd Ali (100%)
\par    Course : CSD2400
\par    Section : A
\date   2024/10/02
\brief
        This is the source file for the Font class, which is part of the main
        graphics system. It defines the Font class and its member functions
        used for loading and rendering fonts using the FreeType library and
        OpenGL.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include "font.h"

namespace Popplio
{
    // Initialize static members

	Font::Font(const std::string& path) : VAO(0), VBO(0), fontPath(path)
	{
        // configure VAO/VBO for texture quads
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
	}

	Font::~Font()
	{
		// Clean up textures
		for (auto& sizeMap : sizedCharacters) {
			for (auto& charPair : sizeMap.second) {
				glDeleteTextures(1, &charPair.second.TextureID);
			}
		}
	}

	void Font::LoadSize(unsigned int fontSize)
	{
        if (sizedCharacters.find(fontSize) != sizedCharacters.end())
            return; // Already loaded

        FT_Library ft;
        if (FT_Init_FreeType(&ft)) // all functions return a value different than 0 whenever an error occurred
        {
            Logger::Critical("FREETYPE: Could not init FreeType Library");
        }

        // load font as face
        FT_Face face;
        if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
        {
            Logger::Error("FREETYPE: Failed to load font");
        }

        FT_Set_Pixel_Sizes(face, 0, fontSize);
        std::map<char, Character>& characters = sizedCharacters[fontSize];
        // Disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        // Load first 128 ASCII characters
        for (unsigned char c = 0; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                Logger::Warning("Failed to load glyph: " + std::string(1, c));
                continue;
            }

            // Generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D
            (
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );

            // Set texture parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Store character
            Character character = 
            {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            characters.insert(std::pair<char, Character>(c, character));
        }

        // destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
	}

    bool Font::IsSizeLoaded(unsigned int fontSize) const
    {
        return sizedCharacters.find(fontSize) != sizedCharacters.end();
    }

	const Character& Font::GetCharacter(char c, unsigned int fontSize) const
	{
        auto sizeIt = sizedCharacters.find(fontSize);
        if (sizeIt == sizedCharacters.end())
            throw std::runtime_error("Font size not loaded: " + std::to_string(fontSize));

        auto charIt = sizeIt->second.find(c);
        if (charIt == sizeIt->second.end())
            throw std::runtime_error("Character not found: " + std::string(1, c));

        return charIt->second;
	}
}
