/******************************************************************************/
/*!
\file   Scene.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the source file for engine Scene Library
        Used at runtime by engine and user scripts

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>

#include "SceneManageSystem.h"

#ifndef IMGUI_DISABLE
#include "../Editor/Editor.h"
#endif
#include "../Utilities/QueueSystem.h"

namespace Popplio
{
    SceneManageSystem::SceneManageSystem(Engine& eng, Registry& reg, Serializer& serialize, 
		PopplioScriptAPI::MonoAPI& mono, CameraManager& cameraMgr)
		: engine(eng), registry(reg), serializer(serialize), monoAPI(mono), 
		currentScene(""), prevScene(""), cameraManager(cameraMgr)
	{

	}

	void SceneManageSystem::Init(const std::string& sceneName)
	{
        currentScene = sceneName;
	}

    bool SceneManageSystem::LoadScene(const std::string& sceneName)
    {
		Logger::Info("SceneManageSystem | Attempting to load scene: " + sceneName);

        if (AssetStore::GetScene(sceneName).empty())
        {
            Logger::Error("SceneManageSystem | Scene does not exist: " + sceneName);
            return false;
        }

		//Logger::Info("SceneManageSystem | Clearing GC...");
		////monoAPI.CleanupStaleHandles();
  //      monoAPI.ClearAllGC();
		//monoAPI.ForceGC();

		auto loadScene = [&](std::string const& scene)
		{ 
			//if (serializer.LoadSceneFromFile(AssetStore::GetScene(sceneName).string(), cameraManager, false))
			if (serializer.LoadSceneFromFile(POPPLIO_SCENE_PATH + scene + POPPLIO_SCENE_EXTENSION, cameraManager, false))
			{
                prevScene = currentScene;
				currentScene = scene;
				#ifndef IMGUI_DISABLE
				//			//registry.GetSystem<Editor>().ResetSelectedEntity();
				//			registry.GetSystem<Editor>().ChangeScene(sceneName);
				//			//registry.GetSystem<Editor>().RunEnd();
				//			//monoAPI.ReloadAssembly();
				//            //registry.GetSystem<Editor>().RunSetup();
				//registry.GetSystem<Editor>().editorState = Editor::State::PLAYING;
				#else
				//			//engine.GameEnd();
				//			engine.GameChangeScene();
				//			//monoAPI.ReloadAssembly();
				//            //engine.GameSetup();
                //Engine::isSceneChanging = false;
				#endif // !IMGUI_DISABLE
				//			//mono.ReloadAssembly(); 
				Logger::Info("SceneManageSystem | Scene loaded successfully: " + scene);
				return true;
			}
			else
			{
				Logger::Error("SceneManageSystem | Failed to load scene: " + scene);
				return false;
			}
		};

#ifndef IMGUI_DISABLE
		//registry.GetSystem<Editor>().ChangeScene(sceneName);
		
        registry.GetSystem<Editor>().editorState = Editor::State::CHANGING;

		auto changeScene = [&]()
		{
            registry.GetSystem<Editor>().ChangeScene(sceneName);
        };
#else
		//engine.GameChangeScene();

        Engine::isSceneChanging = true;

		auto changeScene = [&]()
		{
			engine.GameChangeScene();
		};
#endif // !IMGUI_DISABLE

   //     auto newSceneInit = [&]()
   //     {
			//// new scene setup
			//registry.GetSystem<LogicSystem>().Setup();
			//registry.GetSystem<AnimationSystem>().StartAll();
			//registry.GetSystem<QueueSystem>().Process();
			//registry.Update();

			//registry.GetSystem<LogicSystem>().Init();
			//registry.GetSystem<AudioSystem>().Init();
			//registry.GetSystem<QueueSystem>().Process();
			//registry.Update();
   //     };

		registry.GetSystem<QueueSystem>().Add(changeScene);
		std::string tempVal = sceneName;
		registry.GetSystem<QueueSystem>().Add(std::bind(loadScene, sceneName));

        //registry.GetSystem<QueueSystem>().Add(newSceneInit);

		return true;
    }

	std::string const& SceneManageSystem::GetCurrentScene() const
	{
		return currentScene;
	}
	std::string const& SceneManageSystem::GetPreviousScene() const
	{
		return prevScene;
	}

	bool SceneManageSystem::ReloadCurrentScene()
	{
        Logger::Info("SceneManageSystem | Reloading current scene...");
        return LoadScene(currentScene);
	}
}