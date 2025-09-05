/******************************************************************************

\file   UIComponent.h
\author Team Popplio
\author Hafawati Binte Mohd Ali (100%)
\par    Course : CSD2450
\par    Section : A
\date   2025/03/12 09:50:22 AM (Last Edit)
\brief
This header file defines the UIComponent struct, which represents
interactable UI elements such as buttons, checkboxes, and sliders.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.

******************************************************************************/

#pragma once
#include <functional>
#include "../Math/Vector2D.h"

namespace Popplio {

    /*!*************************************************************************
    \brief Enum representing different UI component types.
    *************************************************************************/
    enum class UIType {
        BUTTON,        
        CHECKBOX,      
        SLIDER_TRACK,  
        SLIDER_HANDLE  
    };

    /*!*************************************************************************
    \brief Struct representing an interactive UI component.

    The UIComponent struct is used to define properties for UI elements such as
    buttons, checkboxes, and sliders. It supports interaction states like hover
    effects and click actions.
    *************************************************************************/
    struct UIComponent {
        bool isActive;  
        UIType type;           

        bool isHovered;       
        bool hasHoverEffect;  

        // Spritesheet properties for UI buttons, if hasHoverEffect is true
        int frameX, frameY;               // Normal frame position in the spritesheet. 
        int hoverFrameX, hoverFrameY;     // Hover state frame position in the spritesheet. 
        int totalColumns;                     // Number of columns in the spritesheet (normal + hover). 
        int totalRows;                        // Number of rows in the spritesheet. 
        float sheetWidth;               
        float sheetHeight;               

        bool checked;        
        float sliderValue;    
        int linkedEntityId;     

        std::vector<std::string> onClickActions;          // List of function names to be called on click. 
        std::vector<std::function<void()>> onClickListeners; // Function pointers for click callbacks. 

        /*!*************************************************************************
        \brief Constructor initializing the UIComponent with a specified type.

        \param[in] type The UIType of the component (default is BUTTON).
        *************************************************************************/
       
        UIComponent(UIType type = UIType::BUTTON)
            : type(type), isActive(true), checked(false), sliderValue(0.5f),
            linkedEntityId(-1), hasHoverEffect(false), isHovered(false),
            frameX(0), frameY(0), hoverFrameX(0), hoverFrameY(0),
            totalColumns(1), totalRows(1), sheetWidth(0.0f), sheetHeight(0.0f) 
        {
            if (type == UIType::BUTTON || type == UIType::CHECKBOX) {
                onClickActions.push_back("None");
            }
        }

        
        UIComponent(UIType type, bool isActive, bool checked, float sliderValue, int linkedEntityId,
            bool hasHoverEffect, int frameX, int frameY, int hoverFrameX, int hoverFrameY,
            int totalColumns, int totalRows, float sheetWidth, float sheetHeight,
            std::vector<std::string> onClickActions)
            : type(type), isActive(isActive), checked(checked), sliderValue(sliderValue),
            linkedEntityId(linkedEntityId), hasHoverEffect(hasHoverEffect),
            frameX(frameX), frameY(frameY), hoverFrameX(hoverFrameX), hoverFrameY(hoverFrameY),
            totalColumns(totalColumns), totalRows(totalRows),
            sheetWidth(sheetWidth), sheetHeight(sheetHeight),
            onClickActions(onClickActions), isHovered(false) {
        }

        /*!*************************************************************************
        \brief Adds a new click event listener.

        \param[in] callback A function to be executed when the UI element is clicked.
        *************************************************************************/
        void AddOnClickListener(std::function<void()> callback) {
            onClickListeners.push_back(callback);
        }

        /*!*************************************************************************
        \brief Clears all registered click event listeners.
        *************************************************************************/
        void ClearOnClickListeners() {
            onClickListeners.clear();
        }

        /*!*************************************************************************
        \brief Computes the UV coordinates for the UI element in the spritesheet.

        This function calculates the UV coordinates based on the current frame
        position in the spritesheet. If the component has a hover effect and is
        hovered, it uses the hover frame coordinates.

        \return glm::vec4 UV coordinates (u1, v1, u2, v2) in normalized texture space.
        *************************************************************************/
        glm::vec4 GetCurrentUV() const {
            if (sheetWidth <= 0 || sheetHeight <= 0 || totalColumns <= 0 || totalRows <= 0) {
                Logger::Error("[UIComponent] Invalid spritesheet dimensions! Using default UVs.");
                return glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
            }

            float frameWidth = sheetWidth / totalColumns;
            float frameHeight = sheetHeight / totalRows;

            int currentX = isHovered ? hoverFrameX : frameX;
            int currentY = isHovered ? hoverFrameY : frameY;

            if (currentX < 0 || currentX >= totalColumns || currentY < 0 || currentY >= totalRows) {
                Logger::Error("[UIComponent] UV index out of bounds! Using default UVs.");
                return glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
            }

            float u1 = (currentX * frameWidth) / sheetWidth;
            float v1 = (currentY * frameHeight) / sheetHeight;
            float u2 = ((currentX + 1) * frameWidth) / sheetWidth;
            float v2 = ((currentY + 1) * frameHeight) / sheetHeight;

            Logger::Debug("[UIComponent] UVs: " +
                std::to_string(u1) + ", " +
                std::to_string(v1) + ", " +
                std::to_string(u2) + ", " +
                std::to_string(v2));

            return glm::vec4(u1, v1, u2, v2);
        }
    };
}
