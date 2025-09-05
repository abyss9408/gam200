/******************************************************************************/
/*!
\file   RigidBodyComponent.h
\author Team Popplio
\author Rio Shannon Yvon Leonardo - 100%
\par    Course : CSD2400/CSD2401 - CSD2450/CSD2451
\par    Section : A
\date   2024/10/03, Last Update: 2025/01/26
\brief
        This file defines the constructors and parameters for the RigidBodyComponent 
        as well as the declarations for the functions. Some parameters include velocity,
        acceleration, mass, gravity and drag to be able to use physics forces in the game.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include "../Math/Vector2D.h" //for vector 
#include "../Physics/Forces.h" //force manager
#include <cmath>

namespace Popplio
{
    struct RigidBodyComponent
    {
        bool isActive;

        PopplioMath::Vec2f velocity;
        PopplioMath::Vec2f acceleration;
        PopplioMath::Vec2f force;

        float mass;
        float gravityScale;
        bool useGravity;    //whether this object uses gravity
        float dragCoefficient;

        ForceManager forceManager; //add default constructor for forces

        //constructor with default values

        RigidBodyComponent (
            float mass_ = 0.0f,
            bool useGravity_ = false,
            float gravityScale_ = 0.0f,
            float dragCoefficient_ = 0.1f
        ) : 
            isActive(true), 
            velocity(),
            acceleration(),
            force(),
            mass(mass_),
            useGravity(useGravity_),
            gravityScale(gravityScale_),
            dragCoefficient(dragCoefficient_)
        {}

        // Constructor with initial velocity
        RigidBodyComponent(
            float mass_,
            bool useGravity_,
            float gravityScale_,
            float initial_vel_x,
            float initial_vel_y,
            float dragCoefficient_ = 0.1f
        ) :
            isActive(true), 
            velocity(initial_vel_x, initial_vel_y),
            acceleration(),
            force(),
            mass(mass_),
            useGravity(useGravity_),
            gravityScale(gravityScale_),
            dragCoefficient(dragCoefficient_)
        {}
       


        //float dx, dy;
        //VelocityComponent(float dx = 0, float dy = 0) : dx(dx), dy(dy) {}
        //PopplioMath::Vec2 velocity;
        //RigidBodyComponent(float dx = 0, float dy = 0) : velocity(dx, dy) {}
        //to test for gravity
        //VelocityComponent(float dx = 1, float dy = 1) : dx(dx), dy(dy) {}
    };
}