/******************************************************************************/
/*!
\file   Texture.h
\author Team Popplio
\author Hafawati Binte Mohd Ali (100%)
\par    Course : CSD2400
\par    Section : A
\date   2024/10/02
\brief
		This header file declares the Texture class, which is used for handling
		textures in OpenGL. It includes functions for loading, using, and
		unbinding textures.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stb/stb_image.h>
#include <string>
#include <iostream>


namespace Popplio
{
	class Texture
	{
	public:

		/*!*****************************************************************************
		\brief
		Default constructor for the Texture class.

		*******************************************************************************/
		Texture();

		/*!*****************************************************************************
		\brief
		Constructor that loads a texture from the specified file path.

		\param[in] texturePath
		[std::string] Path to the texture file.

		*******************************************************************************/
		Texture(const std::string& texturePath);

		GLuint GetTextureID();

		/*!*****************************************************************************
		\brief
		Binds the texture for use in OpenGL.

		*******************************************************************************/
		void Use();

		/*!*****************************************************************************
		\brief
		Unbinds the texture in OpenGL.

		*******************************************************************************/
		void UnUse();

		/*!*************************************************************************
		\brief Returns the width of the texture.

		\return int - Width of the texture.
		*************************************************************************/
		int GetWidth() const { return width; }

		/*!*************************************************************************
		\brief Returns the height of the texture.

		\return int - Height of the texture.
		*************************************************************************/
		int GetHeight() const { return height; }

	private:
		GLuint textureID;
		int width;
		int height;
	};
}