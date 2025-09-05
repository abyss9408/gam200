/******************************************************************************/
/*!
\file		RenderSystem.cpp
\author		Team Popplio
\author		Hafawati Binte Mohd Ali (90%)
\co-author	Pearl Goh (5%) | Ng Jun Heng Shawn (5%)
\par		Course : CSD2450 / CSD2451
\par		Section : A
\date       2025/04/06 01:39:42 PM (Last Edit)
\brief
		This source file contains declaration of the RenderSystem class, which is responsible
		for initializing and managing the graphics system, including loading
		and storing assets such as meshes, shaders, textures, and fonts. It also
		handles the rendering of regular and debug components, as well as text
		components. Additionally, it includes functions for subscribing to events
		and handling key press and release events.

		It also calls the functions for transformations as well as the key to activate it.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>
#include "RenderSystem.h"
#include "../Engine/Engine.h"

namespace Popplio
{

	RenderSystem::RenderSystem(GLFWwindow* window, LayerManager& layerMgr, CameraManager& cameraMgr)
		: window(window), layerManager(layerMgr), cameraManager(cameraMgr)
	{
		RequireComponent<TransformComponent>();
	}

	void RenderSystem::ToggleDebugDrawing()
	{
		debugDrawing = !debugDrawing;
	}


	void RenderSystem::Initialize()
	{
		// Create Meshes
		AssetStore::StoreMeshAsset(Triangle, CreateTriangleMesh());
		AssetStore::StoreMeshAsset(Quad, CreateQuadMesh());
		AssetStore::StoreMeshAsset(Circle, CreateCircleMesh());

		// Set up instancing for meshes that will be instanced
		AssetStore::GetMesh(Quad).SetupInstancing();
		AssetStore::GetMesh(Triangle).SetupInstancing();
		AssetStore::GetMesh(Circle).SetupInstancing();
	}

	void RenderSystem::Update()
	{
		glClearColor(0.7f, 0.7f, 0.7f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Get all layers in order
		auto sortedLayers = layerManager.GetSortedLayers();

		// Render entities in each layer
		for (const auto& layer : sortedLayers)
		{
			if (!layer.active)
			{
				continue;
			}

			// Get entities in this layer
			auto layerEntities = layerManager.GetEntitiesInLayer(layer.name);

			// Separate entities by rendering method
			std::vector<Entity> nonInstancedEntities;
			std::vector<Entity> textEntities;
			std::unordered_map<MeshType, std::vector<Entity>> instancedEntities;

			for (const auto& entity : layerEntities)
			{
				if (entity.HasComponent<ActiveComponent>() == false)
				{
					Logger::Debug("Why does Entity not have ActiveComponent???");
					continue;
				}
				if (entity.GetComponent<ActiveComponent>().isActive == false) continue;
				if (entity.HasComponent<RenderComponent>())
				{
					if (!entity.GetComponent<RenderComponent>().isActive) continue;
					const auto& renderComp = entity.GetComponent<RenderComponent>();
					if (renderComp.useInstancing)
					{
						instancedEntities[renderComp.meshType].push_back(entity);
					}
					else
					{
						nonInstancedEntities.push_back(entity);
					}
				}
				else if (entity.HasComponent<TextComponent>())
				{
					if (!entity.GetComponent<TextComponent>().isActive) continue;
					textEntities.push_back(entity);
				}
			}

			// Render non-instanced entities
			RenderNonInstancedEntities(nonInstancedEntities);

			// Render non-instanced entities
			for (const auto& [meshType, entitiesList] : instancedEntities)
			{
				RenderInstanced(meshType, entitiesList);
			}

			// Render text entities
			RenderTextEntities(textEntities);
		}

		// Render debug colliders if needed
		if (debugDrawing)
		{
			RenderDebugColliders();
		}
	}
	void RenderSystem::RenderNonInstancedEntities(const std::vector<Entity>& entitiesList)
	{
		for (const auto& entity : entitiesList)
		{
			if (!entity.GetComponent<ActiveComponent>().isActive) continue;
			if (!entity.GetComponent<RenderComponent>().isActive) continue;

			const auto& transform = entity.GetComponent<TransformComponent>();
			const auto& renderComp = entity.GetComponent<RenderComponent>();

			Shader* shader = AssetStore::GetShader(renderComp.shaderName);
			shader->Use();

			// Set model, view, and projection matrices if not background mesh type
			if (renderComp.meshType != MeshType::FullscreenQuad)
			{
				shader->SetMatrix3("model", transform.modelMatrix.ToGLM());
				shader->SetMatrix3("view", cameraManager.GetViewMatrix().ToGLM());
				shader->SetMatrix3("projection", cameraManager.GetProjectionMatrix().ToGLM());
				shader->SetVector3f("uColor", renderComp.color);
				//std::cout << std::to_string(static_cast<int>(transform.anchor)) <<std::endl;
			}

			if (renderComp.shaderName != "debug_shader")
			{
				shader->setFloat("uAlpha", renderComp.alpha);
			}

            // Handle particles
			bool particlesActive = 
				entity.HasComponent<ParticleComponent>() && entity.GetComponent<ParticleComponent>().isActive;
			if (particlesActive)
			{
                std::optional<RenderComponent> renderCompOpt = 
					entity.HasComponent<RenderComponent>() ? 
					std::make_optional(entity.GetComponent<RenderComponent>()) : std::nullopt;
                std::optional<AnimationComponent> animCompOpt = 
					entity.HasComponent<AnimationComponent>() ? 
					std::make_optional(entity.GetComponent<AnimationComponent>()) : std::nullopt;
				HandleParticles(entity.GetComponent<ParticleComponent>(), renderCompOpt, animCompOpt, shader);
			}
			else // if entity does not have a particle system
			{
				// Handle textures
				if (!renderComp.textureName.empty()) HandleTexture(renderComp, shader);

				// Handle animations
				if (entity.HasComponent<AnimationComponent>() && entity.GetComponent<AnimationComponent>().isActive)
					HandleAnimation(renderComp, entity.GetComponent<AnimationComponent>(), shader);
			}

			// Apply 9-Slice Scaling (only default shader supports this)
			if (!renderComp.textureName.empty() && renderComp.enableNineSlice && (renderComp.shaderName == "default_shader"))
			{
				shader->setBool("useNineSlice", true);
				shader->SetUniform("spriteSize", glm::vec2(transform.scale.x, transform.scale.y));
				shader->SetUniform("textureSize", glm::vec2(renderComp.textureWidth, renderComp.textureHeight));

				glm::vec4 bSize(
							renderComp.borderLeft,
							renderComp.borderRight,
							renderComp.borderBottom,
							renderComp.borderTop
						);
				shader->SetUniform("borderSize", bSize);

			}
			else
			{
				shader->setBool("useNineSlice", false);
			}


			if (entity.HasComponent<UIComponent>() && entity.GetComponent<UIComponent>().isActive)
			{
				auto& uiComp = entity.GetComponent<UIComponent>();

				if (uiComp.type == UIType::SLIDER_TRACK)
				{
					//shader->SetVector3f("uColor", glm::vec3(uiComp.sliderValue, renderComp.color.g, renderComp.color.b));
				}
				else if (uiComp.type == UIType::SLIDER_HANDLE)
				{
					// Ensure the handle renders in the correct position
					if (uiComp.linkedEntityId >= 0)
					{
						Entity trackEntity(uiComp.linkedEntityId);
						trackEntity.registry = entity.registry;
						if (trackEntity.HasComponent<TransformComponent>())
						{
							auto& trackTransform = trackEntity.GetComponent<TransformComponent>();

							float trackLeft = trackTransform.position.x - (trackTransform.scale.x * 0.5f);
							float offset = uiComp.sliderValue * trackTransform.scale.x;

							glm::mat3 modelMatrix = transform.modelMatrix.ToGLM();
							modelMatrix[2][0] = trackLeft + offset;

							shader->SetMatrix3("model", modelMatrix);
						}
					}
				}
				else if (uiComp.type == UIType::CHECKBOX)
				{
					shader->SetVector3f("uColor", uiComp.checked ? glm::vec3(0.0f, 0.7f, 0.0f) : renderComp.color);
				}
				else if (uiComp.type == UIType::BUTTON && uiComp.hasHoverEffect)
				{
					glm::vec4 uvCoords = uiComp.GetCurrentUV();
					shader->SetUniform("u_TextureCoords", uvCoords);
				}
			}

			if (!particlesActive) RenderMesh(renderComp.meshType);

			glBindTexture(GL_TEXTURE_2D, 0);

			shader->UnUse();
		}
	}

	void RenderSystem::RenderTextEntities(const std::vector<Entity>& entitiesList)
	{
		for (const auto& entity : entitiesList)
		{
			if (!entity.GetComponent<TextComponent>().isActive) continue;

			const auto& textComp = entity.GetComponent<TextComponent>();
			const auto& transComp = entity.GetComponent<TransformComponent>();
			RenderText(textComp, transComp);
		}
	}

	void RenderSystem::RenderInstanced(MeshType meshType, const std::vector<Entity>& entitiesList)
	{
		if (entitiesList.empty())
		{
			return;
		}

		const auto& firstEntity = entitiesList[0];
		const auto& firstrenderComp = firstEntity.GetComponent<RenderComponent>();

		Shader* shader = AssetStore::GetShader(firstrenderComp.shaderName);
		shader->Use();

		Mesh& mesh = AssetStore::GetMesh(meshType);

		// Prepare instance data
		std::vector<InstanceData> instanceData;
		instanceData.reserve(entitiesList.size() * 4);


		for (const auto& entity : entitiesList)
		{
			if (!entity.GetComponent<ActiveComponent>().isActive) continue;
			if (!entity.GetComponent<RenderComponent>().isActive) continue;
			const auto& renderComp = entity.GetComponent<RenderComponent>();

			// If the entity has a ParticleComponent, add instance data for each alive particle.
			if (entity.HasComponent<ParticleComponent>())
			{
				const auto& particleComp = entity.GetComponent<ParticleComponent>();
				// If available, get the AnimationComponent (used for animated particles)
				std::optional<AnimationComponent> animCompOpt =
					entity.HasComponent<AnimationComponent>() ?
					std::make_optional(entity.GetComponent<AnimationComponent>()) : std::nullopt;

				for (const Particle& p : particleComp.pool)
				{
					if (!p.alive)
						continue;

					// Compute model matrix from particle's transform
					PopplioMath::M3x3f translateMat, rotateMat, scaleMat;
					translateMat.Translate(p.pos.x, p.pos.y);
					rotateMat.RotDeg(p.rot);
					scaleMat.Scale(p.scale.x, p.scale.y);
					glm::mat3 modelMatrix = (translateMat * rotateMat * scaleMat).ToGLM();

					// Use the particle's color and alpha
					glm::vec3 instColor(p.col.r, p.col.g, p.col.b);
					float instAlpha = p.col.a;

					// For animated particles, compute a per-particle UV rectangle.
					glm::vec4 uvRect;
					if (particleComp.renderOption == 1 && animCompOpt.has_value())
					{
						const auto& animComp = animCompOpt.value();
						const auto& currentAnim = animComp.animations.at(animComp.currentAnimation);
						float u1 = (p.frameX * currentAnim.frameWidth) / animComp.sheetWidth;
						float v1 = (p.frameY * currentAnim.frameHeight) / animComp.sheetHeight;
						float u2 = ((p.frameX + 1) * currentAnim.frameWidth) / animComp.sheetWidth;
						float v2 = ((p.frameY + 1) * currentAnim.frameHeight) / animComp.sheetHeight;
						uvRect = glm::vec4(u1, v1, u2, v2);
					}
					else
					{
						uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
					}

					instanceData.push_back({ modelMatrix, instColor, instAlpha, uvRect });
				}
			}
			else  // Normal instanced entity
			{
				const auto& transform = entity.GetComponent<TransformComponent>();
				glm::vec4 uvRect;
				if (entity.HasComponent<AnimationComponent>())
				{
					const auto& animComp = entity.GetComponent<AnimationComponent>();
					uvRect = animComp.GetUVCoords();
				}
				else
				{
					uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // Full texture if no animation.
				}

				instanceData.push_back({
					transform.modelMatrix.ToGLM(),
					glm::vec3(renderComp.color.x, renderComp.color.y, renderComp.color.z),
					renderComp.alpha,
					uvRect
					});
			}
		}

		//// Apply 9-Slice Scaling to Instanced Entities with Textures
		//if (!renderComp.textureName.empty() && renderComp.enableNineSlice)
		//{
		//	shader->setBool("useNineSlice", true);
		//	shader->SetUniform("borderSize", glm::vec4(
		//		renderComp.borderLeft / renderComp.textureWidth,
		//		renderComp.borderRight / renderComp.textureWidth,
		//		renderComp.borderTop / renderComp.textureHeight,
		//		renderComp.borderBottom / renderComp.textureHeight
		//	));
		//}
		//else
		//{
		//	shader->setBool("useNineSlice", false);
		//}

		// Update instance buffer
		glBindBuffer(GL_ARRAY_BUFFER, mesh.instanceVBO);
		glBufferData(GL_ARRAY_BUFFER,
			instanceData.size() * sizeof(InstanceData),
			instanceData.data(),
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Set uniforms
		shader->SetMatrix3("view", cameraManager.GetViewMatrix().ToGLM());
		shader->SetMatrix3("projection", cameraManager.GetProjectionMatrix().ToGLM());

		if (!firstrenderComp.textureName.empty())
		{
			unsigned int textureID = AssetStore::GetTexture(firstrenderComp.textureName);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureID);
			shader->setInt("ourTexture", 0);
			shader->setBool("useTexture", true);
		}
		else
		{
			shader->setBool("useTexture", false);
		}

		// Bind the mesh's VAO before drawing
		glBindVertexArray(mesh.VAO);
		glDrawElementsInstanced(GL_TRIANGLES,
			static_cast<GLsizei>(mesh.indices.size()),
			GL_UNSIGNED_INT, 0,
			static_cast<GLsizei>(instanceData.size()));

		/*GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			Logger::Error("OpenGL error in RenderInstanced: " + std::to_string(error));
		}*/

		glBindVertexArray(0);

		shader->UnUse();
	}

	void RenderSystem::RenderDebugColliders()
	{
		for (const auto& entity : GetSystemEntities())
		{
			if (entity.HasComponent<BoxColliderComponent>())
			{
				const auto& collider = entity.GetComponent<BoxColliderComponent>();
				DrawBoxCollider(collider);
			}
		}
	}

	void RenderSystem::RenderMesh(MeshType mesh)
	{
		const auto& meshData = AssetStore::GetMesh(mesh);
		glBindVertexArray(meshData.VAO);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(meshData.indices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void RenderSystem::DrawBoxCollider(const BoxColliderComponent& boxCollider)
	{
		Shader* debugShader = AssetStore::GetShader("debug_shader");
		debugShader->Use();

		// Use the pre-calculated collider matrix
		debugShader->SetMatrix3("model", boxCollider.colliderMatrix.ToGLM());
		debugShader->SetMatrix3("view", cameraManager.GetViewMatrix().ToGLM());
		debugShader->SetMatrix3("projection", cameraManager.GetProjectionMatrix().ToGLM());
		debugShader->SetVector3f("color", glm::vec3(1.0f, 0.0f, 0.0f)); // Red color for colliders

		// Draw the collider
		const Mesh& quadMesh = AssetStore::GetMesh(MeshType::Quad);
		glBindVertexArray(quadMesh.VAO);
		glDrawElements(GL_LINE_LOOP, static_cast<GLsizei>(quadMesh.indices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		debugShader->UnUse();
	}

	void RenderSystem::RenderText(const TextComponent& textComp, const TransformComponent& transform)
	{
		if (textComp.text.empty())
			return;
		// Fetch the latest text and update
		std::string updatedText = textComp.text;
		Font* font = AssetStore::GetFont(textComp.fontName);
		if (!font)
		{
			Logger::Warning("Font not found: " + textComp.fontName);
			return;
		}

		// Ensure font size is loaded
		if (!font->IsSizeLoaded(textComp.fontSize))
		{
			font->LoadSize(textComp.fontSize);
		}

		Shader* textShader = AssetStore::GetShader("font_shader");
		if (!textShader)
		{
			Logger::Warning("Text shader not found");
			return;
		}

		textShader->Use();

		glActiveTexture(GL_TEXTURE0);

		// Set up matrices
		textShader->SetMatrix3("view", cameraManager.GetViewMatrix().ToGLM());
		textShader->SetMatrix3("projection", cameraManager.GetProjectionMatrix().ToGLM());
		textShader->SetVector3f("textColor", glm::vec3(textComp.color));
		textShader->setFloat("uAlpha", textComp.alpha);

		glBindVertexArray(font->GetVAO());

		// Calculate text metrics for centering
		float totalWidth = 0.0f;
		float maxAscent = 0.0f;
		float maxDescent = 0.0f;

		// First pass: calculate metrics
		for (char c : textComp.text)
		{
			const Character& ch = font->GetCharacter(c, textComp.fontSize);
			totalWidth += static_cast<float>(ch.Advance >> 6);

			// Calculate maximum ascent (height above baseline)
			maxAscent = std::max(maxAscent, static_cast<float>(ch.Bearing.y));

			// Calculate maximum descent (height below baseline)
			float descent = static_cast<float>(ch.Size.y) - static_cast<float>(ch.Bearing.y);
			maxDescent = std::max(maxDescent, descent);
		}

		//float totalHeight = maxAscent + maxDescent;

		// Calculate starting positions
		float x = transform.position.x - totalWidth / 2.0f;
		
		// For vertical positioning:
		// Position is the center of the text
		float y = transform.position.y;
		
		// Calculate vertical offset to center the text
		// Make midpoint between the ascent and descent to be at y
		float verticalOffset = (maxAscent - maxDescent) / 2.0f;
		
		// The baseline position is offset from the center
		float baseline = y + verticalOffset;

		// Render each character
		for (char c : textComp.text)
		{
			const Character& ch = font->GetCharacter(c, textComp.fontSize);

			// Position horizontally - add bearing X (horizontal offset)
			float xpos = x + static_cast<float>(ch.Bearing.x);
			
			// Position vertically - FreeType expects:
			// 1. Start at baseline
			// 2. Subtract full glyph height
			// 3. Add bearing Y (which is distance from baseline to top of glyph)
			float ypos = baseline - static_cast<float>(ch.Size.y) + static_cast<float>(ch.Bearing.y);

			float w = static_cast<float>(ch.Size.x);
			float h = static_cast<float>(ch.Size.y);

			// Update VBO
			float vertices[6][4] = {
				{ xpos,     ypos,           0.0f, 1.0f },     // bottom-left
				{ xpos + w, ypos + h,       1.0f, 0.0f },     // top-right
				{ xpos,     ypos + h,       0.0f, 0.0f },     // top-left
				
				{ xpos,     ypos,           0.0f, 1.0f },     // bottom-left
				{ xpos + w, ypos,           1.0f, 1.0f },     // bottom-right
				{ xpos + w, ypos + h,       1.0f, 0.0f }      // top-right
			};

			glBindTexture(GL_TEXTURE_2D, ch.TextureID);

			// Update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, font->GetVBO());
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// Render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);

			// Advance cursor for next glyph
			x += (ch.Advance >> 6); // Advance is in 1/64th pixels
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		textShader->UnUse();
	}

	void RenderSystem::HandleTexture(const RenderComponent& renderComp, Shader* shader)
	{
		unsigned int textureID = AssetStore::GetTexture(renderComp.textureName);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		shader->setInt(renderComp.meshType == MeshType::FullscreenQuad ?
			"backgroundTexture" : "ourTexture", 0);

		//if (renderComp.enableNineSlice)
		//{
		//	shader->setBool("useNineSlice", true);
		//	shader->SetUniform("borderSize", glm::vec4(
		//		renderComp.borderLeft / renderComp.textureWidth,
		//		renderComp.borderRight / renderComp.textureWidth,
		//		renderComp.borderTop / renderComp.textureHeight,
		//		renderComp.borderBottom / renderComp.textureHeight
		//	));
		//}
		//else
		//{
		//	shader->setBool("useNineSlice", false);
		//}
	}

	void RenderSystem::HandleAnimation(const RenderComponent& renderComp,const AnimationComponent& animComp, Shader* shader)
	{
		//auto& animComp = entity.GetComponent<AnimationComponent>();
		unsigned int textureID = AssetStore::GetTexture(renderComp.textureName);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		shader->setInt("ourTexture", 0);
		glm::vec4 uvCoords = animComp.GetUVCoords();
		shader->SetUniform("u_TextureCoords", uvCoords);
	}

	void RenderSystem::HandleParticles(const ParticleComponent& particles, const std::optional<RenderComponent>& renderComp,
		const std::optional<AnimationComponent>& animComp, Shader* shader)
	{
		switch (particles.renderOption)
		{
			case 0: // texture
				HandleTexture(*renderComp, shader);
				break;
			case 1: // animation
				HandleAnimation(*renderComp, *animComp, shader);
				break;
			default:
				Logger::Error("Unsupported particle render option");
				return;
		}

		// Compute transformation matrices
		for (const auto& particle : particles.pool)
		{
			if (!particle.alive) continue;

			PopplioMath::M3x3f transform, translateMat, rotateMat, scaleMat;

			// Apply translation with correct offset
			translateMat.Translate(particle.pos.x, particle.pos.y);

			rotateMat.RotDeg(static_cast<float>(particle.rot));
			scaleMat.Scale(particle.scale.x, particle.scale.y);

			// Compute final transformation matrix
			transform = translateMat * rotateMat * scaleMat;

			shader->SetMatrix3("model", transform.ToGLM());

			shader->SetVector3f("uColor", glm::vec3(particle.col.r, particle.col.g, particle.col.b));
			shader->setFloat("uAlpha", particle.col.a);

			// For animated particles, compute unique UV coordinates per particle
			if (particles.renderOption == 1 && animComp.has_value())
			{
				// Particle has been extended with frameX and frameY:
				const auto& currentAnim = animComp->animations.at(animComp->currentAnimation);
				float u1 = (particle.frameX * currentAnim.frameWidth) / animComp->sheetWidth;
				float v1 = (particle.frameY * currentAnim.frameHeight) / animComp->sheetHeight;
				float u2 = ((particle.frameX + 1) * currentAnim.frameWidth) / animComp->sheetWidth;
				float v2 = ((particle.frameY + 1) * currentAnim.frameHeight) / animComp->sheetHeight;
				glm::vec4 uvCoords(u1, v1, u2, v2);
				shader->SetUniform("u_TextureCoords", uvCoords);
			}
			RenderMesh(renderComp->meshType);
		}

		// TODO set color

		// TODO unique animation frames per particle
	}
}