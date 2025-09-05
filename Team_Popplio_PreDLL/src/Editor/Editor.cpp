/******************************************************************************/
/*!
\file   Editor.cpp
\author Team Popplio
\author			Ng Jun Heng Shawn
\co-author		Pearl | Bryan
\contribution	Shawn - 40% | Pearl - 15% | Bryan - 45%
\par    Course : CSD2401
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
		This is the source file for engine GUI
		Implements imgui library
		Base source file for editor functions
		These include:
		Editor runtime, Graphical User Interface, etc.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>
#include "Editor.h"

#include "../Cloning/CloneEntityEvent.h"
#include "../Graphic/RenderSystem.h"
#include "../PerformanceViewer/PerformanceViewer.h"
#include "../Input/InputSystem.h"
#include "../Utilities/QueueSystem.h"
#include"../UI/UISystem.h"
#include "../Hierarchy/HierarchySystem.h"

#include <ImGuizmo/ImGuizmo.h>

namespace Popplio
{
	//enum class Editor::GuizmoTool
	//{
	//	None = -1,
	//	Translate = ImGuizmo::OPERATION::TRANSLATE,
	//	Rotate = ImGuizmo::OPERATION::ROTATE,
	//	Scale = ImGuizmo::OPERATION::SCALE
	//} currentTool = Editor::GuizmoTool::None;

	void Editor::InitializeFramebuffer(GLuint& framebuffer, GLuint& texture, int width, int height)
	{
		// Create FBO
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		// Create texture for FBO
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Attach texture to FBO
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

		// Check if FBO is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::Critical("Framebuffer is not complete!");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// Init
	void Editor::Init()
	{
		// ImGui //
		// Setup context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		
		
		SetUpImGuiStyle();

		ImGuizmo::Enable(true);
		ImGuizmo::SetOrthographic(true);

		// Setup Platform/Renderer backend
		ImGui_ImplGlfw_InitForOpenGL(win, true);
		ImGui_ImplOpenGL3_Init();
		// ImGui //

		// Initialise other things for engine gui //
		// Get the initial viewport size
		glfwGetFramebufferSize(win, &sceneViewportWidth, &sceneViewportHeight);
		gameViewportWidth = sceneViewportWidth;
		gameViewportHeight = sceneViewportHeight;

		InitializeFramebuffer(sceneFramebuffer, sceneRenderTexture, sceneViewportWidth, sceneViewportHeight);
		InitializeFramebuffer(gameFramebuffer, gameRenderTexture, gameViewportWidth, gameViewportHeight);

		// subscribe to event to handle changes
		eventBus.SubscribeToEvent<SceneChangeEvent<std::any>>(this, &Editor::OnSceneChange);

		//reg.GetSystem<AnimationSystem>().StopAll();

        // check if the required directories are present
        if (!std::filesystem::exists(POPPLIO_TEMP_PATH)) std::filesystem::create_directory(POPPLIO_TEMP_PATH);
        if (!std::filesystem::exists(POPPLIO_AUTO_SAVE_PATH)) std::filesystem::create_directory(POPPLIO_AUTO_SAVE_PATH);
	
		// Set up drag & drop callbacks
		glfwSetWindowUserPointer(win, this);
		glfwSetDropCallback(win, Editor::drop_callback);

		// Load font awesome
		io.Fonts->AddFontDefault();
		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		icons_config.GlyphMinAdvanceX = 13.0f; // Monospace icons

		io.Fonts->AddFontFromFileTTF("Assets/FA Fonts/fa-solid-900.ttf", 13.0f,
			&icons_config, icons_ranges);
		io.Fonts->Build();
	}

	void Editor::ResizeFramebufferTexture(GLuint texture, int width, int height)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Editor::CleanupFramebuffers()
	{
		glDeleteTextures(1, &sceneRenderTexture);
		glDeleteFramebuffers(1, &sceneFramebuffer);

		glDeleteTextures(1, &gameRenderTexture);
		glDeleteFramebuffers(1, &gameFramebuffer);
	}

	// shortcuts
	void Editor::ProcessInput()
	{
		// toggle editor (fullscreen)
		if (InputSystem::IsKeyTriggered(GLFW_KEY_F11))
		{
			ToggleEditor();
		}

		// Reload C# assembly
		if (InputSystem::IsKeyTriggered(GLFW_KEY_F8)) mono.ReloadAssembly();

		// Undo / Redo
		if (InputSystem::IsKeyDown(GLFW_KEY_LEFT_CONTROL) && InputSystem::IsKeyTriggered(GLFW_KEY_Z))
		{
			if (InputSystem::IsKeyDown(GLFW_KEY_LEFT_SHIFT)) // Redo
			{
				Redo();
			}
			else // Undo
			{
				Undo();
			}
		}

		// Save / Save As
		if (InputSystem::IsKeyDown(GLFW_KEY_LEFT_CONTROL) && InputSystem::IsKeyTriggered(GLFW_KEY_S))
		{
			if (InputSystem::IsKeyDown(GLFW_KEY_LEFT_SHIFT)) // Save As
			{
				SaveSceneAsMenu();
			}
			else // Save
			{
				SaveSceneMenu();
			}
		}

		if (!ImGui::GetIO().WantTextInput) // if not inputting in text
		{
			// toggle run

			if (InputSystem::IsKeyTriggered(GLFW_KEY_M))
			{
				if (InputSystem::IsKeyDown(GLFW_KEY_LEFT_SHIFT)) // shift + space -> pause
				{
					if (editorState == Editor::State::PLAYING)
					{
						PauseScene();
						//Engine::isPaused = true;
					}
					else if (editorState == State::PAUSED)
					{
						ResumeScene();
						//Engine::isPaused = false;
					}
				}
				else // space -> toggle run
					ToggleRun();
			}

			// transforms
			if (InputSystem::IsKeyTriggered(GLFW_KEY_W)) currentTool = GuizmoTool::Translate;
			if (InputSystem::IsKeyTriggered(GLFW_KEY_E)) currentTool = GuizmoTool::Rotate;
			if (InputSystem::IsKeyTriggered(GLFW_KEY_R)) currentTool = GuizmoTool::Scale;
			if (InputSystem::IsKeyTriggered(GLFW_KEY_T)) currentTool = GuizmoTool::None;

			// toggle debug drawing
			if (InputSystem::IsKeyTriggered(GLFW_KEY_Q))
			{
				auto& render = reg.GetSystem<RenderSystem>();
				render.ToggleDebugDrawing();
			}

		}
	}

	// Start of main loop
	void Editor::Update()
	{
		// ImGui //
		// Start ImGui Frame
		if (IsEditorEnabled())
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGuizmo::BeginFrame();
			CreateDockspace();
			HandleCameraInput();

			if (editorState == State::EDITING)
			{
				HandleEntitySelection();
				HandleEntityDeletion();

                if (lastNewEntity != latestNewEntity) // emit change event
                {
                    eventBus.EmitEvent<SceneChangeEvent<std::any>>(
						*latestNewEntity, *latestNewEntity, *latestNewEntity, ChangeAction::ADD_ENTITY);

                    lastNewEntity = latestNewEntity;
                }

				// Auto Save
				if (autoSaveEnabled) AutoSaveUpdate();
			}
		}
		//ImGui::ShowDemoWindow(); // test //

		// ImGui //

		// process editor's queued events
		PerformanceViewer::GetInstance()->start("editor queue");
		if (!reg.GetSystem<QueueSystem>().IsProcessingEditor()) reg.GetSystem<QueueSystem>().ProcessEditor();
		PerformanceViewer::GetInstance()->end("editor queue");

		// Go to run start / end if killqueue is still not empty
		if (isStarting)
		{
			StartScene();
			return;
		}

		if (isEnding)
		{
			EndScene();
			return;
		}

		if (editorState == State::CHANGING)
        {
            editorState = State::PLAYING;
            ChangeScene();
            return;
        }
		// Go to run start / end if killqueue is still not empty
	}

	// End of main loop
	void Editor::Render()
	{
		if (!IsEditorEnabled()) {
			// When editor is hidden, only render the game viewport
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, Engine::Config::windowWidth, Engine::Config::windowHeight);

			PerformanceViewer::GetInstance()->start("rendering");
			reg.GetSystem<RenderSystem>().Update();
			PerformanceViewer::GetInstance()->end("rendering");

			PerformanceViewer::GetInstance()->start("ui");
			reg.GetSystem<UISystem>().Update();
			PerformanceViewer::GetInstance()->end("ui");
			return;
		}

		PerformanceViewer::GetInstance()->start("rendering");
		reg.GetSystem<RenderSystem>().Update();
		PerformanceViewer::GetInstance()->end("rendering");

		if (editorState != State::EDITING)
		{
			PerformanceViewer::GetInstance()->start("runtime render");
			RunRender();
			//HandleButtonInput();
            PerformanceViewer::GetInstance()->end("runtime render");
		}

		// Code here for engine gui render //

		Popplio::PerformanceViewer::GetInstance()->start("graphical user interface + imgui");
		// ImGui Dock Windows

		if (showSimulationControls) RenderSimulationControls();
		if (showSceneViewport) RenderSceneViewport();
		if (showGameViewport) RenderGameViewport();
		if (showLogWindow) RenderLogWindow();
		if (showEntityList) RenderEntityList();
		if (showComponentEditor) RenderComponentEditor();
		if (showPerformanceTesting) RenderPerformanceTestingControls();
		if (showPerformanceGUI) RenderPerformanceGUI();
		if (showAssetStoreGUI) RenderAssetStoreGUI();
		if (showChartEditor) RenderChartEditor();
		if (showPrefabEditor) RenderPrefabEditor();
		if (showPrefabListGUI) RenderPrefabList();
		if (showLayerWindow) RenderLayerWindow();
		if (showEntityLayerControls) RenderEntityLayerControls();
		if (showScriptDirectory) RenderScriptDirectoryGUI();
		if (showGizmoTools) RenderToolBar();
		if (showCameraController) RenderCameraControllerWindow();
		//RenderGuizmoEditor();
		// Code here for engine gui render //

		// ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Handle multi-viewport support
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
		Popplio::PerformanceViewer::GetInstance()->end("graphical user interface + imgui");

		//std::cout << "InputSys: " << InputSystem::GetMouse().x << "," << InputSystem::GetMouse().y << std::endl;
		//std::cout << "ImGui: " << ImGui::GetMousePos().x << "," << ImGui::GetMousePos().y << std::endl;
	}

	// Terminate
	void Editor::Terminate()
	{
		if (editorState != State::EDITING) RunEnd();

		CleanupFramebuffers();

		// ImGui
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();
		// ImGui
	}

	void Editor::RenderEntityList()
	{
		// Entity list
		ImGui::Begin(ICON_FA_BARS " Hierarchy");

		static std::string str;
		str = "Entity Count: " + std::to_string(reg.GetNumEntities());
		ImGui::Text(str.c_str());

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Total number of entities in the scene");
			ImGui::EndTooltip();
		}

		if (debugMode)
		{
			str = "Free Ids: " + std::to_string(reg.GetFreeIdCount());
			ImGui::Text(str.c_str());
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Free Ids:");
				for (auto id : reg.GetFreeIds())
				{
					ImGui::Text(std::to_string(id).c_str());
				}
				ImGui::EndTooltip();
			}
		}

		if (ImGui::Button("Add Entity"))
		{
			ImGui::OpenPopup("Add Entity");
		}

		AddEntityPopup();

		// Create a map of parent entities to their children
		std::map<int, std::vector<Entity>> hierarchy;
		std::set<int> hasParent;

		// First pass: build hierarchy map
		for (auto& entity : reg.GetAllEntitiesMutable())
		{
			if (entity.HasComponent<ParentComponent>())
			{
				auto& parentComp = entity.GetComponent<ParentComponent>();
				for (auto& child : parentComp.children)
				{
					hierarchy[entity.GetId()].push_back(child);
					hasParent.insert(child.GetId());
				}
			}
		}

		// Create a container that can receive entities that aren't part of a hierarchy
		ImGui::BeginChild("RootEntitiesDropTarget", ImVec2(0, 0), false);

		// Add a clear visual drop zone for making entities root
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.3f, 0.4f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.4f, 0.5f, 0.7f));

		ImGui::Button("Drop here to make an entity root", ImVec2(ImGui::GetContentRegionAvail().x, 30.0f));

		// Make the button a drop target for entities
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_ITEM"))
			{
				IM_ASSERT(payload->DataSize == sizeof(Entity));
				Entity droppedEntity = *(const Entity*)payload->Data;

				// Only process if entity has a parent
				if (hasParent.count(droppedEntity.GetId()))
				{
					// Find entity's current parent
					Entity currentParent(-1);
					for (auto& ent : reg.GetAllEntitiesMutable())
					{
						if (ent.HasComponent<ParentComponent>())
						{
							auto& parentComp = ent.GetComponent<ParentComponent>();
							auto it = std::find(parentComp.children.begin(), parentComp.children.end(), droppedEntity);
							if (it != parentComp.children.end())
							{
								currentParent = ent;
								break;
							}
						}
					}

					// Remove from current parent if it exists
					if (currentParent.GetId() != -1)
					{
						reg.GetSystem<HierarchySystem>().RemoveChild(currentParent, droppedEntity);

						Logger::Info("Entity " + std::to_string(droppedEntity.GetId()) +
							" made root (removed from parent " + std::to_string(currentParent.GetId()) + ")");

						// Emit a change event for undo/redo
						eventBus.EmitEvent<SceneChangeEvent<std::any>>(droppedEntity, droppedEntity,
							droppedEntity, ChangeAction::MODIFY_ENTITY, "Made root entity");
					}
				}
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::PopStyleColor(2);

		// Allow dropping root entities for reordering at the beginning of the list
		if (isReorderingMode && ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_ITEM", ImGuiDragDropFlags_AcceptBeforeDelivery))
			{
				IM_ASSERT(payload->DataSize == sizeof(Entity));
				Entity droppedEntity = *(const Entity*)payload->Data;

				// Make sure it's a root entity (no parent)
				bool isRootEntity = !hasParent.count(droppedEntity.GetId());

				if (isRootEntity && payload->IsDelivery())
				{
					// Insert at beginning of the list
					auto& allEntities = reg.GetAllEntitiesMutable();
					auto it = std::find(allEntities.begin(), allEntities.end(), droppedEntity);
					if (it != allEntities.end())
					{
						Entity draggedCopy = *it;
						allEntities.erase(it);
						allEntities.insert(allEntities.begin(), draggedCopy);

						eventBus.EmitEvent<SceneChangeEvent<std::any>>(droppedEntity, droppedEntity, droppedEntity,
							ChangeAction::MODIFY_ENTITY, "Root entity reordering");

						Logger::Info("Moved root entity " + std::to_string(droppedEntity.GetId()) + " to beginning of list");
					}
				}
			}
			ImGui::EndDragDropTarget();
		}

		// Second pass: render entities
		for (auto& entity : reg.GetAllEntitiesMutable())
		{
			// Skip entities that have parents (they'll be rendered under their parent)
			if (hasParent.find(entity.GetId()) != hasParent.end())
				continue;

			RenderEntityNode(entity, hierarchy);
		}

		ImGui::EndChild();
		ImGui::End();
	}

	void Editor::RenderEntityNode(Entity& entity, std::map<int, std::vector<Entity>>& hierarchy)
	{
		if (!entity.Exists() || entity.GetId() < 0) return;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

		// Add selected flag if this entity is selected
		if (selectedEntity.has_value() && entity == selectedEntity.value())
			flags |= ImGuiTreeNodeFlags_Selected;

		// Make leaf nodes without children non-expandable
		const auto hierarchyIt = hierarchy.find(entity.GetId());
		if (hierarchyIt == hierarchy.end())
			flags |= ImGuiTreeNodeFlags_Leaf;

		// Create label with tag if exists
		std::string entityName = "";
		entityName += ICON_FA_CUBE " ";
		if (entity.GetComponent<ActiveComponent>().isActive) entityName += ICON_FA_EYE;
		entityName += " " + std::to_string(entity.GetId());

		if (reg.EntityHasAnyTag(entity))
			entityName += " [" + reg.GetEntityTag(entity) + "]";

		// Check if this is the drop target for reordering
		bool isDropTarget = isReorderingMode && dropTargetEntity.has_value() && dropTargetEntity.value() == entity;

		// Add visual indication for drop target when reordering
		if (isDropTarget && draggedIsAboveTarget)
		{
			ImVec2 lineStart = ImGui::GetCursorScreenPos();
			lineStart.y -= 2;
			ImVec2 lineEnd = lineStart;
			lineEnd.x += ImGui::GetContentRegionAvail().x;

			ImGui::GetWindowDrawList()->AddLine(
				lineStart, lineEnd,
				IM_COL32(0, 120, 255, 255),
				2.0f);
		}

		bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)entity.GetId(), flags, "%s", entityName.c_str());

		// Handle selection
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			selectedEntity = entity;
			eventBus.EmitEvent<EntitySelectedEvent>(entity);
		}

		// Begin drag source
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			// Check if Shift key is held for reordering mode
			isReorderingMode = ImGui::GetIO().KeyShift;
			draggedEntity = entity;

			// Set payload to carry the entity ID
			ImGui::SetDragDropPayload("ENTITY_ITEM", &entity, sizeof(Entity));

			if (isReorderingMode)
				ImGui::Text("Reordering %s", entityName.c_str());
			else
				ImGui::Text("Moving %s", entityName.c_str());

			ImGui::EndDragDropSource();
		}

		// Begin drag target
		if (ImGui::BeginDragDropTarget())
		{
			// Accept drop from same type
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_ITEM", ImGuiDragDropFlags_AcceptBeforeDelivery))
			{
				IM_ASSERT(payload->DataSize == sizeof(Entity));
				Entity droppedEntity = *(const Entity*)payload->Data;

				// Don't accept if it's the same entity
				if (droppedEntity.GetId() != entity.GetId())
				{
					// In reordering mode, show visual feedback and handle reordering
					if (isReorderingMode)
					{
						dropTargetEntity = entity;
						ImVec2 mousePos = ImGui::GetMousePos();
						ImVec2 itemMin = ImGui::GetItemRectMin();
						ImVec2 itemMax = ImGui::GetItemRectMax();
						float itemMidY = (itemMin.y + itemMax.y) * 0.5f;
						draggedIsAboveTarget = mousePos.y < itemMidY;

						if (payload->IsDelivery())
						{
							// Handle reordering logic
							HandleEntityReordering(droppedEntity, entity, draggedIsAboveTarget);

							// Reset states
							draggedEntity.reset();
							dropTargetEntity.reset();
						}
					}
					// In normal mode, handle parent-child relationship (existing functionality)
					else if (payload->IsDelivery())
					{
						// Don't accept if it would create a circular reference
						if (!WouldCreateCycle(droppedEntity, entity))
						{
							// Find entity's current parent if any
							Entity currentParent(-1);
							for (auto& ent : reg.GetAllEntitiesMutable())
							{
								if (ent.HasComponent<ParentComponent>())
								{
									auto& parentComp = ent.GetComponent<ParentComponent>();
									auto it = std::find(parentComp.children.begin(), parentComp.children.end(), droppedEntity);
									if (it != parentComp.children.end())
									{
										currentParent = ent;
										break;
									}
								}
							}

							// Remove from current parent if it exists
							if (currentParent.GetId() != -1)
							{
								reg.GetSystem<HierarchySystem>().RemoveChild(currentParent, droppedEntity);
							}

							// Add to new parent
							reg.GetSystem<HierarchySystem>().AddChild(entity, droppedEntity);

							Logger::Info("Entity " + std::to_string(droppedEntity.GetId()) +
								" made child of " + std::to_string(entity.GetId()));

							// Emit a change event for undo/redo
							eventBus.EmitEvent<SceneChangeEvent<std::any>>(entity, entity, entity,
								ChangeAction::MODIFY_ENTITY, "Parent change");
						}
						else
						{
							Logger::Warning("Cannot make entity " + std::to_string(entity.GetId()) +
								" a child of " + std::to_string(droppedEntity.GetId()) +
								" as it would create a circular reference");
						}
					}
				}
			}
			ImGui::EndDragDropTarget();
		}

		// Context menu
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Clone"))
				eventBus.EmitEvent<CloneEntityEvent>(entity.GetId());

			if (ImGui::MenuItem("Delete"))
			{
				// Collect all entities that will be destroyed (the entity and all its children)
				std::vector<Entity> entitiesToDestroy;
				if (entity.HasComponent<ParentComponent>())
				{
					// Recursive function to collect all children
					std::function<void(const Entity&)> collectChildren = [&](const Entity& e) {
						if (e.HasComponent<ParentComponent>())
						{
							for (const auto& child : e.GetComponent<ParentComponent>().children)
							{
								entitiesToDestroy.push_back(child);
								collectChildren(child);
							}
						}
					};

					collectChildren(entity);
				}
				// Add the parent entity itself
				entitiesToDestroy.push_back(entity);

				// Remove all collected entities from their layers and then destroy them
				for (const auto& entityToDestroy : entitiesToDestroy)
				{
					layerManager.RemoveEntityFromCurrentLayer(entityToDestroy);
					ResetSelectedEntity(entityToDestroy);
					reg.KillEntity(entityToDestroy);
				}
				ImGui::CloseCurrentPopup();
			}

			// Move up/down options for children
			if (entity.HasComponent<ParentComponent>() && entity.GetComponent<ParentComponent>().parent.GetId() != -1)
			{
				ImGui::Separator();

				Entity parent = entity.GetComponent<ParentComponent>().parent;
				auto& parentComp = parent.GetComponent<ParentComponent>();
				auto it = std::find(parentComp.children.begin(), parentComp.children.end(), entity);

				if (it != parentComp.children.end())
				{
					bool isFirst = it == parentComp.children.begin();
					bool isLast = (it + 1) == parentComp.children.end();

					if (!isFirst && ImGui::MenuItem("Move Up"))
					{
						// Swap with previous sibling
						std::iter_swap(it, it - 1);
						eventBus.EmitEvent<SceneChangeEvent<std::any>>(parent, parent, parent,
							ChangeAction::MODIFY_ENTITY, "Child reordering");
					}

					if (!isLast && ImGui::MenuItem("Move Down"))
					{
						// Swap with next sibling
						std::iter_swap(it, it + 1);
						eventBus.EmitEvent<SceneChangeEvent<std::any>>(parent, parent, parent,
							ChangeAction::MODIFY_ENTITY, "Child reordering");
					}
				}
			}
			else if (!entity.HasComponent<ParentComponent>() || entity.GetComponent<ParentComponent>().parent.GetId() == -1)
			{
				ImGui::Separator();

				auto& allEntities = reg.GetAllEntitiesMutable();
				auto it = std::find(allEntities.begin(), allEntities.end(), entity);

				if (it != allEntities.end())
				{
					// Find the first entity without a parent before this one
					auto findPrevRoot = [&](auto start) {
						for (auto iter = start; iter != allEntities.begin(); --iter)
						{
							auto& e = *(iter - 1);
							if (!e.HasComponent<ParentComponent>() ||
								e.GetComponent<ParentComponent>().parent.GetId() == -1)
							{
								return iter - 1;
							}
						}
						return allEntities.begin(); // Return beginning if no previous root
					};

					// Find the first entity without a parent after this one
					auto findNextRoot = [&](auto start) {
						for (auto iter = start + 1; iter != allEntities.end(); ++iter)
						{
							if (!iter->HasComponent<ParentComponent>() ||
								iter->GetComponent<ParentComponent>().parent.GetId() == -1)
							{
								return iter;
							}
						}
						return allEntities.end(); // Return end if no next root
					};

					auto prevRoot = findPrevRoot(it);
					auto nextRoot = findNextRoot(it);

					bool isFirst = prevRoot == allEntities.begin() && *prevRoot == entity;
					bool isLast = nextRoot == allEntities.end();

					if (!isFirst && ImGui::MenuItem("Move Up"))
					{
						// Get a copy of the entity
						Entity entityCopy = *it;

						// Erase from current position
						allEntities.erase(it);

						// Find new position and insert
						auto newPos = findPrevRoot(std::find(allEntities.begin(), allEntities.end(), *prevRoot));
						allEntities.insert(newPos + 1, entityCopy);

						eventBus.EmitEvent<SceneChangeEvent<std::any>>(entity, entity, entity,
							ChangeAction::MODIFY_ENTITY, "Root entity reordering");
					}

					if (!isLast && ImGui::MenuItem("Move Down"))
					{
						// Get a copy of the entity
						Entity entityCopy = *it;

						// Erase from current position
						allEntities.erase(it);

						// Insert after the next root entity
						allEntities.insert(nextRoot + 1, entityCopy);

						eventBus.EmitEvent<SceneChangeEvent<std::any>>(entity, entity, entity,
							ChangeAction::MODIFY_ENTITY, "Root entity reordering");
					}
				}
			}

			ImGui::EndPopup();
		}

		// Render children
		if (nodeOpen)
		{
			if (hierarchyIt != hierarchy.end())
			{
				for (auto& child : hierarchyIt->second)
				{
					RenderEntityNode(child, hierarchy);
				}
			}
			ImGui::TreePop();
		}

		// Draw drop line below if needed
		if (isDropTarget && !draggedIsAboveTarget)
		{
			ImVec2 lineStart = ImGui::GetCursorScreenPos();
			ImVec2 lineEnd = lineStart;
			lineEnd.x += ImGui::GetContentRegionAvail().x;

			ImGui::GetWindowDrawList()->AddLine(
				lineStart, lineEnd,
				IM_COL32(0, 120, 255, 255),
				2.0f);
		}
	}

	// Helper function to check for hierarchy cycles
	bool Editor::WouldCreateCycle(Entity potentialChild, Entity potentialParent)
	{
		if (potentialChild.GetId() == potentialParent.GetId())
			return true;

		if (!potentialChild.HasComponent<ParentComponent>())
			return false;

		auto& parentComp = potentialChild.GetComponent<ParentComponent>();
		for (auto child : parentComp.children)
		{
			if (WouldCreateCycle(child, potentialParent))
				return true;
		}

		return false;
	}

	void Editor::RenderComponentEditor()
	{
		ImGui::Begin(ICON_FA_CIRCLE_INFO " Inspector");

		if (selectedEntity.has_value())
		{

			// Tags & Groups
			RenderEntityInfoSection(selectedEntity.value());

			// unified prefab UI
			RenderPrefabUI(selectedEntity.value());

			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("Add Component");
			}

			AddComponentPopup(selectedEntity.value());

			RenderComponentEditor<ActiveComponent>(selectedEntity.value());
			RenderComponentEditor<TransformComponent>(selectedEntity.value());
			RenderComponentEditor<RenderComponent>(selectedEntity.value());
			RenderComponentEditor<RigidBodyComponent>(selectedEntity.value());
			RenderComponentEditor<BoxColliderComponent>(selectedEntity.value());
			RenderComponentEditor<TextComponent>(selectedEntity.value());
			RenderComponentEditor<PrefabInstanceComponent>(selectedEntity.value());
			RenderComponentEditor<AnimationComponent>(selectedEntity.value());
			RenderComponentEditor<LogicComponent>(selectedEntity.value());
			RenderComponentEditor<AudioComponent>(selectedEntity.value());
			RenderComponentEditor<ParentComponent>(selectedEntity.value());
            RenderComponentEditor<PersistOnLoadComponent>(selectedEntity.value());
			RenderComponentEditor<UIComponent>(selectedEntity.value());
            RenderComponentEditor<ParticleComponent>(selectedEntity.value());
			// Add more component types as needed
		}
		ImGui::End();
	}

	void Editor::RenderPerformanceTestingControls()
	{
		ImGui::Begin(ICON_FA_VIALS " Performance Testing");

		if (ImGui::Button("Add 2500 instanced Entities"))
		{
			AddManyInstancingEntities();
		}

		ImGui::SameLine();

		if (ImGui::Button("Clear instanced Entities"))
		{
			//ClearInstancingEntities();
			reg.ClearInstancedEntities();
		}

		ImGui::End();
	}

    void Editor::OnSceneChange(SceneChangeEvent<std::any>& event)
    {
		HandleSceneChange(event);
    }

	void Editor::RenderWindowToggleMenu()
	{
        if (ImGui::BeginMenu("Windows"))
        {
			static std::string show{ "[v]" };
			static std::string hide{ "[ ]" };
			static std::string text{ "" };

            if (!showSimulationControls) text = hide;
            else text = show;
            if (ImGui::MenuItem((text + " Simulation Controls").c_str()))
            {
                showSimulationControls = !showSimulationControls;
            }

			if (!showSceneViewport) text = hide;
			else text = show;
            if (ImGui::MenuItem((text + " Scene View").c_str()))
            {
                showSceneViewport = !showSceneViewport;
            }

            if (!showGameViewport) text = hide;
            else text = show;
            if (ImGui::MenuItem((text + " Game View").c_str()))
            {
                showGameViewport = !showGameViewport;
            }

            if (!showLogWindow) text = hide;
            else text = show;
            if (ImGui::MenuItem((text + " Console").c_str()))
            {
                showLogWindow = !showLogWindow;
            }

            if (!showEntityList) text = hide;
            else text = show;
            if (ImGui::MenuItem((text + " Hierarchy").c_str()))
            {
                showEntityList = !showEntityList;
            }

            if (!showComponentEditor) text = hide;
            else text = show;
            if (ImGui::MenuItem((text + " Inspector").c_str()))
            {
                showComponentEditor = !showComponentEditor;
            }

            if (!showPerformanceTesting) text = hide;
            else text = show;
            if (ImGui::MenuItem((text + " Performance Testing").c_str()))
            {
                showPerformanceTesting = !showPerformanceTesting;
            }
			
            if (!showPerformanceGUI) text = hide;
            else text = show;
            if (ImGui::MenuItem((text + " Performance").c_str()))
            {
                showPerformanceGUI = !showPerformanceGUI;
            }

            if (!showAssetStoreGUI) text = hide;
            else text = show;
            if (ImGui::MenuItem((text + " Asset Browser").c_str()))
            {
                showAssetStoreGUI = !showAssetStoreGUI;
            }

            if (!showChartEditor) text = hide;
            else text = show;
            if (ImGui::MenuItem((text + " Chart Editor").c_str()))
            {
                showChartEditor = !showChartEditor;
            }

            if (!showPrefabEditor) text = hide;
            else text = show;
            if (ImGui::MenuItem((text + " Prefab Editor").c_str()))
            {
                showPrefabEditor = !showPrefabEditor;
            }
			
            if (!showPrefabListGUI) text = hide;
            else text = show;
            if (ImGui::MenuItem((text + " Prefab List").c_str()))
            {
                showPrefabListGUI = !showPrefabListGUI;
            }

            if (!showLayerWindow) text = hide;
            else text = show;
            if (ImGui::MenuItem((text + " Layers").c_str()))
            {
                showLayerWindow = !showLayerWindow;
            }

            if (!showEntityLayerControls) text = hide;
            else text = show;
            if (ImGui::MenuItem((text + " Entity Layer Controls").c_str()))
            {
                showEntityLayerControls = !showEntityLayerControls;
            }

            if (!showScriptDirectory) text = hide;
            else text = show;
            if (ImGui::MenuItem((text + " Script Directory").c_str()))
            {
                showScriptDirectory = !showScriptDirectory;
            }

			if (!showGizmoTools) text = hide;
			else text = show;
			if (ImGui::MenuItem((text + " Gizmo Tools").c_str()))
			{
				showGizmoTools = !showGizmoTools;
			}

			if (!showCameraController) text = hide;
			else text = show;
			if (ImGui::MenuItem((text + " Camera Controller").c_str()))
			{
				showCameraController = !showCameraController;
			}

            ImGui::EndMenu();
        }
	}

	void Editor::ResetSelectedEntity()
	{
		if (selectedEntity.has_value())
		{
			selectedEntity.reset();
			eventBus.EmitEvent<EntityDeselectedEvent>();
		}
	}

	void Editor::ResetSelectedEntity(Entity const& e)
	{
		if (lastNewEntity == e)
			lastNewEntity.reset();
		if (latestNewEntity == e)
			latestNewEntity.reset();

		if (selectedEntity.has_value() && selectedEntity.value() == e)
		{
			selectedEntity.reset();
			eventBus.EmitEvent<EntityDeselectedEvent>();
		}
	}

	//void Editor::RenderGuizmoEditor()
	//{
	//	ImGui::Begin("Transform Controls");

	//	if (ImGui::RadioButton("Translate", currentGizmoOperation == ImGuizmo::TRANSLATE))
	//		currentGizmoOperation = ImGuizmo::TRANSLATE;

	//	ImGui::SameLine();
	//	if (ImGui::RadioButton("Rotate", currentGizmoOperation == ImGuizmo::ROTATE))
	//		currentGizmoOperation = ImGuizmo::ROTATE;

	//	ImGui::SameLine();
	//	if (ImGui::RadioButton("Scale", currentGizmoOperation == ImGuizmo::SCALE))
	//		currentGizmoOperation = ImGuizmo::SCALE;

	//	ImGui::Checkbox("Guizmo is Active", &ImGuizmoActive); // to change between mouse moving entity and mouse controlling imguizmo

	//	ImGui::End();
	//}

	PopplioMath::Vec2f Editor::ScreenToWorldPosition(const ImVec2& screenPos) const
	{
		// Get the viewport size and position
		ImVec2 viewportSize = { viewportBounds[1].x - viewportBounds[0].x, viewportBounds[1].y - viewportBounds[0].y };
		ImVec2 viewportPos = viewportBounds[0];

		// Normalize screen position within the viewport
		ImVec2 normalizedPos = { (screenPos.x - viewportPos.x) / viewportSize.x, (screenPos.y - viewportPos.y) / viewportSize.y };

		// Convert normalized position to NDC (Normalized Device Coordinates)
		float ndcX = normalizedPos.x * 2.0f - 1.0f;
		float ndcY = 1.0f - normalizedPos.y * 2.0f; // Invert Y axis

		// Create a 2D vector for the NDC coordinates
		PopplioMath::Vec2f ndcPos(ndcX, ndcY);

		// Get the inverse of the projection and view matrices
		PopplioMath::M3x3f inverseProjection = cameraManager.GetEditorCamera().GetProjectionMatrix();
		inverseProjection.Inverse();
		PopplioMath::M3x3f inverseView = cameraManager.GetEditorCamera().GetViewMatrix();
		inverseView.Inverse();

		// Transform NDC to world coordinates
		float viewX = inverseProjection[0][0] * ndcX + inverseProjection[0][1] * ndcY + inverseProjection[0][2];
		float viewY = inverseProjection[1][0] * ndcX + inverseProjection[1][1] * ndcY + inverseProjection[1][2];
		float viewZ = inverseProjection[2][0] * ndcX + inverseProjection[2][1] * ndcY + inverseProjection[2][2];

		float worldX = inverseView[0][0] * viewX + inverseView[0][1] * viewY + inverseView[0][2] * viewZ;
		float worldY = inverseView[1][0] * viewX + inverseView[1][1] * viewY + inverseView[1][2] * viewZ;

		// Log intermediate values for debugging
		/*Logger::Info("ScreenPos: " + std::to_string(screenPos.x) + ", " + std::to_string(screenPos.y));
		Logger::Info("ViewportPos: " + std::to_string(viewportPos.x) + ", " + std::to_string(viewportPos.y));
		Logger::Info("ViewportSize: " + std::to_string(viewportSize.x) + ", " + std::to_string(viewportSize.y));
		Logger::Info("NormalizedPos: " + std::to_string(normalizedPos.x) + ", " + std::to_string(normalizedPos.y));
		Logger::Info("NDC: " + std::to_string(ndcX) + ", " + std::to_string(ndcY));
		Logger::Info("WorldPos: " + std::to_string(worldX) + ", " + std::to_string(worldY));*/

		return PopplioMath::Vec2f(worldX, worldY);
	}

	PopplioMath::Vec2f Editor::ScreenToWorldPositionGame(const ImVec2& screenPos) const
	{
		
		// Get the game viewport size and position
		ImVec2 gameViewportSize = { viewportBoundsGame[1].x - viewportBoundsGame[0].x,
									viewportBoundsGame[1].y - viewportBoundsGame[0].y };
		ImVec2 gameViewportPos = viewportBoundsGame[0];

		// Normalize screen position within the game viewport
		ImVec2 normalizedPos = { (screenPos.x - gameViewportPos.x) / gameViewportSize.x,
								  (screenPos.y - gameViewportPos.y) / gameViewportSize.y };

		// Convert normalized position to NDC (Normalized Device Coordinates)
		float ndcX = normalizedPos.x * 2.0f - 1.0f;
		float ndcY = 1.0f - normalizedPos.y * 2.0f; // Invert Y axis

		// Create a 2D vector for the NDC coordinates
		PopplioMath::Vec2f ndcPos(ndcX, ndcY);

		// Get the inverse of the projection and view matrices for the game camera
		PopplioMath::M3x3f inverseProjection = cameraManager.GetGameCamera().GetProjectionMatrix();
		inverseProjection.Inverse();
		PopplioMath::M3x3f inverseView = cameraManager.GetGameCamera().GetViewMatrix();
		inverseView.Inverse();

		// Transform NDC to world coordinates
		float viewX = inverseProjection[0][0] * ndcX + inverseProjection[0][1] * ndcY + inverseProjection[0][2];
		float viewY = inverseProjection[1][0] * ndcX + inverseProjection[1][1] * ndcY + inverseProjection[1][2];
		float viewZ = inverseProjection[2][0] * ndcX + inverseProjection[2][1] * ndcY + inverseProjection[2][2];

		float worldX = inverseView[0][0] * viewX + inverseView[0][1] * viewY + inverseView[0][2] * viewZ;
		float worldY = inverseView[1][0] * viewX + inverseView[1][1] * viewY + inverseView[1][2] * viewZ;

		// Log intermediate values for debugging
		/*Logger::Info("ScreenPos: " + std::to_string(screenPos.x) + ", " + std::to_string(screenPos.y));
		Logger::Info("GameViewportPos: " + std::to_string(gameViewportPos.x) + ", " + std::to_string(gameViewportPos.y));
		Logger::Info("GameViewportSize: " + std::to_string(gameViewportSize.x) + ", " + std::to_string(gameViewportSize.y));
		Logger::Info("NormalizedPos: " + std::to_string(normalizedPos.x) + ", " + std::to_string(normalizedPos.y));
		Logger::Info("NDC: " + std::to_string(ndcX) + ", " + std::to_string(ndcY));
		Logger::Info("WorldPos: " + std::to_string(worldX) + ", " + std::to_string(worldY));*/

		return PopplioMath::Vec2f(worldX, worldY);
	}

	PopplioMath::Vec2f Editor::WorldToScreenPosition(const PopplioMath::Vec2f& worldPos) const
	{
		// Transform world position through view and projection matrices
		PopplioMath::Vec2f viewPos = cameraManager.GetViewMatrix() * worldPos;
		PopplioMath::Vec2f clipPos = cameraManager.GetProjectionMatrix() * viewPos;

		// Convert from clip space [-1,1] to screen space [0,width/height]
		return PopplioMath::Vec2f(
			(clipPos.x + 1.0f) * 0.5f * (viewportBounds[1].x - viewportBounds[0].x) + viewportBounds[0].x,
			(1.0f - (clipPos.y + 1.0f) * 0.5f) * (viewportBounds[1].y - viewportBounds[0].y) + viewportBounds[0].y
		);
	}
}