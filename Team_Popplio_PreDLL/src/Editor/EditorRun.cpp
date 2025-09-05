/******************************************************************************/
/*!
\file   EditorRun.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\co-author Pearl Goh
\contribution Shawn - 95% | Pearl Goh - 5%
\par    Course : CSD2401
\par    Section : A
\date   2025/03/14 04:08:22 PM (Last Edit)
\brief
		This is the source file for the Editor's scene running logic
		Controls in-game logic

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>

#include "Editor.h"

#include "../Collision/CollisionSystem.h"
#include "../Physics/GravitySystem.h"
#include "../Script/LogicSystem.h"
#include "../Transformation/TransformationSystem.h"
#include "../Animation/AnimationSystem.h"
#include "../Physics/MovementSystem.h"
#include "../Cloning/CloneSystem.h"
#include "../PrefabManagement/PrefabSyncSystem.h"
#include "../Audio/AudioSystem.h"
#include "../Particles/ParticleSystem.h"
#include "../Scene/SceneManageSystem.h"
#include "../Utilities/QueueSystem.h"
#include "../UI/UISystem.h"
#include "../Hierarchy/HierarchySystem.h"

#include "../Serialization/Serialization.h"

#include "../PerformanceViewer/PerformanceViewer.h"

#include "../Utilities/OnAppQuit.h"

namespace Popplio
{
	const std::chrono::milliseconds scenePauseCooldown{500}; // 500ms
    static std::chrono::time_point<std::chrono::high_resolution_clock> lastScenePauseTime;

	void Editor::RunSetup()
	{
		Logger::Info("Running setup...");

        //reg.GetSystem<QueueSystem>().Clear(); // clear in case loaded from previous scene
		reg.GetSystem<SceneManageSystem>().Init(AssetStore::GetSceneId(currentScenePath));
        reg.GetSystem<AudioSystem>().Setup();
		reg.GetSystem<AudioSystem>().StopAllAudioInst();
		reg.GetSystem<AudioSystem>().StopAllAudio();
		reg.GetSystem<AnimationSystem>().StartAll();

		reg.GetSystem<LogicSystem>().Setup();
		//reg.GetSystem<LogicSystem>().SubscribeToEvents(eventBus);


		if (!reg.GetSystem<QueueSystem>().IsProcessing()) reg.GetSystem<QueueSystem>().Process();

		// Switch to game camera when starting game
		cameraManager.SetActiveCamera(false);

		reg.Update();
	}

	void Editor::RunInit()
	{
		Logger::Info("Initializing...");
		reg.GetSystem<LogicSystem>().Init();
		//Logger::Info("Done! Game is running");

		reg.GetSystem<AudioSystem>().Init();

		if (!reg.GetSystem<QueueSystem>().IsProcessing()) reg.GetSystem<QueueSystem>().Process();

		reg.Update();
		Logger::Info("Done! Scene is running");
	}

	void Editor::RunUpdate()
	{
		// Fixed Logic updates
		PerformanceViewer::GetInstance()->start("fixed logic");
		reg.GetSystem<LogicSystem>().FixedUpdate();
		PerformanceViewer::GetInstance()->end("fixed logic");

		// Logic updates
		PerformanceViewer::GetInstance()->start("logic");
		reg.GetSystem<LogicSystem>().Update();
		PerformanceViewer::GetInstance()->end("logic");

		// Particle updates
		PerformanceViewer::GetInstance()->start("particles");
		reg.GetSystem<ParticleSystem>().Update(Engine::timer.GetFixedDeltaTime());
		PerformanceViewer::GetInstance()->end("particles");

		// Transform updates
		Popplio::PerformanceViewer::GetInstance()->start("transformation");
		reg.GetSystem<TransformationSystem>().Update();
		Popplio::PerformanceViewer::GetInstance()->end("transformation");

		Popplio::PerformanceViewer::GetInstance()->start("hierarchy");
		reg.GetSystem<HierarchySystem>().Update();
		Popplio::PerformanceViewer::GetInstance()->end("hierarchy");

		// Physics updates
		PerformanceViewer::GetInstance()->start("gravity");
		reg.GetSystem<GravitySystem>().Update();
		PerformanceViewer::GetInstance()->end("gravity");

		PerformanceViewer::GetInstance()->start("Movement");
		reg.GetSystem<MovementSystem>().Update(static_cast<float>(Engine::timer.GetFixedDeltaTime()));
		PerformanceViewer::GetInstance()->end("Movement");

		// Collision updates
		PerformanceViewer::GetInstance()->start("collision");
		reg.GetSystem<CollisionSystem>().Update(reg, static_cast<float>(Engine::timer.GetFixedDeltaTime()));
		PerformanceViewer::GetInstance()->end("collision");

		// resolve queue
		PerformanceViewer::GetInstance()->start("queue");
		if (!reg.GetSystem<QueueSystem>().IsProcessing()) reg.GetSystem<QueueSystem>().Process();
		PerformanceViewer::GetInstance()->end("queue");

		// Animation update
		//PerformanceViewer::GetInstance()->start("animation");
		//reg.GetSystem<AnimationSystem>().Update(static_cast<float>(Engine::timer.GetFixedDeltaTime()));
		//PerformanceViewer::GetInstance()->start("animation");
		
		//PlayAudio();
	}

	void Editor::RunRender()
	{
		// Animation update
		PerformanceViewer::GetInstance()->start("animation");
		reg.GetSystem<AnimationSystem>().Update(static_cast<float>(Engine::timer.GetFixedDeltaTime()));
		PerformanceViewer::GetInstance()->end("animation");

		// Particle update
		PerformanceViewer::GetInstance()->start("particles");
		reg.GetSystem<ParticleSystem>().Update(Engine::timer.GetFixedDeltaTime());
		PerformanceViewer::GetInstance()->end("particles");

		// UI
		PerformanceViewer::GetInstance()->start("ui");
		reg.GetSystem<UISystem>().Update();
		PerformanceViewer::GetInstance()->end("ui");

		// Audio update
		PerformanceViewer::GetInstance()->start("audio");
		reg.GetSystem<AudioSystem>().Update();
		PerformanceViewer::GetInstance()->end("audio");
	}

	void Editor::RunEnd()
	{
		reg.GetSystem<AnimationSystem>().StopAll();

		reg.GetSystem<AudioSystem>().End();
		reg.GetSystem<LogicSystem>().End();

		if (!reg.GetSystem<QueueSystem>().IsProcessing()) reg.GetSystem<QueueSystem>().Process();

		reg.Update();
	}

	void Editor::RunFixedUpdate() // todo: implement
	{

		// Transform updates
		Popplio::PerformanceViewer::GetInstance()->start("fixed transformation");
		reg.GetSystem<TransformationSystem>().Update();
		Popplio::PerformanceViewer::GetInstance()->end("fixed transformation");

		Popplio::PerformanceViewer::GetInstance()->start("fixed hierarchy");
		reg.GetSystem<HierarchySystem>().Update();
		Popplio::PerformanceViewer::GetInstance()->end("fixed hierarchy");

		// Physics updates
		PerformanceViewer::GetInstance()->start("fixed gravity");
		reg.GetSystem<GravitySystem>().Update();
		PerformanceViewer::GetInstance()->end("fixed gravity");

		PerformanceViewer::GetInstance()->start("fixed Movement");
		reg.GetSystem<MovementSystem>().Update(static_cast<float>(Engine::timer.GetFixedDeltaTime()));
		PerformanceViewer::GetInstance()->end("fixed Movement");

		// Collision updates
		PerformanceViewer::GetInstance()->start("fixed collision");
		reg.GetSystem<CollisionSystem>().Update(reg, static_cast<float>(Engine::timer.GetFixedDeltaTime()));
		PerformanceViewer::GetInstance()->end("fixed collision");

		PerformanceViewer::GetInstance()->start("queue");
		if (!reg.GetSystem<QueueSystem>().IsProcessing()) reg.GetSystem<QueueSystem>().Process();
		PerformanceViewer::GetInstance()->end("queue");
	}

	void Editor::StartScene() // start scene
	{
		// Pause chart editor audio when entering play mode
		if (isChartPlaying && m_audioComponent.data >= 0)
		{
			AudioSystem& audioSys = reg.GetSystem<AudioSystem>();
			Audio& audioData = audioSys.GetAudioInst(m_audioComponent.data);
			audioData.Pause();
			// Don't reset isChartPlaying so we know to resume later
		}

		LogClearOnRun(); // clear logs if clear on run enabled

        reg.CachePreRuntime(); // cache ECS info pre runtime to be reset to after runtime

		Logger::Info("Playing Scene...");

		// todo: (maybe) reset all systems here (e.g. when editing stuff in GUI)
		// (e.g. reset audio when playing an audio half way in GUI when pressing play)

		//reg.GetSystem<LogicSystem>().Register(); // register scripts to system

        if (editorState != State::PLAYING) //ClearInstancingEntities();
			reg.ClearInstancedEntities();

		editorState = State::PLAYING;

		
		// return to this func if kill instanced entities is still not empty
		if (!reg.KillEntityQueueIsEmpty())
		{
			isStarting = true;
			return;
		}

		isStarting = false;
		// return to this func if kill instanced entities is still not empty

        // save to temp file
        //std::string curr = currentScenePath;
		SaveScene(tempScenePath);
		//currentScenePath = curr;

		Logger::Info("Saving Scene...");

		mono.ReloadAssembly(); // reload assembly

		RunSetup();
		RunInit();

		// set pause time
        lastScenePauseTime = std::chrono::high_resolution_clock::now();
	}

	void Editor::PauseScene() // pause scene
	{
        // Update the last pause time
		Logger::Info("Pausing Scene...");
		reg.GetSystem<AudioSystem>().PauseAllAudioInst(); // might have issues if there is some audio paused already and not tracked
		reg.GetSystem<AnimationSystem>().PauseAll();
		editorState = State::PAUSED;
		//Engine::isPaused = true;
        lastScenePauseTime = std::chrono::high_resolution_clock::now(); // Update the last pause time
	}

	void Editor::ResumeScene() // resume (from pause) scene
	{
		// Check if the cooldown period has passed since the last pause
		if (std::chrono::duration_cast<std::chrono::microseconds>(
			std::chrono::high_resolution_clock::now() - lastScenePauseTime) < scenePauseCooldown)
		{
			return; // Ignore the resume request
		}

		Logger::Info("Resuming Scene...");
		reg.GetSystem<AudioSystem>().ResumeAllAudioInst(); // resumes already paused audio before pausing scene
		reg.GetSystem<AnimationSystem>().ResumeAll();
		//reg.GetSystem<AudioSystem>().PlayAllAudioInst(); // resumes already paused audio before pausing scene
		//reg.GetSystem<AnimationSystem>().StartAll();
		editorState = State::PLAYING;
		//Engine::isPaused = false;
	}

	void Editor::EndScene() // end scene
	{
		if (!isEnding)
		{
			Logger::Info("Ending Scene...");

			// Save chart editor audio state before ending the scene
			//bool wasChartPlaying = isChartPlaying;
			//int chartAudioInstance = m_audioComponent.data;

			RunEnd();

			eventBus.EmitEvent<PopplioUtil::OnAppQuit>();

			// Switch back to editor camera
			cameraManager.SetActiveCamera(true);

			// reset subscriptions (to clear all events)
			//eventBus.Reset();
			//eventBus.SubscribeToEvent<SceneChangeEvent<std::any>>(this, &Editor::OnSceneChange);
			//reg.GetSystem<CloneSystem>().SubscribeToEvents(eventBus);
			//reg.GetSystem<PrefabSyncSystem>().SubscribeToEvents(eventBus);
			//reg.GetSystem<AudioSystem>().SubscribeToEvents(eventBus);

			if (editorState != State::EDITING) reg.ClearInstancedEntities();

			editorState = State::STOPPED; // todo: reset then change state to EDITING
			editorState = State::EDITING;
		}

		// return to this func if kill instanced entities is still not empty
		if (!reg.KillEntityQueueIsEmpty())
		{
			isEnding = true;
			return;
		}

		isEnding = false;
		// return to this func if kill instanced entities is still not empty


		// remove all entities

		// "Run" part of the deserialization
		// Create entities from current scene
        std::string curr = currentScenePath;
		LoadScene(tempScenePath);
        currentScenePath = curr;

		reg.ResetToPreRuntime();

		if (isChartPlaying && m_audioComponent.data >= 0)
		{
			AudioSystem& audioSys = reg.GetSystem<AudioSystem>();
			Audio& audioData = audioSys.GetAudioInst(m_audioComponent.data);
			audioData.Play();
		}

	}

	void Editor::ChangeScene(std::string const& scene)
	{
		//if (editorState != State::CHANGING)
		//{
  //          //currentScenePath = AssetStore::GetScene(scene).string();
		//	editorState = State::CHANGING;
		//	return;
		//}

		if (editorState == State::CHANGING)
		{
			Logger::Info("Changing Scene...");

			ResetSelectedEntity();

			RunEnd();

			reg.ClearInstancedEntities();

			if (scene != "") currentScenePath = AssetStore::GetScene(scene).string();
		}

		//if (!isChanging)
		//{
		//	Logger::Info("Changing Scene...");

		//	ResetSelectedEntity();

		//	RunEnd();

		//	// TODO change func with one that excludes persistent entities
		//	ClearInstancingEntities();
		//}

		//// return to this func if kill instanced entities is still not empty
		//if (!reg.KillEntityQueueIsEmpty())
		//{
		//	isChanging = true;
		//	return;
		//}

		//isChanging = false;
		//// return to this func if kill instanced entities is still not empty

		else
		{
			//mono.ReloadAssembly();

			//currentScenePath = AssetStore::GetScene(scene).string();

			//editorState = State::PLAYING;

			// new scene setup
			reg.GetSystem<LogicSystem>().Setup();
			reg.GetSystem<AnimationSystem>().StartAll();
            reg.GetSystem<QueueSystem>().Process();
			reg.Update();

			reg.GetSystem<LogicSystem>().Init();
			reg.GetSystem<AudioSystem>().Init();
			reg.GetSystem<QueueSystem>().Process();
			reg.Update();
		}

		//Logger::Info("Changing Scene...");

		//ResetSelectedEntity();

		//RunEnd();

		//reg.ClearInstancedEntities();

		//mono.ReloadAssembly();

		//currentScenePath = AssetStore::GetScene(scene).string();

		//editorState = State::PLAYING;

		// new scene setup
		//reg.GetSystem<LogicSystem>().Setup();
		//reg.GetSystem<AnimationSystem>().StartAll();
		//reg.GetSystem<QueueSystem>().Process();
		//reg.Update();

		//reg.GetSystem<LogicSystem>().Init();
		//reg.GetSystem<AudioSystem>().Init();
		//reg.GetSystem<QueueSystem>().Process();
		//reg.Update();
	}
}