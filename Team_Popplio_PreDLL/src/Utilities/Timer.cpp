/******************************************************************************/
/*!
\file   Timer.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\co-author  Bryan
\contribution Shawn - 50% Bryan - 50%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
		Source file for timer operations.
		Set time and update time.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include "Timer.h"

#include "../Engine/Engine.h"

namespace PopplioUtil
{
    Timer::Timer()
		: currentTime(0.0), deltaTime(0.0), accumulatedTime(0.0), 
        fixedDeltaTime(POPPLIO_DEFAULT_FIXED_DELTA_TIME), systemFixedDeltaTime(POPPLIO_DEFAULT_FIXED_DELTA_TIME),
        currentNumberOfSteps(0), elapsedTime(0.f), 
        frameCount(0), fpsTimer(0.0)
	{

	}

    void Timer::Update(double& framesPerSecond, double fpsCalcInterval)
    {
        // reset
		currentNumberOfSteps = 0;

        // Calculate actual delta time
        double newTime = glfwGetTime();
        deltaTime = newTime - currentTime;
        currentTime = newTime;

        // Accumulate actual game loop time
        accumulatedTime += deltaTime;

        // Check if we should perform a fixed update
        while (accumulatedTime >= fixedDeltaTime)
        {
            accumulatedTime -= fixedDeltaTime;
			++currentNumberOfSteps;
        }

        // Update FPS calculation
        ++frameCount;
        fpsTimer += deltaTime;
        if (fpsTimer >= fpsCalcInterval)
        {
            framesPerSecond = frameCount / fpsTimer;
            frameCount = 0;
            fpsTimer = 0;
        }

        elapsedTime += static_cast<float>(deltaTime);
    }

    double Timer::GetDeltaTime() const
    {
        return deltaTime;
    }

    double Timer::GetFixedDeltaTime() const
    {
        return fixedDeltaTime;
    }

    double Timer::GetAccumulatedTime() const
    {
        return accumulatedTime;
    }


    double Timer::GetSystemFixedDeltaTime() const
    {
        return systemFixedDeltaTime;
    }

    void Timer::SetFixedDeltaTime(double timestep)
	{
		fixedDeltaTime = timestep;
	}

    void Timer::ResetFixedDeltaTime()
    {
        fixedDeltaTime = POPPLIO_DEFAULT_FIXED_DELTA_TIME;
    }

	int Timer::GetCurrentNumberOfSteps() const
	{
		return currentNumberOfSteps;
	}

    void Timer::SetSystemFixedDeltaTime(double timestep)
    {
        Popplio::Logger::Warning("CHANGING SYSTEM FIXED DELTA TIME");
        Popplio::Logger::Warning("Changing to: " + std::to_string(timestep));
        systemFixedDeltaTime = timestep;
    }

    void Timer::ResetSystemFixedDeltaTime()
    {
        systemFixedDeltaTime = POPPLIO_DEFAULT_FIXED_DELTA_TIME;
    }

    float Timer::GetElapsedTime() const
    {
        return elapsedTime;
    }
}