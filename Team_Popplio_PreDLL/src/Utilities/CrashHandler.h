/******************************************************************************/
/*!
\file   CrashHandler.h
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401
\par    Section : A
\date   2025/04/06
\brief
	This file contains the declaration of the CrashHandler class, which
	provides functionality for handling unhandled exceptions and generating
	crash reports. It includes methods for initializing the crash handler,
	setting the crash log path, and generating detailed crash logs with
	exception information and stack traces.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include <Windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "Dbghelp.lib")

namespace Popplio
{
	class CrashHandler
	{
	public:
		CrashHandler() = delete;
		CrashHandler(const CrashHandler&) = delete;
		CrashHandler& operator=(const CrashHandler&) = delete;

		static void Initialize();
		static void SetCrashLogPath(const std::string& path);
	private:
		static inline bool isInitialized = false;
		static inline std::string crashDumpPath = "Crashes";

		static std::string GetExceptionDescription(DWORD exceptionCode);
		static LONG WINAPI UnhandledExceptionHandler(EXCEPTION_POINTERS* exceptionInfo);
	};
}