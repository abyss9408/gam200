/******************************************************************************/
/*!
\file   Scene.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the header file for engine Scene Library
        Used at runtime by engine and user scripts

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "../Engine/Engine.h"
#include "../ECS/ECS.h"
#include "../Serialization/Serialization.h"

#define POPPLIO_SCENE_PATH "Assets/Scenes/"
#define POPPLIO_SCENE_EXTENSION ".scene"

namespace Popplio
{
    class SceneManageSystem : public System
    {
        private:
        Engine& engine;
        Registry& registry;
        Serializer& serializer;
		CameraManager& cameraManager;
        PopplioScriptAPI::MonoAPI& monoAPI;

        std::string currentScene;
        std::string prevScene;

        public:
        /*
        *   SceneManageSystem argument constructor
        */
        SceneManageSystem(Engine& eng, Registry& reg, Serializer& serialize, PopplioScriptAPI::MonoAPI& mono, CameraManager& cameraMgr);

        /*
        *   Initialize SceneManageSystem  
        */
        void Init(const std::string& sceneName);

        /*
        *   Load Scene at runtime
        */
        bool LoadScene(const std::string& sceneName);
        /*
        *   Get Current Scene
        */
        std::string const& GetCurrentScene() const;
        /*
        *   Get Previous Scene
        */
        std::string const& GetPreviousScene() const;
        /*
        *   Reload Current Scene
        */
        bool ReloadCurrentScene();
    };
}