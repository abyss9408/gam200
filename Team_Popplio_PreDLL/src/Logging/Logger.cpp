/******************************************************************************/
/*!
\file   Logger.cpp
\author Team Popplio
\author Bryan Ang Wei Ze
\co-author Shawn Ng Jun Heng
\contribution Bryan - 95% | Shawn - 5%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/05
\brief
	This source file implements the Logger class within the Popplio namespace. 
	The Logger class is responsible for logging messages with different severity 
	levels to various destinations, such as the Windows console and ImGui console. 
	It supports different log levels, including debug, info, warning, error, and critical.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include <Windows.h>

namespace Popplio
{
	// Initialize static members
	bool Logger::showEngine = true;
	bool Logger::showUser = true;

	LogLevel Logger::currentLevel = LogLevel::INFO_LOG;
	bool Logger::isInitialized = false;
	std::vector<LogEntry> Logger::logBuffer;

	void Logger::Initialize(LogLevel level)
	{
		if (!isInitialized)
		{
			currentLevel = level;
			isInitialized = true;
		}
	}

	void Logger::SetLogLevel(LogLevel level)
	{
		currentLevel = level;
	}

	LogLevel Logger::GetLogLevel()
	{
        return currentLevel;
	}

	void Logger::Debug(const std::string& message, LogDestination dest, bool isEngine)
	{
		Log(LogLevel::DEBUG_LOG, message, dest, isEngine);
	}

	void Logger::Info(const std::string& message, LogDestination dest, bool isEngine)
	{
		Log(LogLevel::INFO_LOG, message, dest, isEngine);
	}

	void Logger::Warning(const std::string& message, LogDestination dest, bool isEngine)
	{
		Log(LogLevel::WARNING_LOG, message, dest, isEngine);
	}

	void Logger::Error(const std::string& message, LogDestination dest, bool isEngine)
	{
		Log(LogLevel::ERROR_LOG, message, dest, isEngine);
	}

	void Logger::Critical(const std::string& message, LogDestination dest, bool isEngine)
	{
		Log(LogLevel::CRITICAL_LOG, message, dest, isEngine);
	}

	void Logger::Log(LogLevel level, const std::string& message, LogDestination dest, bool isEngine)
	{
		if (!isInitialized)
		{
			Initialize(); // Initialize with default values if not done explicitly
		}

		if (level < currentLevel)
		{
			return;
		}

        if (!isEngine && !showUser) return;
        if (isEngine && !showEngine) return;

		std::string timestamp = GetCurrentTimestamp();
		std::string levelStr = LogLevelToString(level);

        std::string origin = isEngine ? "ENGINE | " : "USER | ";

		std::string logMessage = origin + timestamp + " " + "[" + levelStr + "]" + " " + message;

		// Windows console colors
		WORD consoleColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		// ImGui colors
#ifndef IMGUI_DISABLE
		ImVec4 imguiColor;
#endif
		switch (level)
		{
		case LogLevel::DEBUG_LOG:
			consoleColor = FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN; // Cyan
#ifndef IMGUI_DISABLE
			imguiColor = ImVec4(0.0f, 1.0f, 1.0f, 1.0f); // Cyan
#endif
			break;
		case LogLevel::INFO_LOG:
			consoleColor = FOREGROUND_INTENSITY | FOREGROUND_GREEN; // Green
#ifndef IMGUI_DISABLE
			imguiColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // Green
#endif
			break;
		case LogLevel::WARNING_LOG:
			consoleColor = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN; // Yellow
#ifndef IMGUI_DISABLE
			imguiColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
#endif
			break;
		case LogLevel::ERROR_LOG:
			consoleColor = FOREGROUND_INTENSITY | FOREGROUND_RED; // Red
#ifndef IMGUI_DISABLE
			imguiColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Red
#endif
			break;
		case LogLevel::CRITICAL_LOG:
			consoleColor = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE;  // Magenta
#ifndef IMGUI_DISABLE
			imguiColor = ImVec4(1.0f, 0.0f, 1.0f, 1.0f); // Magenta
#endif
			break;
		}

		// Log to Windows console if specified
		if (dest & LogDestination::WINDOWS_CONSOLE)
		{
			LogToWindowsConsole(logMessage, consoleColor);
		}

#ifndef IMGUI_DISABLE
		// Log to ImGui if specified
		if (dest & LogDestination::IMGUI_CONSOLE) {
			LogToImGui(logMessage, imguiColor, dest);
		}
#endif // !#ifndef IMGUI_DISABLE


	}

	void Logger::LogToWindowsConsole(const std::string& message, unsigned short color)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, color);
		std::cout << message << std::endl;
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset color
	}

#ifndef IMGUI_DISABLE
	void Logger::LogToImGui(const std::string& message, const ImVec4& color, LogDestination dest)
	{
		logBuffer.emplace_back(message, color, dest);
		if (logBuffer.size() > 1000)
		{
			logBuffer.erase(logBuffer.begin());
		}
	}
#endif

	const std::vector<LogEntry>& Logger::GetLogBuffer()
	{
		return logBuffer;
	}

	void Logger::ClearLogBuffer()
	{
		logBuffer.clear();
	}

	std::string Logger::GetCurrentTimestamp()
	{
		std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		char buffer[32];

		struct tm timeinfo;
		localtime_s(&timeinfo, &now);

		std::strftime(buffer, sizeof(buffer), "%d-%b-%Y %H:%M:%S", &timeinfo);
		return std::string(buffer);
	}

	std::string Logger::LogLevelToString(LogLevel level)
	{
		switch (level)
		{
		case LogLevel::DEBUG_LOG:
			return "DEBUG";
			break;
		case LogLevel::INFO_LOG:
			return "INFO";
			break;
		case LogLevel::WARNING_LOG:
			return "WARNING";
			break;
		case LogLevel::ERROR_LOG:
			return "ERROR";
			break;
		case LogLevel::CRITICAL_LOG:
			return "CRITICAL";
			break;
		default:
			return "UMKNOWN";
		}
	}
}