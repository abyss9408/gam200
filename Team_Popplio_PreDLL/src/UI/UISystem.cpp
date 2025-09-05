/******************************************************************************/
/*!
\file   UISystem.cpp
\author Team Popplio
\author Hafawati Binte Mohd Ali (100%)
\par    Course : CSD2450
\par    Section : A
\date   2025/04/06 09:50:22 AM (Last Edit)
\brief
This header file defines the UISystem class, which manages
interactable UI components in the game engine.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>
#include "UISystem.h"
#include "../Transformation/TransformComponent.h"
#include "../Input/InputSystem.h"


namespace Popplio {

    bool UISystem::wasMousePressed = false;
    PopplioMath::Vec2f UISystem::lastMousePos = PopplioMath::Vec2f(0.0f, 0.0f);
    PopplioMath::Vec2f UISystem::mousePosForTrans = PopplioMath::Vec2f(0.0f, 0.0f);

    UISystem::UISystem(CameraManager& cameraMgr, Registry& registry, GLFWwindow* window): cameraManager(cameraMgr), reg(registry), window(window)
    {
        RequireComponent<UIComponent>();
    }

    void UISystem::Initialize()
    {
        InitializeFunctionRegistry();  // Ensure function registry is initialized
    }


    void UISystem::InitializeButtons()
    {
        auto& registry = FunctionRegistry::GetInstance();

        for (auto& entity : GetSystemEntities())
        {
            if (!entity.HasComponent<UIComponent>()) continue;
            auto& uiComponent = entity.GetComponent<UIComponent>();

            if (uiComponent.type == UIType::BUTTON)
            {
                uiComponent.ClearOnClickListeners(); // Clear old functions

                for (const std::string& action : uiComponent.onClickActions)
                {
                    if (registry.HasFunction(action))  // Ensure function exists
                    {
                        std::function<void()> func = registry.GetFunction(action);
                        if (func)
                        {
                            uiComponent.AddOnClickListener(func);
                        }
                    }
                    else
                    {
                        Logger::Warning("[UI SYSTEM] Function '" + action + "' not found in registry.");
                    }
                }
            }

        }
        Logger::Info("[UI SYSTEM] Buttons initialized with OnClick functions.");
    }

    void UISystem::Update()
    {
        PopplioMath::Vec2f worldPos;

#ifndef IMGUI_DISABLE
        // Get mouse position in screen coordinates
        auto& editor = reg.GetSystem<Editor>();

        if (editor.IsEditorEnabled())
        {
            ImVec2 editorMousePos = ImGui::GetMousePos();
            worldPos = ScreenToWorldPositionGame(editorMousePos);
        }
        else
        {
            PopplioMath::Vec2f mousePos = mousePosForTrans;
            worldPos = cameraManager.GetGameCamera().ScreenToWorld(mousePos);
        }
#else
        PopplioMath::Vec2f mousePos = mousePosForTrans;
        worldPos = cameraManager.GetGameCamera().ScreenToWorld(mousePos);
#endif    

        for (auto& entity : GetSystemEntities())
        {
            if (!entity.GetComponent<ActiveComponent>().isActive) continue;
            if (!entity.HasComponent<UIComponent>() || !entity.HasComponent<TransformComponent>()) continue;

            auto& uiComponent = entity.GetComponent<UIComponent>();
            auto& transform = entity.GetComponent<TransformComponent>();

            float halfWidth = transform.scale.x / 2.0f;
            float halfHeight = transform.scale.y / 2.0f;
            float left = transform.position.x - halfWidth;
            float right = transform.position.x + halfWidth;
            float top = transform.position.y - halfHeight;
            float bottom = transform.position.y + halfHeight;

            bool isMouseOver = (worldPos.x >= left && worldPos.x <= right &&
                worldPos.y >= top && worldPos.y <= bottom);

            // Hover Detection
            if (uiComponent.hasHoverEffect)
            {
                if (isMouseOver && !uiComponent.isHovered)
                {
                    uiComponent.isHovered = true;
                    Logger::Info("Mouse entered UI element!");
                }
                else if (!isMouseOver && uiComponent.isHovered)
                {
                    uiComponent.isHovered = false;
                    Logger::Info("Mouse left UI element!");
                }
            }

            Logger::Debug("[DEBUG] UI Button Actions: ");
            std::string log = "";
            for (const auto& action : uiComponent.onClickActions)
            {
                log += action + ", ";
            }
            Logger::Debug(log);

            auto& registry = FunctionRegistry::GetInstance();

            auto availableFunctions = registry.GetAllFunctionNames();
            Logger::Debug("[DEBUG] Available Functions: ");
            log = "";
            for (const auto& func : availableFunctions)
            {
                log += func + ", ";
            }
            Logger::Debug(log);

            if (isMouseClickDragged)
            {
                if (isMouseOver)
                {
                    if (uiComponent.type == UIType::SLIDER_TRACK)
                    {
                        for (auto& handleEntity : GetSystemEntities())
                        {
                            if (!handleEntity.HasComponent<UIComponent>() || !handleEntity.HasComponent<TransformComponent>()) continue;
                            auto& handleUI = handleEntity.GetComponent<UIComponent>();

                            if (handleUI.type == UIType::SLIDER_HANDLE && handleUI.linkedEntityId == entity.GetId())
                            {
                                auto& handleTransform = handleEntity.GetComponent<TransformComponent>();

                                float normalizedX = (worldPos.x - left) / (right - left);
                                normalizedX = glm::clamp(normalizedX, 0.0f, 1.0f);

                                handleUI.sliderValue = normalizedX;
                                handleTransform.position.x = left + (normalizedX * (right - left));

                                if (entity.HasComponent<RenderComponent>())
                                {
                                    auto& trackRender = entity.GetComponent<RenderComponent>();
                                    trackRender.color.r = normalizedX;
                                }

                                break;
                            }
                        }
                    }
                }
            }

            // Click Handling
            if (isMouseClickTriggered)
            {
                if (isMouseOver)
                {
                    if (uiComponent.type == UIType::CHECKBOX)
                    {
                        uiComponent.checked = !uiComponent.checked;
                    }

                    if (uiComponent.type == UIType::BUTTON)
                    {
                        // Ensure `onClickListeners` is properly assigned from `onClickActions`
                        if (uiComponent.onClickListeners.empty())
                        {
                            uiComponent.onClickListeners.clear();
                            for (const std::string& actionName : uiComponent.onClickActions)
                            {
                                if (registry.HasFunction(actionName))
                                {
                                    uiComponent.onClickListeners.push_back(registry.GetFunction(actionName));
                                }
                            }
                        }

                        // Execute all registered `onClickListeners`
                        if (!uiComponent.onClickListeners.empty())
                        {
                            Logger::Info("[UI SYSTEM] Executing " + 
                                std::to_string(uiComponent.onClickListeners.size()) + " functions");
                            for (auto& callback : uiComponent.onClickListeners)
                            {
                                if (callback)
                                {
                                    callback();
                                }
                                else
                                {
                                    Logger::Error("[UI SYSTEM] Attempted to call a null function.");
                                }
                            }

                        }
                    }
                }
            }
        }
    }


#ifndef IMGUI_DISABLE
    PopplioMath::Vec2f UISystem::ScreenToWorldPositionGame(const ImVec2& screenPos) const
    {
        auto& editor = reg.GetSystem<Editor>(); // Get Editor system from the registry

        // Get the game viewport size and position
        ImVec2 gameViewportSize = { editor.viewportBoundsGame[1].x - editor.viewportBoundsGame[0].x,
                                    editor.viewportBoundsGame[1].y - editor.viewportBoundsGame[0].y };
        ImVec2 gameViewportPos = editor.viewportBoundsGame[0];

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

        return PopplioMath::Vec2f(worldX, worldY);
    }
#endif // !IMGUI_DISABLE

    void UISystem::ProcessInput()
    {
        // Ensure inputs are processed correctly each frame
        if (InputSystem::IsMouseButtonTriggered(InputSystem::MouseButton::Left))
        {
            //std::cout << "[UI SYSTEM] Mouse Click Triggered!" << std::endl;
            isMouseClickTriggered = true;  // Store state for Update()
        }
        else
        {
            isMouseClickTriggered = false;
        }

        if (InputSystem::IsMouseButtonDown(InputSystem::MouseButton::Left))
        {
            PopplioMath::Vec2f currentMousePos = InputSystem::GetMouse(); // Ensure this function exists

            if (currentMousePos != lastMousePos) // Mouse moved while held down
            {
                isMouseClickDragged = true;
            }
            else
            {
                isMouseClickDragged = false;
            }

            lastMousePos = currentMousePos;
        }
        else
        {
            isMouseClickDragged = false;
        }

        mousePosForTrans = InputSystem::GetMouse();
    }

    void UISystem::InitializeFunctionRegistry()
    {
        auto& registry = FunctionRegistry::GetInstance();

        // Toggle Pause
        registry.RegisterFunction("PAUSE_GAME", []() {
            Engine::isPaused = !Engine::isPaused;
            Logger::Info("[UI SYSTEM] Game " + Engine::isPaused ? "Paused" : "Resumed");
            });
        

        registry.RegisterFunction("QUIT", [this]()
        {
            if (window)
            {
                // This triggers the main engine loop to exit 
                glfwSetWindowShouldClose(window, true);
                Logger::Info("[UI SYSTEM] QUIT triggered, setting GLFW window close flag.");
            }
        });

        registry.RegisterFunction("None", []() {
            Logger::Info("[UI SYSTEM] Does Nothing");
        });

         ///////////
        // DEMO ///
       ///////////

        static int textClickCount = 0;
        // Update a text component dynamically
        registry.RegisterFunction("UPDATE_TEXT", [this]() {  
            auto& allEntities = reg.GetAllEntities();
            for (auto& entity : allEntities)  // Loop through ALL entities
            {
                if (!entity.GetComponent<ActiveComponent>().isActive) continue;
                if (entity.HasComponent<TextComponent>())
                {
                    if (entity.HasTag("comboText"))  // Match the tag
                    {
                        
                        auto& textComp = entity.GetComponent<TextComponent>();
                        textClickCount++;  // Increment count
                        textComp.text = std::to_string(textClickCount); // Update text
                        Logger::Info("[UI SYSTEM] Updated text to: " + textComp.text);
                        break;  // Only update the first matching entity
                    }
                }
            }
        });

        // Open Settings Popup
        registry.RegisterFunction("OPEN_SETTINGS", []() {
            Logger::Info("[UI SYSTEM] Opening Settings Popup...");
            // TODO: Implement UI Popup System
            });
    }

}