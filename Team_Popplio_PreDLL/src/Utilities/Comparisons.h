/******************************************************************************/
/*!
\file           Comparisons.h
\author         Team Popplio
\author         Bryan Ang Wei Ze
\co-author      Ng Jun Heng Shawn
\contribution   Bryan - 85% | Shawn - 15%
\par            Course : CSD2401 / CSD2400
\par            Section : A
\date           2024/11/06
\brief
    This is the source file for comparison functions for components
    To be used by scripts and engine

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include <type_traits>
#include "../Utilities/Components.h"

namespace Popplio
{
    template<typename T>
    bool CompareComponents(const T& a, const T& b)
    {
        // early return
        // Add more component types as needed

        if constexpr (std::is_same_v<T, TransformComponent>)
        {
            return a.position == b.position &&
                a.scale == b.scale &&
                a.rotation == b.rotation;
        }

        if constexpr (std::is_same_v<T, ActiveComponent>)
        {
            return a.isActive == b.isActive;
        }

        if constexpr (std::is_same_v<T, RenderComponent>)
        {
            return a.isActive == b.isActive &&
                a.meshType == b.meshType &&
                a.shaderName == b.shaderName &&
                a.textureName == b.textureName &&
                a.useInstancing == b.useInstancing &&
                a.color == b.color;
        }

        if constexpr (std::is_same_v<T, RigidBodyComponent>)
        {
			return a.isActive == b.isActive &&
                a.velocity == b.velocity &&
				a.acceleration == b.acceleration &&
				a.force == b.force &&
				a.mass == b.mass &&
				a.gravityScale == b.gravityScale &&
				a.useGravity == b.useGravity &&
				a.dragCoefficient == b.dragCoefficient;
        }

        if constexpr (std::is_same_v<T, BoxColliderComponent>)
        {
            return a.isActive == b.isActive &&
                a.size == b.size &&
                a.offset == b.offset && 
				a.isTrigger == b.isTrigger &&
				a.isEnabled == b.isEnabled;
        }

        if constexpr (std::is_same_v<T, TextComponent>)
        {
            return a.isActive == b.isActive &&
                a.text == b.text &&
                a.fontName == b.fontName &&
                a.fontSize == b.fontSize &&
                a.color == b.color;
        }

        if constexpr (std::is_same_v<T, AnimationComponent>)
        {
            return a.isActive == b.isActive &&
                //a.animations == b.animations && // TODO fix
                a.currentAnimation == b.currentAnimation &&
                a.currentFrameX == b.currentFrameX &&
                a.sheetWidth == b.sheetWidth &&
                a.sheetHeight == b.sheetHeight &&
                a.timeAccumulator == b.timeAccumulator &&
                a.isPaused == b.isPaused &&
                a.playOnce == b.playOnce;
        }

        if constexpr (std::is_same_v<T, UIComponent>)
        {
            return a.isActive == b.isActive &&
                a.type == b.type &&
                a.isHovered == b.hasHoverEffect &&
                a.frameX == b.frameY &&
                a.hoverFrameX == b.hoverFrameY &&
                a.totalColumns == b.totalColumns &&
                a.totalRows == b.totalRows &&
                a.sheetWidth == b.sheetWidth &&
                a.sheetHeight == b.sheetHeight &&
                a.checked == b.checked &&
                a.sliderValue == b.sliderValue &&
                a.linkedEntityId == b.linkedEntityId;
        }

        if constexpr (std::is_same_v<T, LogicComponent>)
        {
            return a.isActive == b.isActive &&
                a.scriptName == b.scriptName;
        }

        if constexpr (std::is_same_v<T, AudioComponent>)
        {
            return a.isActive == b.isActive &&
                a.audio == b.audio && 
                a.volume == b.volume && 
                a.loop == b.loop && 
                a.bpm == b.bpm && 
                a.timeSig == b.timeSig &&
                a.playOnAwake == b.playOnAwake;
        }

        if constexpr (std::is_same_v<T, ParentComponent>)
        {
            return a.parent == b.parent &&
                a.children == b.children && 
                a.inheritActive == b.inheritActive && 
                a.inheritTransform == b.inheritTransform;
        }

        if constexpr (std::is_same_v<T, PersistOnLoadComponent>)
        {
            return a.persistOnLoad == b.persistOnLoad;
        }

        if constexpr (std::is_same_v<T, ParticleComponent>)
        {
            bool result = a.isActive == b.isActive &&
                a.renderOption == b.renderOption;

            if (a.emission.size() != b.emission.size())
            {
                return false;
            }

            for (size_t i{}; i < a.emission.size(); ++i)
            {
                result = result &&
                    a.emission[i].active == b.emission[i].active &&
                    a.emission[i].loop == b.emission[i].loop &&
                    a.emission[i].elapsedTime == b.emission[i].elapsedTime &&
                    a.emission[i].rate == b.emission[i].rate &&
                    a.emission[i].tLoop == b.emission[i].tLoop &&
                    a.emission[i].minLifeTime == b.emission[i].minLifeTime &&
                    a.emission[i].maxLifeTime == b.emission[i].maxLifeTime &&
                    a.emission[i].minRot == b.emission[i].minRot &&
                    a.emission[i].maxRot == b.emission[i].maxRot &&
                    a.emission[i].minSpinVel == b.emission[i].minSpinVel &&
                    a.emission[i].maxSpinVel == b.emission[i].maxSpinVel &&
                    a.emission[i].minSpinAccel == b.emission[i].minSpinAccel &&
                    a.emission[i].maxSpinAccel == b.emission[i].maxSpinAccel &&
                    a.emission[i].minPos == b.emission[i].minPos &&
                    a.emission[i].maxPos == b.emission[i].maxPos &&
                    a.emission[i].minScale == b.emission[i].minScale &&
                    a.emission[i].maxScale == b.emission[i].maxScale &&
                    a.emission[i].minVel == b.emission[i].minVel &&
                    a.emission[i].maxVel == b.emission[i].maxVel &&
                    a.emission[i].minScaleVel == b.emission[i].minScaleVel &&
                    a.emission[i].maxScaleVel == b.emission[i].maxScaleVel &&
                    a.emission[i].minAccel == b.emission[i].minAccel &&
                    a.emission[i].maxAccel == b.emission[i].maxAccel &&
                    a.emission[i].minScaleAccel == b.emission[i].minScaleAccel &&
                    a.emission[i].maxScaleAccel == b.emission[i].maxScaleAccel &&
                    a.emission[i].minCol == b.emission[i].minCol &&
                    a.emission[i].maxCol == b.emission[i].maxCol &&
                    a.emission[i].minColVel == b.emission[i].minColVel &&
                    a.emission[i].maxColVel == b.emission[i].maxColVel &&
                    a.emission[i].minColAccel == b.emission[i].minColAccel &&
                    a.emission[i].maxColAccel == b.emission[i].maxColAccel; //&&
                    //a.emission[i].texture == b.emission[i].texture &&
                    //a.emission[i].animation == b.emission[i].animation;
            }

            return result;
        }

        // Add more component types as needed
    }
}