/******************************************************************************/
/*!
\file   AnimationComponent.h
\author Team Popplio
\author Hafawati Binte Mohd Ali (100%)
\par    Course : CSD2400
\par    Section : A
\date   2025/03/12 09:50:22 AM (Last Edit)
\brief

This file declares and defines the Animation and AnimationComponent structs, which
are used for handling animations in the game engine. It includes functions for
initializing animations, adding animations, setting the current animation,
pausing and resuming animations, and getting the UV coordinates for the current
frame.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include "../Animation/AnimationSystem.h"

namespace Popplio
{

    class AnimationSystem;
    /*!*************************************************************************
    \brief
    The Animation struct holds individual animation data, including the start
    and end frames, animation speed, name, and frame dimensions.
    *************************************************************************/
    struct Animation
    {
            int startX, startY;   /*!< The starting frame coordinates. */
            int endX, endY;       /*!< The ending frame coordinates. */
            float animationSpeed; /*!< The speed of the animation. */ // DURATION PER FRAME - 1/24f = 24fps
            std::string name;     /*!< The name of the animation. */
            float frameWidth;     /*!< The width of each frame. */
            float frameHeight;    /*!< The height of each frame. */

            // do not serialize
            int dimensionX, dimensionY; /*!< The dimensions of the sprite sheet split into frames. */

            /*!*************************************************************************
            \brief
            Default constructor for the Animation struct.
            *************************************************************************/
            Animation()
                : startX(0), startY(0), endX(0), endY(0), animationSpeed(0),
                name(""), frameWidth(0), frameHeight(0), dimensionX(0), dimensionY(0) {}

            /*!*************************************************************************
            \brief
            Constructor that initializes an Animation with the specified parameters.
            \param[in] sX
                    The starting X coordinate.
            \param[in] sY
                    The starting Y coordinate.
            \param[in] eX
                    The ending X coordinate.
            \param[in] eY
                    The ending Y coordinate.
            \param[in] speed
                    The speed of the animation.
            \param[in] animName
                    The name of the animation.
            \param[in] fWidth
                    The width of each frame.
            \param[in] fHeight
                    The height of each frame.
            *************************************************************************/
            Animation(int sX, int sY, int eX, int eY, float speed,
                    const std::string &animName, float fWidth, float fHeight)
                : startX(sX), startY(sY), endX(eX), endY(eY), animationSpeed(speed),
                name(animName), frameWidth(fWidth), frameHeight(fHeight), 
                dimensionX(0), dimensionY(0) { }
    };

    /*!*************************************************************************
    \brief
    The AnimationComponent struct holds the animations for an entity, including
    the current animation, current frame, sheet dimensions, time accumulator,
    and flags for pausing and playing once.
    *************************************************************************/
    struct AnimationComponent
    {
            bool isActive;
            std::unordered_map<std::string, Animation> animations; /*!< A map of animations. */
            std::string currentAnimation;                          /*!< The name of the current animation. */
            int currentFrameX, currentFrameY;                      /*!< The current frame coordinates. */
            float sheetWidth, sheetHeight;                         /*!< The dimensions of the sprite sheet. */
            bool playOnce;                                         /*!< Flag indicating if the animation should play once. */

            // Do not serialize
            float timeAccumulator;                                 /*!< The time accumulator for the animation. */
            bool isPaused;                                         /*!< Flag indicating if the animation is paused. */
            bool isPlaying;

            // Temporary storage for editing properties
            char newAnimName[64]; 
            int newStartX, newStartY;
            int newEndX, newEndY;
            float newFrameWidth, newFrameHeight;
            float newAnimSpeed;

            int editStartX, editStartY;
            int editEndX, editEndY;
            float editFrameWidth, editFrameHeight;
            float editAnimSpeed;

            bool propertiesLoaded;
            float elapsedTimeSinceLastResume;
            float elapsedTimeCurrentCycle;

            /*!*************************************************************************
            \brief
            Default constructor for the AnimationComponent struct.
            *************************************************************************/
            AnimationComponent()
                : isActive(true), currentFrameX(0), currentFrameY(0), sheetWidth(0), sheetHeight(0),
                timeAccumulator(0), isPaused(false), isPlaying(false), playOnce(false), propertiesLoaded(false),
                elapsedTimeSinceLastResume(0.f), elapsedTimeCurrentCycle(0.f),
                newStartX(0), newStartY(0), newEndX(0), newEndY(0),
                newFrameWidth(0.0f), newFrameHeight(0.0f), newAnimSpeed(1.0f),
                editStartX(0), editStartY(0), editEndX(0), editEndY(0),
                editFrameWidth(0.0f), editFrameHeight(0.0f), editAnimSpeed(1.0f)
            {
                newAnimName[0] = '\0'; 
            }

            /*!*************************************************************************
            \brief
            Constructor that initializes an AnimationComponent with the specified
            sheet dimensions.
            \param[in] sWidth
                    The width of the sprite sheet.
            \param[in] sHeight
                    The height of the sprite sheet.
            *************************************************************************/
            AnimationComponent(float sWidth, float sHeight)
                : isActive(true), sheetWidth(sWidth), sheetHeight(sHeight), timeAccumulator(0.0f),
                currentFrameX(0), currentFrameY(0), 
                isPaused(false), isPlaying(false), playOnce(false), propertiesLoaded(false),
                elapsedTimeSinceLastResume(0.f), elapsedTimeCurrentCycle(0.f),
                newStartX(0), newStartY(0), newEndX(0), newEndY(0),
                newFrameWidth(0.0f), newFrameHeight(0.0f), newAnimSpeed(1.0f),
                editStartX(0), editStartY(0), editEndX(0), editEndY(0),
                editFrameWidth(0.0f), editFrameHeight(0.0f), editAnimSpeed(1.0f)
            {
                newAnimName[0] = '\0'; 
            }

            /*!*************************************************************************
            \brief
            Initializes the AnimationComponent with the specified sheet dimensions.
            \param[in] sWidth
                    The width of the sprite sheet.
            \param[in] sHeight
                    The height of the sprite sheet.
            *************************************************************************/
            void Initialize(float sWidth, float sHeight)
            {
                    sheetWidth = sWidth;
                    sheetHeight = sHeight;
                    timeAccumulator = 0.0f;
                    currentFrameX = 0;
                    currentFrameY = 0;

                    //elapsedTimeCurrentCycle = 0.f;
                    //elapsedTimeSinceLastResume = 0.f;

                    isPaused = false;
                    playOnce = false;
            }

            /*!*************************************************************************
            \brief
            Adds an animation to the AnimationComponent.
            \param[in] name
                    The name of the animation.
            \param[in] startX
                    The starting X coordinate.
            \param[in] startY
                    The starting Y coordinate.
            \param[in] endX
                    The ending X coordinate.
            \param[in] endY
                    The ending Y coordinate.
            \param[in] speed
                    The speed of the animation.
            \param[in] frameWidth
                    The width of each frame.
            \param[in] frameHeight
                    The height of each frame.
            *************************************************************************/
            void AddAnimation(const std::string &name, int startX, int startY,
                            int endX, int endY, float speed,
                            float frameWidth, float frameHeight)
            {
                    animations.emplace(name,
                                    Animation(startX, startY, endX, endY, speed, name, frameWidth, frameHeight));
                    if (currentAnimation.empty())
                    {
                        currentAnimation = name;
                        currentFrameX = startX;
                        currentFrameY = startY;
                    }
                    CalculateDimensions(name);
            }

            /*!*************************************************************************
            \brief
            Sets the current animation.
            \param[in] name
                    The name of the animation.
            \param[in] onlyPlayOnce
                    Flag indicating if the animation should play once.
            *************************************************************************/
            void SetAnimation(const std::string &name, bool onlyPlayOnce = false)
            {
                if (animations.count(name) > 0)
                {
                    currentAnimation = name;
                    const auto& anim = animations[name];
                    currentFrameX = anim.startX;
                    currentFrameY = anim.startY;
                    timeAccumulator = 0.0f;
                    //isPaused = false;
                    this->playOnce = onlyPlayOnce;

                    CalculateDimensions(name);
                }
            }

            /*!*************************************************************************
            \brief
            Stops the current animation by resetting it to its start frame and pausing it.
            *************************************************************************/
            void Stop()
            {
                if (!currentAnimation.empty())
                {
                    const auto& anim = animations[currentAnimation];
                    currentFrameX = anim.startX;
                    currentFrameY = anim.startY;
                    timeAccumulator = 0.0f;
                    //isPaused = true;
                    isPlaying = false;
                }
            }

            ///*!*************************************************************************
            //\brief
            //Starts the current animation by resetting it to its start frame and unpausing it.
            //*************************************************************************/
            //void Start(const std::string& animationName)
            //{
            //    if (animations.count(animationName) > 0)
            //    {
            //        currentAnimation = animationName;
            //        const auto& anim = animations[animationName];
            //        currentFrameX = anim.startX;
            //        currentFrameY = anim.startY;
            //        timeAccumulator = 0.0f;
            //        isPaused = false;
            //    }
            //}

            /*!*************************************************************************
            \brief
            Pauses the current animation.
            *************************************************************************/
            void Pause()
            {
                    isPaused = true;
            }

            /*!*************************************************************************
            \brief
            Play the current animation from a paused or stopped state
            *************************************************************************/
            void Play()
            {
                CalculateDimensions(currentAnimation);
                isPaused = false;
                isPlaying = true;
                elapsedTimeSinceLastResume = 0.f;
            }

            /*!*************************************************************************
            \brief
            Resume the current animation from a paused state
            *************************************************************************/
            void Resume()
            {
                if (!isPlaying) return;

                CalculateDimensions(currentAnimation);
                isPaused = false;
                elapsedTimeSinceLastResume = 0.f;
            }

            /*!*************************************************************************
            \brief
            Gets the UV coordinates for the current frame.
            \return
            A glm::vec4 containing the UV coordinates.
            *************************************************************************/
            glm::vec4 GetUVCoords() const
            {
                    if (currentAnimation.empty())
                    {
                        return glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
                    }
                    const auto &currentAnim = animations.at(currentAnimation);
                    float u1 = (currentFrameX * currentAnim.frameWidth) / sheetWidth;
                    float v1 = (currentFrameY * currentAnim.frameHeight) / sheetHeight;
                    float u2 = ((currentFrameX + 1) * currentAnim.frameWidth) / sheetWidth;
                    float v2 = ((currentFrameY + 1) * currentAnim.frameHeight) / sheetHeight;
                    return glm::vec4(u1, v1, u2, v2);
            }

            /*!*************************************************************************
            \brief
            Calculates the dimensions (frame count in X and Y) of a specific animation.

            This function determines how many frames exist in the spritesheet for a
            given animation by dividing the total sheet dimensions by the individual
            frame size.

            \param[in] name
            The name of the animation for which to calculate dimensions.
            *************************************************************************/
            void CalculateDimensions(std::string const& name)
            {
                Animation& anim = animations[name];
                anim.dimensionX = static_cast<int>(sheetWidth / anim.frameWidth);
                anim.dimensionY = static_cast<int>(sheetHeight / anim.frameHeight);
            }

            /*!*************************************************************************
            \brief
            Calculates the dimensions (frame count in X and Y) for all animations.

            This function iterates through all animations and computes the number of
            frames in both the X and Y directions based on the total spritesheet size
            and individual frame sizes.
            *************************************************************************/
            void CalculateDimensions()
            {
                for (auto& anim : animations)
                {
                    anim.second.dimensionX = static_cast<int>(sheetWidth / anim.second.frameWidth);
                    anim.second.dimensionY = static_cast<int>(sheetHeight / anim.second.frameHeight);
                }
            }

    };

} // namespace Popplio
