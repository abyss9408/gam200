/******************************************************************************/
/*!
\file   Texture.cpp
\author Team Popplio
\author Hafawati Binte Mohd Ali (100%)
\par    Course : CSD2400
\par    Section : A
\date   2024/10/02
\brief
		This source file defines the Texture class, which is used for handling
		textures in OpenGL. It includes functions for loading, using, and
		unbinding textures.

		Known issues:

		- Can't get data for images of 24 bit depth

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include "Texture.h"

#include "../Engine/Engine.h"

namespace Popplio
{
	/*!*****************************************************************************
	\brief
	Default constructor for the Texture class.

	*******************************************************************************/
	Texture::Texture() : textureID(0)
	{
		// default constructor
	}

	/*!*****************************************************************************
	\brief
	Constructor that loads a texture from the specified file path.

	\param[in] texturePath
	[std::string] Path to the texture file.

	*******************************************************************************/
	Texture::Texture(const std::string& texturePath)
	{
		const char* path = texturePath.c_str();

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// load and generate the texture
		int nrChannels;
		stbi_set_flip_vertically_on_load(true); //flip loaded texture's on the y-axis
		unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
		if (data)
		{
			// to use .png, set to GL_RGBA
			// to use .jpg, set to GL_RGB
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			if (Popplio::Engine::Config::verbose)
				Logger::Info("Successfully loaded texture: " + static_cast<std::string>(path));

		}
		else
		{
			Logger::Error("Failed to load texture");
			width = 0;
			height = 0;
		}
		stbi_image_free(data);

		textureID = texture;
	}

	/*!*****************************************************************************
	\brief
	Binds the texture for use in OpenGL.

	*******************************************************************************/
	/*void Texture::Use()
	{
		glBindTexture(GL_TEXTURE_2D, textureID);
	}*/

	/*!*****************************************************************************
	\brief
	Unbinds the texture in OpenGL.

	*******************************************************************************/
	/*void Texture::UnUse()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}*/

	GLuint Texture::GetTextureID()
	{
		return textureID;
	}
}