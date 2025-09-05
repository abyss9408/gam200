/******************************************************************************/
/*!
\file   AnimationSystem.cpp
\author Team Popplio
\author Hafawati Binte Mohd Ali (100%)
\par    Course : CSD2400
\par    Section : A
\date   2025/03/12 09:50:22 AM (Last Edit)
\brief
This source file defines the AnimationSystem class, which is used for handling
animations in the game engine. It includes functions for updating animations,
subscribing to events, and handling key presses.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include "../Engine/Engine.h"
#include "AnimationSystem.h"

namespace Popplio
{
    AnimationSystem::AnimationSystem()
    {
        RequireComponent<AnimationComponent>();
    }

    /*void AnimationSystem::SubscribeToEvents(const std::unique_ptr<EventBus>& eventBus)
    {
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &AnimationSystem::OnKeyPressed);
    }*/

    void AnimationSystem::StopAll()
    {
        for (auto& entity : GetSystemEntities())
        {
            if (!entity.GetComponent<ActiveComponent>().isActive) continue;
            if (!entity.HasComponent<AnimationComponent>()) continue;
            if (!entity.GetComponent<AnimationComponent>().isActive) continue;

            auto& animation = entity.GetComponent<AnimationComponent>();

            // If there is no current animation skip this entity
            if (animation.currentAnimation.empty()) continue;

            animation.Stop(); // why is there no stop?
        }
    }

    void AnimationSystem::PauseAll()
    {
        for (auto& entity : GetSystemEntities())
        {
            if (!entity.GetComponent<ActiveComponent>().isActive) continue;
            if (!entity.HasComponent<AnimationComponent>()) continue;
            if (!entity.GetComponent<AnimationComponent>().isActive) continue;

            auto& animation = entity.GetComponent<AnimationComponent>();

            // If there is no current animation skip this entity
            if (animation.currentAnimation.empty()) continue;

            if (animation.isPlaying) animation.Pause(); // why is there no stop?
        }
    }

    void AnimationSystem::StartAll()
    {
        for (auto& entity : GetSystemEntities())
        {
            if (!entity.GetComponent<ActiveComponent>().isActive) continue;
            if (!entity.HasComponent<AnimationComponent>()) continue;
            if (!entity.GetComponent<AnimationComponent>().isActive) continue;

            auto& animation = entity.GetComponent<AnimationComponent>();

            // If there is no current animation skip this entity
            if (animation.currentAnimation.empty()) continue;

            // Initialize dimensions
            animation.CalculateDimensions();

            animation.Play(); // why is there no start?
        }
    }

    void AnimationSystem::ResumeAll()
    {
        for (auto& entity : GetSystemEntities())
        {
            if (!entity.GetComponent<ActiveComponent>().isActive) continue;
            if (!entity.HasComponent<AnimationComponent>()) continue;
            if (!entity.GetComponent<AnimationComponent>().isActive) continue;

            auto& animation = entity.GetComponent<AnimationComponent>();

            // If there is no current animation skip this entity
            if (animation.currentAnimation.empty()) continue;

            // Initialize dimensions
            animation.CalculateDimensions();

            animation.Resume(); // why is there no start?
        }
    }

    void AnimationSystem::Update(float deltaTime)
    {
        // Iterate through all entities managed by this system
        for (int step = 0; step < Engine::timer.GetCurrentNumberOfSteps(); ++step)
        {
            for (auto& entity : GetSystemEntities())
            {
                if (entity.GetComponent<ActiveComponent>().isActive == false) continue;

                // Check if the entity has an AnimationComponent
                if (entity.HasComponent<AnimationComponent>())
                {
                    if (!entity.GetComponent<AnimationComponent>().isActive) continue;

                    auto& animation = entity.GetComponent<AnimationComponent>();

                    // If there is no current animation or the animation is paused, skip this entity
                    if (animation.currentAnimation.empty() || animation.isPaused)
                        continue;

                    // Get the current animation data using the animation name
                    const auto& currentAnim = animation.animations[animation.currentAnimation];

                    // Accumulate the delta time (time elapsed since the last frame)
                    animation.timeAccumulator += deltaTime;
                    animation.elapsedTimeSinceLastResume += deltaTime;
                    animation.elapsedTimeCurrentCycle += deltaTime;

                    // If enough time has passed to advance the animation frame
                    if (animation.timeAccumulator >= currentAnim.animationSpeed)
                    {
                        // Calculate how many frames to advance
                        int framesToAdvance = static_cast<int>(animation.timeAccumulator / currentAnim.animationSpeed);

                        // Reduce the accumulator by the amount of time for the advanced frames
                        animation.timeAccumulator = fmod(animation.timeAccumulator, currentAnim.animationSpeed);

                        // Update the current frame index based on the frames to advance
                        for (int i = 0; i < framesToAdvance; ++i)
                        {
                            //Logger::Critical(std::to_string(animation.currentFrameX));
                            //Logger::Critical(std::to_string(animation.currentFrameY));

                            ++animation.currentFrameX;

                            // If the end of the animation is reached, loop back to the start
                            if (animation.currentFrameX > currentAnim.endX)
                            {
                                // do not restart prematurely if end row is not reached and
                                // the current frame X is less than the dimension X
                                if (currentAnim.dimensionX > animation.currentFrameX &&
                                    animation.currentFrameY > currentAnim.endY)
                                {
                                    continue;
                                }

                                if (animation.currentFrameY > currentAnim.endY)
                                    animation.currentFrameX = 0;
                                else
                                    animation.currentFrameX = currentAnim.startX;

                                if (animation.currentFrameY <= currentAnim.endY || 
                                    animation.currentFrameY <= 0)
                                {
                                    animation.currentFrameY = currentAnim.startY;
                                    
                                    animation.elapsedTimeCurrentCycle = 0.f;

                                    // If the animation is set to play only once, stop it after one cycle
                                    if (animation.playOnce)
                                    {
                                        animation.Stop();
                                        break;
                                    }
                                }
                                else
                                {
                                    --animation.currentFrameY;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}