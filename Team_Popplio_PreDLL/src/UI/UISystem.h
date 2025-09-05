/******************************************************************************
\file   UISystem.h
\author Team Popplio
\author Hafawati Binte Mohd Ali (100%)
\par    Course : CSD2450
\par    Section : A
\date   2025/02/03 09:50:22 AM (Last Edit)
\brief
This header file defines the UISystem class, which manages
interactable UI components in the game engine.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
******************************************************************************/

#pragma once
#include "UIComponent.h"
#include "FunctionRegistry.h"
#include "../Input/InputSystem.h"
#ifndef IMGUI_DISABLE
#include "../Editor/Editor.h"
#endif
#include "../Camera/CameraManager.h"
#include "../Camera/GameCamera.h"
#include "../Engine/Engine.h"


namespace Popplio {
    // System responsible for handling UI components and interactions
    class UISystem : public System {
    public:
        /*!*************************************************************************
        \brief Constructor for the UISystem class.

        \param[in] cameraMgr
        Reference to the CameraManager.
        \param[in] registry
        Reference to the entity-component system registry.
        \param[in] window
        Pointer to the GLFW window.
        *************************************************************************/
        UISystem(CameraManager& cameraMgr, Registry& registry, GLFWwindow* window);

        /*!*************************************************************************
        \brief Updates the UI system.

        This function is responsible for handling UI interactions, including
        checking hover states, processing click events, and triggering callbacks.
        *************************************************************************/
        void Update();

        /*!*************************************************************************
        \brief Initializes the UI system.

        This function ensures all necessary UI components and systems are set up,
        including initializing function registries for UI elements.
        *************************************************************************/
        void Initialize();

        /*!*************************************************************************
        \brief Initializes button components in the UI system.

        This function assigns appropriate callbacks to UI buttons based on their
        configured actions. It ensures that buttons respond to interactions by
        registering functions to execute on click events.
        *************************************************************************/
        void InitializeButtons();


        /*!*************************************************************************
        \brief Converts screen coordinates to world coordinates in the game viewport.

        \param[in] screenPos
        Screen position in ImGui coordinates.

        \return PopplioMath::Vec2f
        World coordinates corresponding to the screen position.
        *************************************************************************/
#ifndef IMGUI_DISABLE
        PopplioMath::Vec2f ScreenToWorldPositionGame(const ImVec2& screenPos) const;
#endif // !IMGUI_DISABLE

        /*!*************************************************************************
        \brief Processes user input for UI interactions.

        This function handles mouse interactions with UI components, including
        detecting clicks, drags, and updating hover states.
        *************************************************************************/
        void ProcessInput();

        /*!*************************************************************************
        \brief Initializes function registry for UI event callbacks.

        This function registers predefined functions in the FunctionRegistry,
        allowing UI elements to execute specific actions when interacted with.
        *************************************************************************/
        void InitializeFunctionRegistry();

    private:
        static bool wasMousePressed;
        static PopplioMath::Vec2f lastMousePos;
        static PopplioMath::Vec2f mousePosForTrans;
        bool isMouseClickTriggered = false;
        bool isMouseClickDragged = false;

        CameraManager& cameraManager; // Reference to the camera manager for coordinate transformations
        Registry& reg;                // Reference to the entity-component system registry
        GLFWwindow* window;
    };
}