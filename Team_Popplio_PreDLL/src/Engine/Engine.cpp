/******************************************************************************/
/*!
\file   Engine.cpp
\author Team Popplio
\author Bryan Ang Wei Ze
\co-author	Ng Jun Heng Shawn | Shannon | Hafawati Binte Mohd Ali
\contribution Bryan - 70%, Shawn - 15%, Shannon - 5%, Hafawati - 10%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/30
\brief
	This is the source file for the core of the engine

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

//Precompiled header
#include <pch.h>

//Header
#include "Engine.h"

//Debug
#include "../PerformanceViewer/PerformanceViewer.h"

//ECS
#include "../ECS/ECS.h"

//Systems
#include "../Physics/MovementSystem.h"
#include "../Graphic/RenderSystem.h"
#include "../Input/InputSystem.h"
#include "../Cloning/CloneSystem.h"
#include "../Audio/AudioSystem.h"
#include "../Collision/CollisionSystem.h"
#include "../Physics/GravitySystem.h"
#ifndef IMGUI_DISABLE
#include "../Editor/Editor.h"
#endif
#include "../Script/LogicSystem.h"
#include "../Transformation/TransformationSystem.h"
#include "../PrefabManagement/PrefabSyncSystem.h"
#include "../Animation/AnimationSystem.h"
#include "../AssetStore/AssetLoader.h"
#include "../AssetStore/AssetBatchLoader.h"
#include "../UI/UISystem.h"

#include "../Particles/ParticleSystem.h"
#include "../Hierarchy/HierarchySystem.h"
#include "../Scene/SceneManageSystem.h"
#include "../Runtime/RuntimeSystem.h"
#include "../Utilities/QueueSystem.h"

// Mono
#include "../Mono/MonoInternalCall.h"

//Utilities
#include "../Utilities/CrashHandler.h"
#include "../Utilities/OnAppQuit.h"

//Components
#include "../Utilities/Components.h"

#include "../Utilities/Rand.h"

//Libraries
#include <glad/glad.h>

#include <windows.h> // can cause conflicts with other libraries / headers

// test //

// test //

namespace Popplio
{
	LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
	// Hook handle 
	HHOOK keyboardHook;

	static GLFWwindow* g_window = nullptr; // Global pointer for the window

	void SetWindowForHook(GLFWwindow* window)
	{
		g_window = window;
	}

	// configuration

	std::string Engine::Config::engineVer;
    std::string Engine::Config::appVer;

	int Engine::Config::windowWidth;
	int Engine::Config::windowHeight;
	bool Engine::Config::vSync;

	bool Engine::Config::safeMode;
    std::string Engine::Config::lastScene;
    bool Engine::Config::verbose;

    std::string Engine::Config::startScene;

	//Engine::Config config{};

	//bool Engine::isEditorEnabled = true;

	// ===========================

	// runtime

	PopplioUtil::Timer Engine::timer;
	double Engine::framesPerSecond;

	bool Engine::isPaused;
	bool Engine::fullscreen = false;

	// Add new variables to store the original window position and size
	static int windowedPosX = 0, windowedPosY = 0;
	static int windowedWidth = 0, windowedHeight = 0;

	// Flag to track ALT+TAB state
	bool altTabPressed = false;
	bool ctrlAltDelPressed = false;
	static bool wasMaximized = false;


	// test //
	// test //

	PopplioPhysics::Gravity Engine::gravity(20.00); //declare for gravity value

	bool Engine::isSceneChanging = false;

	std::string Engine::programPath = "";

	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
        static_cast<void>(window);

		// make sure the viewport matches the new window dimensions; note that width and
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);

		Engine::Config::windowWidth = width;
		Engine::Config::windowHeight = height;


		// Update Editor's viewport size
#ifndef IMGUI_DISABLE
		Editor* editor = static_cast<Editor*>(glfwGetWindowUserPointer(window));
		if (editor)
		{
			// Scene viewport will be resized dynamically based on window size
			editor->ResizeFramebufferTexture(editor->GetSceneFramebuffer(), width, height);
			editor->SetSceneViewportSize(width, height);

			// Game viewport will be resized dynamically based on window size
			editor->ResizeFramebufferTexture(editor->GetGameFramebuffer(), width, height);
			editor->SetGameViewportSize(width, height);
		}
#endif // !IMGUI_DISABLE
	}

	// Interruption Handler
	// Keyboard hook callback
	LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == HC_ACTION)
		{
			KBDLLHOOKSTRUCT* pKeyBoard = (KBDLLHOOKSTRUCT*)lParam;

			bool altPressed = (GetAsyncKeyState(VK_MENU) & 0x8000); // ALT key
			bool ctrlPressed = (GetAsyncKeyState(VK_CONTROL) & 0x8000); // CTRL key
			bool delPressed = (pKeyBoard->vkCode == VK_DELETE);
			bool tabPressed = (pKeyBoard->vkCode == VK_TAB);

			// Checks if keys are pressed
			if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
			{
				if (altPressed && tabPressed)
				{

					// Track if the window was maximized before Alt-Tab
					wasMaximized = glfwGetWindowAttrib(g_window, GLFW_MAXIMIZED);

					altTabPressed = true;
					//std::cout << "ALT+TAB detected! Minimizing window..." << std::endl;

					Engine::isPaused = true;
					glfwIconifyWindow(g_window);
				}
				if (ctrlPressed && altPressed && delPressed)
				{
					ctrlAltDelPressed = true;
					//std::cout << "CTRL+ALT+DEL detected! Minimizing window..." << std::endl;

					Engine::isPaused = true;
					glfwIconifyWindow(g_window);
				}
			}
			// Checks when key is released
			else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
			{
				if (pKeyBoard->vkCode == VK_MENU || pKeyBoard->vkCode == VK_TAB)
				{
					altTabPressed = false;
				}
				if (pKeyBoard->vkCode == VK_MENU || pKeyBoard->vkCode == VK_CONTROL || pKeyBoard->vkCode == VK_DELETE)
				{
					ctrlAltDelPressed = false;
				}
			}
		}

		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

	// Callback to handle window focus changes 
	void window_focus_callback(GLFWwindow* window, int focused)
	{
		if (!focused)
		{
			//std::cout << "Window lost focus (manual trigger)." << std::endl;
		}
		else
		{
			
			if (glfwGetWindowAttrib(window, GLFW_ICONIFIED))
			{
				glfwRestoreWindow(window);
			}

			// If it was maximized before Alt-Tab, maximize it again
			if (wasMaximized)
			{
				glfwMaximizeWindow(window);
				wasMaximized = false;  // Reset flag after restoring
			}

			Engine::isPaused = false;
			//std::cout << "Window back in focus." << std::endl;
		}
	}

	// Sets window to fullscreen mode
	void Engine::SetFullScreen()
	{
		// Get the monitor and video mode for fullscreen
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		// Store the current window position and size for later restoration
		glfwGetWindowPos(window, &windowedPosX, &windowedPosY);
		glfwGetWindowSize(window, &windowedWidth, &windowedHeight);

		// Switch to fullscreen mode
		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		glfwSwapInterval(Config::vSync); // Refresh context
	}

	// Sets window to windowed mode
	void Engine::SetWindowedMode()
	{
		// Restore the window size and position to the stored values
		glfwSetWindowMonitor(window, nullptr, windowedPosX, windowedPosY,
			windowedWidth, windowedHeight, GLFW_DONT_CARE);
		glfwSwapInterval(Config::vSync); // Refresh context
	}

	Engine::Engine()
		:window(nullptr),
		registry(std::make_unique<Registry>()),
		eventBus(std::make_unique<EventBus>()),
		serializer(std::make_unique<Serializer>(*registry, *layerManager, *prefabManager, *eventBus)),
		prefabManager(std::make_unique<PrefabManager>(*eventBus)),
		layerManager(std::make_unique<LayerManager>()),
		monoAPI(std::make_unique<PopplioScriptAPI::MonoAPI>(*registry))
	{

	}

	void Engine::Initialize()
	{
        programPath = std::filesystem::current_path().string(); // set program path

		// Set up Logger
		Logger::Initialize(LogLevel::DEBUG_LOG);
		Logger::Info("Engine intialization started", LogDestination::WINDOWS_CONSOLE);

		// glfw: initialize and configure
		// ------------------------------
		if (!glfwInit())
		{
			Logger::Critical("Failed to initialize GLFW", LogDestination::WINDOWS_CONSOLE);
			return;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifndef IMGUI_DISABLE
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
#else
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#endif // !IMGUI_DISABLE

        // Load configuration
		if (serializer->LoadConfiguration(POPPLIO_CONFIG_PATH))
		{
            Config::engineVer = serializer->GetConfiguration().engineVer;
            Config::appVer = serializer->GetConfiguration().appVer;
			Config::windowWidth = serializer->GetConfiguration().width;
			Config::windowHeight = serializer->GetConfiguration().height;
			Config::vSync = serializer->GetConfiguration().vSync;
            Config::safeMode = serializer->GetConfiguration().safeMode;
            Config::lastScene = serializer->GetConfiguration().lastScene;
			Config::verbose = serializer->GetConfiguration().verbose;
            Config::startScene = serializer->GetConfiguration().startScene;
		}

		window = glfwCreateWindow(Config::windowWidth, Config::windowHeight, "Team Popplio Engine", nullptr, nullptr);
		if (!window)
		{
			Logger::Critical("Failed to create GLFW window", LogDestination::WINDOWS_CONSOLE);
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(window);
		glfwSwapInterval(Config::vSync);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetWindowFocusCallback(window, window_focus_callback);
		SetWindowForHook(window); // Set the window pointer

		// Install the low-level keyboard hook
		keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
		if (!keyboardHook) {
			std::cerr << "Failed to install keyboard hook!" << std::endl;
			return;
		}

		//GLFWimage image;
		//image.pixels = stbi_load("Team_Popplio.ico", &image.width, &image.height, 0, 4);
		//glfwSetWindowIcon(window, 1, &image);

		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			Logger::Critical("Failed to initialize GLAD", LogDestination::WINDOWS_CONSOLE);
			return;
		}

		// Set fullscreen by default for game build
#ifdef IMGUI_DISABLE
		SetFullScreen();
		fullscreen = true;
#endif

		// Now that basic GL setup is done, we can initialize the loading screen
		loadingScreen = std::make_unique<LoadingScreen>(window);
		if (!loadingScreen->Initialize())
		{
			Logger::Error("Failed to initialize loading screen", LogDestination::WINDOWS_CONSOLE);
			return;
		}

		loadingScreen->Render();

		// Force a buffer swap to ensure the loading screen is visible
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Initialize core systems that need to be ready before asset loading
		cameraManager = std::make_unique<CameraManager>(Config::windowWidth, Config::windowHeight);

		// Add audio and logic systems first since they're needed for asset loading
		registry->AddSystem<AudioSystem>(*registry);
		registry->AddSystem<LogicSystem>(*registry, *eventBus, *layerManager, *monoAPI);

		// Set up general OpenGL settings
		PopplioUtil::InitRand();
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		CrashHandler::Initialize();

		// Setup scripts
		PopplioScriptAPI::SetIntCall(*registry, *layerManager, *eventBus, *prefabManager, *cameraManager);
		AssetLoader::Setup(registry->GetSystem<AudioSystem>(), registry->GetSystem<LogicSystem>());

		

		// Load assets with progress reporting through the loading screen
		loadingScreen->LoadAssets([this](const std::function<void(float)>& updateProgress)
		{
			// Create batch loader
			AssetBatchLoader batchLoader(registry->GetSystem<AudioSystem>(), registry->GetSystem<LogicSystem>());

			// Initialize core systems first (20% of progress)
			updateProgress(0.0f);
			InitializeCoreSystems();
			updateProgress(0.2f);

			// Scan assets
			batchLoader.ScanAssets();

			// Start the loading process
			batchLoader.StartLoading([&updateProgress](float assetProgress) {
				// Asset loading represents 50% of total progress (from 20% to 70%)
				updateProgress(0.2f + (assetProgress * 0.5f));
			});

			// Process batches until complete
			while (batchLoader.ProcessBatch()) {
				// Each batch is processed, allowing UI updates between batches
			}

			// Initialize remaining systems (20% of progress)
			updateProgress(0.7f);
			InitializeRemainingSystems();
			updateProgress(0.9f);

			// Final initialization (10% of progress)
			FinalizeInitialization();
			updateProgress(1.0f);
		});

		



		// Wait for the splash screen to complete if it's active
		while (!loadingScreen->IsComplete()) {
			glfwPollEvents();
			loadingScreen->Render();

			// This gives a small delay to avoid maxing out the CPU
			std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
		}

		// Clean up loading screen resources
		loadingScreen.reset();

		Logger::Info("Engine initialization completed", LogDestination::WINDOWS_CONSOLE);
	}

	// New helper methods to split up the initialization process
	void Engine::InitializeCoreSystems()
	{
		Logger::Info("Initializing core systems...");

		// Initialize utility systems
		InputSystem::Init(window);
		PopplioUtil::InitRand();

		// Configure OpenGL settings
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Initialize crash handler
		CrashHandler::Initialize();

		// Initialize camera manager
		cameraManager = std::make_unique<CameraManager>(Config::windowWidth, Config::windowHeight);

		// Add necessary systems for asset loading
		registry->AddSystem<AudioSystem>(*registry);
		registry->AddSystem<LogicSystem>(*registry, *eventBus, *layerManager, *monoAPI);

		// Set up asset loading dependencies
		PopplioScriptAPI::SetIntCall(*registry, *layerManager, *eventBus, *prefabManager, *cameraManager);
		AssetLoader::Setup(registry->GetSystem<AudioSystem>(), registry->GetSystem<LogicSystem>());

		Logger::Info("Core systems initialized");
	}

	void Engine::InitializeRemainingSystems()
	{
		// Add remaining systems
		registry->AddSystem<MovementSystem>();
		registry->AddSystem<CollisionSystem>(*eventBus, *layerManager);
		registry->AddSystem<GravitySystem>();
		registry->AddSystem<CloneSystem>(*registry, *layerManager);
		registry->AddSystem<PrefabSyncSystem>(*registry, *prefabManager);
		registry->AddSystem<AnimationSystem>();
		registry->AddSystem<TransformationSystem>();
		registry->AddSystem<RenderSystem>(window, *layerManager, *cameraManager);
		registry->AddSystem<ParticleSystem>(*registry);
		registry->AddSystem<HierarchySystem>();
		registry->AddSystem<RuntimeSystem>(*registry, *eventBus, *monoAPI);
		registry->AddSystem<SceneManageSystem>(*this, *registry, *serializer, *monoAPI, *cameraManager);
		registry->AddSystem<QueueSystem>();
		registry->AddSystem<UISystem>(*cameraManager, *registry, window);

		// Initialize subsystems that need it
		registry->GetSystem<RenderSystem>().Initialize();
		registry->GetSystem<LogicSystem>().Initialization();
		registry->GetSystem<UISystem>().Initialize();

		Logger::Info("All systems initialized");
	}

	void Engine::FinalizeInitialization()
	{
		// Load initial scene
#ifndef IMGUI_DISABLE
		registry->AddSystem<Editor>(window, *registry, *serializer, *eventBus, *prefabManager, *layerManager, *cameraManager, *monoAPI);
		auto& editor = registry->GetSystem<Editor>();
		editor.Init();
		editor.LoadScene(Config::lastScene);
		registry->GetSystem<AnimationSystem>().StopAll();
#else
		serializer->LoadSceneFromFile(Config::startScene, *cameraManager);
		monoAPI->ReloadAssembly();
		GameSetup();
		GameInit();
#endif

		// Setup event subscriptions
		registry->GetSystem<CloneSystem>().SubscribeToEvents(*eventBus);
		registry->GetSystem<PrefabSyncSystem>().SubscribeToEvents(*eventBus);
		registry->GetSystem<AudioSystem>().SubscribeToEvents(*eventBus);

		// Set splash textures if needed
		loadingScreen->SetFirstSplashTexture("DigiPen_BLACK");
		loadingScreen->SetSecondSplashTexture("Popplio_TeamLogo");

		Logger::Info("Engine initialization finalized", LogDestination::WINDOWS_CONSOLE);
	}

	void Engine::Run()
	{
		Logger::Info("Engine loop started", LogDestination::WINDOWS_CONSOLE);

		while (!glfwWindowShouldClose(window))
		{
			//if (isPaused) continue;
			// update delta time and fps
			Popplio::PerformanceViewer::GetInstance()->start(PerformanceViewer::_MainEngine);
			timer.Update(framesPerSecond, 1.f);
			Popplio::PerformanceViewer::GetInstance()->start("total input");
			ProcessInput();
			Popplio::PerformanceViewer::GetInstance()->end("total input");
			Popplio::PerformanceViewer::GetInstance()->start("total update");
			Update();
			Popplio::PerformanceViewer::GetInstance()->end("total update");
			Popplio::PerformanceViewer::GetInstance()->start("total render");
			Render();
			Popplio::PerformanceViewer::GetInstance()->end("total render");
			Popplio::PerformanceViewer::GetInstance()->end(PerformanceViewer::_MainEngine);
		}

//#ifndef IMGUI_DISABLE
//		if (registry->GetSystem<Editor>().HasUnsavedChanges())
//		{
//            registry->GetSystem<Editor>().UnsavedPopup();
//		}
//#endif // !IMGUI_DISABLE

		eventBus->EmitEvent<PopplioUtil::OnAppQuit>();

		Logger::Info("Engine loop ended", LogDestination::WINDOWS_CONSOLE);
	}

//	void Engine::Setup()
//	{
//		// Add systems
//		loadingScreen->SetMessage("Initializing Systems...");
//		registry->AddSystem<MovementSystem>(); // translation
//		registry->AddSystem<AudioSystem>(*registry);
//		loadingScreen->CompleteStep("Audio System");
//
//		registry->AddSystem<CollisionSystem>(*eventBus, *layerManager);
//		registry->AddSystem<GravitySystem>(); // rigidbody
//		loadingScreen->CompleteStep("Physics");
//
//		registry->AddSystem<CloneSystem>(*registry, *layerManager);
//		registry->AddSystem<PrefabSyncSystem>(*registry, *prefabManager);
//		registry->AddSystem<AnimationSystem>();
//
//		loadingScreen->SetMessage("Loading Scripts...");
//		registry->AddSystem<LogicSystem>(*registry, *eventBus, *layerManager, *monoAPI); // logic system (scripts)
//		loadingScreen->CompleteStep("Scripts");
//		
//		registry->AddSystem<TransformationSystem>(); // transformation system
//		registry->AddSystem<RenderSystem>(window, *layerManager, *cameraManager); // sprites
//		registry->AddSystem<ParticleSystem>(*registry);
//		loadingScreen->CompleteStep("Graphics");
//
//		registry->AddSystem<HierarchySystem>();
//		registry->AddSystem<RuntimeSystem>(*registry, *eventBus, *monoAPI);
//        registry->AddSystem<SceneManageSystem>(*this, *registry, *serializer, *monoAPI, *cameraManager);
//		registry->AddSystem<QueueSystem>();
//		registry->AddSystem<UISystem>(*cameraManager, *registry,window);
//
//		// Initialize systems
//		loadingScreen->SetMessage("Initializing Renderer...");
//		registry->GetSystem<RenderSystem>().Initialize();
//		registry->GetSystem<LogicSystem>().Initialization();
//		registry->GetSystem<UISystem>().Initialize();
//
//		//get reference to systems
//		//MovementSystem& movementSystem = registry->GetSystem<MovementSystem>();
//		//ParticleSystem& particleSystem = registry->GetSystem<ParticleSystem>();
//
//		//connect movement system to particle system
//		//particleSystem.SetMovementSystem(&movementSystem);
//
//
//		// Mono
//		loadingScreen->SetMessage("Setting up scripting system...");
//		PopplioScriptAPI::SetIntCall(*registry, *layerManager, *eventBus, *prefabManager, *cameraManager); // set up internal call code
//
//		AssetLoader::Setup(registry->GetSystem<AudioSystem>(), registry->GetSystem<LogicSystem>());
//		
//		loadingScreen->SetMessage("Loading assets...");
//		AssetLoader::ReloadAssets();
//
//
//#ifndef IMGUI_DISABLE
//		registry->AddSystem<Editor>(window, *registry, *serializer, *eventBus, *prefabManager, *layerManager, *cameraManager, *monoAPI); // Editor
//		auto& editor = registry->GetSystem<Editor>();
//		editor.Init();
//		editor.LoadScene(Config::lastScene);
//
//		registry->GetSystem<AnimationSystem>().StopAll();
//#else
//		loadingScreen->SetMessage("Loading scene...");
//		serializer->LoadSceneFromFile(Config::startScene, *cameraManager);
//        monoAPI->ReloadAssembly();
//#endif // !IMGUI_DISABLE
//
//		// Perform the subscription of the events for all systems
//		registry->GetSystem<CloneSystem>().SubscribeToEvents(*eventBus);
//		registry->GetSystem<PrefabSyncSystem>().SubscribeToEvents(*eventBus);
//		registry->GetSystem<AudioSystem>().SubscribeToEvents(*eventBus);
//
//		loadingScreen->CompleteStep("Scene");
//		
//#ifdef IMGUI_DISABLE
//		//monoAPI->ReloadAssembly(); // reload assembly
//		GameSetup();
//		GameInit();
//#endif
//	}

	void Engine::ProcessInput()
	{
		Popplio::PerformanceViewer::GetInstance()->start("input");
		glfwPollEvents();

		// Toggle fullscreen/windowed mode
		if (InputSystem::IsKeyTriggered(GLFW_KEY_F1))
		{
			if (fullscreen)
			{
				SetWindowedMode();
			}
			else
			{
				SetFullScreen();
			}

			fullscreen = !fullscreen;
		}

		// Editor toggle input handling
#ifndef IMGUI_DISABLE
		Popplio::PerformanceViewer::GetInstance()->start("editor input");
        registry->GetSystem<Editor>().ProcessInput();
		Popplio::PerformanceViewer::GetInstance()->end("editor input");

#endif
		registry->GetSystem<UISystem>().ProcessInput();

		InputSystem::Update();
		Popplio::PerformanceViewer::GetInstance()->end("input");
	}

	void Engine::Update()
	{
		static bool wasPaused = false;

		//if (isPaused && !wasPaused)
		//{
		//	registry->GetSystem<AudioSystem>().PauseAllAudioInst();
		//	// might have issues if there is some audio paused already and not tracked
		//	registry->GetSystem<AnimationSystem>().PauseAll();
		//	//registry->GetSystem<AudioSystem>().PauseAllAudio();
		//	wasPaused = true;
		//}
		//else if (!isPaused && wasPaused)
		//{
		//	registry->GetSystem<AudioSystem>().ResumeAllAudioInst();
		//	// resumes already paused audio before pausing scene
		//	registry->GetSystem<AnimationSystem>().ResumeAll();
		//	//registry->GetSystem<AudioSystem>().ResumeAllAudio();
		//	wasPaused = false;
		//}

#ifndef IMGUI_DISABLE
		//if (isPaused)
		//{
		//	if (registry->GetSystem<Editor>().editorState == Editor::State::PLAYING)
		//	{
		//		registry->GetSystem<Editor>().PauseScene();
		//	}
		//	//return;
		//}
		//else
		//{
		//	if (registry->GetSystem<Editor>().editorState == Editor::State::PAUSED)
		//	{
		//		registry->GetSystem<Editor>().ResumeScene();
		//	}
		//}
#else
		if (isPaused && !wasPaused)
		{
			registry->GetSystem<AudioSystem>().PauseAllAudioInst();
			// might have issues if there is some audio paused already and not tracked
			registry->GetSystem<AnimationSystem>().PauseAll();
			//registry->GetSystem<AudioSystem>().PauseAllAudio();
			wasPaused = true;
		}
		else if (!isPaused && wasPaused)
		{
			registry->GetSystem<AudioSystem>().ResumeAllAudioInst();
			// resumes already paused audio before pausing scene
			registry->GetSystem<AnimationSystem>().ResumeAll();
			//registry->GetSystem<AudioSystem>().ResumeAllAudio();
			wasPaused = false;
		}
#endif // IMGUI_DISABLE
		
		// skip update if window is not focused and pause game
		if (isPaused) return;
		
		
		// Update the registry to process the entites that are waiting to be created/deleted
		Popplio::PerformanceViewer::GetInstance()->start("registry");
		registry->Update();
		Popplio::PerformanceViewer::GetInstance()->end("registry");

		// Game systems update
		Popplio::PerformanceViewer::GetInstance()->start("prefabs sync");
		registry->GetSystem<PrefabSyncSystem>().Update();
		Popplio::PerformanceViewer::GetInstance()->end("prefabs sync");

#ifndef IMGUI_DISABLE
		auto& editor = registry->GetSystem<Editor>();

		// Get game state from editor
		bool isPlaying = (editor.editorState == Editor::State::PLAYING);

		// Update editor if enabled
		//if (editor.IsEditorEnabled())
		//{
		//	editor.Update();
		//}

		Popplio::PerformanceViewer::GetInstance()->start("editor");
		editor.Update();
		Popplio::PerformanceViewer::GetInstance()->end("editor");

		// Only update game logic if playing
        if (isPlaying)
        {
			Popplio::PerformanceViewer::GetInstance()->start("editor runtime");
            editor.RunUpdate();
            Popplio::PerformanceViewer::GetInstance()->end("editor runtime");
        }
		else
		{
			Popplio::PerformanceViewer::GetInstance()->start("transformation");
			registry->GetSystem<TransformationSystem>().Update();
			Popplio::PerformanceViewer::GetInstance()->end("transformation");

			Popplio::PerformanceViewer::GetInstance()->start("hierarchy");
			registry->GetSystem<HierarchySystem>().Update();
			Popplio::PerformanceViewer::GetInstance()->end("hierarchy");
		}
#else
		if (!isSceneChanging)
		{
			Popplio::PerformanceViewer::GetInstance()->start("runtime");
			GameUpdate();
            Popplio::PerformanceViewer::GetInstance()->end("runtime");
		}
		else
		{
            isSceneChanging = false;
			GameChangeScene();
		}
#endif
	}

	void Engine::Render()
	{
		// skip render if window is not focused and pause game
        if (isPaused) return;

		// Window
		std::stringstream title;

		// Render to FBO
#ifndef IMGUI_DISABLE
		title << "Popplio Engine | FPS: " << framesPerSecond;
		glfwSetWindowTitle(window, title.str().c_str());

		auto& editor = registry->GetSystem<Editor>();

		editor.Render();

		// Update render system
		//PerformanceViewer::GetInstance()->start("rendering");
		//registry->GetSystem<RenderSystem>().Update();
		//PerformanceViewer::GetInstance()->end("rendering");

		//if (!editor.IsEditorEnabled()) editor.RunRender();
#else
		title << "BassNKick";
		glfwSetWindowTitle(window, title.str().c_str());

		// Game-only mode rendering - render directly to screen
		Popplio::PerformanceViewer::GetInstance()->start("runtime render");
		GameRender();
        Popplio::PerformanceViewer::GetInstance()->end("runtime render");
#endif

		// Swap buffers
		glfwSwapBuffers(window);
	}

	// +++++++++++++++++++++++++++++ //

	// Game functions

	// brought from EditorRun.cpp

	void Engine::GameSetup()
	{
		Logger::Info("Running setup...", LogDestination::IMGUI_CONSOLE);

		//registry->GetSystem<QueueSystem>().Clear(); // clear in case loaded from previous scene
		registry->GetSystem<SceneManageSystem>().Init(AssetStore::GetSceneId(Config::startScene));
		registry->GetSystem<AudioSystem>().Setup();
		registry->GetSystem<AudioSystem>().StopAllAudioInst();
		registry->GetSystem<AudioSystem>().StopAllAudio();
		registry->GetSystem<LogicSystem>().Setup();
		//registry->GetSystem<LogicSystem>().SubscribeToEvents(*eventBus);
        registry->GetSystem<AnimationSystem>().StartAll();

		// Initialize UI button actions
		registry->GetSystem<UISystem>().InitializeButtons();

		if (!(registry->GetSystem<QueueSystem>().IsProcessing())) registry->GetSystem<QueueSystem>().Process();
		registry->Update();
	}

	void Engine::GameInit()
	{
		Logger::Info("Initializing...", LogDestination::IMGUI_CONSOLE);
		registry->GetSystem<LogicSystem>().Init();
		//Logger::Info("Done! Game is running", LogDestination::IMGUI_CONSOLE);

		registry->GetSystem<AudioSystem>().Init();

		if (!(registry->GetSystem<QueueSystem>().IsProcessing())) registry->GetSystem<QueueSystem>().Process();

		registry->Update();
		Logger::Info("Done! Scene is running");
	}

	void Engine::GameUpdate() 
	{
		// Fixed Logic updates
		PerformanceViewer::GetInstance()->start("fixed logic");
		registry->GetSystem<LogicSystem>().FixedUpdate();
		PerformanceViewer::GetInstance()->end("fixed logic");

		// Logic updates
		PerformanceViewer::GetInstance()->start("logic");
		registry->GetSystem<LogicSystem>().Update();
		PerformanceViewer::GetInstance()->end("logic");

		// Particle updates
        PerformanceViewer::GetInstance()->start("particles");
        registry->GetSystem<ParticleSystem>().Update(timer.GetFixedDeltaTime());
        PerformanceViewer::GetInstance()->end("particles");

        // Transform updates
		Popplio::PerformanceViewer::GetInstance()->start("transformation");
		registry->GetSystem<TransformationSystem>().Update();
		Popplio::PerformanceViewer::GetInstance()->end("transformation");

		Popplio::PerformanceViewer::GetInstance()->start("hierarchy");
		registry->GetSystem<HierarchySystem>().Update();
		Popplio::PerformanceViewer::GetInstance()->end("hierarchy");

		// Physics updates
		//PerformanceViewer::GetInstance()->start("gravity");
		//registry->GetSystem<GravitySystem>().Update();
		//PerformanceViewer::GetInstance()->end("gravity");

		PerformanceViewer::GetInstance()->start("Movement");
		registry->GetSystem<MovementSystem>().Update(static_cast<float>(timer.GetFixedDeltaTime()));
		PerformanceViewer::GetInstance()->end("Movement");

		// Collision updates
		PerformanceViewer::GetInstance()->start("collision");
		registry->GetSystem<CollisionSystem>().Update(*registry, static_cast<float>(timer.GetFixedDeltaTime()));
		PerformanceViewer::GetInstance()->end("collision");

		// resolve queue
		PerformanceViewer::GetInstance()->start("queue");
		if (!(registry->GetSystem<QueueSystem>().IsProcessing())) registry->GetSystem<QueueSystem>().Process();
		PerformanceViewer::GetInstance()->end("queue");

		// UI
		PerformanceViewer::GetInstance()->start("ui");
		registry->GetSystem<UISystem>().Update();
		PerformanceViewer::GetInstance()->end("ui");
	}

	//void Engine::GameFixedUpdate() // todo: implement
	//{
	//	// Logic updates
	//	PerformanceViewer::GetInstance()->start("fixed logic");
	//	registry->GetSystem<LogicSystem>().FixedUpdate();
	//	PerformanceViewer::GetInstance()->end("fixed logic");

	//	// Transform updates
	//	Popplio::PerformanceViewer::GetInstance()->start("fixed transformation");
	//	registry->GetSystem<TransformationSystem>().Update();
	//	Popplio::PerformanceViewer::GetInstance()->end("fixed transformation");

	//	Popplio::PerformanceViewer::GetInstance()->start("fixed hierarchy");
	//	registry->GetSystem<HierarchySystem>().Update();
	//	Popplio::PerformanceViewer::GetInstance()->end("fixed hierarchy");

	//	// Physics updates
	//	PerformanceViewer::GetInstance()->start("fixed gravity");
	//	registry->GetSystem<GravitySystem>().Update();
	//	PerformanceViewer::GetInstance()->end("fixed gravity");

	//	PerformanceViewer::GetInstance()->start("fixed Movement");
	//	registry->GetSystem<MovementSystem>().Update(static_cast<float>(timer.GetFixedDeltaTime()));
	//	PerformanceViewer::GetInstance()->end("fixed Movement");

	//	// Collision updates
	//	PerformanceViewer::GetInstance()->start("fixed collision");
	//	registry->GetSystem<CollisionSystem>().Update(*registry, static_cast<float>(timer.GetFixedDeltaTime()));
	//	PerformanceViewer::GetInstance()->end("fixed collision");

	//	PerformanceViewer::GetInstance()->start("queue");
	//	if (!(registry->GetSystem<QueueSystem>().IsProcessing())) registry->GetSystem<QueueSystem>().Process();
	//	PerformanceViewer::GetInstance()->end("queue");
	//}

	void Engine::GameRender()
	{
		// Bind to default framebuffer (screen)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Config::windowWidth, Config::windowHeight);

		// Update render system
		PerformanceViewer::GetInstance()->start("rendering");
		registry->GetSystem<RenderSystem>().Update();
		PerformanceViewer::GetInstance()->end("rendering");

		// Animation update
		PerformanceViewer::GetInstance()->start("animation");
		registry->GetSystem<AnimationSystem>().Update(static_cast<float>(timer.GetFixedDeltaTime()));
		PerformanceViewer::GetInstance()->end("animation");

		// Particle update
		PerformanceViewer::GetInstance()->start("particles");
		registry->GetSystem<ParticleSystem>().Update(timer.GetFixedDeltaTime());
		PerformanceViewer::GetInstance()->end("particles");

        // Audio update
		PerformanceViewer::GetInstance()->start("audio");
		registry->GetSystem<AudioSystem>().Update();
		PerformanceViewer::GetInstance()->end("audio");
	}

	void Engine::GameEnd()
	{
		registry->GetSystem<AudioSystem>().End();
		registry->GetSystem<LogicSystem>().End();

		if (!(registry->GetSystem<QueueSystem>().IsProcessing())) registry->GetSystem<QueueSystem>().Process();

		registry->Update();
	}

	void Engine::GameChangeScene()
	{
		//if (!isSceneChanging)
		//{
  //          isSceneChanging = true;
		//	return;
		//}

		if (isSceneChanging)
		{
			Logger::Info("Changing scene...");
			GameEnd();

			// TODO clear instanced entities except persistent ones
			registry->ClearInstancedEntities();
		}
		else
		{
			//monoAPI->ReloadAssembly();

			// new scene setup
			registry->GetSystem<LogicSystem>().Setup();
			registry->GetSystem<AnimationSystem>().StartAll();
            registry->GetSystem<QueueSystem>().Process();
			registry->Update();

			registry->GetSystem<LogicSystem>().Init();
			registry->GetSystem<AudioSystem>().Init();
			registry->GetSystem<QueueSystem>().Process();
			registry->Update();
		}

        //isSceneChanging = false;
	}

	// +++++++++++++++++++++++++++++ //

	void Engine::Destroy()
	{
#ifndef IMGUI_DISABLE
		registry->GetSystem<Editor>().Terminate();
#else
        GameEnd();
#endif // IMGUI_DISABLE
		AssetStore::Clear();
		registry->GetSystem<AudioSystem>().Terminate();
		PerformanceViewer::DestroyInstance();
		UnhookWindowsHookEx(keyboardHook);
		glfwDestroyWindow(window);
		glfwTerminate();
	}
}