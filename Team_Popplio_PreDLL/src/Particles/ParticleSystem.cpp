/******************************************************************************/
/*!
\file   ParticleSystem.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\co-author(s) Rio Shannon Yvon Leonardo
\contribution Shawn - 82.5%, Shannon - 12.5%, Hafawati - 5%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
		Source file for implementing the Engine's Particle System

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/


#include <pch.h>

#include "ParticleSystem.h"

//#include "ParticleComponent.h"

#include "../Engine/Engine.h"

#include "../Utilities/Timer.h"
#include "../Logging/Logger.h"

namespace Popplio
{
	//updated the constructor
	ParticleSystem::ParticleSystem(Registry& r) : reg(r), movementSystem(nullptr)//, pSystems() 
	{
		RequireComponent<ParticleComponent>();
	}

	void ParticleSystem::Update(double const& dt)
	{
        //for (IComponent& pSystem : pSystems)
		//for (ParticleComponent& pSystem : pSystems)
				// Iterate through all entities managed by this system
		//for (int step = 0; step < Engine::timer.GetCurrentNumberOfSteps(); ++step)

		static int fixedSteps{};
		fixedSteps = 0;

		while (fixedSteps < Engine::timer.GetCurrentNumberOfSteps())
		{
			for (auto& entity : GetSystemEntities())
			{
				if (!entity.GetComponent<ActiveComponent>().isActive) continue;
                if (!entity.HasComponent<ParticleComponent>()) continue;

				//pSystem.Update(dt);
				UpdateEnt(entity, dt);
			}

            ++fixedSteps;
		}
	}

	int ParticleSystem::NewPart(ParticleComponent& comp)
	{
		for (size_t i{}; i < comp.pool.size(); ++i)
		{
            Particle& p = comp.pool[i];
			if (!p.alive)
			{
				// initialize particle
				p();

				//if (texture != std::nullopt) p.texture = texture;
				//if (anim != std::nullopt) p.animation = anim;

				++comp.countAlive;

				p.alive = true;

				return static_cast<int>(i);
			}
		}

		return -1; // return if no particles are available
	}

	void ParticleSystem::KillPart(ParticleComponent& comp, int const& p)
	{
		if (comp.pool.size() <= p)
		{
            Logger::Error("ParticleSystem::KillPart | Particle index out of bounds!");
			return;
		}
        if (!comp.pool[p].alive)
		{
			Logger::Warning("ParticleSystem::KillPart | Particle already dead");
			return;
		}

		--comp.countAlive;
		//p.alive = false;
        comp.pool[p].alive = false;
	}

	int ParticleSystem::NewEmit(ParticleComponent& comp)
	{
		for (size_t i{}; i < comp.emission.size(); ++i)
		{
            Emitter& e = comp.emission[i];
			if (!e.active)
			{
				// initialize emitter
				e();

				//if (texture != std::nullopt) e.texture = texture;
				//if (anim != std::nullopt) e.animation = anim;

				++comp.countEmission;
				e.active = true;

				return static_cast<int>(i);
			}
		}

		return -1; // return null if no emitters are available
	}

	void ParticleSystem::KillEmit(ParticleComponent& comp, int const& e)
	{
		if (comp.emission.size() <= e)
		{
			Logger::Error("ParticleSystem::KillEmit | Emitter index out of bounds!");
			return;
		}
		if (!comp.emission[e].active)
		{
			Logger::Warning("ParticleSystem::KillEmit | Emitter already dead");
			return;
		}

		--comp.countEmission;
		//e.active = false;
        comp.emission[e].active = false;
	}

	void ParticleSystem::UpdateEnt(Entity& ent, double const& dt)
	{
        ParticleComponent& comp = ent.GetComponent<ParticleComponent>();

		for (Particle& p : comp.pool)
		{
			if (p.alive)
			{

				if (!comp.usePhysics) {
					p.pos += p.vel * static_cast<float>(dt);
					p.vel += p.accel * static_cast<float>(dt);
                    p.vel.x += p.vel.x >= 0.f ? p.dirAccel.x * static_cast<float>(dt) : -p.dirAccel.x * static_cast<float>(dt);
                    p.vel.y += p.vel.y >= 0.f ? p.dirAccel.y * static_cast<float>(dt) : -p.dirAccel.y * static_cast<float>(dt);
				}
				else {
					ApplyPhysics(p, comp);
				}



				//p.pos += p.vel * static_cast<float>(dt);
				p.rot += p.spinVel * static_cast<float>(dt);
				p.scale += p.scaleVel * static_cast<float>(dt);

				p.col = p.col + p.colVel * static_cast<float>(dt);

				//p.vel += p.accel * static_cast<float>(dt);
				p.spinVel += p.spinAccel * static_cast<float>(dt);
				p.scaleVel += p.scaleAccel * static_cast<float>(dt);

                // add from dir accel to velocity in the same direction as the velocity
                p.spinVel += p.spinVel >= 0.f ?
                    p.spinDirAccel * static_cast<float>(dt) :
                    -p.spinDirAccel * static_cast<float>(dt);
                p.scaleVel.x += p.scaleVel.x >= 0.f ?
                    p.scaleDirAccel.x * static_cast<float>(dt) :
                    -p.scaleDirAccel.x * static_cast<float>(dt);
                p.scaleVel.y += p.scaleVel.y >= 0.f ?
                    p.scaleDirAccel.y * static_cast<float>(dt) :
                    -p.scaleDirAccel.y * static_cast<float>(dt);

				//if (comp.usePhysics) ApplyPhysics(p, comp);

				p.colVel = (p.colVel + p.colAccel) * static_cast<float>(dt);
                p.colVel.r += p.colVel.r >= 0.f ?
                    p.colDirAccel.r * static_cast<float>(dt) :
                    -p.colDirAccel.r * static_cast<float>(dt);
                p.colVel.g += p.colVel.g >= 0.f ?
                    p.colDirAccel.g * static_cast<float>(dt) :
                    -p.colDirAccel.g * static_cast<float>(dt);
                p.colVel.b += p.colVel.b >= 0.f ?
                    p.colDirAccel.b * static_cast<float>(dt) :
                    -p.colDirAccel.b * static_cast<float>(dt);
                p.colVel.a += p.colVel.a >= 0.f ?
                    p.colDirAccel.a * static_cast<float>(dt) :
                    -p.colDirAccel.a * static_cast<float>(dt);

				p.lifeTime -= static_cast<float>(dt);

				if (p.lifeTime <= 0.f) p.alive = false;

				else if (comp.renderOption == 1)
				{
					if (!ent.HasComponent<AnimationComponent>())
					{
						Logger::Error("Particle System has no Animation Component");
						continue;
					}
					auto& animComp = ent.GetComponent<AnimationComponent>();
					const auto& currentAnim = animComp.animations.at(animComp.currentAnimation);
					float frameDuration = currentAnim.animationSpeed; 
					p.animTimeAccumulator += static_cast<float>(dt);

					// Calculate frames to advance
					int framesToAdvance = static_cast<int>(p.animTimeAccumulator / frameDuration);
					p.animTimeAccumulator = fmod(p.animTimeAccumulator, frameDuration);

					if (framesToAdvance > 0)
					{
						//Logger::Debug("Advancing " + std::to_string(framesToAdvance) + " frames for particle");

						// Define starting and ending frames from the animation
						int startX = currentAnim.startX;
						int startY = currentAnim.startY;
						int endX = currentAnim.endX;
						int endY = currentAnim.endY;
						int dimX = currentAnim.dimensionX;

						// Compute current linear frame index relative to the animation start
						int currentIndex = (p.frameY - startY) * dimX + (p.frameX - startX);
						// Total number of frames in the animation range
						int totalFrames = (endY - startY) * dimX + (endX - startX + 1);

						/*Logger::Debug("Particle current linear frame index: " + std::to_string(currentIndex) +
							", totalFrames: " + std::to_string(totalFrames));*/

						// Advance the frame index
						currentIndex = (currentIndex + framesToAdvance) % totalFrames;

						// Convert back to (frameX, frameY)
						p.frameY = startY + (currentIndex / dimX);
						p.frameX = startX + (currentIndex % dimX);

					}
				}

                //Logger::Error("ParticleSystem::UpdateComp | Particle life time expired!");
			}
		}

		for (Emitter& e : comp.emission) // might incur significant overhead
		{
			if (e.active)
			{
				float temp = e.elapsedTime;

				e.elapsedTime += static_cast<float>(dt);

				if (floor(static_cast<double>(temp / (1.f / e.rate))) !=
					floor(static_cast<double>(e.elapsedTime / (1.f / e.rate))))
				{
					if (e.loop ||
						(!e.loop && e.tLoop >= e.elapsedTime))
					{
						int i = NewPart(comp);

						if (i < 0) // not enough particles
						{
							Logger::Debug("ParticleSystem::UpdateComp | Not enough particles!");
							continue;
						}

                        Particle& p = comp.pool[i];

						p.lifeTime = e.minLifeTime +
							((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) *
								(e.maxLifeTime - e.minLifeTime));

						p.rot = e.minRot +
							((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) *
								(e.maxRot - e.minRot));

						p.spinVel = e.minSpinVel +
							((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) *
								(e.maxSpinVel - e.minSpinVel));

						p.spinAccel = e.minSpinAccel +
							((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) *
								(e.maxSpinAccel - e.minSpinAccel));

						p.pos = e.maxPos - e.minPos;
						p.pos.x *= static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
						p.pos.y *= static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
						p.pos += e.minPos;

						p.scale = e.maxScale - e.minScale;
						p.scale.x *= static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
						p.scale.y *= static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
						p.scale += e.minScale;

						p.vel = e.maxVel - e.minVel;
						p.vel.x *= static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
						p.vel.y *= static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
						p.vel += e.minVel;

						p.scaleVel = e.maxScaleVel - e.minScaleVel;
						p.scaleVel.x *= static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
						p.scaleVel.y *= static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
						p.scaleVel += e.minScaleVel;

						p.accel = e.maxAccel - e.minAccel;
						p.accel.x *= static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
						p.accel.y *= static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
						p.accel += e.minAccel;

						p.scaleAccel = e.maxScaleAccel - e.minScaleAccel;
						p.scaleAccel.x *= static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
						p.scaleAccel.y *= static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
						p.scaleAccel += e.minScaleAccel;

                        p.dirAccel = e.maxDirAccel - e.minDirAccel;
                        p.dirAccel.x *= static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
                        p.dirAccel.y *= static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
                        p.dirAccel += e.minDirAccel;

                        p.scaleDirAccel = e.maxScaleDirAccel - e.minScaleDirAccel;
                        p.scaleDirAccel.x *= static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
                        p.scaleDirAccel.y *= static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
                        p.scaleDirAccel += e.minScaleDirAccel;


						p.col = e.maxCol - e.minCol;
						p.col = p.col * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
						p.col = p.col + e.minCol;

						p.colVel = e.maxColVel - e.minColVel;
						p.colVel = p.colVel * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
						p.colVel = p.colVel + e.minColVel;

						p.colAccel = e.maxColAccel - e.minColAccel;
						p.colAccel = p.colAccel * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
						p.colAccel = p.colAccel + e.minColAccel;

                        p.colDirAccel = e.maxColDirAccel - e.minColDirAccel;
                        p.colDirAccel = p.colDirAccel * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
                        p.colDirAccel = p.colDirAccel + e.minColDirAccel;

						if (comp.renderOption == 1 && ent.HasComponent<AnimationComponent>())
						{
							auto& animComp = ent.GetComponent<AnimationComponent>();

							if (!animComp.currentAnimation.empty() &&
								animComp.animations.count(animComp.currentAnimation) > 0)
							{
								const auto& currentAnim = animComp.animations.at(animComp.currentAnimation);

								// Initialize the new particle to start at the animation�s first frame
								p.frameX = currentAnim.startX;
								p.frameY = currentAnim.startY;
								p.animTimeAccumulator = 0.0f;

							}

						}
					}
				}

				if (!e.loop && e.tLoop < e.elapsedTime) // basic loop
				{
					e.active = false;
				}
			}
		}
	}

	void ParticleSystem::ApplyPhysics(Particle& p, const ParticleComponent& comp) // TODO
	{
		float deltaTime = static_cast<float>(Engine::timer.GetFixedDeltaTime());

		if (movementSystem != nullptr) {
			movementSystem->ApplyPhysicsParticles(p,
										comp.particleMass,
										comp.useGravity,
										comp.gravityScale,
										comp.dragCoefficient,
										deltaTime);
		} 
		else {
			if (comp.useGravity) {
				p.accel.y += comp.gravityScale * 9.8f;
			}

			// Apply drag
			float velocityMagnitudeSquared = p.vel.x * p.vel.x + p.vel.y * p.vel.y;
			if (velocityMagnitudeSquared > 0.01f) {
				float dragForceX = -comp.dragCoefficient * p.vel.x;
				float dragForceY = -comp.dragCoefficient * p.vel.y;

				// Apply drag force to acceleration (F = ma, so a = F/m)
				p.accel.x += dragForceX / comp.particleMass;
				p.accel.y += dragForceY / comp.particleMass;
			}

			//update velocity and position 
			p.vel.x += p.accel.x * deltaTime;
			p.vel.y += p.accel.y * deltaTime;
			p.pos.x += p.vel.x * deltaTime;
			p.pos.y += p.vel.y * deltaTime;
		}
	}
}