/******************************************************************************/
/*!
\file   CrashHandler.cpp
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401
\par    Section : A
\date   2025/04/06
\brief
	This file contains the implementation of the CrashHandler class, which
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

#include <pch.h>

#include "CrashHandler.h"
#include "../Logging/Logger.h"

namespace Popplio
{
	void CrashHandler::Initialize()
	{
		if (!isInitialized)
		{
			SetUnhandledExceptionFilter(UnhandledExceptionHandler);
			isInitialized = true;
			Logger::Info("Crash handler initialized", LogDestination::WINDOWS_CONSOLE);
		}
	}

	void CrashHandler::SetCrashLogPath(const std::string& path)
	{
		crashDumpPath = path;
	}

	std::string CrashHandler::GetExceptionDescription(DWORD exceptionCode)
	{
		switch (exceptionCode)
		{
		case EXCEPTION_ACCESS_VIOLATION:
			return "Access Violation";
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			return "Array Bounds Exceeded";
		case EXCEPTION_BREAKPOINT:
			return "Breakpoint";
		case EXCEPTION_DATATYPE_MISALIGNMENT:
			return "Datatype Misalignment";
		case EXCEPTION_FLT_DENORMAL_OPERAND:
			return "Float Denormal Operand";
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
			return "Float Divide By Zero";
		case EXCEPTION_FLT_INEXACT_RESULT:
			return "Float Inexact Result";
		case EXCEPTION_FLT_INVALID_OPERATION:
			return "Float Invalid Operation";
		case EXCEPTION_FLT_OVERFLOW:
			return "Float Overflow";
		case EXCEPTION_FLT_STACK_CHECK:
			return "Float Stack Check";
		case EXCEPTION_FLT_UNDERFLOW:
			return "Float Underflow";
		case EXCEPTION_ILLEGAL_INSTRUCTION:
			return "Illegal Instruction";
		case EXCEPTION_IN_PAGE_ERROR:
			return "In Page Error";
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
			return "Integer Divide By Zero";
		case EXCEPTION_INT_OVERFLOW:
			return "Integer Overflow";
		case EXCEPTION_INVALID_DISPOSITION:
			return "Invalid Disposition";
		case EXCEPTION_NONCONTINUABLE_EXCEPTION:
			return "Noncontinuable Exception";
		case EXCEPTION_PRIV_INSTRUCTION:
			return "Privileged Instruction";
		case EXCEPTION_SINGLE_STEP:
			return "Single Step";
		case EXCEPTION_STACK_OVERFLOW:
			return "Stack Overflow";
		case STATUS_HEAP_CORRUPTION:
			return "Heap Corruption";
		case STATUS_STACK_BUFFER_OVERRUN:
			return "Stack Buffer Overrun";
		case STATUS_ASSERTION_FAILURE:
			return "Assertion Failure";
		default:
			return "Unknown Exception";
		}
	}

	LONG WINAPI CrashHandler::UnhandledExceptionHandler(EXCEPTION_POINTERS* exceptionInfo)
	{
		try
		{
			// Get timestamp for log file time
			std::time_t now = std::time(nullptr);
			char timestamp[26];
			ctime_s(timestamp, sizeof(timestamp), &now);
			std::string timeStr(timestamp);
			timeStr = timeStr.substr(0, timeStr.size() - 1); // Remove newline

			// Replace colons with underscores for file name
			std::replace(timeStr.begin(), timeStr.end(), ':', '_');
			std::replace(timeStr.begin(), timeStr.end(), ' ', '_');

			// Create crashes directory if it doesn't exist
			std::filesystem::create_directories(crashDumpPath);

			// Create crash log file
			std::string logFileName = crashDumpPath + "/crash_" + timeStr + ".log";

			std::ofstream logFile(logFileName);
			if (!logFile)
			{
				return EXCEPTION_CONTINUE_SEARCH;
			}

			// Write crash info to log file
			logFile << "Crash Report\n";
			logFile << "============\n\n";
			logFile << "Time: " << timeStr << "\n\n";

			// Write exception info
			EXCEPTION_RECORD* exception = exceptionInfo->ExceptionRecord;
			logFile << "Exception Code: 0x" << std::hex << std::uppercase << exception->ExceptionCode;
			logFile << " (" << GetExceptionDescription(exception->ExceptionCode) << ")\n";
			logFile << "Exception Address: 0x" << exception->ExceptionAddress << "\n";

			if (exception->NumberParameters > 0)
			{
				logFile << "Exception Parameters:\n";
				for (DWORD i = 0; i < exception->NumberParameters; ++i)
				{
					logFile << "  [" << std::dec << i << "]: 0x"
						<< std::hex << exception->ExceptionInformation[i] << "\n";
				}
			}
			logFile << "\n";

			// Stack trace
			logFile << "Stack Trace:\n";
			logFile << "============\n";

			HANDLE process = GetCurrentProcess();
			HANDLE thread = GetCurrentThread();

			// Initialize symbol handler
			SymInitialize(process, NULL, TRUE);

			CONTEXT* context = exceptionInfo->ContextRecord;
			STACKFRAME64 frame = {};

			frame.AddrPC.Offset = context->Rip;
			frame.AddrPC.Mode = AddrModeFlat;
			frame.AddrFrame.Offset = context->Rbp;
			frame.AddrFrame.Mode = AddrModeFlat;
			frame.AddrStack.Offset = context->Rsp;
			frame.AddrStack.Mode = AddrModeFlat;

			while (StackWalk64(
				IMAGE_FILE_MACHINE_AMD64,
				process,
				thread,
				&frame,
				context,
				NULL,
				SymFunctionTableAccess64,
				SymGetModuleBase64,
				NULL))
			{
				char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
				PSYMBOL_INFO symbol = (PSYMBOL_INFO)buffer;
				symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
				symbol->MaxNameLen = MAX_SYM_NAME;

				DWORD64 displacement = 0;
				if (SymFromAddr(process, frame.AddrPC.Offset, &displacement, symbol))
				{
					logFile << symbol->Name << "+0x" << std::hex << displacement << "\n";
				}
				else
				{
					logFile << "??? [0x" << std::hex << std::uppercase << frame.AddrPC.Offset << "]\n";
				}
			}

			SymCleanup(process);
			logFile.close();

			// Log to engine logger if available
			Logger::Critical("Application crashed. Crash report saved to: " + logFileName, LogDestination::WINDOWS_CONSOLE);
		}
		catch (...)
		{
			// If we fail to write the crash log, at least try to log the error
			Logger::Critical("Failed to write crash report", LogDestination::WINDOWS_CONSOLE);
		}

		return EXCEPTION_CONTINUE_SEARCH;
	}
}