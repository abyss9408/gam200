/******************************************************************************/
/*!
\file   TRS.h
\author Team Popplio
\author Pearl Goh (100%)
\par    Course : CSD2401
\par    Section : A
\date   2024/10/03
\brief
		This header file declares class Sprite which has Mesh struct, and two functions.
		One is to update model matrix according to the TRS, and another is to render the sprite
		with the correct shader.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>
#include "../Shader/Shader.h"
#include "../Mesh/Mesh.h"


namespace Popplio
{
	class Sprite
	{
	public:
		Mesh mesh;

		glm::mat4 updateModelMatrix(glm::mat4 model, glm::vec3 translation, float rotation, glm::vec3 scaling);

		//void RenderSprite(glm::mat4 modelMatrix, std::string shaderKey);
	};
}