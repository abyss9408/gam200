/******************************************************************************/
/*!
\file   Timer.h
\author Team Popplio
\author Ng Jun Heng Shawn
\co-author  Bryan
\contribution Shawn - 50% Bryan - 50%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
        Header file for timer operations.
        Set time and update time.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#define POPPLIO_DEFAULT_FIXED_DELTA_TIME 1.0 / 60.0

namespace PopplioUtil
{
    class Timer
    {
    public:

        Timer();

        void Update(double& framesPerSecond, double fpsCalcInterval);

        // Getters
        double GetDeltaTime() const;
        double GetFixedDeltaTime() const;
        double GetAccumulatedTime() const;
        double GetSystemFixedDeltaTime() const;

        // Setters
        void SetFixedDeltaTime(double timestep);
        void ResetFixedDeltaTime();
        void SetSystemFixedDeltaTime(double timestep);
        void ResetSystemFixedDeltaTime();
        
		// Get current number of fixed updates
		int GetCurrentNumberOfSteps() const;

        float GetElapsedTime() const;

    private:
        double currentTime;      // Current game time
        double deltaTime;        // Time between frames
        double accumulatedTime;      // Accumulated time for fixed updates
        double fixedDeltaTime;   // Fixed time step duration
        double systemFixedDeltaTime; // Fixed time step duration for systems // DO NOT CHANGE UNLESS NECESSARY
		int currentNumberOfSteps;

        float elapsedTime;      // Elapsed time since program start

        int frameCount;          // Number of frames for FPS calculation
        double fpsTimer;         // Timer for FPS calculation
    };
}