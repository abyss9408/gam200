/******************************************************************************/
/*!
\file   Editor.cpp
\author Team Popplio
\author			Ng Jun Heng Shawn
\co-author		Pearl | Bryan
\contribution	Shawn - 40% | Pearl - 15% | Bryan - 45%
\par    Course : CSD2401
\par    Section : A
\date   2025/02/05 03:15:00 PM (Last Edit)
\brief
		This is the source file for engine GUI
		Implements imgui library
		Implements imguizmo library
		Base source file for editor functions
		These include:
		Editor runtime, Graphical User Interface, etc.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#ifndef IMGUI_DISABLE
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/implot.h>
#include <imgui/implot_internal.h>
#include <font awesome/IconsFontAwesome6.h>

#include "../ECS/ECS.h"
#include "../Engine/Engine.h"
#include "../EventBus/EventBus.h"
#include "../Serialization/Serialization.h"
#include "../Camera/EditorCamera.h"
#include "../Camera/CameraManager.h"
#include "../AssetStore/AssetStore.h"
#include "../PrefabManagement/PrefabManager.h"
#include "../PrefabManagement/PrefabInstanceComponent.h"
#include "../Layering/LayerManager.h"
#include "../Utilities/Comparisons.h"

#include "Changes.h"
#include "../Utilities/Any.h"

//wip
#include <ImGuizmo/ImGuizmo.h>

#include <filesystem>


#define POPPLIO_TEMP_PATH "Temp"
#define POPPLIO_AUTO_SAVE_PATH "AutoSave"
#define POPPLIO_TEMP_EXTENSION ".mimikyu"

#define POPPLIO_AUTO_SAVE_TIME 300.f // 5 minutes

namespace Popplio
{

	class Editor : public System
	{
	private:
		Registry& reg;
		GLFWwindow* win;
		Serializer& serializer;

		EventBus& eventBus;
		PrefabManager& prefabManager;
		PopplioScriptAPI::MonoAPI& mono;

		// selected entity
		std::optional<Entity> selectedEntity;

		float rainbowTime = 0.0f;
		const float RAINBOW_SPEED = 1.0f;
		const float BORDER_THICKNESS = 2.0f;
		ImVec4 GetRainbowColor(float offset) const;

		// Layer Editing 
		LayerManager& layerManager;
		std::string selectedLayer;

		// Scene Viewport
		GLuint sceneFramebuffer;
		GLuint sceneRenderTexture;
		int sceneViewportWidth;
		int sceneViewportHeight;

		// Game Viewport
		GLuint gameFramebuffer;
		GLuint gameRenderTexture;
		int gameViewportWidth;
		int gameViewportHeight;

		// Prefab editing
		bool showPrefabNameDialog = false;
		std::optional<std::unique_ptr<Prefab>> currentPrefab;
		char prefabNameBuffer[256];
		std::string currentPrefabPath;

		// Prefab List
		bool showPrefabList = false;
		bool showPrefabContextMenu = false;
		bool showDeleteConfirmation = false;
		std::string selectedPrefabName;

		// Scene Management
		std::string currentScenePath;
		std::list<std::string> recentScenes;

		std::string tempScenePath{ static_cast<std::string>(POPPLIO_TEMP_PATH) + "/scene" + POPPLIO_TEMP_EXTENSION };

		// Chart Management
		std::string currentChartPath;
		bool hasUnsavedChanges_chart = false;
		std::list<std::string> recentCharts;

		std::string tempChartPath{ static_cast<std::string>(POPPLIO_TEMP_PATH) + "/chart" + POPPLIO_TEMP_EXTENSION };

		// Add new camera-related members
		CameraManager& cameraManager;
		float cameraSpeed = 500.0f;  // Units per second
		float zoomSpeed = 0.1f;
		bool isDraggingCamera = false;
		PopplioMath::Vec2f lastMousePos;

		// Viewport state tracking
		bool isViewportHovered = false;
		bool isGameViewportHovered = false;
		ImVec2 viewportBounds[2];

		// Editor mode toggling
		bool isEditorEnabled = true;  // Controls if editor UI is visible

		// Window toggles
		bool showLogWindow = true;
		bool showEntityList = true;
		bool showPerformanceTesting = true;
		bool showComponentEditor = true;
		bool showSceneViewport = true;
		bool showGameViewport = true;
		bool showSimulationControls = true;
		bool showPerformanceGUI = true;
		bool showAssetStoreGUI = true;
		bool showChartEditor = true;
		bool showScriptDirectoryGUI = true;
		bool showPrefabEditor = true;
		bool showPrefabListGUI = true;
		bool showLayerWindow = true;
		bool showScriptDirectory = true;
		bool showEntityLayerControls = true;
		bool showGizmoTools = true;
		bool showCameraController = true;
		//bool showGuizmoEditor = true;

		// Change detection
		std::optional<Entity> lastNewEntity; // last entity created / cloned

		bool hasUnsavedChanges = false;

		// back = top of the stack
		// front = bottom of the stack
		std::deque<SceneChangeEvent<std::any>> undoStack;
		std::deque<SceneChangeEvent<std::any>> redoStack;

		// track last saved change for save dialog on exit / etc.
		SceneChangeEvent<std::any>* lastSavedChange = nullptr;

		// AutoSave
		bool autoSaveEnabled = true;
		float autoSaveInterval = POPPLIO_AUTO_SAVE_TIME; // in seconds
		float timeSinceLastAutoSave = 0.f;

		// In-Game Camera frustum
		bool showGameCameraFrustum = true;

		// debug
		bool debugMode = false;

		// Functions // +++++++++++++++++++++++++++++++++++++++++++++++ //

		// Changes

		/*
		*	handler for scene changes
		*/
		void HandleSceneChange(SceneChangeEvent<std::any>& event);
		/*
		*	set last change event
		*/
		void SetLastSavedChange();

		// Undo / Redo
		/*
		*	show undo / redo buttons
		*/
		void RenderUndoRedoMenu();


		template <typename T>
		/*
		*	undo component change
		*/
		void UndoComponent(SceneChangeEvent<std::any>& latestChange);
		template <typename T>
		/*
		*	redo component change
		*/
		void RedoComponent(SceneChangeEvent<std::any>& latestUndo);

		/*
		*	undo entity change
		*/
		void UndoEntity(SceneChangeEvent<std::any>& latestChange);
		/*
		*	redo entity change
		*/
		void RedoEntity(SceneChangeEvent<std::any>& latestUndo);

		/*
		*	undo last change
		*/
		void Undo();
		/*
		*	redo last undo
		*/
		void Redo();

		// AutoSave
		/*
		*	auto save scene
		*/
		void AutoSaveUpdate();

		// Style
		void SetUpImGuiStyle();

		// Dockspace
		void CreateDockspace();

		// Initialize framebuffer and texture
		void InitializeFramebuffer(GLuint& framebuffer, GLuint& texture, int width, int height);

		// Cleanup
		void CleanupFramebuffers();

		// Windows rendering
		void RenderLogWindow();
		void RenderEntityList();
		void RenderPerformanceTestingControls();
		void RenderEntityInfoSection(Entity& entity);
		void RenderComponentEditor();
		void RenderSceneViewport();
		void RenderGameViewport();
		void RenderSimulationControls();
		void RenderPerformanceGUI();
		void RenderAssetStoreGUI();

		/**
		 * @brief Renders the main chart editor interface including controls, timeline, and note lanes
		 *
		 * This function handles the complete UI rendering for the chart editor, including:
		 * - Audio file selection and playback controls
		 * - BPM and time signature controls
		 * - Timeline visualization with measure markers
		 * - Note placement grid with multiple lanes
		 * - Interaction handling for note placement and deletion
		 */
		void RenderChartEditor();
		void RenderScriptDirectoryGUI();
		void RenderLayerMatrix();

		// Logger
		void LogClearOnRun();

		// Menu Bar
		void RenderReloadMonoMenu();
		void RenderDebugMenu();
		//void RenderGuizmoEditor();

		// Main Menu Bar
		void RenderMainMenuBar();

		void RenderWindowToggleMenu();

		// Entity editing
		void RenderSelectedEntity();

		// Components editing
		template <typename T>
		void AddComponentOption(Entity& entity, const char* componentName);

		//CUSTOM FOR BOX COLLIDER TESTING
		template <typename T>
		void AddCustomComponentOption(Entity& entity, const char* componentName);

		template <typename T>
		void AddComponentOption(Prefab& prefab, const char* componentName);

		void AddComponentPopup(Entity entity);
		void AddComponentPopup(Prefab& prefab);

		template <typename T>
		void RenderComponentEditor(Entity& entity);

		template <typename T>
		void RenderComponentEditor(Prefab& prefab);

		template <typename T>
		void RenderComponentHeader(const char* componentName, Entity& entity);

		template <typename T>
		void RenderComponentHeader(const char* componentName, Prefab& prefab);

		template <typename T>
		void EditComponent(T& component);

		template <>
		void EditComponent(ActiveComponent& active);
		template <>
		void EditComponent(TransformComponent& transform);
		template <>
		void EditComponent(RenderComponent& render);
		template <>
		void EditComponent(RigidBodyComponent& rigidBody);
		template <>
		void EditComponent(BoxColliderComponent& boxCollider);
		template <>
		void EditComponent(TextComponent& textComp);
		template <>
		void EditComponent(LogicComponent& logic);
		template <>
		void EditComponent(AudioComponent& audio);
		template <>
		void EditComponent(PrefabInstanceComponent& prefanInstComp);
		template <>
		void EditComponent(AnimationComponent& animation);
		template<>
		void EditComponent(ParentComponent& parent);
		template<>
		void EditComponent(PersistOnLoadComponent& persist);
		template<>
		void EditComponent(UIComponent& ui);
		template<>
		void EditComponent(ParticleComponent& particles);

		//Add edit animation comp?

		void AddEntityPopup();
		void CreateSlider();
		void ShowNineSliceEditor(RenderComponent& render);
		void DrawNineSlicePreview(RenderComponent& render);
		//void ClearInstancingEntities();

		// Prefab editing
		void RenderPrefabEditor();
		bool SaveCurrentPrefab();
		void LoadPrefabFromFile(const std::string& path);
		void CreateNamedPrefab(const std::string& name);
		bool IsValidPrefabName(const std::string& name);
		void RenderPrefabList();
		void RenderPrefabListMenu();
		void OnPrefabSelected(const std::string& prefabName);
		void DeleteSelectedPrefab();

		// other prefab methods
		void InstantiatePrefab();
		void RegisterCurrentPrefab();

		/**
		 * @brief Finds the closest entity to a given world position
		 * @param worldPos The position in world coordinates to search from
		 * @return Index or ID of the closest entity found
		 */
		int FindClosestEntity(const PopplioMath::Vec2f& worldPos);
		/**
		 * @brief Converts screen coordinates to world space coordinates
		 * @param screenPos The screen position in pixels (likely from ImGui)
		 * @return The corresponding position in world space
		 */
		PopplioMath::Vec2f ScreenToWorldPosition(const ImVec2& screenPos) const;
		PopplioMath::Vec2f WorldToScreenPosition(const PopplioMath::Vec2f& worldPos) const;

		PopplioMath::Vec2f ScreenToWorldPositionGame(const ImVec2& screenPos) const;


		// Layer management UI methods
		void RenderLayerWindow();
		void RenderAddLayerPopup();
		void RenderLayerList();
		void RenderEntityLayerControls();

		// Scene management
		void RenderSceneMenu();
		void HandleNewScene();
		void CreateNewScene();
		void SaveSceneMenu();
		void SaveSceneAsMenu();

		std::string GetScenesDirectory();
		std::string GetCurrentSceneName();

		// Editor Camera controls
		void HandleCameraInput();
		bool IsMouseInViewport() const;
		PopplioMath::Vec2f GetMouseViewportPosition() const;

		// Editor check if entity kill queue is done
		bool isEnding{ false };
		bool isStarting{ false };
		//bool isChanging{ false };

		//void HandleButtonInput();
		//void HandleButtonInputWhenEditorDisabled();

		void HandleEntitySelection();
		void HandleEntityDeletion();
		/**
		* @brief Handles the ImGuizmo manipulation of transform components for the selected entity
		* @details Updates transform components (position, rotation, scale) based on ImGuizmo manipulation.
		* Converts between 3x3 and 4x4 matrices for view/projection and handles transformation decomposition
		*/
		void HandleGuizmo();

		void RenderToolBar();

		// Guizmo
		//ImGuizmo::OPERATION currentGizmoOperation;
		//bool ImGuizmoActive;
		//void ImGuizmoLogic();

		enum class GuizmoTool
		{
			None = -1,
			Translate = ImGuizmo::OPERATION::TRANSLATE,
			Rotate = ImGuizmo::OPERATION::ROTATE,
			Scale = ImGuizmo::OPERATION::SCALE
		} currentTool = GuizmoTool::None;

		// Guizmo Toolbar
		static constexpr float TOOLBAR_HEIGHT = 32.0f;
		static constexpr float TOOLBAR_PADDING = 4.0f;
		static constexpr float BUTTON_SIZE = 30.0f;
		static constexpr float BUTTON_SPACING = 10.0f;

		// Add helper method for toolbar buttons
		void RenderToolButton(const char* icon, const char* tooltip, GuizmoTool toolType);

		// GLFW drag & drop callback
		void HandleAssetDragDrop();
		void ProcessDraggedAsset(const std::filesystem::path& sourcePath);
		bool IsValidAssetType(const std::string& extension);
		static void drop_callback(GLFWwindow* window, int count, const char** paths);

		// Parent-Child relationship helper functions
		void RenderEntityNode(Entity& entity, std::map<int, std::vector<Entity>>& hierarchy);
		bool WouldCreateCycle(Entity potentialChild, Entity potentialParent);

		// Chart Management
		/**
		 * @brief Opens a dialog to create a new chart or load an existing one
		 *
		 * Allows selection of either:
		 * - .wav files to create a new chart
		 * - .grookey files to load existing charts
		 * Creates appropriate data structures and initializes editor state based on selection
		 */
		void OpenNewChartDialog();

		/**
		 * @brief Creates a new chart instance with default settings
		 *
		 * Requires an audio file to be loaded first. Initializes:
		 * - Chart metadata (name, length, time signature)
		 * - Default save path
		 * - Editor state (playhead position, notes)
		 *
		 * @pre isAudioLoaded must be true
		 */
		void CreateNewChart();

		/**
		 * @brief Handles the chart saving process
		 *
		 * If currentChartPath exists, saves to that location
		 * Otherwise opens a Save As dialog for the user to choose location
		 */
		void SaveChartMenu();

		/**
		 * @brief Saves the current chart state to a file
		 *
		 * @param path The file path where the chart should be saved
		 *
		 * Updates chart metadata and converts editor note data to chart format before saving
		 * Creates a .grookey file containing:
		 * - Chart metadata (name, audio file, time signature, BPM)
		 * - Note data (position, type, length for hold notes)
		 *
		 * @pre currentChart must not be null
		 * @pre isAudioLoaded must be true
		 */
		void SaveChart(const std::string& path);

		/**
		 * @brief Opens a dialog to load an existing chart
		 *
		 * Allows selection of:
		 * - .wav files to create new chart
		 * - .grookey files to load existing chart
		 * Handles file selection and delegates to appropriate loading function
		 */
		void OpenLoadChartDialog();

		/**
		 * @brief Loads a chart from a .grookey file
		 *
		 * @param path Path to the .grookey file to load
		 *
		 * Processes:
		 * 1. Clears existing chart data
		 * 2. Loads chart metadata and note data
		 * 3. Converts chart format notes to editor format
		 * 4. Loads associated audio file
		 * 5. Updates editor state
		 */
		void LoadGrookeyChart(const std::string& path);

		// Chart playback state
		bool m_isPlaying{ false };
		double m_playheadPosition{ 0.0 };
		double m_lastFrameTime{ 0.0 };
		double bpm{ 120 };
		int numerator{ 4 };
		int denominator{ 4 };
		int totalMeasures = 128;

		std::vector<Note> notes;

		// Audio-related members
		std::string selectedAudioFile;
		AudioComponent m_audioComponent;
		bool isAudioLoaded = false;

		// Audio file selection
		/**
		 * @brief Renders the audio file selection interface
		 *
		 * Displays:
		 * - Current audio file path
		 * - File browser button
		 * - Playback controls (play/pause, stop) when audio is loaded
		 * - Warning message when no audio is loaded
		 */
		void RenderAudioFileSelector();

		/**
		 * @brief Loads and initializes an audio file for the chart
		 *
		 * @param path Path to the audio file to load
		 *
		 * Handles:
		 * - Audio instance creation and cleanup
		 * - Total measures calculation based on audio length
		 * - Volume initialization
		 * - Playback state reset
		 *
		 * @throws May throw exceptions for file access or audio processing errors
		 */
		void LoadAudioFile(const std::string& path);
		int m_currentAudioInstance = -1;

		/**
		 * @brief Toggles audio playback between playing and paused states
		 *
		 * Updates both the audio playback and the editor's playhead state
		 *
		 * @pre m_currentAudioInstance must be valid (>= 0)
		 */
		void ToggleAudioPlayback();

		/**
		 * @brief Stops audio playback and resets playhead position
		 *
		 * Handles:
		 * - Stopping audio playback
		 * - Resetting playhead to start
		 * - Updating playback state flags
		 */
		void StopAudio();

		bool isChartPlaying = false;  // Separate from main editor state
		float chartPlaybackTime = 0.0f;  // Current playback time in seconds

		// In-game camera controls
		void RenderCameraControllerWindow();
		void DrawGameCameraFrustum();

		// Saving entities as prefabs
		void RenderPrefabUI(Entity& entity);
		void ShowSaveAsPrefabDialog(Entity& entity, bool* popupShown);
		void RenderPrefabPreview(Entity const& entity);

		// Entity reordering
		std::optional<Entity> draggedEntity;
		std::optional<Entity> dropTargetEntity;
		bool draggedIsAboveTarget = false;
		bool isReorderingMode = false; // Set to true when Shift key is held during drag

		void HandleEntityReordering(Entity& draggedEntity, Entity& targetEntity, bool insertBefore);
	public:
		enum State
		{
			EDITING = 0,
			PLAYING,
			PAUSED,
			STOPPED,
			CHANGING
		};

		ImVec2 viewportBoundsGame[2];

		// Editor state
		State editorState{ EDITING };

		// Change detection
		std::optional<Entity> latestNewEntity; // newest entity created / cloned // exposed for CloneSystem.cpp to set change

		Editor(GLFWwindow* window, Registry& registry, Serializer& serializer, EventBus& bus, PrefabManager& pManager,
			LayerManager& layerMgr, CameraManager& camMgr, PopplioScriptAPI::MonoAPI& monoAPI)
			: win(window), reg(registry), serializer(serializer),
			cameraManager(camMgr), eventBus(bus), prefabManager(pManager),
			layerManager(layerMgr), mono(monoAPI), gameFramebuffer(0),
			gameRenderTexture(0), gameViewportWidth(0), gameViewportHeight(0),
			sceneFramebuffer(0), sceneRenderTexture(0), sceneViewportWidth(0),
			sceneViewportHeight(0), undoStack(), redoStack()
		{
			memset(prefabNameBuffer, 0, sizeof(prefabNameBuffer));
		}

		// Editor
		// Init
		void Init();
		// Input
		void ProcessInput();
		// Start of main loop
		void Update();
		// End of main loop
		void Render();
		// Terminate
		void Terminate();

		void AddManyInstancingEntities();

		// Getters
		GLuint GetSceneFramebuffer() const { return sceneFramebuffer; }
		GLuint GetGameFramebuffer() const { return gameFramebuffer; }
		int GetSceneViewportWidth() const { return sceneViewportWidth; }
		int GetSceneViewportHeight() const { return sceneViewportHeight; }
		int GetGameViewportWidth() const { return gameViewportWidth; }
		int GetGameViewportHeight() const { return gameViewportHeight; }

		// Setters
		void SetSceneViewportSize(int width, int height) { sceneViewportWidth = width; sceneViewportHeight = height; }
		void SetGameViewportSize(int width, int height) { gameViewportWidth = width; gameViewportHeight = height; }

		// Resize framebuffer texture
		void ResizeFramebufferTexture(GLuint texture, int width, int height);

		// Run Logic
		void RunSetup();
		void RunInit();
		void RunUpdate();
		void RunRender();
		void RunEnd();
		void RunFixedUpdate();

		void StartScene();
		void PauseScene();
		void ResumeScene();
		void EndScene();
		void ChangeScene(std::string const& scene = "");

		void SaveScene(const std::string& path);
		void LoadScene(const std::string& path);

		void ToggleEditor();
		void ToggleRun();
		bool IsEditorEnabled() const { return isEditorEnabled || editorState == State::EDITING; }
		bool IsEditorRunning() const {
			return editorState == State::PLAYING ||
				editorState == State::PAUSED || editorState == State::CHANGING;
		}

		EditorCamera& GetCamera();
		const EditorCamera& GetCamera() const;

		// Scene Management
		void OnSceneChange(SceneChangeEvent<std::any>& event);

		bool HasUnsavedChanges() { return hasUnsavedChanges; }
		void UnsavedPopup();

		void ResetSelectedEntity();
		void ResetSelectedEntity(Entity const& e);
	};

	template <typename T>
	void Editor::AddComponentOption(Entity& entity, const char* componentName)
	{
		if (!entity.HasComponent<T>() && ImGui::MenuItem(componentName))
		{
			entity.AddComponent<T>();
			// emit change event
			eventBus.EmitEvent<SceneChangeEvent<std::any>>(*selectedEntity, entity.GetComponent<T>(),
				entity.GetComponent<T>(), ChangeAction::ADD_COMPONENT);
		}
	}

	// unimplemented // TODO
	template <typename T>
	void Editor::AddCustomComponentOption(Entity& entity, const char* componentName)
	{
		if (!entity.HasComponent<T>() && ImGui::MenuItem(componentName))
		{

		}
	}

	//TESTING FOR BOX COLLIDER CUSTOM
	template <>
	inline void Editor::AddCustomComponentOption<BoxColliderComponent>(Entity& entity, const char* componentName)
	{
		// Check if entity doesn't have this component type AND the menu item was clicked
		if (!entity.HasComponent<BoxColliderComponent>() && ImGui::MenuItem(componentName))
		{
			// First add the component to the entity
			entity.AddComponent<BoxColliderComponent>();

			// Get a reference to the newly created component
			auto& collider = entity.GetComponent<BoxColliderComponent>();

			// If entity has transform component, scale the collider
			if (entity.HasComponent<TransformComponent>())
			{
				// Get the scale from the existing transform component in the entity
				auto& transform = entity.GetComponent<TransformComponent>();
				collider.size.x *= transform.scale.x;
				collider.size.y *= transform.scale.y;
			}
			// emit change event
			eventBus.EmitEvent<SceneChangeEvent<std::any>>(*selectedEntity, entity.GetComponent<BoxColliderComponent>(),
				entity.GetComponent<BoxColliderComponent>(), ChangeAction::ADD_COMPONENT);
		}
	}

	template <typename T>
	void Editor::AddComponentOption(Prefab& prefab, const char* componentName)
	{
		bool modified = false;

		if (!prefab.HasComponent<T>() && ImGui::MenuItem(componentName))
		{
			prefab.AddComponent<T>();
			modified = true;
		}

		if (modified)
		{
			prefabManager.UpdatePrefab(prefab.GetName(),
				std::make_unique<Prefab>(prefab), std::type_index(typeid(T)));

			// emit change event // TODO: make work for prefabs
			//eventBus.EmitEvent<SceneChangeEvent<std::any>>(prefab, prefab.GetComponent<T>(),
			//	prefab.GetComponent<T>(), ChangeAction::ADD);
		}
	}

	template <typename T>
	void Editor::EditComponent(T& component)
	{
		// Default implementation (can be empty or display a message)
		static_cast<void>(component);
		ImGui::Text("No editor available for this component type");
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

	// Render

	template <typename T>
	void Editor::RenderComponentEditor(Entity& entity)
	{
		if (entity.HasComponent<T>())
		{
			const char* componentName = typeid(T).name();

			// Handle different component types
			if constexpr (std::is_same_v<T, PrefabInstanceComponent>)
			{
				componentName = "Prefab Instance";
			}
			else if constexpr (std::is_same_v<T, ActiveComponent>)
			{
				componentName = "Active";
			}
			else if constexpr (std::is_same_v<T, TransformComponent>)
			{
				componentName = "Transform";
			}
			else if constexpr (std::is_same_v<T, RigidBodyComponent>)
			{
				componentName = "RigidBody";
			}
			else if constexpr (std::is_same_v<T, BoxColliderComponent>)
			{
				componentName = "Box Collider";
			}
			else if constexpr (std::is_same_v<T, RenderComponent>)
			{
				componentName = "Render";
			}
			else if constexpr (std::is_same_v<T, TextComponent>)
			{
				componentName = "Text";
			}
			else if constexpr (std::is_same_v<T, AnimationComponent>)
			{
				componentName = "Animation";
			}
			else if constexpr (std::is_same_v<T, AudioComponent>)
			{
				componentName = "Audio";
			}
			else if constexpr (std::is_same_v<T, LogicComponent>)
			{
				componentName = "Script";
			}
			else if constexpr (std::is_same_v<T, ParentComponent>)
			{
				componentName = "Parent";
			}
			else if constexpr (std::is_same_v<T, PersistOnLoadComponent>)
			{
				componentName = "Persist On Load";
			}
			else if constexpr (std::is_same_v<T, UIComponent>)
			{
				componentName = "UI";
			}
			else if constexpr (std::is_same_v<T, ParticleComponent>)
			{
				componentName = "Particles";
			}
			else
			{
				Logger::Warning("Unsupported component to render");
			}
			// add more component types as needed
			RenderComponentHeader<T>(componentName, entity);
		}
	}

	template <typename T>
	void Editor::RenderComponentEditor(Prefab& prefab)
	{
		if (prefab.HasComponent<T>())
		{
			auto& component = prefab.GetComponent<T>();
			auto originalValues = component;

			RenderComponentHeader<T>(typeid(T).name(), prefab);

			// Check if values changed
			if (!CompareComponents(component, originalValues))
			{
				// Values changed, update the prefab
				prefabManager.UpdatePrefab(prefab.GetName(),
					std::make_unique<Prefab>(prefab), std::type_index(typeid(T)));
			}
		}
	}

	template <typename T>
	void Editor::RenderComponentHeader(const char* componentName, Entity& entity)
	{
		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
		bool opened = ImGui::TreeNodeEx(componentName, treeNodeFlags);

		// Add remove component button
		ImGui::SameLine(ImGui::GetWindowWidth() - 30);

		// Special handling for ParentComponent
		if constexpr (std::is_same_v<T, ParentComponent>)
		{
			auto& parentComp = entity.GetComponent<ParentComponent>();
			bool hasChildren = !parentComp.children.empty();
			bool hasParent = parentComp.parent.GetId() != -1;

			// If component can't be removed, show disabled button with tooltip
			if (hasChildren || hasParent)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f); // Gray out the button
				ImGui::Button(("X##" + std::string(componentName)).c_str(), ImVec2(20, 20));
				ImGui::PopStyleVar();

				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					if (hasChildren && hasParent)
						ImGui::Text("Cannot remove: Entity has both children and a parent");
					else if (hasChildren)
						ImGui::Text("Cannot remove: Entity has children");
					else
						ImGui::Text("Cannot remove: Entity has a parent");
					ImGui::EndTooltip();
				}
			}
			else
			{
				if (ImGui::Button(("X##" + std::string(componentName)).c_str(), ImVec2(20, 20)))
				{
					entity.RemoveComponent<T>();
					ImGui::TreePop();
					return;
				}
			}
		}
		else if constexpr (std::is_same_v<T, TransformComponent> || std::is_same_v<T, ActiveComponent>)
		{
			// These components cannot be removed
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
			ImGui::Button(("X##" + std::string(componentName)).c_str(), ImVec2(20, 20));
			ImGui::PopStyleVar();
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Cannot remove %s component as it is required", componentName);
			}
		}
		else
		{
			if (ImGui::Button(("X##" + std::string(componentName)).c_str(), ImVec2(20, 20)))
			{
				entity.RemoveComponent<T>();
				ImGui::TreePop();
				return;
			}
		}

		if (opened)
		{
			EditComponent(entity.GetComponent<T>());
			ImGui::TreePop();
		}
	}

	template <typename T>
	void Editor::RenderComponentHeader(const char* componentName, Prefab& prefab)
	{
		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
		bool opened = ImGui::TreeNodeEx(componentName, treeNodeFlags);

		// Add remove component button
		ImGui::SameLine(ImGui::GetWindowWidth() - 30);
		if (ImGui::Button(("X##" + std::string(componentName)).c_str(), ImVec2(20, 20)))
		{
			if constexpr (std::is_same_v<T, TransformComponent> || std::is_same_v<T, ActiveComponent>)
			{
				Logger::Warning("Cannot remove " + std::string(componentName) + " component as it is required");
			}
			else
			{
				prefab.RemoveComponent<T>();
				// emit change event // TODO: make work for prefabs
				//eventBus.EmitEvent<SceneChangeEvent<std::any>>(*selectedEntity, entity.GetComponent<BoxColliderComponent>(),
				//	entity.GetComponent<BoxColliderComponent>(), ChangeAction::ADD);
				ImGui::TreePop();
				return;
			}
		}

		if (opened)
		{
			EditComponent(prefab.GetComponent<T>());
			ImGui::TreePop();
		}
	}
}
#endif // IMGUI_DISABLE

