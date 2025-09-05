/******************************************************************************/
/*!
\file   Engine.h
\author Team Popplio
\author Bryan Ang Wei Ze
\co-author	Ng Jun Heng Shawn | Shannon | Hafawati Binte Mohd Ali
\contribution Bryan - 70%, Shawn - 15%, Shannon - 5%, Hafawati - 10%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/30 
\brief
    This is the header file for the core of the engine

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include "../ECS/ECS.h"
#include "../Serialization/Serialization.h"
#include "../EventBus/EventBus.h"
#include "../Utilities/UtilLib.h"
#include "../Physics/Physics.h"
#include "../Logging/Logger.h"
#include "../PrefabManagement/PrefabManager.h"
#include "../Layering/LayerManager.h"
#include "../Camera/CameraManager.h"
#include "../LoadingScreen/LoadingScreen.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// ScriptAPI
#include "../Mono/MonoAPI.h"

namespace Popplio
{
    /**
     * @struct GLFWwindowDeleter
     * @brief A custom deleter for GLFWwindow pointers.
     * 
     * This struct provides a custom deleter for GLFWwindow pointers. It is used to automatically destroy GLFW windows when they are no longer needed.
     */
	//struct GLFWwindowDeleter
	//{
 //       /**
 //        * @brief Operator function for deleting a GLFWwindow.
 //        * 
 //        * This operator function is called when a GLFWwindow pointer needs to be deleted. It destroys the GLFW window using glfwDestroyWindow().
 //        * 
 //        * @param window The GLFWwindow pointer to be deleted.
 //        */
	//	void operator()(GLFWwindow* window)
	//	{
	//		glfwDestroyWindow(window);
	//	}
	//};

    /**
     * @class Engine
     * @brief Represents an engine instance.
     * 
     * The Engine class is responsible for managing the game loop and handling various game-related operations such as initialization, updating, rendering, and audio playback.
     */
	class Engine
	{
	public:
        /**
         * @brief Default constructor for the Engine class.
         */
		Engine();

        /**
         * @brief Initializes the program.
         * 
         * This function initializes the game by creating the program window, setting up the ECS registry, event bus, serializer, and other necessary components.
         */
		void Initialize();

        /**
         * @brief Performs the initial setup for the program.
         * 
         * This function performs the initial setup for the program, such as loading assets, setting up the game scene, and initializing any required systems.
         */
		//void Setup();

        /**
         * @brief Runs the program loop.
         * 
         * This function runs the main program loop, which includes processing input, updating game logic, rendering the game scene, and playing audio.
         */
		void Run();

        /**
         * @brief Processes user input.
         * 
         * This function processes user input, such as keyboard and mouse events, and updates the game state accordingly.
         */
		void ProcessInput();

        /**
         * @brief Updates the program logic.
         * 
         * This function updates the program logic, such as updating entity positions, handling collisions, and executing game events.
         */
		void Update();

        /**
         * @brief Renders the program.
         * 
         * This function renders the program and scene, including all entities, sprites, and other visual elements.
         */
		void Render();

        /**
         * @brief Game loop for the program.
         */

        void GameSetup();
        void GameInit();
        void GameUpdate();
        //void GameFixedUpdate();
        void GameRender();
        void GameEnd();
        void GameChangeScene();

        /**
         * @brief Destroys the program.
         * 
         * This function destroys the game by cleaning up any allocated resources and shutting down the game engine.
         */
		void Destroy();

        /**
         * @brief Sets the window to fullscreen mode.
         *
         * This function sets the window to fullscreen mode from windowed mode.
         */
        void SetFullScreen();

        /**
         * @brief Sets the window to windowed mode.
         *
         * This function sets the window to windowed mode from fullscreen mode.
         */
        void SetWindowedMode();

        // ++++++++++++++++++++++++++++++++ //

        // Configuration

        struct Config
        {
            // Version // ========================

            /**
             * @brief engine version. v0.milestone.week | date
             */
            static std::string engineVer;

            /**
             * @brief app version. v0.milestone.week | date
             */
            static std::string appVer;

            // Program settings // ========================

            /**
             * @brief The width of the program window.
             */
            static int windowWidth;

            /**
             * @brief The height of the program window.
             */
            static int windowHeight;

            /**
             * @brief Flag indicating whether vertical sync is enabled.
             */
            static bool vSync;

            // Editor // ========================

            /**
             * @brief Flag indicating if safe mode is enabled.
             */
            static bool safeMode;

            /**
             * @brief Scene to load in editor upon program start.
             */
            static std::string lastScene;

            /**
             * @brief Flag indicating if verbose logging is enabled (only to relevant systems).
             */
            static bool verbose;

            // Build settings // ========================

            /**
             * @brief Start scene to load in build.
             */
            static std::string startScene;
        };

        //static Config config;

        // ++++++++++++++++++++++++++++++++ //

        /**
         * @brief The timer used for measuring frame time and calculating frames per second.
         */
		static PopplioUtil::Timer timer;

        /**
         * @brief The current frames per second (FPS) of the game.
         */
		static double framesPerSecond;

        /**
         * @brief Flag indicating whether game is paused.
         */
        static bool isPaused;

        /**
         * @brief Flag indicating whether windows is in fullscreen mode.
         */
        static bool fullscreen;

        static PopplioPhysics::Gravity gravity;

        static bool isSceneChanging;

        static std::string programPath;

	private:
        GLFWwindow* window; /**< The GLFW window used for rendering. */
        std::unique_ptr<Registry> registry; /**< The ECS registry for managing entities and components. */
        std::unique_ptr<EventBus> eventBus; /**< The event bus for handling game events. */
        std::unique_ptr<PrefabManager> prefabManager; /**< The prefab manager for managing prefabs. */
        std::unique_ptr<LayerManager> layerManager; /**< The layer manager for managing entity layers. */
        std::unique_ptr<CameraManager> cameraManager; /**< The camera manager for managing cameras. */
        std::unique_ptr<Serializer> serializer; /**< The serializer for saving and loading game data. */
        std::unique_ptr<LoadingScreen> loadingScreen;
        std::unique_ptr<PopplioScriptAPI::MonoAPI> monoAPI; /**< The Mono API for interacting with C# scripts. */

        void InitializeCoreSystems();
        void InitializeRemainingSystems();
        void FinalizeInitialization();
    };
}