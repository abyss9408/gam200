/******************************************************************************/
/*!
\file   LogWindow.cpp
\author Team Popplio
\author Bryan Ang Wei Ze
\co-author Shawn Ng Jun Heng
\contribution Bryan - 60% | Shawn - 40%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
    This file defines the RenderLogWindow method within the Editor class in the 
    Popplio namespace. The method is responsible for rendering a log window using 
    the ImGui library, which displays log messages and provides options for 
    auto-scrolling, text wrapping, and clearing the log.

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
    namespace
    {
        static bool clearOnRun = true;
    }

    void Editor::RenderLogWindow()
    {
        ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
        if (ImGui::Begin(ICON_FA_TERMINAL " Console"))
        {
            // Options
            static bool autoScroll = true;
            static bool wrapText = false;
            ImGui::Checkbox("Auto-scroll", &autoScroll);
            ImGui::SameLine();
            ImGui::Checkbox("Wrap text", &wrapText);
            ImGui::SameLine();
            ImGui::Checkbox("Clear on run", &clearOnRun);
            ImGui::SameLine();

            // Log level filters
            static bool showDebug = (Logger::GetLogLevel() <= LogLevel::DEBUG_LOG);
            static bool showInfo = (Logger::GetLogLevel() <= LogLevel::INFO_LOG);
            static bool showWarning = (Logger::GetLogLevel() <= LogLevel::WARNING_LOG);
            static bool showError = (Logger::GetLogLevel() <= LogLevel::ERROR_LOG);
            //static bool showCritical = (Logger::GetLogLevel() <= LogLevel::CRITICAL_LOG);

            showDebug = (Logger::GetLogLevel() <= LogLevel::DEBUG_LOG);
            showInfo = (Logger::GetLogLevel() <= LogLevel::INFO_LOG);
            showWarning = (Logger::GetLogLevel() <= LogLevel::WARNING_LOG);
            showError = (Logger::GetLogLevel() <= LogLevel::ERROR_LOG);

            ImGui::Text("|");
            ImGui::SameLine();

            ImGui::Text("Log Filters:");
            ImGui::SameLine();
            ImGui::Checkbox("Debug", &showDebug);
            ImGui::SameLine();
            ImGui::Checkbox("Info", &showInfo);
            ImGui::SameLine();
            ImGui::Checkbox("Warning", &showWarning);
            ImGui::SameLine();
            ImGui::Checkbox("Error", &showError);
            ImGui::SameLine();
            //ImGui::Checkbox("Critical", &showCritical);
            //ImGui::SameLine();

            if (showDebug) Logger::SetLogLevel(LogLevel::DEBUG_LOG);
            else if (showInfo) Logger::SetLogLevel(LogLevel::INFO_LOG);
            else if (showWarning) Logger::SetLogLevel(LogLevel::WARNING_LOG);
            else if (showError) Logger::SetLogLevel(LogLevel::ERROR_LOG);

            //if (!showError) Logger::SetLogLevel(LogLevel::CRITICAL_LOG);
            //else if (!showWarning) Logger::SetLogLevel(LogLevel::ERROR_LOG);
            //else if (!showInfo) Logger::SetLogLevel(LogLevel::WARNING_LOG);
            //else if (!showDebug) Logger::SetLogLevel(LogLevel::INFO_LOG);

            ImGui::Text("|");
            ImGui::SameLine();

            ImGui::Checkbox("Engine", &Logger::showEngine);
            ImGui::SameLine();
            ImGui::Checkbox("User", &Logger::showUser);
            ImGui::SameLine();

            ImGui::Text("|");
            ImGui::SameLine();

            // Clear log buffer
            if (ImGui::Button("Clear"))
            {
                Logger::ClearLogBuffer();
            }

            ImGui::Separator();

            // Log content
            ImGui::BeginChild("scolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

            if (wrapText)
            {
                ImGui::PushTextWrapPos();
            }

            const auto& logBuffer = Logger::GetLogBuffer();
            for (const auto& entry : logBuffer)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, entry.color);
                ImGui::TextUnformatted(entry.message.data(), entry.message.data() + entry.message.size());
                ImGui::PopStyleColor();
            }

            if (wrapText)
            {
                ImGui::PopTextWrapPos();
            }

            if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            {
                ImGui::SetScrollHereY(1.0f);
            }

            ImGui::EndChild();
        }

        ImGui::End();
    }

    void Editor::LogClearOnRun()
    {
        if (clearOnRun) Logger::ClearLogBuffer();
    }
}