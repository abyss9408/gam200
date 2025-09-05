/******************************************************************************/
/*!
\file   ParticleSystem.h
\author Team Popplio
\author Ng Jun Heng Shawn
\co-author(s) Rio Shannon Yvon Leonardo
\contribution Shawn - 98%, Shannon - 2%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
		Header file for implementing the Engine's Particle System

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "ParticleComponent.h"

#include "../Math/Vector2D.h"
#include "../ECS/ECS.h"

#include "../Graphic/GraphicComponent.h"
#include "../Animation/AnimationComponent.h"

#include "../Graphic/Color.h"

#include "../Physics/MovementSystem.h"

//#define PARTICLE_SYSTEM_PARTICLE_MAX 2048
//#define PARTICLE_SYSTEM_EMISSION_MAX 32

// TODO: how to render particles?

namespace Popplio
{
    class ParticleSystem : public System
	{
		//public:

		//class IComponent // per component gotten by system from all entities
		//{
		//	ParticleSystem& sys;

		//	std::vector<Particle>	pool;
		//	std::vector<Emitter>	emission;

		//	size_t countAlive;
		//	size_t countEmission;

		//	//ParticleSystemComponent* comp; // ECS component

		//	public:
		//		
		//	IComponent(ParticleSystem& s);

		//	std::optional<Particle> NewPart(std::optional<RenderComponent> texture = std::nullopt, 
		//		std::optional<AnimationComponent> anim = std::nullopt);
		//	void KillPart(Particle& p);
		//	std::optional<Emitter> NewEmit(std::optional<RenderComponent> texture = std::nullopt,
		//		std::optional<AnimationComponent> anim = std::nullopt);
  //          void KillEmit(Emitter& e);

  //          void Update(double const& dt);
		//};

		private:

		Registry& reg;
		MovementSystem* movementSystem; //reference to movement System

		//std::vector<IComponent> pSystems;
		//std::vector<ParticleComponent> pSystems;

        void ApplyPhysics(Particle& p, const ParticleComponent& comp);

		public:

		ParticleSystem(Registry& r);

		//set the movement system reference
		void SetMovementSystem(MovementSystem* ms) { movementSystem = ms; }

		void Update(double const& dt);

		void UpdateEnt(Entity& ent, double const& dt);

        // Helper functions

		int NewPart(ParticleComponent& comp);

		void KillPart(ParticleComponent& comp, int const& p);

		int NewEmit(ParticleComponent& comp);

		void KillEmit(ParticleComponent& comp, int const& e);
	};
}