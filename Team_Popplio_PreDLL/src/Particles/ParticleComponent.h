/******************************************************************************/
/*!
\file   ParticleComponent.h
\author Team Popplio
\author Ng Jun Heng Shawn
\co-authors(s) Rio Shannon Yvon Leonardo
\contribution Shawn - 90%, Shannon - 10%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        Component for an instance of a Particle System attached to an entity

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

//#include "ParticleSystem.h"

#include "../Graphic/Color.h"
#include "../Utilities/Constants.h"

#include <optional>

namespace Popplio
{
	//Particle System structures
	struct Particle
	{
		public:
		//unsigned int id;
		bool alive;

		//AEVec2 pos;
		//AEVec2 scale;

		float lifeTime;

		float rot, spinVel, spinAccel, spinDirAccel;

		PopplioMath::Vec2f vel, scaleVel;
		PopplioMath::Vec2f accel, scaleAccel;
        PopplioMath::Vec2f dirAccel, scaleDirAccel;

		PopplioMath::Vec2f pos, scale;

		Color col, colVel, colAccel, colDirAccel;

		// for animation state
		int frameX, frameY;
		float animTimeAccumulator;

		//std::optional<RenderComponent> texture;
		//std::optional<AnimationComponent> animation;

		Particle() : alive(false), lifeTime(0.f), rot(0.f), vel(), spinVel(0.f), scaleVel(),
			accel(), spinAccel(0.f), scaleAccel(), pos(), scale(), col(), colVel(Color().Zero()), colAccel(Color().Zero()),
            colDirAccel(Color().Zero()), spinDirAccel(0.f), dirAccel(), scaleDirAccel(),
			frameX(0), frameY(0), animTimeAccumulator(0.f)
		{}

		Particle& operator()() { *this = Particle(); return *this; }
	};

	struct Emitter
	{
		public:
		// serialize
		bool active;

		bool loop;

		float elapsedTime; // set to non-zero for warmup
		float rate;
		float tLoop; // duration per loop

		//float tWarmUp;

		float	minLifeTime, maxLifeTime;

		float	minRot, maxRot, minSpinVel, maxSpinVel, minSpinAccel, maxSpinAccel, 
			minSpinDirAccel, maxSpinDirAccel;

		PopplioMath::Vec2f	minPos, maxPos,
			minScale, maxScale;

		PopplioMath::Vec2f	minVel, maxVel,
			minScaleVel, maxScaleVel;

		PopplioMath::Vec2f	minAccel, maxAccel,
			minScaleAccel, maxScaleAccel;

		PopplioMath::Vec2f	minDirAccel, maxDirAccel,
			minScaleDirAccel, maxScaleDirAccel;

		Color	minCol, maxCol,
			minColVel, maxColVel,
			minColAccel, maxColAccel,
			minColDirAccel, maxColDirAccel;

		// runtime DO NOT SERIALIZE
        //std::optional<RenderComponent> texture; 
        //std::optional<AnimationComponent> animation;

        Emitter() : active(false), loop(false), 
			elapsedTime(0.f), rate(0.f), tLoop(0.f),
			minLifeTime(0.f), maxLifeTime(0.f),
			minRot(0.f), maxRot(0.f), minSpinVel(0.f), maxSpinVel(0.f), minSpinAccel(0.f), maxSpinAccel(0.f),
            minSpinDirAccel(0.f), maxSpinDirAccel(0.f),
			minPos(), maxPos(), minVel(), maxVel(), minAccel(), maxAccel(),
			minScale(), maxScale(), minScaleVel(), maxScaleVel(), minScaleAccel(), maxScaleAccel(),
            minDirAccel(), maxDirAccel(), minScaleDirAccel(), maxScaleDirAccel(),
			minCol(Color().Zero()), maxCol(), minColVel(Color().Zero()), maxColVel(Color().Zero()),
			minColAccel(Color().Zero()), maxColAccel(Color().Zero()),
            minColDirAccel(Color().Zero()), maxColDirAccel(Color().Zero())
		{}

		Emitter& operator()() { *this = Emitter(); return *this; }
	};

    struct ParticleComponent
    {
		public:

        bool isActive; // serialize
        int renderOption; // serialize
		// 0 = Texture, 1 = Animation // Get from entity's RenderComponent or AnimationComponent

		//bool usePhysics; // serialize

        //ParticleSystem::IComponent* instance;

		//ParticleSystem& sys;

		std::vector<Particle>	pool; // runtime DO NOT SERIALIZE
		std::vector<Emitter>	emission; // serialize

		size_t countAlive; // runtime DO NOT SERIALIZE
		size_t countEmission; // serialize

		//Physics Properties (similar to RigidBodyComponent)
		bool usePhysics; // serialize
		float particleMass;
		float useGravity;
		float gravityScale;
		float dragCoefficient;

        ParticleComponent() : isActive(true), renderOption(0), usePhysics(false), 
			pool(PARTICLE_SYSTEM_PARTICLE_MAX), emission(PARTICLE_SYSTEM_EMISSION_MAX), 
			countAlive(0), countEmission(0),
			//initialize physics properties with defaults
			particleMass(1.0f), useGravity(true), gravityScale(1.0f), dragCoefficient(0.1f)
		{
			//pool.reserve(PARTICLE_SYSTEM_PARTICLE_MAX);
			//emission.reserve(PARTICLE_SYSTEM_EMISSION_MAX);
		}
	};
}