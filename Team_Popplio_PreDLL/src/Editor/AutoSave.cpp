/******************************************************************************/
/*!
\file   AutoSave.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the source file for Editor Auto Save Functionality

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>

#include "Editor.h"

namespace Popplio
{
    void Editor::AutoSaveUpdate()
    {
        //timeSinceLastAutoSave += static_cast<float>(Engine::timer.GetSystemFixedDeltaTime());
        //timeSinceLastAutoSave += static_cast<float>(Engine::timer.GetFixedDeltaTime());
        timeSinceLastAutoSave += static_cast<float>(Engine::timer.GetDeltaTime());
        if (timeSinceLastAutoSave >= autoSaveInterval)
        {
            Logger::Info("Attempting Auto Save...");
            if (!Popplio::Engine::programPath.empty())
                std::filesystem::current_path(std::filesystem::path(Popplio::Engine::programPath));
            // Reset timer
            timeSinceLastAutoSave = 0.f;
            SaveScene(static_cast<std::string>(POPPLIO_AUTO_SAVE_PATH) + "/" + GetCurrentSceneName() + POPPLIO_TEMP_EXTENSION);
            std::stringstream ss{ "" };
            ss << "Successfully Auto Saved Scene: " << GetCurrentSceneName();
            Logger::Info(ss.str());
        }
    }
}