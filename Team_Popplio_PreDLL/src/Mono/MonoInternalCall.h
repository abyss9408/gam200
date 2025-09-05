/******************************************************************************/
/*!
\file   MonoInternalCall.h
\author Team Popplio
\author Ng Jun Heng Shawn
\co-author Hafawati Binte Mohd Ali
\contribution Shawn - 92%, Hafawati - 8%
\par    Course : CSD2401 / CSD2450 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the header file for consolidating all Internal Calls to Mono
        Registers all internal calls to Mono API
        Uses Mono API to call C# functions from C++

        Add existing C++ engine code here to be used by C# scripts

        Currently incredibly unscalable

        Required for a function call between C# and C++:
        1 Add C++ function declaration to here with extern "C" static ...
        2 Add C++ function definition to MonoInternalCall.cpp with same signature
        3 Register internal call in RegisterInternalCalls() in MonoInternalCall.cpp
        4 Add C# function declaration to C# script with same signature 
        - with ... static extern ... and
        - [MethodImpl(MethodImplOptions.InternalCall)]

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

// includes for code to be used in this interface
#include "../Engine/Engine.h"

#include <mono/metadata/object.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>
#include <cstdio>

#define DLL_EXPORT __declspec(dllexport)
#define MONO_CALL __cdecl

namespace PopplioScriptAPI
{
    /*
    *   register internal calls to Mono API
    */
    extern "C" void RegisterInternalCalls();

    void SetIntCall(Popplio::Registry& registry, Popplio::LayerManager& layer, 
        Popplio::EventBus& eventB, Popplio::PrefabManager& preMan, Popplio::CameraManager& cam);

    // Internal Calls

    // Mono //

    namespace PopplioMono
    {
        extern "C"
        {
            static void AddGC(MonoObject* obj);
            static void RemoveGC(MonoObject* obj);
        }
    }
    
    // Logging/Logger.h //

    namespace PopplioLog
    {
        /*
        *   log critical messages to engine
        */
        extern "C" static void Critical(MonoString* message);
        /*
        *   log warning messages to engine
        */
        extern "C" static void Warning(MonoString* message);
        /*
        *   log debug messages to engine
        */
        extern "C" static void Debug(MonoString* message);
        /*
        *   log error messages to engine
        */
        extern "C" static void Error(MonoString* message);
        /*
        *   log info messages to engine
        */
        extern "C" static void Info(MonoString* message);

        extern "C" static void EngineCritical(MonoString* message);
        extern "C" static void EngineWarning(MonoString* message);
        extern "C" static void EngineDebug(MonoString* message);
        extern "C" static void EngineError(MonoString* message);
        extern "C" static void EngineInfo(MonoString* message);
    }

    // Engine/Engine.h //

    namespace PopplioEngine
    {
        extern "C"
        {
            static double GetFPS();
            static void AppQuit();
        }
    }

    // Input/InputSystem.h //

    namespace PopplioInput
    {
        extern "C"
        {
            static bool KeyTriggered(int key);
            
            static bool KeyDown(int key);
            
            static bool KeyReleased(int key);

            static MonoArray* KeysTriggered();

            static MonoArray* KeysDown();

            static MonoArray* KeysReleased();
            
            static bool MouseButtonTriggered(int button);
            
            static bool MouseButtonDown(int button);
            
            static bool MouseButtonReleased(int button);
            
            static float MouseX();
            
            static float MouseY();
            
            static float MouseDeltaX();
            
            static float MouseDeltaY();
            
            static float MousePrevX();
            
            static float MousePrevY();
            
            static float MouseScrollX();
            
            static float MouseScrollY();
            
            static double InputLatency(int key);
        }
    }

    // Camera/GameCamera.h //

    namespace PopplioCamera
    {
        extern "C"
        {
            static float GetPosX();
            static float GetPosY();
            static void SetPosition(float x, float y);
            static float GetZoom();
            static void SetZoom(float value);
            static float GetAspectRatio();
            static void SetTarget(int entity);
            static void ClearTarget();
            static bool HasTarget();
        }
    }

    // Utilities/Timer.h //

    namespace PopplioTime
    {
        extern "C"
        {
            static double GetDeltaTime();
            static double GetFixedDeltaTime();
            static double GetTime();
            static void SetFixedDeltaTime(double value);
            static void ResetFixedDeltaTime();
        }
    }

    // Utilities/Rand.h //

    namespace PopplioRand
    {
        extern "C"
        {
            static void SetRand (unsigned int seed);
            static int GetRand();
            static int RandInt(int min, int max);
            static float RandFloat(float min, float max);
        }
    }

    // Animation/AnimationSystem.h //

    namespace PopplioAnim
    {
        extern "C"
        {
            static void PlayAll();
            static void StopAll();
            static void ResumeAll();
            static void PauseAll();
        }
    }

    // Audio/AudioSystem.h //

    namespace PopplioAudio
    {
        extern "C"
        {
            static unsigned int GetDefaultUnit();
            
            static bool GetIsPlaying(int nativeID);
            
            static bool GetIsPaused(int nativeID);
            
            static unsigned int GetLength(int nativeID);
            
            static float GetBeatInterval(int nativeID);
            
            static unsigned int GetCurrentTime(int nativeID);

            static void SetCurrentTime(int nativeID, unsigned int time);
            
            static int GetOffBeat(int nativeID);
            
            static void Play(int nativeID);
            
            static void Pause(int nativeID);
            
            static void Stop(int nativeID);
            
            static MonoArray* GetSpectrum(int nativeID, int channel, float magMod);

            static void SetMute(int nativeID, bool m);

            static bool GetMute(int nativeID);

            
            static float GetMasterVolume();
            
            static void SetMasterVolume(float volume);
            
            static float GetBGMVolume();
            
            static void SetBGMVolume(float volume);
            
            static float GetSFXVolume();
            
            static void SetSFXVolume(float volume);
            
            static float GetGroupVolume(int group);
            
            static void SetGroupVolume(int group, float volume);
        }
    }

    // Scene/SceneManageSystem.h //

    namespace PopplioScene
    {
        extern "C"
        {
            static void Load(MonoString* sceneName, bool reloadMono);
            static MonoString* GetCurrent();
            static MonoString* GetPrevious();
            static void Reload(bool reloadMono);
        }
    }

    // Math //

    namespace PopplioMath
    {
        extern "C"
        {
            static float Dist2D(float x1, float y1, float x2, float y2);
        }
    }

    // JSON //

    namespace PopplioJson
    {
        extern "C"
        {
            static intptr_t LoadJson(MonoString* filename);
            static intptr_t GetCurrentDoc();
            static MonoString* GetCurrentDocName();
            static int LoadInt(MonoString* key);
            static float LoadFloat(MonoString* key);
            static bool LoadBool(MonoString* key);
            static MonoString* LoadString(MonoString* key);
            static void CloseIf();
            static void ClearObjs();
            static MonoArray* LoadObjArray(MonoString* key);
            static intptr_t LoadObjWithObj(intptr_t obj);
            static intptr_t LoadObj(MonoString* key);
            static int LoadObjInt(intptr_t objPtr, MonoString* key);
            static float LoadObjFloat(intptr_t objPtr, MonoString* key);
            static bool LoadObjBool(intptr_t objPtr, MonoString* key);
            static MonoString* LoadObjString(intptr_t objPtr, MonoString* key);
            static intptr_t LoadObjObj(intptr_t objPtr, MonoString* key);

            static intptr_t SaveJson1();
            static intptr_t NewObjValue();
            static intptr_t NewArrayValue();
            static void AddToArray(intptr_t array, intptr_t value);
            static intptr_t SaveBool(MonoString* key, bool value, intptr_t member);
            static intptr_t SaveInt(MonoString* key, int value, intptr_t member);
            static intptr_t SaveFloat(MonoString* key, float value, intptr_t member);
            static intptr_t SaveString(MonoString* key, MonoString* value, intptr_t member);
            static void SaveObj(MonoString* key, intptr_t value, intptr_t array);
            static bool SaveJson2(MonoString* filename);
            //static void InitValues();
        }
    }

    // ECS/ECS.h //

    namespace PopplioECS
    {
        extern "C" // Entity
        {
            /*
            *   kill entity
            */
            static void Kill(int instance);
            /*
            *   get entity id
            */
            //int GetId(void const* instance);
            /*
            *   gets entity's tag
            */
            static MonoString* GetTag(int instance);
            /*
            *   tags entity
            */
            static void Tag(int instance, MonoString* tag);
            /*
            *   check if entity has tag
            */
            static bool HasTag(int instance, MonoString* tag);
            /*
            *   add entity to group
            */
            static void Group(int instance, MonoString* group);
            /*
            *   check if entity is in group
            */
            static bool BelongsToGroup(int instance, MonoString* group);
            /*
            *   check if entity exists
            */
            static bool Exists(int instance);

            //static void OnScriptDestroy(int instance);
        }
    }

    // ECS/ECS.h // C# API //

    namespace PopplioECS
    {
        extern "C"
        {
            /*
            *   Instantiate entity
            */
            static int InstEnt(int entity);
            /*
            *   Destroy entity
            */
            static void DelEnt(int entity);

            static int GetEntCount();

            //template <typename TComponent, typename ...TArgs>
            //void AddComponent(void* instance, TArgs&& ...args);

            //template <typename TComponent>
            //void RemoveComponent(void* instance);

            //template <typename TComponent>
            //bool HasComponent(void const* instance);

            //template <typename TComponent>
            //TComponent& GetComponent(void const* instance);

            //class Registry* registry;

            // Active Component

            /*
            *   check if entity is active
            */
            static bool GetActive(int instance);

            /*
            *   set entity active state
            */
            static void SetActive(int instance, bool value);


            // Render Component

            /*
            *   check if render component exists  
            */
            static bool HasRenderComponent(int instance);

            // Rigidbody Component

            /*
            *   check if rigidbody component exists
            */
            static bool HasRigidBodyComponent(int instance);

            // BoxCollider Component

            /*
            *   check if boxcollider component exists
            */
            static bool HasBoxColliderComponent(int entity);

            // Text Component

            /*
            *   check if text component exists
            */
            static bool HasTextComponent(int entity);

            // Logic Component

            /*
            *   check if logic component exists
            */
            static bool HasLogicComponent(int entity);

            // Animation Component

            /*
            *   check if animation component exists
            */
            static bool HasAnimationComponent(int instance);

            // Audio Component

            /*
            *   check if audio component exists
            */
            static bool HasAudioComponent(int entity);

            // Parent Component

            /*
            *   check if parent component exists
            */
            static bool HasParentComponent(int entity);

            // Parent Component

            /*
            *   check if parent component exists
            */
            static bool HasUIComponent(int entity);

            // PersistOnLoad Component

            /*
            *   check if persistonload component exists
            */
            //static bool HasPersistentComponent(int entity);


            static void AddRenderComponent(int entity);
            
            static void AddRigidBodyComponent(int entity);
            
            static void AddBoxColliderComponent(int entity);
            
            static void AddTextComponent(int entity);
            
            static void AddLogicComponent(int entity);
            
            static void AddAnimationComponent(int entity);
            
            static void AddAudioComponent(int entity);

            static void AddUIComponent(int entity);

            /*
            *   add parent component to entity
            */
            static void AddParentComponent(int entity);

            //static void AddPersistentComponent(int entity);
        }
    }

    // Transformation/TransformComponent.h //

    namespace PopplioComponentTrans
    {
        extern "C" // Transform Component
        {
            static float GetPositionX(int instance);

            static void SetPositionX(int instance, float value);

            static float GetPositionY(int instance);

            static void SetPositionY(int instance, float value);

            static float GetScaleX(int instance);

            static void SetScaleX(int instance, float value);

            static float GetScaleY(int instance);

            static void SetScaleY(int instance, float value);

            static double GetRotation(int instance);

            static void SetRotation(int instance, double value);
        }
    }

    // Graphic/GraphicComponent.h //

    namespace PopplioComponentRender
    {
        extern "C" // Render Component
        {
            static float GetColorR(int instance);
            
            static void SetColorR(int instance, float value);
            
            static float GetColorG(int instance);
            
            static void SetColorG(int instance, float value);
            
            static float GetColorB(int instance);
            
            static void SetColorB(int instance, float value);
            
            static float GetColorA(int instance);
            
            static void SetColorA(int instance, float value);
            
            static int GetMeshType(int instance);
            
            static void SetMeshType(int instance, int value);
            
            static bool GetActive(int instance);
            
            static void SetActive(int instance, bool value);
            
            static MonoString* GetShader(int instance);
            
            static void SetShader(int instance, MonoString* value);
            
            static MonoString* GetTexture(int instance);
            
            static void SetTexture(int instance, MonoString* value);
            
            static bool GetInstancing(int instance);
            
            static void SetInstancing(int instance, bool value);
        }
    }

    // Physics/RigidBodyComponent.h //

    namespace PopplioComponentRB
    {
        extern "C"
        {
            static float GetVelocityX(int instance);

            static void SetVelocityX(int instance, float value);

            static float GetVelocityY(int instance);

            static void SetVelocityY(int instance, float value);


            static float GetAccelerationX(int instance);

            static void SetAccelerationX(int instance, float value);

            static float GetAccelerationY(int instance);

            static void SetAccelerationY(int instance, float value);


            static float GetForceX(int instance);

            static void SetForceX(int instance, float value);

            static float GetForceY(int instance);

            static void SetForceY(int instance, float value);


            static bool GetActive(int instance);

            static void SetActive(int instance, bool value);


            static float GetMass(int instance);

            static void SetMass(int instance, float value);

            static float GetGravityScale(int instance);

            static void SetGravityScale(int instance, float value);

            static bool GetUseGravity(int instance);

            static void SetUseGravity(int instance, bool value);

            static float GetDragCoefficient(int instance);

            static void SetDragCoefficient(int instance, float value);
        }
    }

    // Collision/BoxColliderComponent.h //

    namespace PopplioComponentBoxCollider
    {
        extern "C"
        {
            static float GetSizeX(int instance);
            
            static void SetSizeX(int instance, float value);
            
            static float GetSizeY(int instance);
            
            static void SetSizeY(int instance, float value);
            
            static float GetOffsetX(int instance);
            
            static void SetOffsetX(int instance, float value);
            
            static float GetOffsetY(int instance);
            
            static void SetOffsetY(int instance, float value);
            
            static bool GetActive(int instance);
            
            static void SetActive(int instance, bool value);
            
            static bool GetTrigger(int instance);
            
            static void SetTrigger(int instance, bool value);
            
            static bool GetEnabled(int instance);
            
            static void SetEnabled(int instance, bool value);
        }
    }

    // Graphic/GraphicComponent.h // Text Component

    namespace PopplioComponentText
    {
        extern "C"
        {
            static float GetColorR(int instance);
            
            static void SetColorR(int instance, float value);
            
            static float GetColorG(int instance);
            
            static void SetColorG(int instance, float value);
            
            static float GetColorB(int instance);
            
            static void SetColorB(int instance, float value);
            
            static float GetColorA(int instance);
            
            static void SetColorA(int instance, float value);

            
            static bool GetActive(int instance);
            
            static void SetActive(int instance, bool value);
            
            static MonoString* GetText(int instance);
            
            static void SetText(int instance, MonoString* value);
            
            static MonoString* GetFontName(int instance);
            
            static void SetFontName(int instance, MonoString* value);
            
            static unsigned int GetFontSize(int instance);
            
            static void SetFontSize(int instance, unsigned int value);

            static float GetWidth(int instance); 

            static float GetHeight(int instance);
        }
    }

    // Script/LogicComponent.h //

    namespace PopplioComponentLogic
    {
        extern "C"
        {
            static bool GetActive(int instance);
        
            static void SetActive(int instance, bool value);
        
            static MonoString* GetScriptName(int instance);
        
            static void SetScriptName(int instance, MonoString* value);
        
            static int GetScriptLanguage(int instance);
        
            static void SetScriptLanguage(int instance, int value);

            static void SetScriptObj(int instance, MonoObject* scriptObj);
        }
    }

    // Animation/AnimationComponent.h //

    namespace PopplioComponentAnim
    {
        extern "C" // AnimationComponent
        {
            static int GetAnimStartX(int instance, MonoString* id);
            
            static void SetAnimStartX(int instance, MonoString* id, int value);

            
            static int GetAnimStartY(int instance, MonoString* id);
            
            static void SetAnimStartY(int instance, MonoString* id, int value);

            
            static int GetAnimEndX(int instance, MonoString* id);
            
            static void SetAnimEndX(int instance, MonoString* id, int value);

            
            static int GetAnimEndY(int instance, MonoString* id);
            
            static void SetAnimEndY(int instance, MonoString* id, int value);

            
            static float GetAnimSpeed(int instance, MonoString* id);
            
            static void SetAnimSpeed(int instance, MonoString* id, float value);

            
            static MonoString* GetAnimName(int instance, MonoString* id);
            
            static void SetAnimName(int instance, MonoString* id, MonoString* value);

            
            static float GetAnimFrameWidth(int instance, MonoString* id);
            
            static void SetAnimFrameWidth(int instance, MonoString* id, float value);

            
            static float GetAnimFrameHeight(int instance, MonoString* id);
            
            static void SetAnimFrameHeight(int instance, MonoString* id, float value);

            
            static bool GetActive(int instance);
            
            static void SetActive(int instance, bool value);

            
            static MonoString* GetCurrentAnim(int instance);
            
            static void SetAnimation(int instance, MonoString* value, bool playOnce);

            
            static float GetSheetWidth(int instance);
            
            static void SetSheetWidth(int instance, float value);

            
            static float GetSheetHeight(int instance);
            
            static void SetSheetHeight(int instance, float value);

            
            static float GetTimeElapsedCurrentCycle(int instance);

            static float GetTimeElapsedSinceLastResume(int instance);
            
            static void SetTimeElapsed(int instance, float value);


            static int GetFrameX(int instance);

            static void SetFrameX(int instance, int value);

            static int GetFrameY(int instance);

            static void SetFrameY(int instance, int value);

            
            static bool GetPaused(int instance);

            static bool GetPlaying(int instance);

            
            static bool GetPlayOnce(int instance);
            
            static void SetPlayOnce(int instance, bool value);

            static void Play(int instance);

            static void Pause(int instance);

            static void Stop(int instance);

            static void Resume(int instance);

            static MonoArray* GetAnimations(int instance);
        }
    }

    // Audio/AudioComponent.h //

    namespace PopplioComponentAudio
    {
        extern "C"
        {
            static bool GetActive(int instance);
            
            static void SetActive(int instance, bool value);
            
            static MonoString* GetName(int instance);
            
            static void SetName(int instance, MonoString* value);
            
            static float GetVolume(int instance);
            
            static void SetVolume(int instance, float value);
            
            static bool GetLoop(int instance);
            
            static void SetLoop(int instance, bool value);
            
            static float GetBPM(int instance);
            
            static void SetBPM(int instance, float value);
            
            static int GetTimeSignatureTop(int instance);
            
            static int GetTimeSignatureBottom(int instance);
            
            static void SetTimeSignature(int instance, int top, int bottom);
            
            static bool GetPlayOnAwake(int instance);
            
            static void SetPlayOnAwake(int instance, bool value);
            
            static int GetChannelGroup(int instance);
            
            static void SetChannelGroup(int instance, int value);
        }
    }

    // Hierarchy/ParentComponent.h //

    namespace PopplioComponentParent
    {
        extern "C" // Parent Component
        {
            static int GetParentNotStupidWindows(int entity);
            static void SetParentNotStupidWindows(int entity, int parent);
            static void AddChild(int entity, int child);
            static void RemoveChild(int entity, int child);
            static MonoArray* GetChildren(int entity);

            static bool GetInheritActive(int entity);
            static void SetInheritActive(int entity, bool value);
            static bool GetInheritTransform(int entity);
            static void SetInheritTransform(int entity, bool value);
            static float GetLocalPosX(int entity);
            static void SetLocalPosX(int entity, float value);
            static float GetLocalPosY(int entity);
            static void SetLocalPosY(int entity, float value);
            static float GetLocalScaleX(int entity);
            static void SetLocalScaleX(int entity, float value);
            static float GetLocalScaleY(int entity);
            static void SetLocalScaleY(int entity, float value);
            static double GetLocalRotation(int entity);
            static void SetLocalRotation(int entity, double value);

            static void UpdateLocalTransform(int entity);
        }
    }

    // Scene/PersistOnLoadComponent.h //
    namespace PopplioComponentPersist
    {
        extern "C" 
        {
            static bool GetPersist(int entity);
            static void SetPersist(int entity, bool value);
        }
    }

    // PrefabManagement/PrefabManager.h //
    namespace PopplioPrefab
    {
        extern "C"
        {
            static bool HasPrefab(MonoString* name);
            static int InstantiatePrefab(MonoString* name);
        }
    }

    // UIComponent.h //
    namespace PopplioComponentUI
    {
        extern "C"
        {
            static int GetUIType(int instance);
            static void SetUIType(int instance, int value);

            static bool GetActive(int instance);
            static void SetActive(int instance, bool value);

            static bool GetChecked(int instance);
            static void SetChecked(int instance, bool value);

            static float GetSliderValue(int instance);
            static void SetSliderValue(int instance, float value);

            static int GetLinkedEntity(int instance);
            static void SetLinkedEntity(int instance, int value);

            static void AddOnClickAction(int instance, MonoString* action);
            static void RemoveOnClickAction(int instance, MonoString* action);

            static MonoArray* GetOnClickActions(int instance);
            static void SetOnClickActions(int instance, MonoArray* actions);

            static bool GetHasHoverEffect(int instance);
            static void SetHasHoverEffect(int instance, bool value);

            static bool GetIsHovered(int instance);
            static void SetIsHovered(int instance, bool value);

            static int GetFrameX(int instance);
            static void SetFrameX(int instance, int value);

            static int GetFrameY(int instance);
            static void SetFrameY(int instance, int value);

            static int GetHoverFrameX(int instance);
            static void SetHoverFrameX(int instance, int value);

            static int GetHoverFrameY(int instance);
            static void SetHoverFrameY(int instance, int value);

            static int GetTotalColumns(int instance);
            static void SetTotalColumns(int instance, int value);

            static int GetTotalRows(int instance);
            static void SetTotalRows(int instance, int value);

            static float GetSheetWidth(int instance);
            static void SetSheetWidth(int instance, float value);

            static float GetSheetHeight(int instance);
            static void SetSheetHeight(int instance, float value);
        }

        extern "C" static void RegisterUICallback(int instance, MonoString* name, MonoObject* callback);
        extern "C" static void ExecuteUIAction(int instance, MonoString* actionName);

    }

    // ParticleComponent.h / ParticleSystem.h //
    namespace PopplioComponentParticles
    {
        extern "C"
        {
            static bool GetActive(int instance);
            static void SetActive(int instance, bool value);

            static int GetRenderOption(int instance);
            static void SetRenderOption(int instance, int value);

            static bool GetUsePhysics(int instance);
            static void SetUsePhysics(int instance, bool value);

            static int SpawnParticle(int instance);
            static void KillParticle(int instance, int index);

            static int SpawnEmitter(int instance);
            static void KillEmitter(int instance, int index);

            // get / set

            static bool GetActiveEmitter(int instance, int index);
            static void SetActiveEmitter(int instance, int index, bool value);

            static bool GetLoopEmitter(int instance, int index);
            static void SetLoopEmitter(int instance, int index, bool value);

            static float GetElapsedTimeEmitter(int instance, int index);
            static void SetElapsedTimeEmitter(int instance, int index, float value);

            static float GetRateEmitter(int instance, int index);
            static void SetRateEmitter(int instance, int index, float value);

            static float GetTLoopEmitter(int instance, int index);
            static void SetTLoopEmitter(int instance, int index, float value);

            static float GetMinLifeTimeEmitter(int instance, int index);
            static void SetMinLifeTimeEmitter(int instance, int index, float value);

            static float GetMaxLifeTimeEmitter(int instance, int index);
            static void SetMaxLifeTimeEmitter(int instance, int index, float value);

            static float GetMinRotEmitter(int instance, int index);
            static void SetMinRotEmitter(int instance, int index, float value);

            static float GetMaxRotEmitter(int instance, int index);
            static void SetMaxRotEmitter(int instance, int index, float value);

            static float GetMinSpinVelEmitter(int instance, int index);
            static void SetMinSpinVelEmitter(int instance, int index, float value);

            static float GetMaxSpinVelEmitter(int instance, int index);
            static void SetMaxSpinVelEmitter(int instance, int index, float value);

            static float GetMinSpinAccelEmitter(int instance, int index);
            static void SetMinSpinAccelEmitter(int instance, int index, float value);

            static float GetMaxSpinAccelEmitter(int instance, int index);
            static void SetMaxSpinAccelEmitter(int instance, int index, float value);

            static float GetMinPosXEmitter(int instance, int index);
            static void SetMinPosXEmitter(int instance, int index, float value);

            static float GetMaxPosXEmitter(int instance, int index);
            static void SetMaxPosXEmitter(int instance, int index, float value);

            static float GetMinPosYEmitter(int instance, int index);
            static void SetMinPosYEmitter(int instance, int index, float value);

            static float GetMaxPosYEmitter(int instance, int index);
            static void SetMaxPosYEmitter(int instance, int index, float value);

            static float GetMinScaleXEmitter(int instance, int index);
            static void SetMinScaleXEmitter(int instance, int index, float value);

            static float GetMaxScaleXEmitter(int instance, int index);
            static void SetMaxScaleXEmitter(int instance, int index, float value);

            static float GetMinScaleYEmitter(int instance, int index);
            static void SetMinScaleYEmitter(int instance, int index, float value);

            static float GetMaxScaleYEmitter(int instance, int index);
            static void SetMaxScaleYEmitter(int instance, int index, float value);

            static float GetMinVelXEmitter(int instance, int index);
            static void SetMinVelXEmitter(int instance, int index, float value);

            static float GetMaxVelXEmitter(int instance, int index);
            static void SetMaxVelXEmitter(int instance, int index, float value);

            static float GetMinVelYEmitter(int instance, int index);
            static void SetMinVelYEmitter(int instance, int index, float value);

            static float GetMaxVelYEmitter(int instance, int index);
            static void SetMaxVelYEmitter(int instance, int index, float value);

            static float GetMinScaleVelXEmitter(int instance, int index);
            static void SetMinScaleVelXEmitter(int instance, int index, float value);

            static float GetMaxScaleVelXEmitter(int instance, int index);
            static void SetMaxScaleVelXEmitter(int instance, int index, float value);

            static float GetMinScaleVelYEmitter(int instance, int index);
            static void SetMinScaleVelYEmitter(int instance, int index, float value);

            static float GetMaxScaleVelYEmitter(int instance, int index);
            static void SetMaxScaleVelYEmitter(int instance, int index, float value);

            static float GetMinAccelXEmitter(int instance, int index);
            static void SetMinAccelXEmitter(int instance, int index, float value);

            static float GetMaxAccelXEmitter(int instance, int index);
            static void SetMaxAccelXEmitter(int instance, int index, float value);

            static float GetMinAccelYEmitter(int instance, int index);
            static void SetMinAccelYEmitter(int instance, int index, float value);

            static float GetMaxAccelYEmitter(int instance, int index);
            static void SetMaxAccelYEmitter(int instance, int index, float value);

            static float GetMinScaleAccelXEmitter(int instance, int index);
            static void SetMinScaleAccelXEmitter(int instance, int index, float value);

            static float GetMaxScaleAccelXEmitter(int instance, int index);
            static void SetMaxScaleAccelXEmitter(int instance, int index, float value);

            static float GetMinScaleAccelYEmitter(int instance, int index);
            static void SetMinScaleAccelYEmitter(int instance, int index, float value);

            static float GetMaxScaleAccelYEmitter(int instance, int index);
            static void SetMaxScaleAccelYEmitter(int instance, int index, float value);

            static float GetMinColREmitter(int instance, int index);
            static void SetMinColREmitter(int instance, int index, float value);

            static float GetMaxColREmitter(int instance, int index);
            static void SetMaxColREmitter(int instance, int index, float value);

            static float GetMinColGEmitter(int instance, int index);
            static void SetMinColGEmitter(int instance, int index, float value);

            static float GetMaxColGEmitter(int instance, int index);
            static void SetMaxColGEmitter(int instance, int index, float value);

            static float GetMinColBEmitter(int instance, int index);
            static void SetMinColBEmitter(int instance, int index, float value);

            static float GetMaxColBEmitter(int instance, int index);
            static void SetMaxColBEmitter(int instance, int index, float value);

            static float GetMinColAEmitter(int instance, int index);
            static void SetMinColAEmitter(int instance, int index, float value);

            static float GetMaxColAEmitter(int instance, int index);
            static void SetMaxColAEmitter(int instance, int index, float value);

            static float GetMinColVelREmitter(int instance, int index);
            static void SetMinColVelREmitter(int instance, int index, float value);

            static float GetMaxColVelREmitter(int instance, int index);
            static void SetMaxColVelREmitter(int instance, int index, float value);

            static float GetMinColVelGEmitter(int instance, int index);
            static void SetMinColVelGEmitter(int instance, int index, float value);

            static float GetMaxColVelGEmitter(int instance, int index);
            static void SetMaxColVelGEmitter(int instance, int index, float value);

            static float GetMinColVelBEmitter(int instance, int index);
            static void SetMinColVelBEmitter(int instance, int index, float value);

            static float GetMaxColVelBEmitter(int instance, int index);
            static void SetMaxColVelBEmitter(int instance, int index, float value);

            static float GetMinColVelAEmitter(int instance, int index);
            static void SetMinColVelAEmitter(int instance, int index, float value);

            static float GetMaxColVelAEmitter(int instance, int index);
            static void SetMaxColVelAEmitter(int instance, int index, float value);

            static float GetMinColAccelREmitter(int instance, int index);
            static void SetMinColAccelREmitter(int instance, int index, float value);

            static float GetMaxColAccelREmitter(int instance, int index);
            static void SetMaxColAccelREmitter(int instance, int index, float value);

            static float GetMinColAccelGEmitter(int instance, int index);
            static void SetMinColAccelGEmitter(int instance, int index, float value);

            static float GetMaxColAccelGEmitter(int instance, int index);
            static void SetMaxColAccelGEmitter(int instance, int index, float value);

            static float GetMinColAccelBEmitter(int instance, int index);
            static void SetMinColAccelBEmitter(int instance, int index, float value);

            static float GetMaxColAccelBEmitter(int instance, int index);
            static void SetMaxColAccelBEmitter(int instance, int index, float value);

            static float GetMinColAccelAEmitter(int instance, int index);
            static void SetMinColAccelAEmitter(int instance, int index, float value);

            static float GetMaxColAccelAEmitter(int instance, int index);
            static void SetMaxColAccelAEmitter(int instance, int index, float value);

            static float GetMinSpinDirAccelEmitter(int instance, int index);
            static void SetMinSpinDirAccelEmitter(int instance, int index, float value);
            static float GetMaxSpinDirAccelEmitter(int instance, int index);
            static void SetMaxSpinDirAccelEmitter(int instance, int index, float value);

            static float GetMinDirAccelXEmitter(int instance, int index);
            static void SetMinDirAccelXEmitter(int instance, int index, float value);
            static float GetMaxDirAccelXEmitter(int instance, int index);
            static void SetMaxDirAccelXEmitter(int instance, int index, float value);
            static float GetMinDirAccelYEmitter(int instance, int index);
            static void SetMinDirAccelYEmitter(int instance, int index, float value);
            static float GetMaxDirAccelYEmitter(int instance, int index);
            static void SetMaxDirAccelYEmitter(int instance, int index, float value);

            static float GetMinScaleDirAccelXEmitter(int instance, int index);
            static void SetMinScaleDirAccelXEmitter(int instance, int index, float value);
            static float GetMaxScaleDirAccelXEmitter(int instance, int index);
            static void SetMaxScaleDirAccelXEmitter(int instance, int index, float value);
            static float GetMinScaleDirAccelYEmitter(int instance, int index);
            static void SetMinScaleDirAccelYEmitter(int instance, int index, float value);
            static float GetMaxScaleDirAccelYEmitter(int instance, int index);
            static void SetMaxScaleDirAccelYEmitter(int instance, int index, float value);

            static float GetMinColDirAccelREmitter(int instance, int index);
            static void SetMinColDirAccelREmitter(int instance, int index, float value);
            static float GetMaxColDirAccelREmitter(int instance, int index);
            static void SetMaxColDirAccelREmitter(int instance, int index, float value);
            static float GetMinColDirAccelGEmitter(int instance, int index);
            static void SetMinColDirAccelGEmitter(int instance, int index, float value);
            static float GetMaxColDirAccelGEmitter(int instance, int index);
            static void SetMaxColDirAccelGEmitter(int instance, int index, float value);
            static float GetMinColDirAccelBEmitter(int instance, int index);
            static void SetMinColDirAccelBEmitter(int instance, int index, float value);
            static float GetMaxColDirAccelBEmitter(int instance, int index);
            static void SetMaxColDirAccelBEmitter(int instance, int index, float value);
            static float GetMinColDirAccelAEmitter(int instance, int index);
            static void SetMinColDirAccelAEmitter(int instance, int index, float value);
            static float GetMaxColDirAccelAEmitter(int instance, int index);
            static void SetMaxColDirAccelAEmitter(int instance, int index, float value);
        }
    }
}