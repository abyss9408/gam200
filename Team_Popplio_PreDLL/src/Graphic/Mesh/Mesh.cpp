/******************************************************************************/
/*!
\file   Mesh.cpp
\author Team Popplio
\author Hafawati Binte Mohd Ali (100%)
\par    Course : CSD2400
\par    Section : A
\date   2025/03/12 09:50:22 AM (Last Edit)
\brief
		This source file defines various mesh creation functions for the graphics
		system, including functions to create triangle, quad, circle, points, lines,
		and background meshes.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#define _USE_MATH_DEFINES

#include "Mesh.h"
#include "../RenderSystem.h"
#include <cmath>


namespace Popplio
{
	void Mesh::SetupInstancing()
	{
		glGenBuffers(1, &instanceVBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

		// Allocate buffer space
		glBufferData(GL_ARRAY_BUFFER,
			sizeof(InstanceData) * MAX_INSTANCES, // Define MAX_INSTANCES as needed
			nullptr,
			GL_DYNAMIC_DRAW);

		// Set up matrix attributes (locations 3,4,5)
		size_t vec3Size = sizeof(glm::vec3);
		size_t matrixSize = sizeof(glm::mat3);
		size_t floatSize = sizeof(float);       

		// Matrix columns
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), 
			reinterpret_cast<void*>(0));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), 
			reinterpret_cast<void*>(vec3Size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), 
			reinterpret_cast<void*>(2 * vec3Size));

		// Color attribute (location 6)
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
			reinterpret_cast<void*>(matrixSize));

		// Instance alpha (location 7)
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(sizeof(glm::mat3) + vec3Size));

		// Instance UV rect (location 8)
		glEnableVertexAttribArray(8);
		glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(sizeof(glm::mat3) + vec3Size + floatSize));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
		glVertexAttribDivisor(8, 1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	/*!*****************************************************************************
	\brief
	Creates a triangle mesh.

	\return
	[Mesh] The created triangle mesh.

	*******************************************************************************/
	Mesh CreateTriangleMesh()
	{
		Mesh triangleMesh;
		triangleMesh.vertices = {
			{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			{{ 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
			{{ 0.0f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f}}
		};
		triangleMesh.indices = { 0, 1, 2 };

		glGenVertexArrays(1, &triangleMesh.VAO);
		glGenBuffers(1, &triangleMesh.VBO);
		glGenBuffers(1, &triangleMesh.EBO);

		glBindVertexArray(triangleMesh.VAO);

		glBindBuffer(GL_ARRAY_BUFFER, triangleMesh.VBO);
		glBufferData(GL_ARRAY_BUFFER, triangleMesh.vertices.size() * sizeof(Vertex), &triangleMesh.vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleMesh.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleMesh.indices.size() * sizeof(unsigned int),
			&triangleMesh.indices[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex colour
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

		//// Set up instanced array, tbc in graphic system
		//unsigned int instanceVBO;
		//glGenBuffers(1, &instanceVBO);
		//glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		//glBufferData(GL_ARRAY_BUFFER, 2500 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

		//// Set up instanced array attributes
		//std::size_t vec4Size = sizeof(glm::vec4);
		//glEnableVertexAttribArray(2);
		//glEnableVertexAttribArray(3);
		//glEnableVertexAttribArray(4);
		//glEnableVertexAttribArray(5);
		//glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * static_cast<GLsizei>(vec4Size), (void*)0);
		//glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * static_cast<GLsizei>(vec4Size), (void*)(vec4Size));
		//glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * static_cast<GLsizei>(vec4Size), (void*)(2 * vec4Size));
		//glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * static_cast<GLsizei>(vec4Size), (void*)(3 * vec4Size));
		//glVertexAttribDivisor(2, 1);
		//glVertexAttribDivisor(3, 1);
		//glVertexAttribDivisor(4, 1);
		//glVertexAttribDivisor(5, 1);

		glBindVertexArray(0);

		return triangleMesh;
	}


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
	Mesh CreateQuadMesh()
	{
		Mesh quadMesh;
		quadMesh.vertices = {
			{{-0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{ 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
			{{ 0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{{-0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
		};
		quadMesh.indices = { 0, 1, 2, 2, 3, 0 };

		glGenVertexArrays(1, &quadMesh.VAO);
		glGenBuffers(1, &quadMesh.VBO);
		glGenBuffers(1, &quadMesh.EBO);

		glBindVertexArray(quadMesh.VAO);

		glBindBuffer(GL_ARRAY_BUFFER, quadMesh.VBO);
		glBufferData(GL_ARRAY_BUFFER, quadMesh.vertices.size() * sizeof(Vertex), &quadMesh.vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadMesh.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, quadMesh.indices.size() * sizeof(unsigned int), &quadMesh.indices[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		// vertex colour
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
		
		glBindVertexArray(0);

		return quadMesh;
	}

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
	Mesh CreateCircleMesh()
	{
		Mesh circleMesh;
		const int numSegments = 32;
		const float radius = 0.5f;

		circleMesh.vertices.push_back({ {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.5f, 0.5f} });

		for (int i = 0; i <= numSegments; ++i)
		{
			float angle = 2.0f * static_cast<float>(M_PI) * i / numSegments;
			float x = radius * cosf(angle);
			float y = radius * sinf(angle);
			circleMesh.vertices.push_back({ {x, y}, {1.0f, 1.0f, 1.0f}, {(x + 1.0f) / 2.0f, (y + 1.0f) / 2.0f} });

			if (i > 0)
			{
				circleMesh.indices.push_back(0);
				circleMesh.indices.push_back(i);
				circleMesh.indices.push_back(i + 1);
			}
		}

		glGenVertexArrays(1, &circleMesh.VAO);
		glGenBuffers(1, &circleMesh.VBO);
		glGenBuffers(1, &circleMesh.EBO);

		glBindVertexArray(circleMesh.VAO);

		glBindBuffer(GL_ARRAY_BUFFER, circleMesh.VBO);
		glBufferData(GL_ARRAY_BUFFER, circleMesh.vertices.size() * sizeof(Vertex), &circleMesh.vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, circleMesh.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, circleMesh.indices.size() * sizeof(unsigned int), &circleMesh.indices[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex colors
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

		glBindVertexArray(0);

		return circleMesh;

	}

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
	//Mesh Points(Vertex minPos, Vertex maxPos)
	//{
	//	unsigned int VAO, VBO, EBO;
	//	glGenVertexArrays(1, &VAO);
	//	glGenBuffers(1, &VBO);
	//	glGenBuffers(1, &EBO);

	//	glBindVertexArray(VAO);
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//	Vertex vert1, vert2, vert3, vert4;

	//	vert1.position = { maxPos.position.x, minPos.position.y, 1.0f };
	//	vert2.position = { maxPos.position.x,maxPos.position.y, 1.0f };
	//	vert3.position = { minPos.position.x, maxPos.position.y, 1.0f };
	//	vert4.position = { minPos.position.x,minPos.position.y, 1.0f };

	//	vert1.color = { 1.0f,0.0f,0.0f };
	//	vert2.color = { 1.0f,0.0f,0.0f };
	//	vert3.color = { 1.0f,0.0f,0.0f };
	//	vert4.color = { 1.0f,0.0f,0.0f };

	//	std::vector<Vertex> vertices = { vert1, vert2, vert3, vert4 };

	//	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	//	std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };


	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
	//		&indices[0], GL_STATIC_DRAW);

	//	// vertex positions
	//	glEnableVertexAttribArray(0);
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	//	// vertex colour
	//	glEnableVertexAttribArray(1);
	//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	//	glBindVertexArray(0);

	//	Mesh pointsMesh;
	//	pointsMesh.VBO = VBO;
	//	pointsMesh.VAO = VAO;
	//	pointsMesh.EBO = EBO;
	//	pointsMesh.primitive_type = GL_POINTS;
	//	pointsMesh.indices_size = (unsigned int)indices.size();
	//	pointsMesh.vertices_size = (unsigned int)vertices.size();
	//	return pointsMesh;
	//}

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
	//Mesh Lines(Vertex minPos, Vertex maxPos)
	//{
	//	unsigned int VAO, VBO, EBO;
	//	glGenVertexArrays(1, &VAO);
	//	glGenBuffers(1, &VBO);
	//	glGenBuffers(1, &EBO);

	//	glBindVertexArray(VAO);
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//	Vertex vert1, vert2, vert3, vert4;

	//	vert1.position = { maxPos.position.x, minPos.position.y, 1.0f };
	//	vert2.position = { maxPos.position.x,maxPos.position.y, 1.0f };
	//	vert3.position = { minPos.position.x, maxPos.position.y, 1.0f };
	//	vert4.position = { minPos.position.x,minPos.position.y, 1.0f };

	//	vert1.color = { 1.0f,0.0f,0.0f };
	//	vert2.color = { 1.0f,0.0f,0.0f };
	//	vert3.color = { 1.0f,0.0f,0.0f };
	//	vert4.color = { 1.0f,0.0f,0.0f };

	//	std::vector<Vertex> vertices;

	//	//pushing the vertices in an anti-clockwise order

	//	//Left Line
	//	vertices.push_back(vert1);
	//	vertices.push_back(vert2);

	//	//Top Line
	//	vertices.push_back(vert2);
	//	vertices.push_back(vert3);

	//	//Right Line
	//	//Right Line
	//	vertices.push_back(vert3);
	//	vertices.push_back(vert4);

	//	//Bottom Line
	//	vertices.push_back(vert4);
	//	vertices.push_back(vert1);

	//	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	//	std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };


	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
	//		&indices[0], GL_STATIC_DRAW);

	//	// vertex positions
	//	glEnableVertexAttribArray(0);
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	//	// vertex colour
	//	glEnableVertexAttribArray(1);
	//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	//	glBindVertexArray(0);

	//	Mesh linesMesh;
	//	linesMesh.VBO = VBO;
	//	linesMesh.VAO = VAO;
	//	linesMesh.EBO = EBO;
	//	linesMesh.primitive_type = GL_LINES;
	//	linesMesh.indices_size = (unsigned int)indices.size();
	//	linesMesh.vertices_size = (unsigned int)vertices.size();
	//	return linesMesh;
	//}

	/*!*****************************************************************************
	\brief
	Creates a background mesh.

	\return
	[Mesh] The created background mesh.

	*******************************************************************************/
}