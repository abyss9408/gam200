/******************************************************************************/
/*!
\file   MonoInternalCall.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\co-author Hafawati Binte Mohd Ali
\contribution Shawn - 90%, Hafawati - 10%
\par    Course : CSD2401 / CSD2450 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the source file for consolidating all Internal Calls to Mono
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

#include <pch.h>

#include "MonoInternalCall.h"

#include "MonoUtilities.h"

#ifndef IMGUI_DISABLE
#include "../Editor/Editor.h"
#endif

#include "../Utilities/Directory.h"
#include "../Utilities/QueueSystem.h"

#include "../Runtime/RuntimeSystem.h"
#include "../Scene/SceneManageSystem.h"
#include "../Hierarchy/HierarchySystem.h"
#include "../Camera/CameraManager.h"
#include "../Particles/ParticleSystem.h"

#include <mono/jit/jit.h>
#include <mono/metadata/debug-helpers.h>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

#include <filesystem>


namespace PopplioScriptAPI
{
    Popplio::Registry* reg;
    Popplio::LayerManager* lay;
    Popplio::EventBus* event;
    Popplio::PrefabManager* prefab;
    Popplio::CameraManager* camera;

    // Register Internal Calls

    extern "C" void RegisterInternalCalls()
    {
        // Mono //
        mono_add_internal_call("Team_Popplio.Mono::AddGC", PopplioMono::AddGC);
        mono_add_internal_call("Team_Popplio.Mono::RemoveGC", PopplioMono::RemoveGC);

        // Logging/Logger.h //
        mono_add_internal_call("Team_Popplio.Libraries.Logger::Critical", PopplioLog::Critical);
        mono_add_internal_call("Team_Popplio.Libraries.Logger::Warning", PopplioLog::Warning);
        mono_add_internal_call("Team_Popplio.Libraries.Logger::Debug", PopplioLog::Debug);
        mono_add_internal_call("Team_Popplio.Libraries.Logger::Error", PopplioLog::Error);
        mono_add_internal_call("Team_Popplio.Libraries.Logger::Info", PopplioLog::Info);

        mono_add_internal_call("Team_Popplio.Libraries.Logger::EngineCritical", PopplioLog::EngineCritical);
        mono_add_internal_call("Team_Popplio.Libraries.Logger::EngineWarning", PopplioLog::EngineWarning);
        mono_add_internal_call("Team_Popplio.Libraries.Logger::EngineDebug", PopplioLog::EngineDebug);
        mono_add_internal_call("Team_Popplio.Libraries.Logger::EngineError", PopplioLog::EngineError);
        mono_add_internal_call("Team_Popplio.Libraries.Logger::EngineInfo", PopplioLog::EngineInfo);

        // Engine/Engine.h //
        mono_add_internal_call("Team_Popplio.Libraries.Engine::GetFPS", PopplioEngine::GetFPS);
        mono_add_internal_call("Team_Popplio.Libraries.Engine::AppQuit", PopplioEngine::AppQuit);

        // Input/InputSystem.h //
        mono_add_internal_call("Team_Popplio.Libraries.Input::KeyTriggered", PopplioInput::KeyTriggered);
        mono_add_internal_call("Team_Popplio.Libraries.Input::KeyDown", PopplioInput::KeyDown);
        mono_add_internal_call("Team_Popplio.Libraries.Input::KeyReleased", PopplioInput::KeyReleased);
        mono_add_internal_call("Team_Popplio.Libraries.Input::KeysTriggered", PopplioInput::KeysTriggered);
        mono_add_internal_call("Team_Popplio.Libraries.Input::KeysDown", PopplioInput::KeysDown);
        mono_add_internal_call("Team_Popplio.Libraries.Input::KeysReleased", PopplioInput::KeysReleased);
        mono_add_internal_call("Team_Popplio.Libraries.Input::MouseButtonTriggered", PopplioInput::MouseButtonTriggered);
        mono_add_internal_call("Team_Popplio.Libraries.Input::MouseButtonDown", PopplioInput::MouseButtonDown);
        mono_add_internal_call("Team_Popplio.Libraries.Input::MouseButtonReleased", PopplioInput::MouseButtonReleased);
        mono_add_internal_call("Team_Popplio.Libraries.Input::MouseX", PopplioInput::MouseX);
        mono_add_internal_call("Team_Popplio.Libraries.Input::MouseY", PopplioInput::MouseY);
        mono_add_internal_call("Team_Popplio.Libraries.Input::MouseDeltaX", PopplioInput::MouseDeltaX);
        mono_add_internal_call("Team_Popplio.Libraries.Input::MouseDeltaY", PopplioInput::MouseDeltaY);
        mono_add_internal_call("Team_Popplio.Libraries.Input::MousePrevX", PopplioInput::MousePrevX);
        mono_add_internal_call("Team_Popplio.Libraries.Input::MousePrevY", PopplioInput::MousePrevY);
        mono_add_internal_call("Team_Popplio.Libraries.Input::MouseScrollX", PopplioInput::MouseScrollX);
        mono_add_internal_call("Team_Popplio.Libraries.Input::MouseScrollY", PopplioInput::MouseScrollY);
        mono_add_internal_call("Team_Popplio.Libraries.Input::InputLatency", PopplioInput::InputLatency);

        // Camera/GameCamera.h //
        mono_add_internal_call("Team_Popplio.Libraries.Camera::GetPosX", PopplioCamera::GetPosX);
        mono_add_internal_call("Team_Popplio.Libraries.Camera::GetPosY", PopplioCamera::GetPosY);
        mono_add_internal_call("Team_Popplio.Libraries.Camera::SetPosition", PopplioCamera::SetPosition);
        mono_add_internal_call("Team_Popplio.Libraries.Camera::GetZoom", PopplioCamera::GetZoom);
        mono_add_internal_call("Team_Popplio.Libraries.Camera::SetZoom", PopplioCamera::SetZoom);
        mono_add_internal_call("Team_Popplio.Libraries.Camera::GetAspectRatio", PopplioCamera::GetAspectRatio);
        mono_add_internal_call("Team_Popplio.Libraries.Camera::SetTarget", PopplioCamera::SetTarget);
        mono_add_internal_call("Team_Popplio.Libraries.Camera::ClearTarget", PopplioCamera::ClearTarget);
        mono_add_internal_call("Team_Popplio.Libraries.Camera::HasTarget", PopplioCamera::HasTarget);

        // Utilities/Timer.h //
        mono_add_internal_call("Team_Popplio.Libraries.Time::GetDeltaTime", PopplioTime::GetDeltaTime);
        mono_add_internal_call("Team_Popplio.Libraries.Time::GetFixedDeltaTime", PopplioTime::GetFixedDeltaTime);
        mono_add_internal_call("Team_Popplio.Libraries.Time::GetTime", PopplioTime::GetTime);
        mono_add_internal_call("Team_Popplio.Libraries.Time::SetFixedDeltaTime", PopplioTime::SetFixedDeltaTime);
        mono_add_internal_call("Team_Popplio.Libraries.Time::ResetFixedDeltaTime", PopplioTime::ResetFixedDeltaTime);

        // Utilities/Rand.h //
        mono_add_internal_call("Team_Popplio.Libraries.Rand::SetRand", PopplioRand::SetRand);
        mono_add_internal_call("Team_Popplio.Libraries.Rand::GetRand", PopplioRand::GetRand);
        mono_add_internal_call("Team_Popplio.Libraries.Rand::RandInt", PopplioRand::RandInt);
        mono_add_internal_call("Team_Popplio.Libraries.Rand::RandFloat", PopplioRand::RandFloat);

        // Animation/AnimationSystem.h //
        mono_add_internal_call("Team_Popplio.Libraries.Animation::PlayAll", PopplioAnim::PlayAll);
        mono_add_internal_call("Team_Popplio.Libraries.Animation::StopAll", PopplioAnim::StopAll);
        mono_add_internal_call("Team_Popplio.Libraries.Animation::ResumeAll", PopplioAnim::ResumeAll);
        mono_add_internal_call("Team_Popplio.Libraries.Animation::PauseAll", PopplioAnim::PauseAll);

        // Audio/AudioSystem.h //
        mono_add_internal_call("Team_Popplio.Components.Audio::GetDefaultUnit", PopplioAudio::GetDefaultUnit);
        mono_add_internal_call("Team_Popplio.Components.Audio::GetIsPlaying", PopplioAudio::GetIsPlaying);
        mono_add_internal_call("Team_Popplio.Components.Audio::GetIsPaused", PopplioAudio::GetIsPaused);
        mono_add_internal_call("Team_Popplio.Components.Audio::GetLength", PopplioAudio::GetLength);
        mono_add_internal_call("Team_Popplio.Components.Audio::GetBeatInterval", PopplioAudio::GetBeatInterval);
        mono_add_internal_call("Team_Popplio.Components.Audio::GetCurrentTime", PopplioAudio::GetCurrentTime);
        mono_add_internal_call("Team_Popplio.Components.Audio::SetCurrentTime", PopplioAudio::SetCurrentTime);
        mono_add_internal_call("Team_Popplio.Components.Audio::GetOffBeat", PopplioAudio::GetOffBeat);
        mono_add_internal_call("Team_Popplio.Components.Audio::Play", PopplioAudio::Play);
        mono_add_internal_call("Team_Popplio.Components.Audio::Pause", PopplioAudio::Pause);
        mono_add_internal_call("Team_Popplio.Components.Audio::Stop", PopplioAudio::Stop);
        mono_add_internal_call("Team_Popplio.Components.Audio::GetSpectrum", PopplioAudio::GetSpectrum);
        mono_add_internal_call("Team_Popplio.Components.Audio::SetMute", PopplioAudio::SetMute);
        mono_add_internal_call("Team_Popplio.Components.Audio::GetMute", PopplioAudio::GetMute);

        mono_add_internal_call("Team_Popplio.Components.Audio::GetMasterVolume", PopplioAudio::GetMasterVolume);
        mono_add_internal_call("Team_Popplio.Components.Audio::SetMasterVolume", PopplioAudio::SetMasterVolume);
        mono_add_internal_call("Team_Popplio.Components.Audio::GetBGMVolume", PopplioAudio::GetBGMVolume);
        mono_add_internal_call("Team_Popplio.Components.Audio::SetBGMVolume", PopplioAudio::SetBGMVolume);
        mono_add_internal_call("Team_Popplio.Components.Audio::GetSFXVolume", PopplioAudio::GetSFXVolume);
        mono_add_internal_call("Team_Popplio.Components.Audio::SetSFXVolume", PopplioAudio::SetSFXVolume);
        mono_add_internal_call("Team_Popplio.Components.Audio::GetGroupVolume", PopplioAudio::GetGroupVolume);
        mono_add_internal_call("Team_Popplio.Components.Audio::SetGroupVolume", PopplioAudio::SetGroupVolume);

        // Scene/SceneManageSystem.h //
        mono_add_internal_call("Team_Popplio.Libraries.SceneManager::Load", PopplioScene::Load);
        mono_add_internal_call("Team_Popplio.Libraries.SceneManager::GetCurrent", PopplioScene::GetCurrent);
        mono_add_internal_call("Team_Popplio.Libraries.SceneManager::GetPrevious", PopplioScene::GetPrevious);
        mono_add_internal_call("Team_Popplio.Libraries.SceneManager::Reload", PopplioScene::Reload);

        // Math //
        mono_add_internal_call("Team_Popplio.Libraries.Mathematics::Dist2D", PopplioMath::Dist2D);

        // JSON //
        mono_add_internal_call("Team_Popplio.Libraries.Json::LoadJson", PopplioJson::LoadJson);
        mono_add_internal_call("Team_Popplio.Libraries.Json::GetCurrentDoc", PopplioJson::GetCurrentDoc);
        mono_add_internal_call("Team_Popplio.Libraries.Json::GetCurrentDocName", PopplioJson::GetCurrentDocName);
        mono_add_internal_call("Team_Popplio.Libraries.Json::LoadInt", PopplioJson::LoadInt);
        mono_add_internal_call("Team_Popplio.Libraries.Json::LoadFloat", PopplioJson::LoadFloat);
        mono_add_internal_call("Team_Popplio.Libraries.Json::LoadBool", PopplioJson::LoadBool);
        mono_add_internal_call("Team_Popplio.Libraries.Json::LoadString", PopplioJson::LoadString);
        mono_add_internal_call("Team_Popplio.Libraries.Json::Close", PopplioJson::CloseIf);
        mono_add_internal_call("Team_Popplio.Libraries.Json::ClearObjs", PopplioJson::ClearObjs);
        mono_add_internal_call("Team_Popplio.Libraries.Json::LoadObjArray", PopplioJson::LoadObjArray);
        mono_add_internal_call("Team_Popplio.Libraries.Json::LoadObjWithObj", PopplioJson::LoadObjWithObj);
        mono_add_internal_call("Team_Popplio.Libraries.Json::LoadObj", PopplioJson::LoadObj);
        mono_add_internal_call("Team_Popplio.Libraries.Json::LoadObjInt", PopplioJson::LoadObjInt);
        mono_add_internal_call("Team_Popplio.Libraries.Json::LoadObjFloat", PopplioJson::LoadObjFloat);
        mono_add_internal_call("Team_Popplio.Libraries.Json::LoadObjBool", PopplioJson::LoadObjBool);
        mono_add_internal_call("Team_Popplio.Libraries.Json::LoadObjString", PopplioJson::LoadObjString);
        mono_add_internal_call("Team_Popplio.Libraries.Json::LoadObjObj", PopplioJson::LoadObjObj);

        mono_add_internal_call("Team_Popplio.Libraries.Json::SaveJson1", PopplioJson::SaveJson1);
        mono_add_internal_call("Team_Popplio.Libraries.Json::NewObjValue", PopplioJson::NewObjValue);
        mono_add_internal_call("Team_Popplio.Libraries.Json::NewArrayValue", PopplioJson::NewArrayValue);
        mono_add_internal_call("Team_Popplio.Libraries.Json::AddToArray", PopplioJson::AddToArray);
        mono_add_internal_call("Team_Popplio.Libraries.Json::SaveBool", PopplioJson::SaveBool);
        mono_add_internal_call("Team_Popplio.Libraries.Json::SaveInt", PopplioJson::SaveInt);
        mono_add_internal_call("Team_Popplio.Libraries.Json::SaveFloat", PopplioJson::SaveFloat);
        mono_add_internal_call("Team_Popplio.Libraries.Json::SaveString", PopplioJson::SaveString);
        mono_add_internal_call("Team_Popplio.Libraries.Json::SaveObj", PopplioJson::SaveObj);
        mono_add_internal_call("Team_Popplio.Libraries.Json::SaveJson2", PopplioJson::SaveJson2);
        //mono_add_internal_call("Team_Popplio.Libraries.Json::InitValues", PopplioJson::InitValues);

        // ECS/ECS.h //
        mono_add_internal_call("Team_Popplio.ECS.Entity::Kill", PopplioECS::Kill);
        mono_add_internal_call("Team_Popplio.ECS.Entity::GetTag", PopplioECS::GetTag);
        mono_add_internal_call("Team_Popplio.ECS.Entity::Tag", PopplioECS::Tag);
        mono_add_internal_call("Team_Popplio.ECS.Entity::HasTag", PopplioECS::HasTag);
        mono_add_internal_call("Team_Popplio.ECS.Entity::Group", PopplioECS::Group);
        mono_add_internal_call("Team_Popplio.ECS.Entity::BelongsToGroup", PopplioECS::BelongsToGroup);
        //mono_add_internal_call("Team_Popplio.ECS.Entity::GetId", PopplioECS::GetId);
        mono_add_internal_call("Team_Popplio.ECS.Entity::Exists", PopplioECS::Exists);
        //mono_add_internal_call("Team_Popplio.ECS.Entity::OnScriptDestroy", PopplioECS::OnScriptDestroy);

        // C# API //
        mono_add_internal_call("Team_Popplio.ECS.Entity::InstEnt", PopplioECS::InstEnt);
        mono_add_internal_call("Team_Popplio.ECS.Entity::DelEnt", PopplioECS::DelEnt);

        mono_add_internal_call("Team_Popplio.ECS.Entity::GetEntCount", PopplioECS::GetEntCount);

        mono_add_internal_call("Team_Popplio.ECS.Entity::GetActive", PopplioECS::GetActive);
        mono_add_internal_call("Team_Popplio.ECS.Entity::SetActive", PopplioECS::SetActive);

        mono_add_internal_call("Team_Popplio.ECS.Entity::HasRenderComponent", PopplioECS::HasRenderComponent);
        mono_add_internal_call("Team_Popplio.ECS.Entity::HasRigidBodyComponent", PopplioECS::HasRigidBodyComponent);
        mono_add_internal_call("Team_Popplio.ECS.Entity::HasBoxColliderComponent", PopplioECS::HasBoxColliderComponent);
        mono_add_internal_call("Team_Popplio.ECS.Entity::HasTextComponent", PopplioECS::HasTextComponent);
        mono_add_internal_call("Team_Popplio.ECS.Entity::HasLogicComponent", PopplioECS::HasLogicComponent);
        mono_add_internal_call("Team_Popplio.ECS.Entity::HasAnimationComponent", PopplioECS::HasAnimationComponent);
        mono_add_internal_call("Team_Popplio.ECS.Entity::HasAudioComponent", PopplioECS::HasAudioComponent);
        mono_add_internal_call("Team_Popplio.ECS.Entity::HasParentComponent", PopplioECS::HasParentComponent);
        mono_add_internal_call("Team_Popplio.ECS.Entity::HasUIComponent", PopplioECS::HasUIComponent);

        //mono_add_internal_call("Team_Popplio.ECS.Entity::HasPersistentComponent", PopplioECS::HasPersistentComponent);

        mono_add_internal_call("Team_Popplio.ECS.Entity::AddRenderComponent", PopplioECS::AddRenderComponent);
        mono_add_internal_call("Team_Popplio.ECS.Entity::AddRigidBodyComponent", PopplioECS::AddRigidBodyComponent);
        mono_add_internal_call("Team_Popplio.ECS.Entity::AddBoxColliderComponent", PopplioECS::AddBoxColliderComponent);
        mono_add_internal_call("Team_Popplio.ECS.Entity::AddTextComponent", PopplioECS::AddTextComponent);
        mono_add_internal_call("Team_Popplio.ECS.Entity::AddLogicComponent", PopplioECS::AddLogicComponent);
        mono_add_internal_call("Team_Popplio.ECS.Entity::AddAnimationComponent", PopplioECS::AddAnimationComponent);
        mono_add_internal_call("Team_Popplio.ECS.Entity::AddAudioComponent", PopplioECS::AddAudioComponent);
        mono_add_internal_call("Team_Popplio.ECS.Entity::AddParentComponent", PopplioECS::AddParentComponent);
        mono_add_internal_call("Team_Popplio.ECS.Entity::AddUIComponent", PopplioECS::AddUIComponent);

        //mono_add_internal_call("Team_Popplio.ECS.Entity::AddPersistentComponent", PopplioECS::AddPersistentComponent);

        // Transformation/TransformComponent.h //
        mono_add_internal_call("Team_Popplio.Components.Transform2D::GetPositionX", PopplioComponentTrans::GetPositionX);
        mono_add_internal_call("Team_Popplio.Components.Transform2D::SetPositionX", PopplioComponentTrans::SetPositionX);
        mono_add_internal_call("Team_Popplio.Components.Transform2D::GetPositionY", PopplioComponentTrans::GetPositionY);
        mono_add_internal_call("Team_Popplio.Components.Transform2D::SetPositionY", PopplioComponentTrans::SetPositionY);
        mono_add_internal_call("Team_Popplio.Components.Transform2D::GetScaleX", PopplioComponentTrans::GetScaleX);
        mono_add_internal_call("Team_Popplio.Components.Transform2D::SetScaleX", PopplioComponentTrans::SetScaleX);
        mono_add_internal_call("Team_Popplio.Components.Transform2D::GetScaleY", PopplioComponentTrans::GetScaleY);
        mono_add_internal_call("Team_Popplio.Components.Transform2D::SetScaleY", PopplioComponentTrans::SetScaleY);
        mono_add_internal_call("Team_Popplio.Components.Transform2D::GetRotation", PopplioComponentTrans::GetRotation);
        mono_add_internal_call("Team_Popplio.Components.Transform2D::SetRotation", PopplioComponentTrans::SetRotation);

        // Graphic/GraphicComponent.h // Render Component
        mono_add_internal_call("Team_Popplio.Components.Render2D::GetColorR", PopplioComponentRender::GetColorR);
        mono_add_internal_call("Team_Popplio.Components.Render2D::SetColorR", PopplioComponentRender::SetColorR);
        mono_add_internal_call("Team_Popplio.Components.Render2D::GetColorG", PopplioComponentRender::GetColorG);
        mono_add_internal_call("Team_Popplio.Components.Render2D::SetColorG", PopplioComponentRender::SetColorG);
        mono_add_internal_call("Team_Popplio.Components.Render2D::GetColorB", PopplioComponentRender::GetColorB);
        mono_add_internal_call("Team_Popplio.Components.Render2D::SetColorB", PopplioComponentRender::SetColorB);
        mono_add_internal_call("Team_Popplio.Components.Render2D::GetColorA", PopplioComponentRender::GetColorA);
        mono_add_internal_call("Team_Popplio.Components.Render2D::SetColorA", PopplioComponentRender::SetColorA);
        mono_add_internal_call("Team_Popplio.Components.Render2D::GetMeshType", PopplioComponentRender::GetMeshType);
        mono_add_internal_call("Team_Popplio.Components.Render2D::SetMeshType", PopplioComponentRender::SetMeshType);
        mono_add_internal_call("Team_Popplio.Components.Render2D::GetActive", PopplioComponentRender::GetActive);
        mono_add_internal_call("Team_Popplio.Components.Render2D::SetActive", PopplioComponentRender::SetActive);
        mono_add_internal_call("Team_Popplio.Components.Render2D::GetShader", PopplioComponentRender::GetShader);
        mono_add_internal_call("Team_Popplio.Components.Render2D::SetShader", PopplioComponentRender::SetShader);
        mono_add_internal_call("Team_Popplio.Components.Render2D::GetTexture", PopplioComponentRender::GetTexture);
        mono_add_internal_call("Team_Popplio.Components.Render2D::SetTexture", PopplioComponentRender::SetTexture);
        mono_add_internal_call("Team_Popplio.Components.Render2D::GetInstancing", PopplioComponentRender::GetInstancing);
        mono_add_internal_call("Team_Popplio.Components.Render2D::SetInstancing", PopplioComponentRender::SetInstancing);

        // Physics/RigidBodyComponent.h //
        mono_add_internal_call("Team_Popplio.Components.RigidBody2D::GetVelocityX", PopplioComponentRB::GetVelocityX);
        mono_add_internal_call("Team_Popplio.Components.RigidBody2D::SetVelocityX", PopplioComponentRB::SetVelocityX);
        mono_add_internal_call("Team_Popplio.Components.RigidBody2D::GetVelocityY", PopplioComponentRB::GetVelocityY);
        mono_add_internal_call("Team_Popplio.Components.RigidBody2D::SetVelocityY", PopplioComponentRB::SetVelocityY);
        mono_add_internal_call("Team_Popplio.Components.RigidBody2D::GetAccelerationX", PopplioComponentRB::GetAccelerationX);
        mono_add_internal_call("Team_Popplio.Components.RigidBody2D::SetAccelerationX", PopplioComponentRB::SetAccelerationX);
        mono_add_internal_call("Team_Popplio.Components.RigidBody2D::GetAccelerationY", PopplioComponentRB::GetAccelerationY);
        mono_add_internal_call("Team_Popplio.Components.RigidBody2D::SetAccelerationY", PopplioComponentRB::SetAccelerationY);
        mono_add_internal_call("Team_Popplio.Components.RigidBody2D::GetForceX", PopplioComponentRB::GetForceX);
        mono_add_internal_call("Team_Popplio.Components.RigidBody2D::SetForceX", PopplioComponentRB::SetForceX);
        mono_add_internal_call("Team_Popplio.Components.RigidBody2D::GetForceY", PopplioComponentRB::GetForceY);
        mono_add_internal_call("Team_Popplio.Components.RigidBody2D::SetForceY", PopplioComponentRB::SetForceY);
        mono_add_internal_call("Team_Popplio.Components.RigidBody2D::GetActive", PopplioComponentRB::GetActive);
        mono_add_internal_call("Team_Popplio.Components.RigidBody2D::SetActive", PopplioComponentRB::SetActive);
        mono_add_internal_call("Team_Popplio.Components.RigidBody2D::GetMass", PopplioComponentRB::GetMass);
        mono_add_internal_call("Team_Popplio.Components.RigidBody2D::SetMass", PopplioComponentRB::SetMass);
        mono_add_internal_call("Team_Popplio.Components.RigidBody2D::GetGravityScale", PopplioComponentRB::GetGravityScale);
        mono_add_internal_call("Team_Popplio.Components.RigidBody2D::SetGravityScale", PopplioComponentRB::SetGravityScale);
        mono_add_internal_call("Team_Popplio.Components.RigidBody2D::GetUseGravity", PopplioComponentRB::GetUseGravity);
        mono_add_internal_call("Team_Popplio.Components.RigidBody2D::SetUseGravity", PopplioComponentRB::SetUseGravity);
        mono_add_internal_call("Team_Popplio.Components.RigidBody2D::GetDragCoefficient", PopplioComponentRB::GetDragCoefficient);
        mono_add_internal_call("Team_Popplio.Components.RigidBody2D::SetDragCoefficient", PopplioComponentRB::SetDragCoefficient);

        // Collision/BoxColliderComponent.h //
        mono_add_internal_call("Team_Popplio.Components.BoxCollider2D::GetSizeX", PopplioComponentBoxCollider::GetSizeX);
        mono_add_internal_call("Team_Popplio.Components.BoxCollider2D::SetSizeX", PopplioComponentBoxCollider::SetSizeX);
        mono_add_internal_call("Team_Popplio.Components.BoxCollider2D::GetSizeY", PopplioComponentBoxCollider::GetSizeY);
        mono_add_internal_call("Team_Popplio.Components.BoxCollider2D::SetSizeY", PopplioComponentBoxCollider::SetSizeY);
        mono_add_internal_call("Team_Popplio.Components.BoxCollider2D::GetOffsetX", PopplioComponentBoxCollider::GetOffsetX);
        mono_add_internal_call("Team_Popplio.Components.BoxCollider2D::SetOffsetX", PopplioComponentBoxCollider::SetOffsetX);
        mono_add_internal_call("Team_Popplio.Components.BoxCollider2D::GetOffsetY", PopplioComponentBoxCollider::GetOffsetY);
        mono_add_internal_call("Team_Popplio.Components.BoxCollider2D::SetOffsetY", PopplioComponentBoxCollider::SetOffsetY);
        mono_add_internal_call("Team_Popplio.Components.BoxCollider2D::GetActive", PopplioComponentBoxCollider::GetActive);
        mono_add_internal_call("Team_Popplio.Components.BoxCollider2D::SetActive", PopplioComponentBoxCollider::SetActive);
        mono_add_internal_call("Team_Popplio.Components.BoxCollider2D::GetTrigger", PopplioComponentBoxCollider::GetTrigger);
        mono_add_internal_call("Team_Popplio.Components.BoxCollider2D::SetTrigger", PopplioComponentBoxCollider::SetTrigger);
        mono_add_internal_call("Team_Popplio.Components.BoxCollider2D::GetEnabled", PopplioComponentBoxCollider::GetEnabled);
        mono_add_internal_call("Team_Popplio.Components.BoxCollider2D::SetEnabled", PopplioComponentBoxCollider::SetEnabled);

        // Graphic/GraphicComponent.h // Text Component
        mono_add_internal_call("Team_Popplio.Components.Text2D::GetColorR", PopplioComponentText::GetColorR);
        mono_add_internal_call("Team_Popplio.Components.Text2D::SetColorR", PopplioComponentText::SetColorR);
        mono_add_internal_call("Team_Popplio.Components.Text2D::GetColorG", PopplioComponentText::GetColorG);
        mono_add_internal_call("Team_Popplio.Components.Text2D::SetColorG", PopplioComponentText::SetColorG);
        mono_add_internal_call("Team_Popplio.Components.Text2D::GetColorB", PopplioComponentText::GetColorB);
        mono_add_internal_call("Team_Popplio.Components.Text2D::SetColorB", PopplioComponentText::SetColorB);
        mono_add_internal_call("Team_Popplio.Components.Text2D::GetColorA", PopplioComponentText::GetColorA);
        mono_add_internal_call("Team_Popplio.Components.Text2D::SetColorA", PopplioComponentText::SetColorA);
        mono_add_internal_call("Team_Popplio.Components.Text2D::GetActive", PopplioComponentText::GetActive);
        mono_add_internal_call("Team_Popplio.Components.Text2D::SetActive", PopplioComponentText::SetActive);
        mono_add_internal_call("Team_Popplio.Components.Text2D::GetText", PopplioComponentText::GetText);
        mono_add_internal_call("Team_Popplio.Components.Text2D::SetText", PopplioComponentText::SetText);
        mono_add_internal_call("Team_Popplio.Components.Text2D::GetFontName", PopplioComponentText::GetFontName);
        mono_add_internal_call("Team_Popplio.Components.Text2D::SetFontName", PopplioComponentText::SetFontName);
        mono_add_internal_call("Team_Popplio.Components.Text2D::GetFontSize", PopplioComponentText::GetFontSize);
        mono_add_internal_call("Team_Popplio.Components.Text2D::SetFontSize", PopplioComponentText::SetFontSize);
        mono_add_internal_call("Team_Popplio.Components.Text2D::GetWidth", PopplioComponentText::GetWidth);
        mono_add_internal_call("Team_Popplio.Components.Text2D::GetHeight", PopplioComponentText::GetHeight);


        // Script/LogicComponent.h //
        mono_add_internal_call("Team_Popplio.Components.Logic::GetActive", PopplioComponentLogic::GetActive);
        mono_add_internal_call("Team_Popplio.Components.Logic::SetActive", PopplioComponentLogic::SetActive);
        mono_add_internal_call("Team_Popplio.Components.Logic::GetScriptName", PopplioComponentLogic::GetScriptName);
        mono_add_internal_call("Team_Popplio.Components.Logic::SetScriptName", PopplioComponentLogic::SetScriptName);
        mono_add_internal_call("Team_Popplio.Components.Logic::GetScriptLanguage", PopplioComponentLogic::GetScriptLanguage);
        mono_add_internal_call("Team_Popplio.Components.Logic::SetScriptLanguage", PopplioComponentLogic::SetScriptLanguage);
        mono_add_internal_call("Team_Popplio.Components.Logic::SetScriptObj", PopplioComponentLogic::SetScriptObj);

        // Animation/AnimationComponent.h //
        mono_add_internal_call("Team_Popplio.Components.Animation2D::GetAnimStartX", PopplioComponentAnim::GetAnimStartX);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::SetAnimStartX", PopplioComponentAnim::SetAnimStartX);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::GetAnimStartY", PopplioComponentAnim::GetAnimStartY);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::SetAnimStartY", PopplioComponentAnim::SetAnimStartY);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::GetAnimEndX", PopplioComponentAnim::GetAnimEndX);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::SetAnimEndX", PopplioComponentAnim::SetAnimEndX);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::GetAnimEndY", PopplioComponentAnim::GetAnimEndY);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::SetAnimEndY", PopplioComponentAnim::SetAnimEndY);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::GetAnimSpeed", PopplioComponentAnim::GetAnimSpeed);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::SetAnimSpeed", PopplioComponentAnim::SetAnimSpeed);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::GetAnimName", PopplioComponentAnim::GetAnimName);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::SetAnimName", PopplioComponentAnim::SetAnimName);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::GetAnimFrameWidth", PopplioComponentAnim::GetAnimFrameWidth);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::SetAnimFrameWidth", PopplioComponentAnim::SetAnimFrameWidth);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::GetAnimFrameHeight", PopplioComponentAnim::GetAnimFrameHeight);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::SetAnimFrameHeight", PopplioComponentAnim::SetAnimFrameHeight);

        mono_add_internal_call("Team_Popplio.Components.Animation2D::GetActive", PopplioComponentAnim::GetActive);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::SetActive", PopplioComponentAnim::SetActive);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::GetCurrentAnim", PopplioComponentAnim::GetCurrentAnim);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::SetAnimation", PopplioComponentAnim::SetAnimation);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::GetSheetWidth", PopplioComponentAnim::GetSheetWidth);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::SetSheetWidth", PopplioComponentAnim::SetSheetWidth);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::GetSheetHeight", PopplioComponentAnim::GetSheetHeight);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::SetSheetHeight", PopplioComponentAnim::SetSheetHeight);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::GetTimeElapsedCurrentCycle", PopplioComponentAnim::GetTimeElapsedCurrentCycle);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::GetTimeElapsedSinceLastResume", PopplioComponentAnim::GetTimeElapsedSinceLastResume);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::SetTimeElapsed", PopplioComponentAnim::SetTimeElapsed);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::SetFrameX", PopplioComponentAnim::SetFrameX);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::SetFrameY", PopplioComponentAnim::SetFrameY);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::GetFrameX", PopplioComponentAnim::GetFrameX);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::GetFrameY", PopplioComponentAnim::GetFrameY);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::GetPaused", PopplioComponentAnim::GetPaused);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::GetPlaying", PopplioComponentAnim::GetPlaying);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::GetPlayOnce", PopplioComponentAnim::GetPlayOnce);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::SetPlayOnce", PopplioComponentAnim::SetPlayOnce);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::Play", PopplioComponentAnim::Play);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::Pause", PopplioComponentAnim::Pause);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::Stop", PopplioComponentAnim::Stop);
        mono_add_internal_call("Team_Popplio.Components.Animation2D::Resume", PopplioComponentAnim::Resume);

        mono_add_internal_call("Team_Popplio.Components.Animation2D::GetAnimations", PopplioComponentAnim::GetAnimations);

        // Audio/AudioComponent.h //
        mono_add_internal_call("Team_Popplio.Components.Audio::GetActive", PopplioComponentAudio::GetActive);
        mono_add_internal_call("Team_Popplio.Components.Audio::SetActive", PopplioComponentAudio::SetActive);
        mono_add_internal_call("Team_Popplio.Components.Audio::GetName", PopplioComponentAudio::GetName);
        mono_add_internal_call("Team_Popplio.Components.Audio::SetName", PopplioComponentAudio::SetName);
        mono_add_internal_call("Team_Popplio.Components.Audio::GetVolume", PopplioComponentAudio::GetVolume);
        mono_add_internal_call("Team_Popplio.Components.Audio::SetVolume", PopplioComponentAudio::SetVolume);
        mono_add_internal_call("Team_Popplio.Components.Audio::GetLoop", PopplioComponentAudio::GetLoop);
        mono_add_internal_call("Team_Popplio.Components.Audio::SetLoop", PopplioComponentAudio::SetLoop);
        mono_add_internal_call("Team_Popplio.Components.Audio::GetBPM", PopplioComponentAudio::GetBPM);
        mono_add_internal_call("Team_Popplio.Components.Audio::SetBPM", PopplioComponentAudio::SetBPM);
        mono_add_internal_call("Team_Popplio.Components.Audio::GetTimeSignatureTop", PopplioComponentAudio::GetTimeSignatureTop);
        mono_add_internal_call("Team_Popplio.Components.Audio::GetTimeSignatureBottom", PopplioComponentAudio::GetTimeSignatureBottom);
        mono_add_internal_call("Team_Popplio.Components.Audio::SetTimeSignature", PopplioComponentAudio::SetTimeSignature);
        mono_add_internal_call("Team_Popplio.Components.Audio::GetPlayOnAwake", PopplioComponentAudio::GetPlayOnAwake);
        mono_add_internal_call("Team_Popplio.Components.Audio::SetPlayOnAwake", PopplioComponentAudio::SetPlayOnAwake);
        mono_add_internal_call("Team_Popplio.Components.Audio::GetChannelGroup", PopplioComponentAudio::GetChannelGroup);
        mono_add_internal_call("Team_Popplio.Components.Audio::SetChannelGroup", PopplioComponentAudio::SetChannelGroup);

        // Hierarchy/ParentComponent.h //
        mono_add_internal_call("Team_Popplio.Components.Hierarchy::GetParent", PopplioComponentParent::GetParentNotStupidWindows);
        mono_add_internal_call("Team_Popplio.Components.Hierarchy::SetParent", PopplioComponentParent::SetParentNotStupidWindows);
        mono_add_internal_call("Team_Popplio.Components.Hierarchy::AddChild", PopplioComponentParent::AddChild);
        mono_add_internal_call("Team_Popplio.Components.Hierarchy::RemoveChild", PopplioComponentParent::RemoveChild);
        mono_add_internal_call("Team_Popplio.Components.Hierarchy::GetChildren", PopplioComponentParent::GetChildren);
        mono_add_internal_call("Team_Popplio.Components.Hierarchy::GetInheritActive", PopplioComponentParent::GetInheritActive);
        mono_add_internal_call("Team_Popplio.Components.Hierarchy::SetInheritActive", PopplioComponentParent::SetInheritActive);
        mono_add_internal_call("Team_Popplio.Components.Hierarchy::GetInheritTransform", PopplioComponentParent::GetInheritTransform);
        mono_add_internal_call("Team_Popplio.Components.Hierarchy::SetInheritTransform", PopplioComponentParent::SetInheritTransform);
        mono_add_internal_call("Team_Popplio.Components.Hierarchy::GetLocalPosX", PopplioComponentParent::GetLocalPosX);
        mono_add_internal_call("Team_Popplio.Components.Hierarchy::SetLocalPosX", PopplioComponentParent::SetLocalPosX);
        mono_add_internal_call("Team_Popplio.Components.Hierarchy::GetLocalPosY", PopplioComponentParent::GetLocalPosY);
        mono_add_internal_call("Team_Popplio.Components.Hierarchy::SetLocalPosY", PopplioComponentParent::SetLocalPosY);
        mono_add_internal_call("Team_Popplio.Components.Hierarchy::GetLocalScaleX", PopplioComponentParent::GetLocalScaleX);
        mono_add_internal_call("Team_Popplio.Components.Hierarchy::SetLocalScaleX", PopplioComponentParent::SetLocalScaleX);
        mono_add_internal_call("Team_Popplio.Components.Hierarchy::GetLocalScaleY", PopplioComponentParent::GetLocalScaleY);
        mono_add_internal_call("Team_Popplio.Components.Hierarchy::SetLocalScaleY", PopplioComponentParent::SetLocalScaleY);
        mono_add_internal_call("Team_Popplio.Components.Hierarchy::GetLocalRotation", PopplioComponentParent::GetLocalRotation);
        mono_add_internal_call("Team_Popplio.Components.Hierarchy::SetLocalRotation", PopplioComponentParent::SetLocalRotation);
        mono_add_internal_call("Team_Popplio.Components.Hierarchy::UpdateLocalTransform", PopplioComponentParent::UpdateLocalTransform);

        // Scene/PersistOnLoadComponent.h //
        mono_add_internal_call("Team_Popplio.ECS.Entity::GetPersist", PopplioComponentPersist::GetPersist);
        mono_add_internal_call("Team_Popplio.ECS.Entity::SetPersist", PopplioComponentPersist::SetPersist);

        // PrefabManagement/PrefabManager.h //
        mono_add_internal_call("Team_Popplio.ECS.Prefab::HasPrefab", PopplioPrefab::HasPrefab);
        mono_add_internal_call("Team_Popplio.ECS.Prefab::InstantiatePrefab", PopplioPrefab::InstantiatePrefab);
    
        // UIComponent Internal Calls
        mono_add_internal_call("Team_Popplio.Components.UI::GetUIType", PopplioComponentUI::GetUIType);
        mono_add_internal_call("Team_Popplio.Components.UI::SetUIType", PopplioComponentUI::SetUIType);
        mono_add_internal_call("Team_Popplio.Components.UI::GetActive", PopplioComponentUI::GetActive);
        mono_add_internal_call("Team_Popplio.Components.UI::SetActive", PopplioComponentUI::SetActive);
        mono_add_internal_call("Team_Popplio.Components.UI::GetChecked", PopplioComponentUI::GetChecked);
        mono_add_internal_call("Team_Popplio.Components.UI::SetChecked", PopplioComponentUI::SetChecked);
        mono_add_internal_call("Team_Popplio.Components.UI::GetSliderValue", PopplioComponentUI::GetSliderValue);
        mono_add_internal_call("Team_Popplio.Components.UI::SetSliderValue", PopplioComponentUI::SetSliderValue);
        mono_add_internal_call("Team_Popplio.Components.UI::GetLinkedEntity", PopplioComponentUI::GetLinkedEntity);
        mono_add_internal_call("Team_Popplio.Components.UI::SetLinkedEntity", PopplioComponentUI::SetLinkedEntity);
        mono_add_internal_call("Team_Popplio.Components.UI::AddOnClickAction", PopplioComponentUI::AddOnClickAction);
        mono_add_internal_call("Team_Popplio.Components.UI::RemoveOnClickAction", PopplioComponentUI::RemoveOnClickAction);
        mono_add_internal_call("Team_Popplio.Components.UI::GetOnClickActions", PopplioComponentUI::GetOnClickActions);
        mono_add_internal_call("Team_Popplio.Components.UI::SetOnClickActions", PopplioComponentUI::SetOnClickActions);
        mono_add_internal_call("Team_Popplio.Components.UI::GetHasHoverEffect", PopplioComponentUI::GetHasHoverEffect);
        mono_add_internal_call("Team_Popplio.Components.UI::SetHasHoverEffect", PopplioComponentUI::SetHasHoverEffect);
        mono_add_internal_call("Team_Popplio.Components.UI::GetIsHovered", PopplioComponentUI::GetIsHovered);
        mono_add_internal_call("Team_Popplio.Components.UI::SetIsHovered", PopplioComponentUI::SetIsHovered);
        mono_add_internal_call("Team_Popplio.Components.UI::GetFrameX", PopplioComponentUI::GetFrameX);
        mono_add_internal_call("Team_Popplio.Components.UI::SetFrameX", PopplioComponentUI::SetFrameX);
        mono_add_internal_call("Team_Popplio.Components.UI::GetFrameY", PopplioComponentUI::GetFrameY);
        mono_add_internal_call("Team_Popplio.Components.UI::SetFrameY", PopplioComponentUI::SetFrameY);
        mono_add_internal_call("Team_Popplio.Components.UI::GetHoverFrameX", PopplioComponentUI::GetHoverFrameX);
        mono_add_internal_call("Team_Popplio.Components.UI::SetHoverFrameX", PopplioComponentUI::SetHoverFrameX);
        mono_add_internal_call("Team_Popplio.Components.UI::GetHoverFrameY", PopplioComponentUI::GetHoverFrameY);
        mono_add_internal_call("Team_Popplio.Components.UI::SetHoverFrameY", PopplioComponentUI::SetHoverFrameY);
        mono_add_internal_call("Team_Popplio.Components.UI::GetTotalColumns", PopplioComponentUI::GetTotalColumns);
        mono_add_internal_call("Team_Popplio.Components.UI::SetTotalColumns", PopplioComponentUI::SetTotalColumns);
        mono_add_internal_call("Team_Popplio.Components.UI::GetTotalRows", PopplioComponentUI::GetTotalRows);
        mono_add_internal_call("Team_Popplio.Components.UI::SetTotalRows", PopplioComponentUI::SetTotalRows);
        mono_add_internal_call("Team_Popplio.Components.UI::GetSheetWidth", PopplioComponentUI::GetSheetWidth);
        mono_add_internal_call("Team_Popplio.Components.UI::SetSheetWidth", PopplioComponentUI::SetSheetWidth);
        mono_add_internal_call("Team_Popplio.Components.UI::GetSheetHeight", PopplioComponentUI::GetSheetHeight);
        mono_add_internal_call("Team_Popplio.Components.UI::SetSheetHeight", PopplioComponentUI::SetSheetHeight);
        mono_add_internal_call("Team_Popplio.Components.UI::RegisterUICallback", PopplioComponentUI::RegisterUICallback);
        mono_add_internal_call("Team_Popplio.Components.UI::ExecuteUIAction", PopplioComponentUI::ExecuteUIAction);

        // ParticleComponent.h / ParticleSystem.h //
        mono_add_internal_call("Team_Popplio.Components.Particle::GetActive", PopplioComponentParticles::GetActive);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetActive", PopplioComponentParticles::SetActive);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetRenderOption", PopplioComponentParticles::GetRenderOption);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetRenderOption", PopplioComponentParticles::SetRenderOption);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetUsePhysics", PopplioComponentParticles::GetUsePhysics);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetUsePhysics", PopplioComponentParticles::SetUsePhysics);
        mono_add_internal_call("Team_Popplio.Components.Particle::SpawnParticle", PopplioComponentParticles::SpawnParticle);
        mono_add_internal_call("Team_Popplio.Components.Particle::KillParticle", PopplioComponentParticles::KillParticle);
        mono_add_internal_call("Team_Popplio.Components.Particle::SpawnEmitter", PopplioComponentParticles::SpawnEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::KillEmitter", PopplioComponentParticles::KillEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetActiveEmitter", PopplioComponentParticles::GetActiveEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetActiveEmitter", PopplioComponentParticles::SetActiveEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetLoopEmitter", PopplioComponentParticles::GetLoopEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetLoopEmitter", PopplioComponentParticles::SetLoopEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetElapsedTimeEmitter", PopplioComponentParticles::GetElapsedTimeEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetElapsedTimeEmitter", PopplioComponentParticles::SetElapsedTimeEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetRateEmitter", PopplioComponentParticles::GetRateEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetRateEmitter", PopplioComponentParticles::SetRateEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetTLoopEmitter", PopplioComponentParticles::GetTLoopEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetTLoopEmitter", PopplioComponentParticles::SetTLoopEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinLifeTimeEmitter", PopplioComponentParticles::GetMinLifeTimeEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinLifeTimeEmitter", PopplioComponentParticles::SetMinLifeTimeEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxLifeTimeEmitter", PopplioComponentParticles::GetMaxLifeTimeEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxLifeTimeEmitter", PopplioComponentParticles::SetMaxLifeTimeEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinRotEmitter", PopplioComponentParticles::GetMinRotEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinRotEmitter", PopplioComponentParticles::SetMinRotEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxRotEmitter", PopplioComponentParticles::GetMaxRotEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxRotEmitter", PopplioComponentParticles::SetMaxRotEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinSpinVelEmitter", PopplioComponentParticles::GetMinSpinVelEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinSpinVelEmitter", PopplioComponentParticles::SetMinSpinVelEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxSpinVelEmitter", PopplioComponentParticles::GetMaxSpinVelEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxSpinVelEmitter", PopplioComponentParticles::SetMaxSpinVelEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinSpinAccelEmitter", PopplioComponentParticles::GetMinSpinAccelEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinSpinAccelEmitter", PopplioComponentParticles::SetMinSpinAccelEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxSpinAccelEmitter", PopplioComponentParticles::GetMaxSpinAccelEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxSpinAccelEmitter", PopplioComponentParticles::SetMaxSpinAccelEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinPosXEmitter", PopplioComponentParticles::GetMinPosXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinPosXEmitter", PopplioComponentParticles::SetMinPosXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxPosXEmitter", PopplioComponentParticles::GetMaxPosXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxPosXEmitter", PopplioComponentParticles::SetMaxPosXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinPosYEmitter", PopplioComponentParticles::GetMinPosYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinPosYEmitter", PopplioComponentParticles::SetMinPosYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxPosYEmitter", PopplioComponentParticles::GetMaxPosYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxPosYEmitter", PopplioComponentParticles::SetMaxPosYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinScaleXEmitter", PopplioComponentParticles::GetMinScaleXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinScaleXEmitter", PopplioComponentParticles::SetMinScaleXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxScaleXEmitter", PopplioComponentParticles::GetMaxScaleXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxScaleXEmitter", PopplioComponentParticles::SetMaxScaleXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinScaleYEmitter", PopplioComponentParticles::GetMinScaleYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinScaleYEmitter", PopplioComponentParticles::SetMinScaleYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxScaleYEmitter", PopplioComponentParticles::GetMaxScaleYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxScaleYEmitter", PopplioComponentParticles::SetMaxScaleYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinVelXEmitter", PopplioComponentParticles::GetMinVelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinVelXEmitter", PopplioComponentParticles::SetMinVelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxVelXEmitter", PopplioComponentParticles::GetMaxVelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxVelXEmitter", PopplioComponentParticles::SetMaxVelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinVelYEmitter", PopplioComponentParticles::GetMinVelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinVelYEmitter", PopplioComponentParticles::SetMinVelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxVelYEmitter", PopplioComponentParticles::GetMaxVelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxVelYEmitter", PopplioComponentParticles::SetMaxVelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinScaleVelXEmitter", PopplioComponentParticles::GetMinScaleVelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinScaleVelXEmitter", PopplioComponentParticles::SetMinScaleVelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxScaleVelXEmitter", PopplioComponentParticles::GetMaxScaleVelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxScaleVelXEmitter", PopplioComponentParticles::SetMaxScaleVelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinScaleVelYEmitter", PopplioComponentParticles::GetMinScaleVelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinScaleVelYEmitter", PopplioComponentParticles::SetMinScaleVelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxScaleVelYEmitter", PopplioComponentParticles::GetMaxScaleVelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxScaleVelYEmitter", PopplioComponentParticles::SetMaxScaleVelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinAccelXEmitter", PopplioComponentParticles::GetMinAccelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinAccelXEmitter", PopplioComponentParticles::SetMinAccelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxAccelXEmitter", PopplioComponentParticles::GetMaxAccelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxAccelXEmitter", PopplioComponentParticles::SetMaxAccelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinAccelYEmitter", PopplioComponentParticles::GetMinAccelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinAccelYEmitter", PopplioComponentParticles::SetMinAccelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxAccelYEmitter", PopplioComponentParticles::GetMaxAccelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxAccelYEmitter", PopplioComponentParticles::SetMaxAccelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinScaleAccelXEmitter", PopplioComponentParticles::GetMinScaleAccelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinScaleAccelXEmitter", PopplioComponentParticles::SetMinScaleAccelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxScaleAccelXEmitter", PopplioComponentParticles::GetMaxScaleAccelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxScaleAccelXEmitter", PopplioComponentParticles::SetMaxScaleAccelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinScaleAccelYEmitter", PopplioComponentParticles::GetMinScaleAccelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinScaleAccelYEmitter", PopplioComponentParticles::SetMinScaleAccelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxScaleAccelYEmitter", PopplioComponentParticles::GetMaxScaleAccelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxScaleAccelYEmitter", PopplioComponentParticles::SetMaxScaleAccelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinColREmitter", PopplioComponentParticles::GetMinColREmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinColREmitter", PopplioComponentParticles::SetMinColREmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxColREmitter", PopplioComponentParticles::GetMaxColREmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxColREmitter", PopplioComponentParticles::SetMaxColREmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinColGEmitter", PopplioComponentParticles::GetMinColGEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinColGEmitter", PopplioComponentParticles::SetMinColGEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxColGEmitter", PopplioComponentParticles::GetMaxColGEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxColGEmitter", PopplioComponentParticles::SetMaxColGEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinColBEmitter", PopplioComponentParticles::GetMinColBEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinColBEmitter", PopplioComponentParticles::SetMinColBEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxColBEmitter", PopplioComponentParticles::GetMaxColBEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxColBEmitter", PopplioComponentParticles::SetMaxColBEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinColAEmitter", PopplioComponentParticles::GetMinColAEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinColAEmitter", PopplioComponentParticles::SetMinColAEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxColAEmitter", PopplioComponentParticles::GetMaxColAEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxColAEmitter", PopplioComponentParticles::SetMaxColAEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinColVelREmitter", PopplioComponentParticles::GetMinColVelREmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinColVelREmitter", PopplioComponentParticles::SetMinColVelREmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxColVelREmitter", PopplioComponentParticles::GetMaxColVelREmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxColVelREmitter", PopplioComponentParticles::SetMaxColVelREmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinColVelGEmitter", PopplioComponentParticles::GetMinColVelGEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinColVelGEmitter", PopplioComponentParticles::SetMinColVelGEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxColVelGEmitter", PopplioComponentParticles::GetMaxColVelGEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxColVelGEmitter", PopplioComponentParticles::SetMaxColVelGEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinColVelBEmitter", PopplioComponentParticles::GetMinColVelBEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinColVelBEmitter", PopplioComponentParticles::SetMinColVelBEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxColVelBEmitter", PopplioComponentParticles::GetMaxColVelBEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxColVelBEmitter", PopplioComponentParticles::SetMaxColVelBEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinColVelAEmitter", PopplioComponentParticles::GetMinColVelAEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinColVelAEmitter", PopplioComponentParticles::SetMinColVelAEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxColVelAEmitter", PopplioComponentParticles::GetMaxColVelAEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxColVelAEmitter", PopplioComponentParticles::SetMaxColVelAEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinColAccelREmitter", PopplioComponentParticles::GetMinColAccelREmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinColAccelREmitter", PopplioComponentParticles::SetMinColAccelREmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxColAccelREmitter", PopplioComponentParticles::GetMaxColAccelREmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxColAccelREmitter", PopplioComponentParticles::SetMaxColAccelREmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinColAccelGEmitter", PopplioComponentParticles::GetMinColAccelGEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinColAccelGEmitter", PopplioComponentParticles::SetMinColAccelGEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxColAccelGEmitter", PopplioComponentParticles::GetMaxColAccelGEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxColAccelGEmitter", PopplioComponentParticles::SetMaxColAccelGEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinColAccelBEmitter", PopplioComponentParticles::GetMinColAccelBEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinColAccelBEmitter", PopplioComponentParticles::SetMinColAccelBEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxColAccelBEmitter", PopplioComponentParticles::GetMaxColAccelBEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxColAccelBEmitter", PopplioComponentParticles::SetMaxColAccelBEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinColAccelAEmitter", PopplioComponentParticles::GetMinColAccelAEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinColAccelAEmitter", PopplioComponentParticles::SetMinColAccelAEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxColAccelAEmitter", PopplioComponentParticles::GetMaxColAccelAEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxColAccelAEmitter", PopplioComponentParticles::SetMaxColAccelAEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinSpinDirAccelEmitter", PopplioComponentParticles::GetMinSpinDirAccelEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinSpinDirAccelEmitter", PopplioComponentParticles::SetMinSpinDirAccelEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxSpinDirAccelEmitter", PopplioComponentParticles::GetMaxSpinDirAccelEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxSpinDirAccelEmitter", PopplioComponentParticles::SetMaxSpinDirAccelEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinDirAccelXEmitter", PopplioComponentParticles::GetMinDirAccelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinDirAccelXEmitter", PopplioComponentParticles::SetMinDirAccelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxDirAccelXEmitter", PopplioComponentParticles::GetMaxDirAccelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxDirAccelXEmitter", PopplioComponentParticles::SetMaxDirAccelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinDirAccelYEmitter", PopplioComponentParticles::GetMinDirAccelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinDirAccelYEmitter", PopplioComponentParticles::SetMinDirAccelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxDirAccelYEmitter", PopplioComponentParticles::GetMaxDirAccelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxDirAccelYEmitter", PopplioComponentParticles::SetMaxDirAccelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinScaleDirAccelXEmitter", PopplioComponentParticles::GetMinScaleDirAccelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinScaleDirAccelXEmitter", PopplioComponentParticles::SetMinScaleDirAccelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxScaleDirAccelXEmitter", PopplioComponentParticles::GetMaxScaleDirAccelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxScaleDirAccelXEmitter", PopplioComponentParticles::SetMaxScaleDirAccelXEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinScaleDirAccelYEmitter", PopplioComponentParticles::GetMinScaleDirAccelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinScaleDirAccelYEmitter", PopplioComponentParticles::SetMinScaleDirAccelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxScaleDirAccelYEmitter", PopplioComponentParticles::GetMaxScaleDirAccelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxScaleDirAccelYEmitter", PopplioComponentParticles::SetMaxScaleDirAccelYEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinColDirAccelREmitter", PopplioComponentParticles::GetMinColDirAccelREmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinColDirAccelREmitter", PopplioComponentParticles::SetMinColDirAccelREmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxColDirAccelREmitter", PopplioComponentParticles::GetMaxColDirAccelREmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxColDirAccelREmitter", PopplioComponentParticles::SetMaxColDirAccelREmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinColDirAccelGEmitter", PopplioComponentParticles::GetMinColDirAccelGEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinColDirAccelGEmitter", PopplioComponentParticles::SetMinColDirAccelGEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxColDirAccelGEmitter", PopplioComponentParticles::GetMaxColDirAccelGEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxColDirAccelGEmitter", PopplioComponentParticles::SetMaxColDirAccelGEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinColDirAccelBEmitter", PopplioComponentParticles::GetMinColDirAccelBEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinColDirAccelBEmitter", PopplioComponentParticles::SetMinColDirAccelBEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxColDirAccelBEmitter", PopplioComponentParticles::GetMaxColDirAccelBEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxColDirAccelBEmitter", PopplioComponentParticles::SetMaxColDirAccelBEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMinColDirAccelAEmitter", PopplioComponentParticles::GetMinColDirAccelAEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMinColDirAccelAEmitter", PopplioComponentParticles::SetMinColDirAccelAEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::GetMaxColDirAccelAEmitter", PopplioComponentParticles::GetMaxColDirAccelAEmitter);
        mono_add_internal_call("Team_Popplio.Components.Particle::SetMaxColDirAccelAEmitter", PopplioComponentParticles::SetMaxColDirAccelAEmitter);
    }

    void SetIntCall(Popplio::Registry& registry, Popplio::LayerManager& layer, 
        Popplio::EventBus& eventB, Popplio::PrefabManager& preMan, Popplio::CameraManager& cam)
    {
        reg = &registry;
        lay = &layer;
        event = &eventB;
        prefab = &preMan;
        camera = &cam;
    }

    // temp buffers
    namespace
    {
        static char* charPtrMS = nullptr;
        static std::string strMS = "";
    }

    // Internal Calls

    // Mono //

    namespace PopplioMono
    {
        extern "C"
        {
            static void AddGC(MonoObject* obj)
            {
                reg->GetSystem<Popplio::LogicSystem>().monoAPI.AddGC(obj, false);
            }
            static void RemoveGC(MonoObject* obj)
            {
                reg->GetSystem<Popplio::LogicSystem>().monoAPI.RemoveGC(obj, false);
            }
        }
    }

    // Logging/Logger.h //

    namespace PopplioLog
    {
        extern "C" static void Critical(MonoString* message)
        {
            //return;
#ifndef IMGUI_DISABLE
            PopplioScriptAPI::MonoToString(message, strMS);
            Popplio::Logger::Critical(strMS, Popplio::LogDestination::ALL, false);
#else
            (void)message;
#endif
            
        }

        extern "C" static void Warning(MonoString* message)
        {
            //return;
#ifndef IMGUI_DISABLE
            PopplioScriptAPI::MonoToString(message, strMS);
            Popplio::Logger::Warning(strMS, Popplio::LogDestination::ALL, false);
#else
            (void)message;
#endif

        }

        extern "C" static void Debug(MonoString* message)
        {
            //return;
#ifndef IMGUI_DISABLE
            PopplioScriptAPI::MonoToString(message, strMS);
            Popplio::Logger::Debug(strMS, Popplio::LogDestination::ALL, false);
#else
            (void)message;
#endif

        }

        extern "C" static void Error(MonoString* message)
        {
            //return;
#ifndef IMGUI_DISABLE
            PopplioScriptAPI::MonoToString(message, strMS);
            Popplio::Logger::Error(strMS, Popplio::LogDestination::ALL, false);
#else
            (void)message;
#endif

        }

        extern "C" static void Info(MonoString* message)
        {
            //return;
#ifndef IMGUI_DISABLE
            PopplioScriptAPI::MonoToString(message, strMS);
            Popplio::Logger::Info(strMS, Popplio::LogDestination::ALL, false);
#else
            (void)message;
#endif

        }

        extern "C" static void EngineCritical(MonoString* message)
        {
            //return;
#ifndef IMGUI_DISABLE
            PopplioScriptAPI::MonoToString(message, strMS);
            Popplio::Logger::Critical(strMS);
#else
            (void)message;
#endif

        }

        extern "C" static void EngineWarning(MonoString* message)
        {
            //return;
#ifndef IMGUI_DISABLE
            PopplioScriptAPI::MonoToString(message, strMS);
            Popplio::Logger::Warning(strMS);
#else
            (void)message;
#endif

        }

        extern "C" static void EngineDebug(MonoString* message)
        {
            //return;
#ifndef IMGUI_DISABLE
            PopplioScriptAPI::MonoToString(message, strMS);
            Popplio::Logger::Debug(strMS);
#else
            (void)message;
#endif

        }

        extern "C" static void EngineError(MonoString* message)
        {
            //return;
#ifndef IMGUI_DISABLE
            PopplioScriptAPI::MonoToString(message, strMS);
            Popplio::Logger::Error(strMS);
#else
            (void)message;
#endif

        }

        extern "C" static void EngineInfo(MonoString* message)
        {
            //return;
#ifndef IMGUI_DISABLE
            PopplioScriptAPI::MonoToString(message, strMS);
            Popplio::Logger::Info(strMS);
#else
            (void)message;
#endif

        }
    }

    // Engine/Engine.h //

    namespace PopplioEngine
    {
        extern "C"
        {
            static double GetFPS()
            {
                return Popplio::Engine::framesPerSecond;
            }
            static void AppQuit()
            {
                if (Popplio::FunctionRegistry::GetInstance().HasFunction("QUIT"))
                    Popplio::FunctionRegistry::GetInstance().GetFunction("QUIT")();
            }
        }
    }

    // Input/InputSystem.h //

    namespace PopplioInput
    {
        extern "C"
        {
            static bool KeyTriggered(int key)
            {
                return Popplio::InputSystem::IsKeyTriggered(key);
            }
            static bool KeyDown(int key)
            {
                return Popplio::InputSystem::IsKeyDown(key);
            }
            static bool KeyReleased(int key)
            {
                return Popplio::InputSystem::IsKeyReleased(key);
            }
            static MonoArray* KeysTriggered()
            {
                return ArrayToMono1D(Popplio::InputSystem::GetKeysTriggered(),
                    &reg->GetSystem<Popplio::LogicSystem>().monoAPI.GetAppDomain());
            }
            static MonoArray* KeysDown()
            {
                return ArrayToMono1D(Popplio::InputSystem::GetKeysDown(),
                    &reg->GetSystem<Popplio::LogicSystem>().monoAPI.GetAppDomain());
            }
            static MonoArray* KeysReleased()
            {
                return ArrayToMono1D(Popplio::InputSystem::GetKeysReleased(),
                    &reg->GetSystem<Popplio::LogicSystem>().monoAPI.GetAppDomain());
            }
            static bool MouseButtonTriggered(int button)
            {
                return Popplio::InputSystem::IsMouseButtonTriggered(static_cast<Popplio::InputSystem::MouseButton>(button));
            }
            static bool MouseButtonDown(int button)
            {
                return Popplio::InputSystem::IsMouseButtonDown(static_cast<Popplio::InputSystem::MouseButton>(button));
            }
            static bool MouseButtonReleased(int button)
            {
                return Popplio::InputSystem::IsMouseButtonReleased(static_cast<Popplio::InputSystem::MouseButton>(button));
            }
            static float MouseX()
            {
                return Popplio::InputSystem::GetMouse().x;
            }
            static float MouseY()
            {
                return Popplio::InputSystem::GetMouse().y;
            }
            static float MouseDeltaX()
            {
                return Popplio::InputSystem::GetMouseDelta().x;
            }
            static float MouseDeltaY()
            {
                return Popplio::InputSystem::GetMouseDelta().y;
            }
            static float MousePrevX()
            {
                return Popplio::InputSystem::GetMousePrevious().x;
            }
            static float MousePrevY()
            {
                return Popplio::InputSystem::GetMousePrevious().y;
            }
            static float MouseScrollX()
            {
                return Popplio::InputSystem::GetMouseScroll().x;
            }
            static float MouseScrollY()
            {
                return Popplio::InputSystem::GetMouseScroll().y;
            }
            static double InputLatency(int key)
            {
                return Popplio::InputSystem::GetInputLatency(key);
            }
        }
    }

    // Camera/GameCamera.h //

    namespace PopplioCamera
    {
        extern "C"
        {
            static float GetPosX()
            {
                return camera->GetGameCamera().GetPosition().x;
            }
            static float GetPosY()
            {
                return camera->GetGameCamera().GetPosition().y;
            }
            static void SetPosition(float x, float y)
            {
                camera->GetGameCamera().SetPosition({ x, y });
            }
            static float GetZoom()
            {
                return camera->GetGameCamera().GetZoom();
            }
            static void SetZoom(float value)
            {
                camera->GetGameCamera().SetZoom(value);
            }
            static float GetAspectRatio()
            {
                return camera->GetGameCamera().GetAspectRatio();
            }
            static void SetTarget(int entity)
            {
                std::weak_ptr<Popplio::Entity> ent{};
                ent.lock() = std::make_shared<Popplio::Entity>(entity);
                camera->GetGameCamera().SetTarget(ent);
            }
            static void ClearTarget()
            {
                camera->GetGameCamera().ClearTarget();
            }
            static bool HasTarget()
            {
                return camera->GetGameCamera().HasTarget();
            }
        }
    }

    // Utilities/Timer.h //

    namespace PopplioTime
    {
        extern "C"
        {
            static double GetDeltaTime()
            {
                return Popplio::Engine::timer.GetDeltaTime();
            }
            static double GetFixedDeltaTime()
            {
                return Popplio::Engine::timer.GetFixedDeltaTime();
            }
            static double GetTime()
            {
                return Popplio::Engine::timer.GetElapsedTime();
            }
            static void SetFixedDeltaTime(double value)
            {
                Popplio::Engine::timer.SetFixedDeltaTime(value);
            }
            static void ResetFixedDeltaTime()
            {
                Popplio::Engine::timer.ResetFixedDeltaTime();
            }
        }
    }

    // Utilities/Rand.h //

    namespace PopplioRand
    {
        extern "C"
        {
            static void SetRand(unsigned int seed)
            {
                PopplioUtil::SetRand(seed);
            }
            static int GetRand()
            {
                return PopplioUtil::Rand();
            }
            static int RandInt(int min, int max)
            {
                return PopplioUtil::RandInt(min, max);
            }
            static float RandFloat(float min, float max)
            {
                return PopplioUtil::RandFloat(min, max);
            }
        }
    }

    // Animation/AnimationSystem.h //

    namespace PopplioAnim
    {
        extern "C"
        {
            static void PlayAll()
            {
                reg->GetSystem<Popplio::AnimationSystem>().StartAll();
            }
            static void StopAll()
            {
                reg->GetSystem<Popplio::AnimationSystem>().StopAll();
            }
            static void ResumeAll()
            {
                reg->GetSystem<Popplio::AnimationSystem>().ResumeAll();
            }
            static void PauseAll()
            {
                reg->GetSystem<Popplio::AnimationSystem>().PauseAll();
            }
        }
    }

    // Audio/AudioSystem.h //

    namespace PopplioAudio
    {
        extern "C"
        {
            static unsigned int GetDefaultUnit()
            {
                return static_cast<unsigned int>(Popplio::Audio::defaultUnit);
            }
            static bool GetIsPlaying(int nativeID)
            {
                int const& id = reg->GetEntityById(nativeID).GetComponent<Popplio::AudioComponent>().data;
                Popplio::AudioSystem& sys = reg->GetSystem<Popplio::AudioSystem>();
                return sys.GetAudioInst(id).GetPlaying();
            }
            static bool GetIsPaused(int nativeID)
            {
                int const& id = reg->GetEntityById(nativeID).GetComponent<Popplio::AudioComponent>().data;
                Popplio::AudioSystem& sys = reg->GetSystem<Popplio::AudioSystem>();
                return sys.GetAudioInst(id).GetPaused();
            }
            static unsigned int GetLength(int nativeID)
            {
                int const& id = reg->GetEntityById(nativeID).GetComponent<Popplio::AudioComponent>().data;
                Popplio::AudioSystem& sys = reg->GetSystem<Popplio::AudioSystem>();
                return sys.GetAudioInst(id).GetLen();
            }
            static float GetBeatInterval(int nativeID)
            {
                int const& id = reg->GetEntityById(nativeID).GetComponent<Popplio::AudioComponent>().data;
                Popplio::AudioSystem& sys = reg->GetSystem<Popplio::AudioSystem>();
                return sys.GetAudioInst(id).GetBeatInterval();
            }
            static unsigned int GetCurrentTime(int nativeID)
            {
                int const& id = reg->GetEntityById(nativeID).GetComponent<Popplio::AudioComponent>().data;
                Popplio::AudioSystem& sys = reg->GetSystem<Popplio::AudioSystem>();
                return sys.GetAudioInst(id).GetCurr();
            }
            static void SetCurrentTime(int nativeID, unsigned int time)
            {
                int const& id = reg->GetEntityById(nativeID).GetComponent<Popplio::AudioComponent>().data;
                Popplio::AudioSystem& sys = reg->GetSystem<Popplio::AudioSystem>();
                sys.GetAudioInst(id).SetCurr(time);
            }
            static int GetOffBeat(int nativeID)
            {
                int const& id = reg->GetEntityById(nativeID).GetComponent<Popplio::AudioComponent>().data;
                Popplio::AudioSystem& sys = reg->GetSystem<Popplio::AudioSystem>();
                return sys.GetAudioInst(id).OffBeat();
            }
            static void Play(int nativeID)
            {
                int const& id = reg->GetEntityById(nativeID).GetComponent<Popplio::AudioComponent>().data;
                Popplio::AudioSystem& sys = reg->GetSystem<Popplio::AudioSystem>();
                sys.GetAudioInst(id).Play();
            }
            static void Pause(int nativeID)
            {
                int const& id = reg->GetEntityById(nativeID).GetComponent<Popplio::AudioComponent>().data;
                Popplio::AudioSystem& sys = reg->GetSystem<Popplio::AudioSystem>();
                sys.GetAudioInst(id).Pause();
            }
            static void Stop(int nativeID)
            {
                int const& id = reg->GetEntityById(nativeID).GetComponent<Popplio::AudioComponent>().data;
                Popplio::AudioSystem& sys = reg->GetSystem<Popplio::AudioSystem>();
                sys.GetAudioInst(id).Stop();
            }
            static MonoArray* GetSpectrum(int nativeID, int channel, float magMod)
            {
                int const& id = reg->GetEntityById(nativeID).GetComponent<Popplio::AudioComponent>().data;
                Popplio::AudioSystem& sys = reg->GetSystem<Popplio::AudioSystem>();

                // TODO optimize this
                float* data[2]; 
                float dataL[POPPLIO_AUDIO_FFT_BIN_COUNT]{};
                float dataR[POPPLIO_AUDIO_FFT_BIN_COUNT]{};
                data[0] = dataL;
                data[1] = dataR;

                std::vector<float> val(POPPLIO_AUDIO_FFT_BIN_COUNT);

                bool result = sys.GetAudioInst(id).GetSpectrumLog(data, magMod);
                //if (!result) return false;
                Popplio::LogicSystem& logic = reg->GetSystem<Popplio::LogicSystem>();
                (void)result; // TODO

                for (int i = 0; i < POPPLIO_AUDIO_FFT_BIN_COUNT; i++)
                {
                    val[i] = data[channel][i];
                }

                return ArrayToMono1D<float>(val, &logic.monoAPI.GetAppDomain());
                //return ArrayToMono2D<float>(data, 2, POPPLIO_AUDIO_FFT_BIN_COUNT, &logic.monoAPI.GetAppDomain());
                //fft = ArrayToMono2D<float>(data, 2, POPPLIO_AUDIO_FFT_BIN_COUNT, &logic.monoAPI.GetAppDomain());
                //return result;
                //return true;
            }

            static void SetMute(int nativeID, bool m)
            {
                int const& id = reg->GetEntityById(nativeID).GetComponent<Popplio::AudioComponent>().data;
                Popplio::AudioSystem& sys = reg->GetSystem<Popplio::AudioSystem>();
                sys.GetAudioInst(id).SetMute(m);
            }

            static bool GetMute(int nativeID)
            {
                int const& id = reg->GetEntityById(nativeID).GetComponent<Popplio::AudioComponent>().data;
                Popplio::AudioSystem& sys = reg->GetSystem<Popplio::AudioSystem>();
                return sys.GetAudioInst(id).GetMute();
            }


            static float GetMasterVolume()
            {
                float val{-1.f};
                reg->GetSystem<Popplio::AudioSystem>().GetCGroup(0)->getVolume(&val);
                return val;
            }

            static void SetMasterVolume(float volume)
            {
                if (volume < 0.f) volume = 0.f;
                if (volume > 1.f) volume = 1.f;
                reg->GetSystem<Popplio::AudioSystem>().GetCGroup(0)->setVolume(volume);
            }

            static float GetBGMVolume()
            {
                float val{ -1.f };
                reg->GetSystem<Popplio::AudioSystem>().GetCGroup(1)->getVolume(&val);
                return val;
            }

            static void SetBGMVolume(float volume)
            {
                if (volume < 0.f) volume = 0.f;
                if (volume > 1.f) volume = 1.f;
                reg->GetSystem<Popplio::AudioSystem>().GetCGroup(1)->setVolume(volume);
            }

            static float GetSFXVolume()
            {
                float val{ -1.f };
                reg->GetSystem<Popplio::AudioSystem>().GetCGroup(2)->getVolume(&val);
                return val;
            }

            static void SetSFXVolume(float volume)
            {
                if (volume < 0.f) volume = 0.f;
                if (volume > 1.f) volume = 1.f;
                reg->GetSystem<Popplio::AudioSystem>().GetCGroup(2)->setVolume(volume);
            }

            static float GetGroupVolume(int group)
            {
                float val{ -1.f };
                try
                {
                    reg->GetSystem<Popplio::AudioSystem>().GetCGroup(group)->getVolume(&val);
                }
                catch (const std::exception& e)
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | PopplioAudio::GetGroupVolume | " + std::string(e.what()));
                    return -1.f;
                }
                return val;
            }

            static void SetGroupVolume(int group, float volume)
            {
                try
                {
                    reg->GetSystem<Popplio::AudioSystem>().GetCGroup(group)->setVolume(volume);
                }
                catch (const std::exception& e)
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | PopplioAudio::SetGroupVolume | " + std::string(e.what()));
                }
            }
        }
    }

    // Scene/SceneManageSystem.h //

    namespace PopplioScene
    {
        extern "C"
        {
            static float lastSceneLoadTime = 0.f;
            //const float sceneLoadCooldown = 1.f;

            static void Load(MonoString* sceneName, bool reloadMono)
            {
                if (Popplio::Engine::timer.GetElapsedTime() - lastSceneLoadTime < POPPLIO_SCENE_LOAD_COOLDOWN) return;
                else lastSceneLoadTime = Popplio::Engine::timer.GetElapsedTime();

                PopplioScriptAPI::MonoToString(sceneName, strMS);
                auto loadScene = [](std::string const& sceneName)
                {
                    reg->GetSystem<Popplio::SceneManageSystem>().LoadScene(sceneName);
                };
                std::string val = strMS;
                reg->GetSystem<Popplio::QueueSystem>().Add(std::bind(loadScene, val));

                reg->GetSystem<Popplio::LogicSystem>().reloadMono = reloadMono;
            }
            static MonoString* GetCurrent()
            {
                return PopplioScriptAPI::StringToMono(reg->GetSystem<Popplio::SceneManageSystem>().GetCurrentScene());
            }
            static MonoString* GetPrevious()
            {
                return PopplioScriptAPI::StringToMono(reg->GetSystem<Popplio::SceneManageSystem>().GetPreviousScene());
            }
            static void Reload(bool reloadMono)
            {
                auto loadScene = []()
                {
                    reg->GetSystem<Popplio::SceneManageSystem>().ReloadCurrentScene();
                };
                reg->GetSystem<Popplio::QueueSystem>().Add(loadScene);

                reg->GetSystem<Popplio::LogicSystem>().reloadMono = reloadMono;
            }
        }
    }

    // Math //

    namespace PopplioMath
    {
        extern "C"
        {
            static float Dist2D(float x1, float y1, float x2, float y2)
            {
                ::PopplioMath::Vec2f v1(x1, y1);
                ::PopplioMath::Vec2f v2(x2, y2);
                return ::PopplioMath::Dist2D(v1, v2);
            }
        }
    }

    // JSON //

    namespace PopplioJson
    {
        extern "C"
        {
            static rapidjson::Document document;
            static std::string currentDoc;
            static std::ifstream ifs;
            static std::vector<rapidjson::GenericObject<false, rapidjson::Value>> objects{};
            static std::vector<rapidjson::Value*> values{};
            //static std::vector<rapidjson::GenericValue<rapidjson::Value>*> values{};
            //static std::vector<rapidjson::Value> values;
            //static std::vector<rapidjson::GenericValue<rapidjson::Value>> values{};

            static intptr_t LoadJson(MonoString* filename)
            {
                document = rapidjson::Document{}; // clear the document
                currentDoc = "";

                if (ifs.is_open()) ifs.close();
                if (!objects.empty()) ClearObjs();

                PopplioUtil::ResetCurrentDirectory();

                PopplioScriptAPI::MonoToString(filename, strMS);
                ifs = std::ifstream(strMS);
                if (!ifs)
                {
                    Popplio::Logger::Error(
                        "MonoInternalCall.cpp | PopplioJson::Load | Error opening file: " + strMS + " for reading");
                    return reinterpret_cast<intptr_t>(nullptr);
                }

                currentDoc = strMS;

                rapidjson::IStreamWrapper isw(ifs);
                document.ParseStream(isw);

                if (document.HasParseError())
                {
                    Popplio::Logger::Error(
                        "MonoInternalCall.cpp | PopplioJson::Load | Error opening file: " + strMS + " for reading");

                    Popplio::Logger::Error(
                        "MonoInternalCall.cpp | PopplioJson::Load | JSON parse error: " + std::to_string(document.GetParseError()));
                    return reinterpret_cast<intptr_t>(nullptr);
                }

                if (!document.IsObject())
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | PopplioJson::Load | Invalid JSON structure");
                    return reinterpret_cast<intptr_t>(nullptr);
                }

                return reinterpret_cast<intptr_t>(&document);
            }

            static intptr_t GetCurrentDoc() { return reinterpret_cast<intptr_t>(&document); }
            static MonoString* GetCurrentDocName() { return PopplioScriptAPI::StringToMono(currentDoc); }

            static int LoadInt(MonoString* key)
            {
                PopplioScriptAPI::MonoToString(key, strMS);

                if (document.HasMember(strMS.c_str()) && document[strMS.c_str()].IsInt())
                {
                    return document[strMS.c_str()].GetInt();
                }

                return 0;
            }
            static float LoadFloat(MonoString* key)
            {
                PopplioScriptAPI::MonoToString(key, strMS);

                if (document.HasMember(strMS.c_str()) && document[strMS.c_str()].IsFloat())
                {
                    return document[strMS.c_str()].GetFloat();
                }

                return 0.f;
            }
            static bool LoadBool(MonoString* key)
            {
                PopplioScriptAPI::MonoToString(key, strMS);

                if (document.HasMember(strMS.c_str()) && document[strMS.c_str()].IsBool())
                {
                    return document[strMS.c_str()].GetBool();
                }

                return false;
            }
            static MonoString* LoadString(MonoString* key)
            {
                PopplioScriptAPI::MonoToString(key, strMS);

                if (document.HasMember(strMS.c_str()) && document[strMS.c_str()].IsString())
                {
                    return PopplioScriptAPI::StringToMono(document[strMS.c_str()].GetString());
                }

                return nullptr;
            }

            static void CloseIf()
            {
                if (ifs.is_open()) ifs.close();
                document = rapidjson::Document{};
                currentDoc = "";
                PopplioUtil::ResetCurrentDirectory();
            }

            static void ClearObjs()
            {
                objects.clear();
                values.clear();
            }

            static MonoArray* LoadObjArray(MonoString* key)
            {
                PopplioScriptAPI::MonoToString(key, strMS);

                if (document.HasMember(strMS.c_str()) && document[strMS.c_str()].IsArray())
                {
                    rapidjson::Value& arr = document[strMS.c_str()];
                    static std::vector<rapidjson::Value*> arrJson{};
                    static std::vector<intptr_t> arrObj{};
                    
                    arrObj.clear();
                    arrJson.clear();

                    for (auto& v : arr.GetArray())
                    {
                        arrJson.push_back(&v);
                        arrObj.push_back(reinterpret_cast<intptr_t>(arrJson.back()));
                    }

                    return ArrayToMono1D(arrObj, &reg->GetSystem<Popplio::LogicSystem>().monoAPI.GetAppDomain());
                }

                Popplio::Logger::Error("MonoInternalCall.cpp | PopplioJson::LoadObjArray | failed to get array");
                return nullptr;
            }

            static intptr_t LoadObjWithObj(intptr_t obj)
            {
                rapidjson::GenericObject<false, rapidjson::Value>* object = 
                    reinterpret_cast<rapidjson::GenericObject<false, rapidjson::Value>*>(obj);
                if (object == nullptr)
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | PopplioJson::LoadObjWithObj | object is nullptr");
                    return reinterpret_cast<intptr_t>(nullptr);
                }
                Popplio::Logger::Warning(std::to_string(object->HasMember(strMS.c_str())));
                Popplio::Logger::Warning(std::to_string((*object)[strMS.c_str()].IsObject()));
                if (object->HasMember(strMS.c_str()) && (*object)[strMS.c_str()].IsObject())
                {
                    objects.push_back((*object)[strMS.c_str()].GetObject());
                    return reinterpret_cast<intptr_t>(&objects.back());
                }

                Popplio::Logger::Error("MonoInternalCall.cpp | PopplioJson::LoadObjWithObj | failed to get obj");
                return reinterpret_cast<intptr_t>(nullptr);
            }
            static intptr_t LoadObj(MonoString* key)
            {
                PopplioScriptAPI::MonoToString(key, strMS);

                if (document.HasMember(strMS.c_str()) && document[strMS.c_str()].IsObject())
                {
                    objects.push_back(document[strMS.c_str()].GetObject());
                    return reinterpret_cast<intptr_t>(&objects.back());
                }

                return reinterpret_cast<intptr_t>(nullptr);
            }
            static int LoadObjInt(intptr_t objPtr, MonoString* key)
            {
                PopplioScriptAPI::MonoToString(key, strMS);
                rapidjson::Value* obj =
                    reinterpret_cast<rapidjson::Value*>(objPtr);
                if (obj == nullptr)
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | PopplioJson::LoadObjInt | objPtr is nullptr | " + strMS);
                    return 0;
                }
                std::string str = strMS;

                if (!obj->IsObject())
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | PopplioJson::LoadObjInt | is not object | " + strMS);
                    return 0;
                }

                if (obj->HasMember(strMS.c_str()) && (*obj)[strMS.c_str()].IsInt())
                {
                    return (*obj)[strMS.c_str()].GetInt();
                }
                Popplio::Logger::Error("MonoInternalCall.cpp | PopplioJson::LoadObjInt | failed to get int | " + strMS);
                return 0;
            }
            static float LoadObjFloat(intptr_t objPtr, MonoString* key)
            {
                PopplioScriptAPI::MonoToString(key, strMS);
                rapidjson::Value* obj =
                    reinterpret_cast<rapidjson::Value*>(objPtr);
                if (obj == nullptr)
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | PopplioJson::LoadObjFloat | objPtr is nullptr | " + strMS);
                    return 0.f;
                }
                std::string str = strMS;

                if (!obj->IsObject())
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | PopplioJson::LoadObjFloat | is not object | " + strMS);
                    return 0.f;
                }

                if (obj->HasMember(strMS.c_str()) && (*obj)[strMS.c_str()].IsFloat())
                {
                    return (*obj)[strMS.c_str()].GetFloat();
                }
                Popplio::Logger::Error("MonoInternalCall.cpp | PopplioJson::LoadObjFloat | failed to get float | " + strMS);
                return 0;
            }
            static bool LoadObjBool(intptr_t objPtr, MonoString* key)
            {
                PopplioScriptAPI::MonoToString(key, strMS);
                rapidjson::Value* obj = 
                    reinterpret_cast<rapidjson::Value*>(objPtr);
                if (obj == nullptr) return false;

                if (!obj->IsObject())
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | PopplioJson::LoadObjBool | is not object");
                    return 0;
                }

                if (obj->HasMember(strMS.c_str()) && (*obj)[strMS.c_str()].IsBool())
                {
                    return (*obj)[strMS.c_str()].GetBool();
                }

                return false;
            }
            static MonoString* LoadObjString(intptr_t objPtr, MonoString* key)
            {
                PopplioScriptAPI::MonoToString(key, strMS);
                rapidjson::Value* obj =
                    reinterpret_cast<rapidjson::Value*>(objPtr);
                if (obj == nullptr)
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | PopplioJson::LoadObjString | objPtr is nullptr | " + strMS);
                    return nullptr;
                }
                std::string str = strMS;

                if (!obj->IsObject())
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | PopplioJson::LoadObjString | is not object | " + strMS);
                    return nullptr;
                }

                if (obj->HasMember(strMS.c_str()) && (*obj)[strMS.c_str()].IsString())
                {
                    return StringToMono((*obj)[strMS.c_str()].GetString());
                }
                Popplio::Logger::Error("MonoInternalCall.cpp | PopplioJson::LoadObjString | failed to get string | " + strMS);
                return nullptr;
            }
            static intptr_t LoadObjObj(intptr_t objPtr, MonoString* key)
            {
                PopplioScriptAPI::MonoToString(key, strMS);
                rapidjson::Value* obj =
                    reinterpret_cast<rapidjson::Value*>(objPtr);
                if (obj == nullptr)
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | PopplioJson::LoadObjObj | objPtr is nullptr | " + strMS);
                    return reinterpret_cast<intptr_t>(nullptr);
                }
                std::string str = strMS;

                if (!obj->IsObject())
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | PopplioJson::LoadObjObj | is not object | " + strMS);
                    return reinterpret_cast<intptr_t>(nullptr);
                }

                if (obj->HasMember(strMS.c_str()) && (*obj)[strMS.c_str()].IsObject())
                {
                    objects.push_back((*obj)[strMS.c_str()].GetObject());
                    return reinterpret_cast<intptr_t>(&objects.back());
                }
                Popplio::Logger::Error("MonoInternalCall.cpp | PopplioJson::LoadObjInt | failed to get int | " + strMS);
                return reinterpret_cast<intptr_t>(nullptr);
            }

            static std::ofstream ofs;
            static rapidjson::MemoryPoolAllocator<>* allocator;

            static intptr_t SaveJson1()
            {
                document = rapidjson::Document{}; // clear the document
                currentDoc = "";

                if (ofs.is_open()) ofs.close();
                if (!objects.empty()) ClearObjs();
                //allocator.reset();

                PopplioUtil::ResetCurrentDirectory();

                // Clear the existing document and set it as an object
                document.SetObject();
                allocator = &document.GetAllocator();

                return reinterpret_cast<intptr_t>(&document);
            }

            static intptr_t NewObjValue()
            {
                rapidjson::Value val(rapidjson::kObjectType);
                values.push_back(&val);
                return reinterpret_cast<intptr_t>(values.back());
            }

            static intptr_t NewArrayValue()
            {
                rapidjson::Value val(rapidjson::kArrayType);
                values.push_back(&val);
                return reinterpret_cast<intptr_t>(values.back());
            }

            static void AddToArray(intptr_t array, intptr_t value)
            {
                rapidjson::Value* arr = reinterpret_cast<rapidjson::Value*>(array);
                rapidjson::Value* val = reinterpret_cast<rapidjson::Value*>(value);
                if (arr == nullptr || val == nullptr)
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | PopplioJson::AddToArray | array or value is nullptr");
                    return;
                }
                arr->PushBack(*val, *allocator);
            }

            static intptr_t SaveBool(MonoString* key, bool value, intptr_t member)
            {
                rapidjson::Value val{};
                val.SetBool(value);
                PopplioScriptAPI::MonoToString(key, strMS);

                rapidjson::Value* obj = reinterpret_cast<rapidjson::Value*>(member);

                if (obj == nullptr)document.AddMember(rapidjson::Value(strMS.c_str(), *allocator), val, *allocator);
                else obj->AddMember(rapidjson::Value(strMS.c_str(), *allocator), val, *allocator);

                values.push_back(&val);
                return reinterpret_cast<intptr_t>(&values.back());
            }

            static intptr_t SaveInt(MonoString* key, int value, intptr_t member)
            {
                rapidjson::Value val{};
                val.SetInt(value);
                PopplioScriptAPI::MonoToString(key, strMS);

                rapidjson::Value* obj = reinterpret_cast<rapidjson::Value*>(member);

                if (obj == nullptr)document.AddMember(rapidjson::Value(strMS.c_str(), *allocator), val, *allocator);
                else obj->AddMember(rapidjson::Value(strMS.c_str(), *allocator), val, *allocator);

                values.push_back(&val);
                return reinterpret_cast<intptr_t>(&values.back());
            }

            static intptr_t SaveFloat(MonoString* key, float value, intptr_t member)
            {
                rapidjson::Value val{};
                val.SetFloat(value);
                PopplioScriptAPI::MonoToString(key, strMS);

                rapidjson::Value* obj = reinterpret_cast<rapidjson::Value*>(member);

                if (obj == nullptr)document.AddMember(rapidjson::Value(strMS.c_str(), *allocator), val, *allocator);
                else obj->AddMember(rapidjson::Value(strMS.c_str(), *allocator), val, *allocator);

                values.push_back(&val);
                return reinterpret_cast<intptr_t>(&values.back());
            }

            static intptr_t SaveString(MonoString* key, MonoString* value, intptr_t member)
            {
                rapidjson::Value val{};
                PopplioScriptAPI::MonoToString(value, strMS);
                val.SetString(strMS.c_str(), static_cast<rapidjson::SizeType>(strMS.length()), *allocator);
                PopplioScriptAPI::MonoToString(key, strMS);

                rapidjson::Value* obj = reinterpret_cast<rapidjson::Value*>(member);

                if (obj == nullptr)document.AddMember(rapidjson::Value(strMS.c_str(), *allocator), val, *allocator);
                else obj->AddMember(rapidjson::Value(strMS.c_str(), *allocator), val, *allocator);

                values.push_back(&val);
                return reinterpret_cast<intptr_t>(&values.back());
            }

            static void SaveObj(MonoString* key, intptr_t value, intptr_t array)
            {
                rapidjson::Value* obj = reinterpret_cast<rapidjson::Value*>(array);
                PopplioScriptAPI::MonoToString(key, strMS);
                if (strMS != "" && obj == nullptr)
                    document.AddMember(rapidjson::Value(strMS.c_str(), *allocator), 
                        *reinterpret_cast<rapidjson::Value*>(value), *allocator);
                else obj->PushBack(*reinterpret_cast<rapidjson::Value*>(value), *allocator);

                //values.push_back(reinterpret_cast<rapidjson::Value*>(value));
                //return reinterpret_cast<intptr_t>(&values.back());
            }

            static bool SaveJson2(MonoString* filename)
            {
                rapidjson::StringBuffer buffer;
                rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
                document.Accept(writer);

                if (ofs.is_open()) ofs.close();

                PopplioScriptAPI::MonoToString(filename, strMS);
                ofs = std::ofstream(strMS);
                if (!ofs)
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | Failed to open file for writing: " + strMS);
                    std::filesystem::current_path(std::filesystem::path(Popplio::Engine::programPath));
                    return false;
                }

                ofs << buffer.GetString();
                ofs.close();

                Popplio::Logger::Info("MonoInternalCall.cpp | Saved level data to file: " + strMS);
                PopplioUtil::ResetCurrentDirectory();

                allocator = nullptr;

                return true;
            }

            //static void InitValues()
            //{
            //    if (values.size() > 0) values.clear();
            //    values.reserve(1);
            //}
        }
    }

    // ECS/ECS.h //

    namespace PopplioECS
    {
        extern "C" // Entity
        {
            static void Kill(int instance)
            {
                DelEnt(instance);

                //lay->RemoveEntityFromCurrentLayer(e);
                //e.Kill();
//#ifndef IMGUI_DISABLE
//                reg->GetSystem<Popplio::Editor>().ResetSelectedEntity(e);
//#endif
            }
            //int GetId(int const& instance)
            //{
            //    return static_cast<Popplio::Entity const*>(instance)->GetId();
            //}
            static MonoString* GetTag(int instance)
            {
                return PopplioScriptAPI::StringToMono(reg->GetEntityTag(instance));
            }
            static void Tag(int instance, MonoString* tag)
            {
                PopplioScriptAPI::MonoToString(tag, strMS);

                reg->GetEntityById(instance).Tag(strMS);
            }
            static bool HasTag(int instance, MonoString* tag)
            {
                PopplioScriptAPI::MonoToString(tag, strMS);

                return reg->GetEntityById(instance).HasTag(strMS);
            }
            static void Group(int instance, MonoString* group)
            {
                PopplioScriptAPI::MonoToString(group, strMS);
                reg->GetEntityById(instance).Group(strMS);
            }
            static bool BelongsToGroup(int instance, MonoString* group)
            {
                PopplioScriptAPI::MonoToString(group, strMS);
                return reg->GetEntityById(instance).BelongsToGroup(strMS);
            }
            static bool Exists(int instance)
            {
                return reg->EntityExists(instance);
            }
            //static void OnScriptDestroy(int instance)
            //{
            //    reg->GetSystem<Popplio::LogicSystem>().CSRemoveScriptObj(instance);
            //}
        }
    }

    // C# API //

    namespace PopplioECS
    {
        extern "C"
        {
            static int InstEnt(int entity) // TODO Test
            {
                if (entity == -1)
                {
                    Popplio::Entity newE = reg->CreateEntity();
                    event->EmitEvent<Popplio::EntityRuntimeChangeEvent>(Popplio::EntityRuntimeChangeEvent::Change::ADDED, newE.GetId());
                    reg->RuntimeUpdate(newE);
                    if (!newE.Exists()) Popplio::Logger::Error("MonoInternalCall.cpp | InstEnt : Entity does not exist");
                    return newE.GetId();
                }
                if (reg->EntityExists(entity))
                {
                    int newId = reg->GetSystem<Popplio::RuntimeSystem>().InstantiateEntity(entity);
                    event->EmitEvent<Popplio::EntityRuntimeChangeEvent>(Popplio::EntityRuntimeChangeEvent::Change::ADDED, newId);
                   
                    //reg->RuntimeUpdate(reg->GetEntityById(newId));
                    if (!reg->EntityExists(newId)) Popplio::Logger::Error("MonoInternalCall.cpp | InstEnt : Entity does not exist");
                    return newId;
                }

                Popplio::Logger::Error("MonoInternalCall.cpp | InstEnt : Cannot find entity");

                return -1;
            }
            static void DelEnt(int entity) // TODO Test
            {
                Popplio::Entity e = reg->GetEntityById(entity);
                reg->KillEntityRuntime(e); // queue to kill
                reg->RuntimeUpdate(e);
                //std::set<Popplio::Entity> entitiesToKill{};
                //entitiesToKill = reg->KillEntityRuntime(e, entitiesToKill);
                //reg->GetSystem<Popplio::LogicSystem>().KillEntity(e);
                //reg->Update();
//                lay->RemoveEntityFromCurrentLayer(e);
//                e.Kill();
//#ifndef IMGUI_DISABLE
//                reg->GetSystem<Popplio::Editor>().ResetSelectedEntity(e);
//#endif
            }

            static int GetEntCount()
            {
                return reg->GetNumEntities();
            }

            static bool GetActive(int instance) 
            { 
                if (!reg->EntityExists(instance))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | GetActive : Cannot find entity");
                    return false;
                }
                return reg->GetEntityById(instance).GetComponent<Popplio::ActiveComponent>().isActive; 
            }
            static void SetActive(int instance, bool value)
            {
                if (!reg->EntityExists(instance))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | SetActive : Cannot find entity");
                    return;
                }
                reg->GetEntityById(instance).GetComponent<Popplio::ActiveComponent>().isActive = value;
            }
            static bool HasRenderComponent(int instance)
            {
                if (!reg->EntityExists(instance))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | HasRenderComponent : Cannot find entity");
                    return false;
                }
                return reg->GetEntityById(instance).HasComponent<Popplio::RenderComponent>();
            }
            static bool HasRigidBodyComponent(int instance)
            {
                if (!reg->EntityExists(instance))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | HasRigidBodyComponent : Cannot find entity");
                    return false;
                }
                return reg->GetEntityById(instance).HasComponent<Popplio::RigidBodyComponent>();
            }
            static bool HasBoxColliderComponent(int entity)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | HasBoxColliderComponent : Cannot find entity");
                    return false;
                }
                return reg->GetEntityById(entity).HasComponent<Popplio::BoxColliderComponent>();
            }
            static bool HasTextComponent(int entity)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | HasTextComponent : Cannot find entity");
                    return false;
                }
                return reg->GetEntityById(entity).HasComponent<Popplio::TextComponent>();
            }
            static bool HasLogicComponent(int entity)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | HasLogicComponent : Cannot find entity " + std::to_string(entity));
                    return false;
                }
                return reg->GetEntityById(entity).HasComponent<Popplio::LogicComponent>();
            }
            static bool HasAnimationComponent(int instance)
            {
                if (!reg->EntityExists(instance))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | HasAnimationComponent : Cannot find entity");
                    return false;
                }
                return reg->GetEntityById(instance).HasComponent<Popplio::AnimationComponent>();
            }
            static bool HasAudioComponent(int entity)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | HasAudioComponent : Cannot find entity");
                    return false;
                }
                return reg->GetEntityById(entity).HasComponent<Popplio::AudioComponent>();
            }
            static bool HasParentComponent(int entity)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | HasParentComponent : Cannot find entity");
                    return false;
                }
                return reg->GetEntityById(entity).HasComponent<Popplio::ParentComponent>();
            }
            static bool HasUIComponent(int entity)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | HasUIComponent : Cannot find entity");
                    return false;
                }
                return reg->GetEntityById(entity).HasComponent<Popplio::UIComponent>();
            }
            //static bool HasPersistentComponent(int entity)
            //{
            //    return reg->GetEntityById(entity).HasComponent<Popplio::PersistOnLoadComponent>();
            //}

            static void AddRenderComponent(int entity)
            {
                if (reg->HasComponent<Popplio::RenderComponent>(Popplio::Entity(entity)))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | AddRenderComponent : RenderComponent already exists");
                    return;
                }
                reg->AddComponent<Popplio::RenderComponent>(Popplio::Entity(entity));
                
            }
            static void AddRigidBodyComponent(int entity)
            {
                if (reg->HasComponent<Popplio::RigidBodyComponent>(Popplio::Entity(entity)))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | AddRigidBodyComponent : RigidBodyComponent already exists");
                    return;
                }
                reg->AddComponent<Popplio::RigidBodyComponent>(Popplio::Entity(entity));
                
            }
            static void AddBoxColliderComponent(int entity)
            {
                if (reg->HasComponent<Popplio::BoxColliderComponent>(Popplio::Entity(entity)))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | AddBoxColliderComponent : BoxColliderComponent already exists");
                    return;
                }
                reg->AddComponent<Popplio::BoxColliderComponent>(Popplio::Entity(entity));
                
            }
            static void AddTextComponent(int entity)
            {
                if (reg->HasComponent<Popplio::TextComponent>(Popplio::Entity(entity)))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | AddTextComponent : TextComponent already exists");
                    return;
                }
                reg->AddComponent<Popplio::TextComponent>(Popplio::Entity(entity));
                
            }
            static void AddLogicComponent(int entity)
            {
                if (reg->HasComponent<Popplio::LogicComponent>(Popplio::Entity(entity)))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | AddLogicComponent : LogicComponent already exists");
                    return;
                }
                reg->AddComponent<Popplio::LogicComponent>(Popplio::Entity(entity));
                
            }
            static void AddAnimationComponent(int entity)
            {
                if (reg->HasComponent<Popplio::AnimationComponent>(Popplio::Entity(entity)))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | AddAnimationComponent : AnimationComponent already exists");
                    return;
                }
                reg->AddComponent<Popplio::AnimationComponent>(Popplio::Entity(entity));
                
            }
            static void AddAudioComponent(int entity)
            {
                if (reg->HasComponent<Popplio::AudioComponent>(Popplio::Entity(entity)))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | AddAudioComponent : AudioComponent already exists");
                    return;
                }
                reg->AddComponent<Popplio::AudioComponent>(Popplio::Entity(entity));
                
            }
            static void AddParentComponent(int entity)
            {
                if (reg->HasComponent<Popplio::ParentComponent>(Popplio::Entity(entity)))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | AddParentComponent : ParentComponent already exists");
                    return;
                }
                reg->AddComponent<Popplio::ParentComponent>(Popplio::Entity(entity));
                
            }
            static void AddUIComponent(int entity)
            {
                reg->AddComponent<Popplio::UIComponent>(Popplio::Entity(entity));

            }
            //static void AddPersistentComponent(int entity)
            //{
            //    reg->AddComponent<Popplio::PersistOnLoadComponent>(Popplio::Entity(entity));
            //}
        }
    }

    // Transformation/TransformComponent.h //

    namespace PopplioComponentTrans
    {
        extern "C" // Transform Component
        {
            static float GetPositionX(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::TransformComponent>().position.x;
            }
            static void SetPositionX(int instance, float value)
            {
                auto entity = reg->GetEntityById(instance);
                entity.GetComponent<Popplio::TransformComponent>().position.x = value;
                if (entity.HasComponent<Popplio::ParentComponent>() &&
                    entity.GetComponent<Popplio::ParentComponent>().parent.Exists() &&
                    entity.GetComponent<Popplio::ParentComponent>().inheritTransform)
                {
                    entity.GetComponent<Popplio::ParentComponent>().UpdateLocalTransform(
                        entity.GetComponent<Popplio::TransformComponent>(),
                        entity.GetComponent<Popplio::ParentComponent>().parent.GetComponent<Popplio::TransformComponent>());
                }
            }
            static float GetPositionY(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::TransformComponent>().position.y;
            }
            static void SetPositionY(int instance, float value)
            {
                auto entity = reg->GetEntityById(instance);
                entity.GetComponent<Popplio::TransformComponent>().position.y = value;
                if (entity.HasComponent<Popplio::ParentComponent>() &&
                    entity.GetComponent<Popplio::ParentComponent>().parent.Exists() &&
                    entity.GetComponent<Popplio::ParentComponent>().inheritTransform)
                {
                    entity.GetComponent<Popplio::ParentComponent>().UpdateLocalTransform(
                        entity.GetComponent<Popplio::TransformComponent>(),
                        entity.GetComponent<Popplio::ParentComponent>().parent.GetComponent<Popplio::TransformComponent>());
                }
            }
            static float GetScaleX(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::TransformComponent>().scale.x;
            }
            static void SetScaleX(int instance, float value)
            {
                auto entity = reg->GetEntityById(instance);
                entity.GetComponent<Popplio::TransformComponent>().scale.x = value;
                if (entity.HasComponent<Popplio::ParentComponent>() &&
                    entity.GetComponent<Popplio::ParentComponent>().parent.Exists() &&
                    entity.GetComponent<Popplio::ParentComponent>().inheritTransform)
                {
                    entity.GetComponent<Popplio::ParentComponent>().UpdateLocalTransform(
                        entity.GetComponent<Popplio::TransformComponent>(),
                        entity.GetComponent<Popplio::ParentComponent>().parent.GetComponent<Popplio::TransformComponent>());
                }
            }
            static float GetScaleY(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::TransformComponent>().scale.y;
            }
            static void SetScaleY(int instance, float value)
            {
                auto entity = reg->GetEntityById(instance);
                entity.GetComponent<Popplio::TransformComponent>().scale.y = value;
                if (entity.HasComponent<Popplio::ParentComponent>() &&
                    entity.GetComponent<Popplio::ParentComponent>().parent.Exists() &&
                    entity.GetComponent<Popplio::ParentComponent>().inheritTransform)
                {
                    entity.GetComponent<Popplio::ParentComponent>().UpdateLocalTransform(
                        entity.GetComponent<Popplio::TransformComponent>(),
                        entity.GetComponent<Popplio::ParentComponent>().parent.GetComponent<Popplio::TransformComponent>());
                }
            }
            static double GetRotation(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::TransformComponent>().rotation;
            }
            static void SetRotation(int instance, double value)
            {
                auto entity = reg->GetEntityById(instance);
                entity.GetComponent<Popplio::TransformComponent>().rotation = value;
                if (entity.HasComponent<Popplio::ParentComponent>() &&
                    entity.GetComponent<Popplio::ParentComponent>().parent.Exists() &&
                    entity.GetComponent<Popplio::ParentComponent>().inheritTransform)
                {
                    entity.GetComponent<Popplio::ParentComponent>().UpdateLocalTransform(
                        entity.GetComponent<Popplio::TransformComponent>(),
                        entity.GetComponent<Popplio::ParentComponent>().parent.GetComponent<Popplio::TransformComponent>());
                }
            }
        }
    }

    // Graphic/GraphicComponent.h //

    namespace PopplioComponentRender
    {
        extern "C" // Render Component
        {
            static float GetColorR(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::RenderComponent>().color.x;
            }
            static void SetColorR(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::RenderComponent>().color.x = value;
            }
            static float GetColorG(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::RenderComponent>().color.y;
            }
            static void SetColorG(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::RenderComponent>().color.y = value;
            }
            static float GetColorB(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::RenderComponent>().color.z;
            }
            static void SetColorB(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::RenderComponent>().color.z = value;
            }
            static float GetColorA(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::RenderComponent>().alpha;
            }
            static void SetColorA(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::RenderComponent>().alpha = value;
            }
            static int GetMeshType(int instance)
            {
                return static_cast<int>(reg->GetEntityById(instance).GetComponent<Popplio::RenderComponent>().meshType);
            }
            static void SetMeshType(int instance, int value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::RenderComponent>().meshType = static_cast<Popplio::MeshType>(value);
            }
            static bool GetActive(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::RenderComponent>().isActive;
            }
            static void SetActive(int instance, bool value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::RenderComponent>().isActive = value;
            }
            static MonoString* GetShader(int instance)
            {
                return StringToMono(reg->GetEntityById(instance).GetComponent<Popplio::RenderComponent>().shaderName);
            }
            static void SetShader(int instance, MonoString* value)
            {
                PopplioScriptAPI::MonoToString(value, strMS);
                reg->GetEntityById(instance).GetComponent<Popplio::RenderComponent>().shaderName = strMS;
            }
            static MonoString* GetTexture(int instance)
            {
                return StringToMono(reg->GetEntityById(instance).GetComponent<Popplio::RenderComponent>().textureName);
            }
            static void SetTexture(int instance, MonoString* value)
            {
                PopplioScriptAPI::MonoToString(value, strMS);
                reg->GetEntityById(instance).GetComponent<Popplio::RenderComponent>().textureName = strMS;
            }
            static bool GetInstancing(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::RenderComponent>().useInstancing;
            }
            static void SetInstancing(int instance, bool value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::RenderComponent>().useInstancing = value;
            }
        }
    }

    // Physics/RigidBodyComponent.h //

    namespace PopplioComponentRB
    {
        extern "C"
        {
            static float GetVelocityX(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::RigidBodyComponent>().velocity.x;
            }
            static void SetVelocityX(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::RigidBodyComponent>().velocity.x = value;
            }
            static float GetVelocityY(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::RigidBodyComponent>().velocity.y;
            }
            static void SetVelocityY(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::RigidBodyComponent>().velocity.y = value;
            }

            static float GetAccelerationX(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::RigidBodyComponent>().acceleration.x;
            }
            static void SetAccelerationX(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::RigidBodyComponent>().acceleration.x = value;
            }
            static float GetAccelerationY(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::RigidBodyComponent>().acceleration.y;
            }
            static void SetAccelerationY(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::RigidBodyComponent>().acceleration.y = value;
            }

            static float GetForceX(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::RigidBodyComponent>().force.x;
            }
            static void SetForceX(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::RigidBodyComponent>().force.x = value;
            }
            static float GetForceY(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::RigidBodyComponent>().force.y;
            }
            static void SetForceY(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::RigidBodyComponent>().force.y = value;
            }

            static bool GetActive(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::RigidBodyComponent>().isActive;
            }
            static void SetActive(int instance, bool value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::RigidBodyComponent>().isActive = value;
            }

            static float GetMass(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::RigidBodyComponent>().mass;
            }
            static void SetMass(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::RigidBodyComponent>().mass = value;
            }
            static float GetGravityScale(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::RigidBodyComponent>().gravityScale;
            }
            static void SetGravityScale(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::RigidBodyComponent>().gravityScale = value;
            }
            static bool GetUseGravity(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::RigidBodyComponent>().useGravity;
            }
            static void SetUseGravity(int instance, bool value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::RigidBodyComponent>().useGravity = value;
            }
            static float GetDragCoefficient(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::RigidBodyComponent>().dragCoefficient;
            }
            static void SetDragCoefficient(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::RigidBodyComponent>().dragCoefficient = value;
            }
        }
    }

    // Collision/BoxColliderComponent.h //

    namespace PopplioComponentBoxCollider
    {
        extern "C"
        {
            static float GetSizeX(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::BoxColliderComponent>().size.x;
            }
            static void SetSizeX(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::BoxColliderComponent>().size.x = value;
            }
            static float GetSizeY(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::BoxColliderComponent>().size.y;
            }
            static void SetSizeY(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::BoxColliderComponent>().size.y = value;
            }
            static float GetOffsetX(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::BoxColliderComponent>().offset.x;
            }
            static void SetOffsetX(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::BoxColliderComponent>().offset.x = value;
            }
            static float GetOffsetY(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::BoxColliderComponent>().offset.y;
            }
            static void SetOffsetY(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::BoxColliderComponent>().offset.y = value;
            }
            static bool GetActive(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::BoxColliderComponent>().isActive;
            }
            static void SetActive(int instance, bool value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::BoxColliderComponent>().isActive = value;
            }
            static bool GetTrigger(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::BoxColliderComponent>().isTrigger;
            }
            static void SetTrigger(int instance, bool value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::BoxColliderComponent>().isTrigger = value;
            }
            static bool GetEnabled(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::BoxColliderComponent>().isEnabled;
            }
            static void SetEnabled(int instance, bool value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::BoxColliderComponent>().isEnabled = value;
            }
        }
    }

    // Graphic/GraphicComponent.h // Text Component

    namespace PopplioComponentText
    {
        extern "C"
        {
            static float GetColorR(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::TextComponent>().color.x;
            }
            static void SetColorR(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::TextComponent>().color.x = value;
            }
            static float GetColorG(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::TextComponent>().color.y;
            }
            static void SetColorG(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::TextComponent>().color.y = value;
            }
            static float GetColorB(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::TextComponent>().color.z;
            }
            static void SetColorB(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::TextComponent>().color.z = value;
            }
            static float GetColorA(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::TextComponent>().alpha;
            }
            static void SetColorA(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::TextComponent>().alpha = value;
            }

            static bool GetActive(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::TextComponent>().isActive;
            }
            static void SetActive(int instance, bool value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::TextComponent>().isActive = value;
            }
            static MonoString* GetText(int instance)
            {
                return StringToMono(reg->GetEntityById(instance).GetComponent<Popplio::TextComponent>().text);
            }
            static void SetText(int instance, MonoString* value)
            {
                MonoToString(value, strMS);
                reg->GetEntityById(instance).GetComponent<Popplio::TextComponent>().text = strMS;
            }
            static MonoString* GetFontName(int instance)
            {
                return StringToMono(reg->GetEntityById(instance).GetComponent<Popplio::TextComponent>().fontName);
            }
            static void SetFontName(int instance, MonoString* value)
            {
                MonoToString(value, strMS);
                reg->GetEntityById(instance).GetComponent<Popplio::TextComponent>().fontName = strMS;
            }
            static unsigned int GetFontSize(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::TextComponent>().fontSize;
            }
            static void SetFontSize(int instance, unsigned int value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::TextComponent>().fontSize = value;
            }
            static float GetWidth(int instance)
            {
                auto& textComp = reg->GetEntityById(instance).GetComponent<Popplio::TextComponent>();
                return textComp.GetWidth();
            }
            static float GetHeight(int instance)
            {
                auto& textComp = reg->GetEntityById(instance).GetComponent<Popplio::TextComponent>();
                return textComp.GetHeight();
            }

        }
    }

    // Script/LogicComponent.h //

    namespace PopplioComponentLogic
    {
        extern "C"
        {
            static bool GetActive(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::LogicComponent>().isActive;
            }
            static void SetActive(int instance, bool value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::LogicComponent>().isActive = value;
            }
            static MonoString* GetScriptName(int instance)
            {
                return StringToMono(reg->GetEntityById(instance).GetComponent<Popplio::LogicComponent>().scriptName);
            }
            static void SetScriptName(int instance, MonoString* value)
            {
                MonoToString(value, strMS);
                reg->GetEntityById(instance).GetComponent<Popplio::LogicComponent>().scriptName = strMS;
            }
            static int GetScriptLanguage(int instance)
            {
                return static_cast<int>(reg->GetEntityById(instance).GetComponent<Popplio::LogicComponent>().scriptLang);
            }
            static void SetScriptLanguage(int instance, int value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::LogicComponent>().scriptLang = 
                    static_cast<Popplio::ScriptLang>(value);
            }

            static void SetScriptObj(int instance, MonoObject* scriptObj)
            {
                static_cast<void>(instance);
                static_cast<void>(scriptObj);

                Popplio::Logger::Warning("MonoInternalCall.cpp | SetScriptObj : Not implemented");

                return;

                //reg->GetEntityById(instance).GetComponent<Popplio::LogicComponent>().logicSystem = 
                //    &reg->GetSystem<Popplio::LogicSystem>();
                //reg->GetEntityById(instance).GetComponent<Popplio::LogicComponent>().scriptObject = scriptObj;

                //reg->GetSystem<Popplio::LogicSystem>().monoAPI.AddGC(scriptObj);

                //reg->GetSystem<Popplio::LogicSystem>().CSAddToLoadedScripts(instance);
            }
        }
    }

    // Animation/AnimationComponent.h //

    namespace PopplioComponentAnim
    {
        extern "C" // AnimationComponent
        {
            static int GetAnimStartX(int instance, MonoString* id)
            {
                PopplioScriptAPI::MonoToString(id, strMS);

                return reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().animations[strMS].startX;
            }

            static void SetAnimStartX(int instance, MonoString* id, int value)
            {
                PopplioScriptAPI::MonoToString(id, strMS);
                
                reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().animations[strMS].startX = value;
                
            }


            static int GetAnimStartY(int instance, MonoString* id)
            {
                PopplioScriptAPI::MonoToString(id, strMS);

                return reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().animations[strMS].startY;
            }

            static void SetAnimStartY(int instance, MonoString* id, int value)
            {
                PopplioScriptAPI::MonoToString(id, strMS);
                
                reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().animations[strMS].startY = value;
                
            }


            static int GetAnimEndX(int instance, MonoString* id)
            {
                PopplioScriptAPI::MonoToString(id, strMS);

                return reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().animations[strMS].endX;
            }

            static void SetAnimEndX(int instance, MonoString* id, int value)
            {
                PopplioScriptAPI::MonoToString(id, strMS);
                
                reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().animations[strMS].endX = value;
                
            }


            static int GetAnimEndY(int instance, MonoString* id)
            {
                PopplioScriptAPI::MonoToString(id, strMS);

                return reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().animations[strMS].endY;
            }

            static void SetAnimEndY(int instance, MonoString* id, int value)
            {
                PopplioScriptAPI::MonoToString(id, strMS);
                
                reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().animations[strMS].endY = value;
                
            }


            static float GetAnimSpeed(int instance, MonoString* id)
            {
                PopplioScriptAPI::MonoToString(id, strMS);
                return reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().animations[strMS].animationSpeed;
            }

            static void SetAnimSpeed(int instance, MonoString* id, float value)
            {
                PopplioScriptAPI::MonoToString(id, strMS);
                reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().animations[strMS].animationSpeed = value;
            }


            static MonoString* GetAnimName(int instance, MonoString* id)
            {
                PopplioScriptAPI::MonoToString(id, strMS);
                return StringToMono(reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().animations[strMS].name);
            }

            static void SetAnimName(int instance, MonoString* id, MonoString* value)
            {
                PopplioScriptAPI::MonoToString(id, strMS);
                reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().animations[strMS].name = 
                    MonoToString(value);
            }


            static float GetAnimFrameWidth(int instance, MonoString* id)
            {
                PopplioScriptAPI::MonoToString(id, strMS);
                return reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().animations[strMS].frameWidth;
            }

            static void SetAnimFrameWidth(int instance, MonoString* id, float value)
            {
                PopplioScriptAPI::MonoToString(id, strMS);
                reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().animations[strMS].frameWidth = value;
            }


            static float GetAnimFrameHeight(int instance, MonoString* id)
            {
                PopplioScriptAPI::MonoToString(id, strMS);
                return reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().animations[strMS].frameHeight;
            }

            static void SetAnimFrameHeight(int instance, MonoString* id, float value)
            {
                PopplioScriptAPI::MonoToString(id, strMS);
                reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().animations[strMS].frameHeight = value;
            }

            // +++++++++++++++++++++++++++++ //

            static bool GetActive(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().isActive;
            }

            static void SetActive(int instance, bool value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().isActive = value;
            }


            static MonoString* GetCurrentAnim(int instance)
            {
                return StringToMono(reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().currentAnimation);
            }

            static void SetAnimation(int instance, MonoString* value, bool playOnce)
            {
                PopplioScriptAPI::MonoToString(value, strMS);
                reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().SetAnimation(strMS, playOnce);
            }


            static float GetSheetWidth(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().sheetWidth;
            }

            static void SetSheetWidth(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().sheetWidth = value;
            }


            static float GetSheetHeight(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().sheetHeight;
            }

            static void SetSheetHeight(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().sheetHeight = value;
            }


            static float GetTimeElapsedCurrentCycle(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().elapsedTimeCurrentCycle;
            }

            static float GetTimeElapsedSinceLastResume(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().elapsedTimeSinceLastResume;
            }

            static void SetTimeElapsed(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().timeAccumulator = value;
            }

            static void SetFrameX(int instance, int value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().currentFrameX = value;
            }

            static int GetFrameX(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().currentFrameX;
            }

            static void SetFrameY(int instance, int value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().currentFrameY = value;
            }

            static int GetFrameY(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().currentFrameY;
            }


            static bool GetPaused(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().isPaused;
            }

            static bool GetPlaying(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().isPlaying;
            }

            static bool GetPlayOnce(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().playOnce;
            }

            static void SetPlayOnce(int instance, bool value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().playOnce = value;
            }

            static void Play(int instance)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().Play();
            }

            static void Pause(int instance)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().Pause();
            }

            static void Stop(int instance)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().Stop();
            }

            static void Resume(int instance)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().Resume();
            }
            
            static MonoArray* GetAnimations(int instance)
            {
                auto& anim = reg->GetEntityById(instance).GetComponent<Popplio::AnimationComponent>().animations;

                std::vector<std::string> animNames{};

                for (auto& [key, value] : anim)
                {
                    animNames.push_back(key);
                }

                return ArrayToMono1D<std::string>(animNames, &reg->GetSystem<Popplio::LogicSystem>().monoAPI.GetAppDomain());
            }
        }
    }

    // Audio/AudioComponent.h //

    namespace PopplioComponentAudio
    {
        extern "C"
        {
            static bool GetActive(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().isActive;
            }
            static void SetActive(int instance, bool value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().isActive = value;
            }
            static MonoString* GetName(int instance)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().Sync(reg->GetSystem<Popplio::AudioSystem>());
                return StringToMono(reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().audio);
            }
            static void SetName(int instance, MonoString* value)
            {
                MonoToString(value, strMS);
                reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().audio = strMS;
                reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().Update(reg->GetSystem<Popplio::AudioSystem>());
            }
            static float GetVolume(int instance)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().Sync(reg->GetSystem<Popplio::AudioSystem>());
                return reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().volume;
            }
            static void SetVolume(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().volume = value;
                reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().Update(reg->GetSystem<Popplio::AudioSystem>());
            }
            static bool GetLoop(int instance)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().Sync(reg->GetSystem<Popplio::AudioSystem>());
                return reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().loop;
            }
            static void SetLoop(int instance, bool value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().loop = value;
                reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().Update(reg->GetSystem<Popplio::AudioSystem>());
            }
            static float GetBPM(int instance)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().Sync(reg->GetSystem<Popplio::AudioSystem>());
                return reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().bpm;
            }
            static void SetBPM(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().bpm = value;
                reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().Update(reg->GetSystem<Popplio::AudioSystem>());
            }
            static int GetTimeSignatureTop(int instance)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().Sync(reg->GetSystem<Popplio::AudioSystem>());
                return reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().timeSig.first;
            }
            static int GetTimeSignatureBottom(int instance)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().Sync(reg->GetSystem<Popplio::AudioSystem>());
                return reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().timeSig.second;
            }
            static void SetTimeSignature(int instance, int top, int bottom)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().timeSig = { top, bottom };
                reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().Update(reg->GetSystem<Popplio::AudioSystem>());
            }
            static bool GetPlayOnAwake(int instance)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().Sync(reg->GetSystem<Popplio::AudioSystem>());
                return reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().playOnAwake;
            }
            static void SetPlayOnAwake(int instance, bool value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().playOnAwake = value;
                reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().Update(reg->GetSystem<Popplio::AudioSystem>());
            }
            static int GetChannelGroup(int instance)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().Sync(reg->GetSystem<Popplio::AudioSystem>());
                return reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().grp;
            }
            static void SetChannelGroup(int instance, int value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().grp = value;
                reg->GetEntityById(instance).GetComponent<Popplio::AudioComponent>().Update(reg->GetSystem<Popplio::AudioSystem>());
            }
        }
    }

    // Hierarchy/ParentComponent.h //

    namespace PopplioComponentParent
    {
        extern "C" // Parent Component
        {
            static int GetParentNotStupidWindows(int entity)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | GetParentNotStupidWindows : Entity does not exist");
                    return -1;
                }

                if (!reg->GetEntityById(entity).HasComponent<Popplio::ParentComponent>())
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | GetParentNotStupidWindows : Entity does not have a ParentComponent");
                    return -1;
                }

                return reg->GetEntityById(entity).GetComponent<Popplio::ParentComponent>().parent.GetId();
            }
            static void SetParentNotStupidWindows(int entity, int parent)
            {
                if (!reg->EntityExists(entity) || (!reg->EntityExists(parent) && parent != -1))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | SetParentNotStupidWindows : Entity / Parent does not exist");
                    return;
                }

                if (parent == -1) // set entity to root
                {
                    if (reg->GetEntityById(entity).HasComponent<Popplio::ParentComponent>())
                    {
                        if (reg->GetEntityById(entity).GetComponent<Popplio::ParentComponent>().parent.GetId() != -1)
                        {
                            reg->GetEntityById(entity).GetComponent<Popplio::ParentComponent>().parent.
                                GetComponent<Popplio::ParentComponent>().RemoveChild(reg->GetEntityById(entity));

                            return;
                        }
                    }

                    return;
                }

                reg->GetSystem<Popplio::HierarchySystem>().AddChild(reg->GetEntityById(parent), reg->GetEntityById(entity));
            }
            static void AddChild(int entity, int child)
            {
                if (!reg->EntityExists(entity) || !reg->EntityExists(child))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | AddChild : Entity / Child does not exist");
                    return;
                }
                reg->GetSystem<Popplio::HierarchySystem>().AddChild(reg->GetEntityById(entity), reg->GetEntityById(child));
                
            }
            static void RemoveChild(int entity, int child)
            {
                if (!reg->EntityExists(entity) || !reg->EntityExists(child))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | RemoveChild : Entity / Child does not exist");
                    return;
                }
                reg->GetSystem<Popplio::HierarchySystem>().RemoveChild(reg->GetEntityById(entity), reg->GetEntityById(child));
                
            }
            static MonoArray* GetChildren(int entity)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | GetChildren : Entity does not exist");
                    return nullptr;
                }

                if (!reg->GetEntityById(entity).HasComponent<Popplio::ParentComponent>())
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | GetChildren : Entity does not have a ParentComponent");
                    return nullptr;
                }

                std::vector<int> children{};
                for (auto& e : reg->GetEntityById(entity).GetComponent<Popplio::ParentComponent>().children)
                {
                    children.push_back(e.GetId());
                }

                return ArrayToMono1D<int>(children, &reg->GetSystem<Popplio::LogicSystem>().monoAPI.GetAppDomain());
            }

            static bool GetInheritActive(int entity)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | GetInheritActive : Entity does not exist");
                    return false;
                }

                if (!reg->GetEntityById(entity).HasComponent<Popplio::ParentComponent>())
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | GetInheritActive : Entity does not have a ParentComponent");
                    return false;
                }

                return reg->GetEntityById(entity).GetComponent<Popplio::ParentComponent>().inheritActive;
            }
            static void SetInheritActive(int entity, bool value)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | SetInheritActive : Entity does not exist");
                    return;
                }

                if (!reg->GetEntityById(entity).HasComponent<Popplio::ParentComponent>())
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | SetInheritActive : Entity does not have a ParentComponent");
                    return;
                }

                reg->GetEntityById(entity).GetComponent<Popplio::ParentComponent>().inheritActive = value;
            }
            static bool GetInheritTransform(int entity)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | GetInheritTransform : Entity does not exist");
                    return false;
                }

                if (!reg->GetEntityById(entity).HasComponent<Popplio::ParentComponent>())
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | GetInheritTransform : Entity does not have a ParentComponent");
                    return false;
                }

                return reg->GetEntityById(entity).GetComponent<Popplio::ParentComponent>().inheritTransform;
            }
            static void SetInheritTransform(int entity, bool value)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | SetInheritTransform : Entity does not exist");
                    return;
                }

                if (!reg->GetEntityById(entity).HasComponent<Popplio::ParentComponent>())
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | SetInheritTransform : Entity does not have a ParentComponent");
                    return;
                }

                reg->GetEntityById(entity).GetComponent<Popplio::ParentComponent>().inheritTransform = value;
            }
            static float GetLocalPosX(int entity)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | GetLocalPosX : Entity does not exist");
                    return 0.f;
                }

                if (!reg->GetEntityById(entity).HasComponent<Popplio::ParentComponent>())
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | GetLocalPosX : Entity does not have a ParentComponent");
                    return 0.f;
                }

                return reg->GetEntityById(entity).GetComponent<Popplio::ParentComponent>().localPosition.x;
            }
            static void SetLocalPosX(int entity, float value)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | SetLocalPosX : Entity does not exist");
                    return;
                }

                auto e = reg->GetEntityById(entity);
                
                if (!e.HasComponent<Popplio::ParentComponent>() ||
                    !e.GetComponent<Popplio::ParentComponent>().parent.Exists() ||
                    !e.GetComponent<Popplio::ParentComponent>().inheritTransform)
                {
                    e.GetComponent<Popplio::TransformComponent>().position.x = value;
                }
                else
                {
                    //e.GetComponent<Popplio::TransformComponent>().position.x = value;

                    ::PopplioMath::Vec2f pos = e.GetComponent<Popplio::ParentComponent>().localPosition;
                    pos.x = value;
                    e.GetComponent<Popplio::ParentComponent>().SetLocalPos(pos,
                        e.GetComponent<Popplio::TransformComponent>(),
                        e.GetComponent<Popplio::ParentComponent>().parent.GetComponent<Popplio::TransformComponent>(), 
                        e.GetComponent<Popplio::ParentComponent>().parent.GetComponent<Popplio::ParentComponent>());
                    
                    e.GetComponent<Popplio::ParentComponent>().localModified = true;

                    //e.GetComponent<Popplio::ParentComponent>().UpdateLocalTransform(
                    //    e.GetComponent<Popplio::TransformComponent>(),
                    //    e.GetComponent<Popplio::ParentComponent>().parent.GetComponent<Popplio::TransformComponent>());

                    //e.GetComponent<Popplio::ParentComponent>().localPosition.x = value;
                    //e.GetComponent<Popplio::ParentComponent>().UpdateWorldTransform(
                    //    e.GetComponent<Popplio::TransformComponent>(),
                    //    e.GetComponent<Popplio::ParentComponent>().parent.GetComponent<Popplio::TransformComponent>());
                }
            }
            static float GetLocalPosY(int entity)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | GetLocalPosY : Entity does not exist");
                    return 0.f;
                }

                if (!reg->GetEntityById(entity).HasComponent<Popplio::ParentComponent>())
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | GetLocalPosY : Entity does not have a ParentComponent");
                    return 0.f;
                }

                return reg->GetEntityById(entity).GetComponent<Popplio::ParentComponent>().localPosition.y;
            }
            static void SetLocalPosY(int entity, float value)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | SetLocalPosY : Entity does not exist");
                    return;
                }

                auto e = reg->GetEntityById(entity);

                if (!e.HasComponent<Popplio::ParentComponent>() ||
                    !e.GetComponent<Popplio::ParentComponent>().parent.Exists() ||
                    !e.GetComponent<Popplio::ParentComponent>().inheritTransform)
                {
                    e.GetComponent<Popplio::TransformComponent>().position.y = value;
                }
                else
                {
                    ::PopplioMath::Vec2f pos = e.GetComponent<Popplio::ParentComponent>().localPosition;
                    pos.y = value;
                    e.GetComponent<Popplio::ParentComponent>().SetLocalPos(pos,
                        e.GetComponent<Popplio::TransformComponent>(),
                        e.GetComponent<Popplio::ParentComponent>().parent.GetComponent<Popplio::TransformComponent>(),
                        e.GetComponent<Popplio::ParentComponent>().parent.GetComponent<Popplio::ParentComponent>());

                    e.GetComponent<Popplio::ParentComponent>().localModified = true;

                    //e.GetComponent<Popplio::ParentComponent>().UpdateLocalTransform(
                    //    e.GetComponent<Popplio::TransformComponent>(),
                    //    e.GetComponent<Popplio::ParentComponent>().parent.GetComponent<Popplio::TransformComponent>());
                }
            }
            static float GetLocalScaleX(int entity)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | GetLocalScaleX : Entity does not exist");
                    return 0.f;
                }

                if (!reg->GetEntityById(entity).HasComponent<Popplio::ParentComponent>())
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | GetLocalScaleX : Entity does not have a ParentComponent");
                    return 0.f;
                }

                return reg->GetEntityById(entity).GetComponent<Popplio::ParentComponent>().localScale.x;
            }
            static void SetLocalScaleX(int entity, float value)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | SetLocalScaleX : Entity does not exist");
                    return;
                }

                auto e = reg->GetEntityById(entity);

                if (!e.HasComponent<Popplio::ParentComponent>() ||
                    !e.GetComponent<Popplio::ParentComponent>().parent.Exists() ||
                    !e.GetComponent<Popplio::ParentComponent>().inheritTransform)
                {
                    e.GetComponent<Popplio::TransformComponent>().scale.x = value;
                }
                else
                {
                    ::PopplioMath::Vec2f scale = e.GetComponent<Popplio::ParentComponent>().localScale;
                    scale.x = value;
                    e.GetComponent<Popplio::ParentComponent>().SetLocalScale(scale,
                        e.GetComponent<Popplio::TransformComponent>(),
                        e.GetComponent<Popplio::ParentComponent>().parent.GetComponent<Popplio::TransformComponent>());

                    e.GetComponent<Popplio::ParentComponent>().localModified = true;

                    //e.GetComponent<Popplio::ParentComponent>().UpdateLocalTransform(
                    //    e.GetComponent<Popplio::TransformComponent>(),
                    //    e.GetComponent<Popplio::ParentComponent>().parent.GetComponent<Popplio::TransformComponent>());
                }
            }
            static float GetLocalScaleY(int entity)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | GetLocalScaleY : Entity does not exist");
                    return 0.f;
                }

                if (!reg->GetEntityById(entity).HasComponent<Popplio::ParentComponent>())
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | GetLocalScaleY : Entity does not have a ParentComponent");
                    return 0.f;
                }

                return reg->GetEntityById(entity).GetComponent<Popplio::ParentComponent>().localScale.y;
            }
            static void SetLocalScaleY(int entity, float value)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | SetLocalScaleY : Entity does not exist");
                    return;
                }

                auto e = reg->GetEntityById(entity);

                if (!e.HasComponent<Popplio::ParentComponent>() ||
                    !e.GetComponent<Popplio::ParentComponent>().parent.Exists() ||
                    !e.GetComponent<Popplio::ParentComponent>().inheritTransform)
                {
                    e.GetComponent<Popplio::TransformComponent>().scale.y = value;
                }
                else
                {
                    ::PopplioMath::Vec2f scale = e.GetComponent<Popplio::ParentComponent>().localScale;
                    scale.y = value;
                    e.GetComponent<Popplio::ParentComponent>().SetLocalScale(scale,
                        e.GetComponent<Popplio::TransformComponent>(),
                        e.GetComponent<Popplio::ParentComponent>().parent.GetComponent<Popplio::TransformComponent>());

                    e.GetComponent<Popplio::ParentComponent>().localModified = true;

                    //e.GetComponent<Popplio::ParentComponent>().UpdateLocalTransform(
                    //    e.GetComponent<Popplio::TransformComponent>(),
                    //    e.GetComponent<Popplio::ParentComponent>().parent.GetComponent<Popplio::TransformComponent>());
                }
            }
            static double GetLocalRotation(int entity)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | GetLocalRotation : Entity does not exist");
                    return 0.0;
                }

                if (!reg->GetEntityById(entity).HasComponent<Popplio::ParentComponent>())
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | GetLocalRotation : Entity does not have a ParentComponent");
                    return 0.0;
                }

                return reg->GetEntityById(entity).GetComponent<Popplio::ParentComponent>().localRotation;
            }
            static void SetLocalRotation(int entity, double value)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | SetLocalRotation : Entity does not exist");
                    return;
                }

                auto e = reg->GetEntityById(entity);

                if (!e.HasComponent<Popplio::ParentComponent>() ||
                    !e.GetComponent<Popplio::ParentComponent>().parent.Exists() ||
                    !e.GetComponent<Popplio::ParentComponent>().inheritTransform)
                {
                    e.GetComponent<Popplio::TransformComponent>().rotation = value;
                }
                else
                {
                    e.GetComponent<Popplio::ParentComponent>().SetLocalRot(value,
                        e.GetComponent<Popplio::TransformComponent>(),
                        e.GetComponent<Popplio::ParentComponent>().parent.GetComponent<Popplio::TransformComponent>());

                    e.GetComponent<Popplio::ParentComponent>().localModified = true;

                    //e.GetComponent<Popplio::ParentComponent>().UpdateLocalTransform(
                    //    e.GetComponent<Popplio::TransformComponent>(),
                    //    e.GetComponent<Popplio::ParentComponent>().parent.GetComponent<Popplio::TransformComponent>());
                }
            }

            static void UpdateLocalTransform(int entity)
            {
                static_cast<void>(entity);

                //auto e = reg->GetEntityById(entity);

                //e.GetComponent<Popplio::ParentComponent>()
                
                //e.GetComponent<Popplio::ParentComponent>().UpdateLocalTransform(
                //    e.GetComponent<Popplio::TransformComponent>(),
                //    e.GetComponent<Popplio::ParentComponent>().parent.GetComponent<Popplio::TransformComponent>());
            }
        }
    }

    // Scene/PersistOnLoadComponent.h //
    namespace PopplioComponentPersist
    {
        extern "C"
        {
            static bool GetPersist(int entity)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | GetPersist : Entity does not exist");
                    return false;
                }
                if (!reg->GetEntityById(entity).HasComponent<Popplio::PersistOnLoadComponent>()) return false;

                return reg->GetEntityById(entity).GetComponent<Popplio::PersistOnLoadComponent>().persistOnLoad;
            }
            static void SetPersist(int entity, bool value)
            {
                if (!reg->EntityExists(entity))
                {
                    Popplio::Logger::Error("MonoInternalCall.cpp | SetPersist : Entity does not exist");
                    return;
                }

                if (!reg->GetEntityById(entity).HasComponent<Popplio::PersistOnLoadComponent>())
                {
                    reg->AddComponent<Popplio::PersistOnLoadComponent>(reg->GetEntityById(entity));
                    
                }

                reg->GetEntityById(entity).GetComponent<Popplio::PersistOnLoadComponent>().persistOnLoad = value;
            }
        }
    }

    // PrefabManagement/PrefabManager.h //
    namespace PopplioPrefab
    {
        extern "C"
        {
            static bool HasPrefab(MonoString* name)
            {
                MonoToString(name, strMS);
                return prefab->PrefabExists(strMS);
            }
            static int InstantiatePrefab(MonoString* name)
            {
                MonoToString(name, strMS);
                std::optional<Popplio::Entity> ent = prefab->InstantiatePrefab(*reg, strMS);
                //if (ent != std::nullopt) reg->RuntimeUpdate(ent.value());
                if (ent == std::nullopt) return -1;
                else return ent->GetId();
            }
        }
    }

    // UIComponent.h //
    namespace PopplioComponentUI
    {
        extern "C"
        {
            static int GetUIType(int instance)
            {
                return static_cast<int>(reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().type);
            }

            static void SetUIType(int instance, int value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().type = static_cast<Popplio::UIType>(value);
            }

            static bool GetActive(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().isActive;
            }

            static void SetActive(int instance, bool value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().isActive = value;
            }

            static bool GetChecked(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().checked;
            }

            static void SetChecked(int instance, bool value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().checked = value;
            }

            static float GetSliderValue(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().sliderValue;
            }

            static void SetSliderValue(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().sliderValue = value;
            }

            static void AddOnClickAction(int instance, MonoString* action)
            {
                std::string actionStr = PopplioScriptAPI::MonoToString(action);
                auto& uiComponent = reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>();
                uiComponent.onClickActions.push_back(actionStr);
                Popplio::Logger::Info("[MonoInternalCall] Added OnClick Action: " + actionStr);
                Popplio::Logger::Info("[MonoInternalCall] Current Actions Count: " + std::to_string(uiComponent.onClickActions.size()));
            }

            static void RemoveOnClickAction(int instance, MonoString* action)
            {
                std::string actionStr = PopplioScriptAPI::MonoToString(action);
                auto& uiComponent = reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>();

                auto& actions = uiComponent.onClickActions;
                actions.erase(std::remove(actions.begin(), actions.end(), actionStr), actions.end());
            }

            static int GetLinkedEntity(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().linkedEntityId;
            }

            static void SetLinkedEntity(int instance, int value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().linkedEntityId = value;
            }

            static MonoArray* GetOnClickActions(int instance)
            {
                if (!reg->GetEntityById(instance).HasComponent<Popplio::UIComponent>())
                {
                    Popplio::Logger::Error("[MonoInternalCall] Entity does not have a UIComponent!");
                    return nullptr;
                }

                auto& uiComponent = reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>();
                const auto& actions = uiComponent.onClickActions;

                Popplio::Logger::Info("[MonoInternalCall] GetOnClickActions Count: " + std::to_string(actions.size()));

                MonoDomain* domain = mono_domain_get();
                MonoArray* monoArray = mono_array_new(domain, mono_get_string_class(), static_cast<int>(actions.size()));

                for (size_t i = 0; i < actions.size(); i++)
                {
                    mono_array_set(monoArray, MonoString*, static_cast<int>(i), PopplioScriptAPI::StringToMono(actions[i]));
                }

                return monoArray;
            }

            static void SetOnClickActions(int instance, MonoArray* actions)
            {
                if (!reg->GetEntityById(instance).HasComponent<Popplio::UIComponent>())
                {
                    Popplio::Logger::Error("[MonoInternalCall] Entity does not have a UIComponent!");
                    return;
                }

                auto& uiComponent = reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>();

                Popplio::Logger::Info("[MonoInternalCall] Before Clearing: " + std::to_string(uiComponent.onClickActions.size()));

                // Clear existing actions
                uiComponent.onClickActions.clear();

                if (actions == nullptr)
                {
                    Popplio::Logger::Error("[MonoInternalCall] SetOnClickActions received a null array.");
                    return;
                }

                size_t actionCount = mono_array_length(actions);
                for (size_t i = 0; i < actionCount; i++)
                {
                    MonoString* monoStr = mono_array_get(actions, MonoString*, static_cast<int>(i));
                    std::string actionStr = PopplioScriptAPI::MonoToString(monoStr);
                    uiComponent.onClickActions.push_back(actionStr);
                }

                Popplio::Logger::Info("[MonoInternalCall] After Setting Actions: " + std::to_string(uiComponent.onClickActions.size()));

            }

            static bool GetHasHoverEffect(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().hasHoverEffect;
            }

            static void SetHasHoverEffect(int instance, bool value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().hasHoverEffect = value;
            }

            static bool GetIsHovered(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().isHovered;
            }

            static void SetIsHovered(int instance, bool value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().isHovered = value;
            }

            static int GetFrameX(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().frameX;
            }

            static void SetFrameX(int instance, int value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().frameX = value;
            }

            static int GetFrameY(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().frameY;
            }

            static void SetFrameY(int instance, int value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().frameY = value;
            }

            static int GetHoverFrameX(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().hoverFrameX;
            }

            static void SetHoverFrameX(int instance, int value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().hoverFrameX = value;
            }

            static int GetHoverFrameY(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().hoverFrameY;
            }

            static void SetHoverFrameY(int instance, int value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().hoverFrameY = value;
            }

            static int GetTotalColumns(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().totalColumns;
            }

            static void SetTotalColumns(int instance, int value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().totalColumns = value;
            }

            static int GetTotalRows(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().totalRows;
            }

            static void SetTotalRows(int instance, int value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().totalRows = value;
            }

            static float GetSheetWidth(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().sheetWidth;
            }

            static void SetSheetWidth(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().sheetWidth = value;
            }

            static float GetSheetHeight(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().sheetHeight;
            }

            static void SetSheetHeight(int instance, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::UIComponent>().sheetHeight = value;
            }

            static void RegisterUICallback(int entityID, MonoString* name, MonoObject* callback)
            {
                if (!callback || !name)
                {
                    Popplio::Logger::Error("[MonoInternalCall] Invalid callback or name provided to RegisterUICallback.");
                    return;
                }

                char* functionName = mono_string_to_utf8(name);
                MonoClass* callbackClass = mono_object_get_class(callback);
                MonoMethod* method = mono_class_get_method_from_name(callbackClass, "Invoke", 1); // Expects 1 parameter (entityID)

                if (!method)
                {
                    Popplio::Logger::Error("[MonoInternalCall] Failed to get method for function: " + std::string(functionName));
                    //mono_free(functionName);
                    MonoAPI::MonoFree(functionName);
                    return;
                }

                // **Store a strong reference to prevent garbage collection**
                //uint32_t handle = mono_gchandle_new(callback, true); // Strong reference
                reg->GetSystem<Popplio::LogicSystem>().monoAPI.AddGC(callback, true); // Strong reference
                uint32_t handle = reg->GetSystem<Popplio::LogicSystem>().monoAPI.GetGC(callback, true);

                Popplio::FunctionRegistry::GetInstance().RegisterFunction(
                    functionName,
                    [method, handle, entityID]()
                    {
                        MonoObject* exception = nullptr;
                        void* params[1];
                        int entityCopy = entityID;
                        params[0] = &entityCopy;

                        // Retrieve the callback object safely
                        MonoObject* callbackRef = mono_gchandle_get_target(handle);
                        if (!callbackRef)
                        {
                            Popplio::Logger::Error("[MonoInternalCall] Callback reference is null!");
                            return;
                        }

                        // Safely invoke the function
                        mono_runtime_invoke(method, callbackRef, params, &exception);

                        if (exception)
                        {
                            Popplio::Logger::Error("[MonoInternalCall] Exception occurred when invoking C# function.");
                        }
                    });

                Popplio::Logger::Info("[MonoInternalCall] Registered C# function: " + std::string(functionName) + " for entity ID: " + std::to_string(entityID));

                //mono_free(functionName);
                MonoAPI::MonoFree(functionName);
            }

            void PopplioComponentUI::ExecuteUIAction(int entity, MonoString* actionName)
            {
                static_cast<void>(entity);

                std::string name = PopplioScriptAPI::MonoToString(actionName);
                auto& registry = Popplio::FunctionRegistry::GetInstance();
                //auto& uiComponent = reg->GetEntityById(entity).GetComponent<Popplio::UIComponent>();

                if (registry.HasFunction(name))
                {
                    registry.GetFunction(name)(); // Execute function
                    Popplio::Logger::Info("[MonoInternalCall] Executed UI Action: " + name);
                }
                else
                {
                    Popplio::Logger::Error("[MonoInternalCall] Function not found: " + name);
                }
            }



        }
    }

    // TODO add to internal calls

    // ParticleComponent.h / ParticleSystem.h //
    namespace PopplioComponentParticles
    {
        extern "C"
        {
            static bool GetActive(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().isActive;
            }
            static void SetActive(int instance, bool value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().isActive = value;
            }

            static int GetRenderOption(int instance)
            {
                return static_cast<int>(reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().renderOption);
            }
            static void SetRenderOption(int instance, int value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().renderOption = value;
            }

            static bool GetUsePhysics(int instance)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().usePhysics;
            }
            static void SetUsePhysics(int instance, bool value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().usePhysics = value;
            }

            static int SpawnParticle(int instance)
            {
                return reg->GetSystem<Popplio::ParticleSystem>().NewPart(
                    reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>());
            }
            static void KillParticle(int instance, int index)
            {
                reg->GetSystem<Popplio::ParticleSystem>().KillPart(
                    reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>(), index);
            }

            static int SpawnEmitter(int instance)
            {
                return reg->GetSystem<Popplio::ParticleSystem>().NewEmit(
                    reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>());
            }
            static void KillEmitter(int instance, int index)
            {
                reg->GetSystem<Popplio::ParticleSystem>().KillEmit(
                    reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>(), index);
            }

            // get / set

            static bool GetActiveEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].active;
            }
            static void SetActiveEmitter(int instance, int index, bool value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].active = value;
            }

            static bool GetLoopEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].loop;
            }
            static void SetLoopEmitter(int instance, int index, bool value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].loop = value;
            }

            static float GetElapsedTimeEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].elapsedTime;
            }
            static void SetElapsedTimeEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].elapsedTime = value;
            }

            static float GetRateEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].rate;
            }
            static void SetRateEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].rate = value;
            }

            static float GetTLoopEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].tLoop;
            }
            static void SetTLoopEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].tLoop = value;
            }

            static float GetMinLifeTimeEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minLifeTime;
            }
            static void SetMinLifeTimeEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minLifeTime = value;
            }

            static float GetMaxLifeTimeEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxLifeTime;
            }
            static void SetMaxLifeTimeEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxLifeTime = value;
            }

            static float GetMinRotEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minRot;
            }
            static void SetMinRotEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minRot = value;
            }

            static float GetMaxRotEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxRot;
            }
            static void SetMaxRotEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxRot = value;
            }

            static float GetMinSpinVelEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minSpinVel;
            }
            static void SetMinSpinVelEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minSpinVel = value;
            }

            static float GetMaxSpinVelEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxSpinVel;
            }
            static void SetMaxSpinVelEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxSpinVel = value;
            }

            static float GetMinSpinAccelEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minSpinAccel;
            }
            static void SetMinSpinAccelEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minSpinAccel = value;
            }

            static float GetMaxSpinAccelEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxSpinAccel;
            }
            static void SetMaxSpinAccelEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxSpinAccel = value;
            }

            static float GetMinPosXEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minPos.x;
            }
            static void SetMinPosXEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minPos.x = value;
            }

            static float GetMaxPosXEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxPos.x;
            }
            static void SetMaxPosXEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxPos.x = value;
            }

            static float GetMinPosYEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minPos.y;
            }
            static void SetMinPosYEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minPos.y = value;
            }

            static float GetMaxPosYEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxPos.y;
            }
            static void SetMaxPosYEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxPos.y = value;
            }

            static float GetMinScaleXEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minScale.x;
            }
            static void SetMinScaleXEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minScale.x = value;
            }

            static float GetMaxScaleXEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxScale.x;
            }
            static void SetMaxScaleXEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxScale.x = value;
            }

            static float GetMinScaleYEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minScale.y;
            }
            static void SetMinScaleYEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minScale.y = value;
            }

            static float GetMaxScaleYEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxScale.y;
            }
            static void SetMaxScaleYEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxScale.y = value;
            }

            static float GetMinVelXEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minVel.x;
            }
            static void SetMinVelXEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minVel.x = value;
            }

            static float GetMaxVelXEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxVel.x;
            }
            static void SetMaxVelXEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxVel.x = value;
            }

            static float GetMinVelYEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minVel.y;
            }
            static void SetMinVelYEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minVel.y = value;
            }

            static float GetMaxVelYEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxVel.y;
            }
            static void SetMaxVelYEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxVel.y = value;
            }

            static float GetMinScaleVelXEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minScaleVel.x;
            }
            static void SetMinScaleVelXEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minScaleVel.x = value;
            }

            static float GetMaxScaleVelXEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxScaleVel.x;
            }
            static void SetMaxScaleVelXEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxScaleVel.x = value;
            }

            static float GetMinScaleVelYEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minScaleVel.y;
            }
            static void SetMinScaleVelYEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minScaleVel.y = value;
            }

            static float GetMaxScaleVelYEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxScaleVel.y;
            }
            static void SetMaxScaleVelYEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxScaleVel.y = value;
            }

            static float GetMinAccelXEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minAccel.x;
            }
            static void SetMinAccelXEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minAccel.x = value;
            }

            static float GetMaxAccelXEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxAccel.x;
            }
            static void SetMaxAccelXEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxAccel.x = value;
            }

            static float GetMinAccelYEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minAccel.y;
            }
            static void SetMinAccelYEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minAccel.y = value;
            }

            static float GetMaxAccelYEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxAccel.y;
            }
            static void SetMaxAccelYEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxAccel.y = value;
            }

            static float GetMinScaleAccelXEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minScaleAccel.x;
            }
            static void SetMinScaleAccelXEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minScaleAccel.x = value;
            }

            static float GetMaxScaleAccelXEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxScaleAccel.x;
            }
            static void SetMaxScaleAccelXEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxScaleAccel.x = value;
            }

            static float GetMinScaleAccelYEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minScaleAccel.y;
            }
            static void SetMinScaleAccelYEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minScaleAccel.y = value;
            }

            static float GetMaxScaleAccelYEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxScaleAccel.y;
            }
            static void SetMaxScaleAccelYEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxScaleAccel.y = value;
            }

            static float GetMinColREmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minCol.r;
            }
            static void SetMinColREmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minCol.r = value;
            }

            static float GetMaxColREmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxCol.r;
            }
            static void SetMaxColREmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxCol.r = value;
            }

            static float GetMinColGEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minCol.g;
            }
            static void SetMinColGEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minCol.g = value;
            }

            static float GetMaxColGEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxCol.g;
            }
            static void SetMaxColGEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxCol.g = value;
            }

            static float GetMinColBEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minCol.b;
            }
            static void SetMinColBEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minCol.b = value;
            }

            static float GetMaxColBEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxCol.b;
            }
            static void SetMaxColBEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxCol.b = value;
            }

            static float GetMinColAEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minCol.a;
            }
            static void SetMinColAEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minCol.a = value;
            }

            static float GetMaxColAEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxCol.a;
            }
            static void SetMaxColAEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxCol.a = value;
            }

            static float GetMinColVelREmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColVel.r;
            }
            static void SetMinColVelREmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColVel.r = value;
            }

            static float GetMaxColVelREmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColVel.r;
            }
            static void SetMaxColVelREmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColVel.r = value;
            }

            static float GetMinColVelGEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColVel.g;
            }
            static void SetMinColVelGEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColVel.g = value;
            }

            static float GetMaxColVelGEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColVel.g;
            }
            static void SetMaxColVelGEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColVel.g = value;
            }

            static float GetMinColVelBEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColVel.b;
            }
            static void SetMinColVelBEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColVel.b = value;
            }

            static float GetMaxColVelBEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColVel.b;
            }
            static void SetMaxColVelBEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColVel.b = value;
            }

            static float GetMinColVelAEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColVel.a;
            }
            static void SetMinColVelAEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColVel.a = value;
            }

            static float GetMaxColVelAEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColVel.a;
            }
            static void SetMaxColVelAEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColVel.a = value;
            }

            static float GetMinColAccelREmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColAccel.r;
            }
            static void SetMinColAccelREmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColAccel.r = value;
            }

            static float GetMaxColAccelREmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColAccel.r;
            }
            static void SetMaxColAccelREmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColAccel.r = value;
            }

            static float GetMinColAccelGEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColAccel.g;
            }
            static void SetMinColAccelGEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColAccel.g = value;
            }

            static float GetMaxColAccelGEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColAccel.g;
            }
            static void SetMaxColAccelGEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColAccel.g = value;
            }

            static float GetMinColAccelBEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColAccel.b;
            }
            static void SetMinColAccelBEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColAccel.b = value;
            }

            static float GetMaxColAccelBEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColAccel.b;
            }
            static void SetMaxColAccelBEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColAccel.b = value;
            }

            static float GetMinColAccelAEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColAccel.a;
            }
            static void SetMinColAccelAEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColAccel.a = value;
            }

            static float GetMaxColAccelAEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColAccel.a;
            }
            static void SetMaxColAccelAEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColAccel.a = value;
            }

            static float GetMinSpinDirAccelEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minSpinDirAccel;
            }

            static void SetMinSpinDirAccelEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minSpinDirAccel = value;
            }

            static float GetMaxSpinDirAccelEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxSpinDirAccel;
            }

            static void SetMaxSpinDirAccelEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxSpinDirAccel = value;
            }

            static float GetMinDirAccelXEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minDirAccel.x;
            }

            static void SetMinDirAccelXEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minDirAccel.x = value;
            }

            static float GetMaxDirAccelXEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxDirAccel.x;
            }

            static void SetMaxDirAccelXEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxDirAccel.x = value;
            }

            static float GetMinDirAccelYEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minDirAccel.y;
            }

            static void SetMinDirAccelYEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minDirAccel.y = value;
            }

            static float GetMaxDirAccelYEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxDirAccel.y;
            }

            static void SetMaxDirAccelYEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxDirAccel.y = value;
            }

            static float GetMinScaleDirAccelXEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minScaleDirAccel.x;
            }

            static void SetMinScaleDirAccelXEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minScaleDirAccel.x = value;
            }

            static float GetMaxScaleDirAccelXEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxScaleDirAccel.x;
            }

            static void SetMaxScaleDirAccelXEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxScaleDirAccel.x = value;
            }

            static float GetMinScaleDirAccelYEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minScaleDirAccel.y;
            }

            static void SetMinScaleDirAccelYEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minScaleDirAccel.y = value;
            }

            static float GetMaxScaleDirAccelYEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxScaleDirAccel.y;
            }

            static void SetMaxScaleDirAccelYEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxScaleDirAccel.y = value;
            }

            static float GetMinColDirAccelREmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColDirAccel.r;
            }

            static void SetMinColDirAccelREmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColDirAccel.r = value;
            }

            static float GetMaxColDirAccelREmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColDirAccel.r;
            }

            static void SetMaxColDirAccelREmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColDirAccel.r = value;
            }

            static float GetMinColDirAccelGEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColDirAccel.g;
            }

            static void SetMinColDirAccelGEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColDirAccel.g = value;
            }

            static float GetMaxColDirAccelGEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColDirAccel.g;
            }

            static void SetMaxColDirAccelGEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColDirAccel.g = value;
            }

            static float GetMinColDirAccelBEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColDirAccel.b;
            }

            static void SetMinColDirAccelBEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColDirAccel.b = value;
            }

            static float GetMaxColDirAccelBEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColDirAccel.b;
            }

            static void SetMaxColDirAccelBEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColDirAccel.b = value;
            }

            static float GetMinColDirAccelAEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColDirAccel.a;
            }

            static void SetMinColDirAccelAEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].minColDirAccel.a = value;
            }

            static float GetMaxColDirAccelAEmitter(int instance, int index)
            {
                return reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColDirAccel.a;
            }

            static void SetMaxColDirAccelAEmitter(int instance, int index, float value)
            {
                reg->GetEntityById(instance).GetComponent<Popplio::ParticleComponent>().emission[index].maxColDirAccel.a = value;
            }
        }
    }
}