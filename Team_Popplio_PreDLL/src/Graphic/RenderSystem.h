/******************************************************************************/
/*!
\file		RenderSystem.h
\author		Team Popplio
\author		Hafawati Binte Mohd Ali (90%)
\co-author	Pearl Goh (5%) | Ng Jun Heng Shawn (5%)
\par		Course : CSD2450 / CSD2451
\par		Section : A
\date		2025/01/14
\brief
		This header file defines the RenderSystem class, which is responsible
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
#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../ECS/ECS.h"

#include "../Transformation/TransformComponent.h"
#include "../Collision/BoxColliderComponent.h"
#include "../Graphic/GraphicComponent.h"
#include "../UI/UIComponent.h"
#include "../Animation/AnimationComponent.h"
#include "../Particles/ParticleComponent.h"

#include "../Graphic/Mesh/Mesh.h"
#include "../Graphic/Shader/Shader.h"
#include "../Graphic/Texture/Texture.h"
#include "../AssetStore/AssetStore.h" // for list of assets
#include "../Graphic/TRS/TRS.h"
#include "../Graphic/Font/Font.h"
#include "../EventBus/EventBus.h"
#include "../Camera/CameraManager.h"
#include "../Layering/LayerManager.h"
#include "../Editor/EntitySelectionEvents.h"

#include <vector>
#include <unordered_map>

namespace Popplio
{
	struct InstanceData
	{
		glm::mat3 modelMatrix;
		glm::vec3 color;
		float alpha;       
		glm::vec4 uvRect;
	};

	/*!*****************************************************************************
    \brief
    Class representing the graphics system. It is responsible for initializing
    and managing the graphics system, including loading and storing assets such
    as meshes, shaders, textures, and fonts. It also handles the rendering of
    regular and debug components, as well as text components.

    *******************************************************************************/
	class RenderSystem : public System
	{
	public:

		/*!*****************************************************************************
		\brief
		Constructor for the GraphicSystem class.

		\param[in] window
		[GLFWwindow*] Pointer to the GLFW window.

		*******************************************************************************/
		RenderSystem(GLFWwindow* window, LayerManager& layerMgr, CameraManager& cameraMgr);

		

		/*!*****************************************************************************
		\brief
		Initializes the graphics system, including loading and storing assets such
		as meshes, shaders, textures, and fonts.

		*******************************************************************************/
		void Initialize();

		/*!*****************************************************************************
		\brief
		Updates the graphics system, including handling key input, updating the
		rotation of the triangle, and rendering regular, debug, and text components.

		*******************************************************************************/
		void Update();

		/*!*****************************************************************************
		\brief
			Toggles the debug drawing mode on or off.

		This function enables or disables the rendering of debug colliders and overlays
		for visual debugging purposes.

		*******************************************************************************/
		void ToggleDebugDrawing();

	private:
		GLFWwindow* window;
		LayerManager& layerManager;
		CameraManager& cameraManager;
		bool debugDrawing = false;

		/*!*****************************************************************************
		\brief
			Renders non-instanced entities using their mesh and transform components.

		\param[in] entities
			A list of entities to be rendered using standard non-instanced rendering.

		*******************************************************************************/
		void RenderNonInstancedEntities(const std::vector<Entity>& entities);

		/*!*****************************************************************************
		\brief
		Renders instanced triangles using the specified shader and projection matrix.

		This function binds the vertex array object (VAO) of the mesh and renders multiple
		instances of the mesh using instanced rendering. It sets the projection matrix uniform
		in the shader and draws the instances.

		*******************************************************************************/
		void RenderInstanced(MeshType mesh, const std::vector<Entity>& entities);

		/*!*****************************************************************************
		\brief
			Renders all entities that have text components.

		\param[in] entities
			A list of entities containing text components to be rendered on screen.

		*******************************************************************************/
		void RenderTextEntities(const std::vector<Entity>& entities);

		/*!*****************************************************************************
		\brief
		Renders a mesh based on the given mesh index and transform component.

		\param[in] meshIndex
		[int] Index of the mesh to render.

		\param[in] transform
		[const TransformComponent&] Reference to the transform component.

		*******************************************************************************/
		void RenderMesh(MeshType type);

		/*!*****************************************************************************
		\brief
			Renders visual representations of all active debug colliders in the scene.

		*******************************************************************************/
		void RenderDebugColliders();

		/*!*****************************************************************************
		\brief
		Renders a box collider

		\param[in] meshIndex
		[int] Index of the debug mesh to render.

		*******************************************************************************/
		void DrawBoxCollider(const BoxColliderComponent& collider);

		/*!*****************************************************************************
		\brief
			Renders a text string using the provided TextComponent and its world transform.

		\param[in] textComp
			The text component containing the string and styling information.

		\param[in] transform
			The transform component defining the position and scale of the text in world space.

		*******************************************************************************/
		void RenderText(const TextComponent& textComp, const TransformComponent& transform);

		/*!*****************************************************************************
		\brief
			Handles applying textures to the shader for a given render component.

		\param[in] renderComp
			The render component which may contain texture information.

		\param[in] shader
			The shader to which the texture data should be applied.

		*******************************************************************************/
        void HandleTexture(const RenderComponent& renderComp, Shader* shader);

		/*!*****************************************************************************
		\brief
			Handles animation state and updates shader uniforms accordingly.

		\param[in] renderComp
			The render component associated with the entity.

		\param[in] animComp
			The animation component that defines the current animation state.

		\param[in] shader
			The shader being used for rendering the animated object.

		*******************************************************************************/
        void HandleAnimation(const RenderComponent& renderComp, const AnimationComponent& animComp, Shader* shader);

		/*!*****************************************************************************
		\brief
			Renders particles using the particle component.

		\param[in] particles
			The particle component containing particle system parameters.

		\param[in] renderComp
			Optional render component for textured or mesh-based particles.

		\param[in] animComp
			Optional animation component for animated particles.

		\param[in] shader
			The shader used to render the particle effects.

		*******************************************************************************/
		void HandleParticles(const ParticleComponent& particles, const std::optional<RenderComponent>& renderComp,
			const std::optional<AnimationComponent>& animComp, Shader* shader);
	};
}