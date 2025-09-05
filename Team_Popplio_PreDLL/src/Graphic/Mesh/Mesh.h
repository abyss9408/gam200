/******************************************************************************/
/*!
\file   Mesh.h
\author Team Popplio
\author Hafawati Binte Mohd Ali (100%)
\par    Course : CSD2400
\par    Section : A
\date   2024/10/02
\brief
		This header file declares various mesh creation functions for the graphics
		system, including functions to create triangle, quad, circle, points, lines,
		and background meshes.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>

namespace Popplio
{
	/*!*****************************************************************************
	\brief
	Structure representing a vertex, which includes position, color, and texture
	coordinates.

	*******************************************************************************/
	struct Vertex
	{
		glm::vec2 position;
		glm::vec3 color;
		glm::vec2 texCoords;
	};

	/*!*****************************************************************************
	\brief
	Structure representing a mesh, which includes VAO, VBO, EBO, primitive type,
	indices size, vertices size, and minimum and maximum position vertices.

	*******************************************************************************/
	struct Mesh
	{
		unsigned int VAO, VBO, EBO;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		// Fields for instancing
		GLuint instanceVBO;
		static const size_t MAX_INSTANCES = 10000; // Adjust as needed

		Mesh() : VAO(0), VBO(0), EBO(0), instanceVBO(0) {}

		void SetupInstancing();
	};

	/*!*****************************************************************************
	\brief
	Creates a triangle mesh.

	\return
	[Mesh] The created triangle mesh.

	*******************************************************************************/
	Mesh CreateTriangleMesh();

	/*!*****************************************************************************
	\brief
	Creates a quad mesh.

	\param[in] minPos
	[Vertex] The minimum position vertex.

	\param[in] maxPos
	[Vertex] The maximum position vertex.

	\return
	[Mesh] The created quad mesh.

	*******************************************************************************/
	Mesh CreateQuadMesh();

	/*!*****************************************************************************
	\brief
	Creates a circle mesh.

	\param[in] minPos
	[Vertex] The minimum position vertex.

	\param[in] maxPos
	[Vertex] The maximum position vertex.

	\return
	[Mesh] The created circle mesh.

	*******************************************************************************/
	Mesh CreateCircleMesh();

	/*!*****************************************************************************
	\brief
	Creates a point mesh.

	\param[in] minPos
	[Vertex] The minimum position vertex.

	\param[in] maxPos
	[Vertex] The maximum position vertex.

	\return
	[Mesh] The created point mesh.

	*******************************************************************************/
	//extern Mesh Points(Vertex minPos, Vertex maxPos);
	
	/*!*****************************************************************************
	\brief
	Creates a line mesh.

	\param[in] minPos
	[Vertex] The minimum position vertex.

	\param[in] maxPos
	[Vertex] The maximum position vertex.

	\return
	[Mesh] The created line mesh.

	*******************************************************************************/
	//extern Mesh Lines(Vertex minPos, Vertex maxPos);

	/*!*****************************************************************************
	\brief
	Creates a background mesh.

	\return
	[Mesh] The created background mesh.

	*******************************************************************************/
}