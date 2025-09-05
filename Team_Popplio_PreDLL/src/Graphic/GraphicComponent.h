/******************************************************************************/
/*!
\file		GraphicsComponent.h
\author		Team Popplio
\author		Hafawati Binte Mohd Ali (100%)
\par		Course : CSD2450 
\par		Section : A
\date		2025/03/23
\brief
		This header file defines various components used in the graphics system,
		including render components, debug render components, and text components.
		It also defines enumerations for different mesh types and debug mesh types.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

namespace Popplio
{
	/*!*****************************************************************************
	\brief
	Enumeration for different types of meshes.

	*******************************************************************************/
	enum MeshType
	{
		Triangle,
		Quad,
		Circle,
		FullscreenQuad
	};

	/*!*****************************************************************************
	\brief
	Enumeration for different types of debug meshes.

	*******************************************************************************/
	/*!*****************************************************************************
	\brief
	Structure representing a render component, which includes mesh reference,
	shader mode, color, shader reference, and texture reference.

	*******************************************************************************/
	struct RenderComponent
	{
		bool isActive;

		MeshType meshType;
		std::string shaderName;
		std::string textureName;
		bool useInstancing;
		glm::vec3 color;
		float alpha;
		int textureWidth;
		int textureHeight;

		// 9-Slice Scaling
		bool enableNineSlice;
		float borderLeft;
		float borderRight;
		float borderTop;
		float borderBottom;

		/*!*****************************************************************************
		\brief
		Constructor for RenderComponent.

		\param[in] meshRef
		[meshType] Reference to the mesh type.

		\param[in] ShdMode
		[GLint] Shader mode.

		*******************************************************************************/
		RenderComponent(MeshType type = MeshType::Quad, std::string shader = "default_shader", std::string texName = "",
			bool instanced = false, glm::vec3 col = glm::vec3(1.0f), float al = 1.0f, int texWidth = 0, int texHeight = 0, 
			bool nineSlice = false, float bLeft = 1.0f, float bRight = 1.0f, float  bTop = 1.0f, float bBot = 1.0f)
			: isActive(true), meshType(type), shaderName(shader), textureName(texName), useInstancing(instanced), color(col), alpha(al)
			, textureWidth(texWidth), textureHeight(texHeight), enableNineSlice(nineSlice), borderLeft(bLeft), borderRight(bRight),
			borderTop(bTop), borderBottom(bBot)
		{
		}
	};

	/*!*****************************************************************************
	\brief
	Structure representing a debug render component, which includes mesh reference,
	color, and shader reference.

	*******************************************************************************/
#if 0
	struct DebugRenderComponent
	{
		bool drawCollider;
		glm::vec3 colliderColor;

		/*!*****************************************************************************
		\brief
		Constructor for RenderDebugComponent.

		\param[in] meshRef
		[debugMeshType] Reference to the debug mesh type.

		*******************************************************************************/
		DebugRenderComponent(bool draw = false, glm::vec3 col = glm::vec3(1.f, 0.f, 0.f))
			: drawCollider(draw), colliderColor(col) {}
	};
#endif
	/*!*****************************************************************************
	\brief
	Structure representing a text component, which includes shader reference, font
	reference, text, position, scale, and color.

	*******************************************************************************/

	struct TextComponent
	{
		bool isActive;

		std::string text;
		std::string fontName;
		unsigned int fontSize;
		glm::vec3 color;
		float alpha;

		TextComponent(
			const std::string& t = "",
			const std::string& font = "default",
			unsigned int size = 32,
			float al = 1.0f,
			glm::vec3 col = glm::vec3(1.0f)
		); //: isActive(true), text(t), fontName(font), fontSize(size), alpha(al), color(col) {}

		float GetWidth() const;
		float GetHeight() const;
	};
}