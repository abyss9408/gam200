/******************************************************************************/
/*!
\file   Font.h
\author Team Popplio
\author Hafawati Binte Mohd Ali (100%)
\par    Course : CSD2400
\par    Section : A
\date   2024/10/02
\brief
		This is the header file for the Font class, which is part of the main
		graphics system. It declares the Font class and its member functions
		used for loading and rendering fonts using the FreeType library and
		OpenGL.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "../Shader/Shader.h"
#include "../Camera/CameraManager.h"
#include "../Texture/Texture.h"

namespace Popplio
{;
	struct Character {
		unsigned int TextureID;  // OpenGL texture ID
		glm::ivec2   Size;      // Size of glyph
		glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
		unsigned int Advance;    // Offset to advance to next glyph
	};

	class Font
	{
	public:
		Font(const std::string& path);
		Font(const Font&) = delete;
		Font& operator=(const Font&) = delete;
		~Font();

		void LoadSize(unsigned int fontSize);
		bool IsSizeLoaded(unsigned int fontSize) const;
		const Character& GetCharacter(char c, unsigned int fontSize) const;

		unsigned int GetVAO() const { return VAO; }
		unsigned int GetVBO() const { return VBO; }
	private:
		std::string fontPath;
		std::map<unsigned int, std::map<char, Character>> sizedCharacters;
		unsigned int VAO, VBO;
	};
}