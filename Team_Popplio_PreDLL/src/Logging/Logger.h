/******************************************************************************/
/*!
\file   Logger.h
\author Team Popplio
\author Bryan Ang Wei Ze
\co-author Shawn Ng Jun Heng
\contribution Bryan - 95% | Shawn - 5%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/10/02
\brief
    This header file defines the Logger class within the Popplio namespace. 
    The Logger class provides logging functionality with different log levels 
    and destinations. It supports logging messages to the Windows console and 
    ImGui console, with options for different severity levels such as debug, 
    info, warning, error, and critical.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <mutex>
#include <vector>
#include <imgui/imgui.h>

namespace Popplio
{
    enum class LogLevel
    {
        DEBUG_LOG,
        INFO_LOG,
        WARNING_LOG,
        ERROR_LOG,
        CRITICAL_LOG
    };

    enum class LogDestination
    {
        WINDOWS_CONSOLE = 1,
        IMGUI_CONSOLE = 2,
        ALL = WINDOWS_CONSOLE | IMGUI_CONSOLE
    };

    struct LogEntry
    {
        std::string message;

#ifndef IMGUI_DISABLE
        ImVec4 color;
#endif
        LogDestination destination;


#ifndef IMGUI_DISABLE
        LogEntry(const std::string& msg, const ImVec4& clr, LogDestination dest)
            : message(msg), color(clr), destination(dest) {
        }
#else
        LogEntry(const std::string& msg, LogDestination dest)
            : message(msg), destination(dest) {
        }
#endif




    };

    /**
    * @class Logger
    * @brief Provides logging functionality with different log levels.
    */
    class Logger
    {
    public:

        static bool showEngine;
        static bool showUser;

        /**
            * Initializes the Logger with the specified log level
            * @param level The log level to set. Default is LogLevel::INFO.
            * @param useColor Determines whether to use color in the log output. Default is true.
            */
        static void Initialize(LogLevel level = LogLevel::INFO_LOG);

        /**
            * Sets the log level for the Logger.
            * @param level The log level to set.
            */
        static void SetLogLevel(LogLevel level);

        /**
            * Gets the log level for the Logger.
            * @return Current log level.
            */
        static LogLevel GetLogLevel();

        /**
            * Logs a debug message.
            * @param message The message to log.
            */
        static void Debug(const std::string& message, LogDestination dest = LogDestination::ALL, bool isEngine = true);

        /**
            * Logs an info message.
            * @param message The message to log.
            */
        static void Info(const std::string& message, LogDestination dest = LogDestination::ALL, bool isEngine = true);

        /**
            * Logs a warning message.
            * @param message The message to log.
            */
        static void Warning(const std::string& message, LogDestination dest = LogDestination::ALL, bool isEngine = true);

        /**
            * Logs an error message.
            * @param message The message to log.
            */
        static void Error(const std::string& message, LogDestination dest = LogDestination::ALL, bool isEngine = true);

        /**
            * Logs a critical message.
            * @param message The message to log.
            */
        static void Critical(const std::string& message, LogDestination dest = LogDestination::ALL, bool isEngine = true);

        /**
            * Retrieves the log buffer containing all the logged entries.
            * @return A constant reference to the log buffer.
            */
        static const std::vector<LogEntry>& GetLogBuffer();

        /**
            * Clears the log buffer, removing all logged entries.
            */
        static void ClearLogBuffer();

    private:
        /**
            * Logs a message with the specified log level.
            * @param level The log level of the message.
            * @param message The message to log.
            */
        static void Log(LogLevel level, const std::string& message, LogDestination dest, bool isEngine = true);

        /**
        * Logs the message to the Windows console with the specified color.
        * @param message The message to log.
        * @param color The color to use for the log message.
        */
        static void LogToWindowsConsole(const std::string& message, unsigned short color);

        /**
            * Logs the message to the ImGui console with the specified color and destination.
            * @param message The message to log.
            * @param color The color to use for the log message.
            * @param dest The destination to log the message to.
            */
#ifndef IMGUI_DISABLE
        static void LogToImGui(const std::string& message, const ImVec4& color, LogDestination dest);
#endif
        /**
            * Gets the current timestamp in the format "YYYY-MM-DD HH:MM:SS".
            * @return The current timestamp.
            */
        static std::string GetCurrentTimestamp();

        /**
            * Converts a log level enum value to its string representation.
            * @param level The log level to convert.
            * @return The string representation of the log level.
            */
        static std::string LogLevelToString(LogLevel level);

        static LogLevel currentLevel;
        static bool isInitialized;
        static std::vector<LogEntry> logBuffer;
    };

    /**
        * @brief Overload the bitwise OR operator for combining LogDestination values.
        * @param a The first LogDestination value.
        * @param b The second LogDestination value.
        * @return The combined LogDestination value.
        */
    inline LogDestination operator|(LogDestination a, LogDestination b)
    {
        return static_cast<LogDestination>
            (
                static_cast<int>(a) | static_cast<int>(b)
                );
    }

    /**
        * @brief Overload the bitwise AND operator for checking if LogDestination values have common flags.
        * @param a The first LogDestination value.
        * @param b The second LogDestination value.
        * @return True if the LogDestination values have common flags, false otherwise.
        */
    inline bool operator&(LogDestination a, LogDestination b)
    {
        return static_cast<bool>
            (
                static_cast<int>(a) & static_cast<int>(b)
                );
    }
}