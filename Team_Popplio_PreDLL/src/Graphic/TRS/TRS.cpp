/******************************************************************************/
/*!
\file   TRS.cpp
\author Team Popplio
\author Pearl Goh (100%)
\par    Course : CSD2401
\par    Section : A
\date   2024/10/03
\brief
		This source file defines class Sprite. It implements two functions.
		One is to update model matrix according to the TRS, and another is to render the sprite
		with the correct shader.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include "TRS.h"
#include "../../AssetStore/AssetStore.h"

namespace Popplio
{
	glm::mat4 Sprite::updateModelMatrix(glm::mat4 mode, glm::vec3 translation, float rotation, glm::vec3 scaling)
	{
		glm::mat4 model = mode;
		
		model = glm::translate(model, translation); // Translate
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate
		model = glm::scale(model, scaling); // Scale
		return model;
	}

	//void Sprite::RenderSprite(glm::mat4 modelMatrix, std::string shaderKey)
	//{
	//	//shaders["../shaders/default_shader"]->use();
	//	//AssetStore::GetShader(shaderKey).use();

	//	// Pass the model matrix to the shader
	//	//shaders["../shaders/default_shader"].SetUniform("model", modelMatrix);
	//	//AssetStore::GetShader(shaderKey).SetUniform("model", modelMatrix);
	//}
}