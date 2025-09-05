/******************************************************************************/
/*!
\file			FileDialog.cpp
\author         Team Popplio
\author         Bryan Ang Wei Ze
\contribution   Bryan - 100%
\par            Course : CSD2401 / CSD2400
\par            Section : A
\date           2024/11/30
\brief
	This file contains the implementation of the FileDialog class, which provides
	functionality for opening and saving files using the Windows file dialog.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include "FileDialog.h"
#include <filesystem>

namespace Popplio
{
	std::string FileDialog::OpenFile(const char* filter, const char* extension, HWND owner)
	{
		OPENFILENAMEA ofn;
		char fileName[MAX_PATH] = "";
		ZeroMemory(&ofn, sizeof(ofn));

		// Get the path of the executable
		wchar_t exePath[MAX_PATH];
		GetModuleFileName(NULL, exePath, MAX_PATH);

		// Get the directory of the executable
		std::filesystem::path exeDir = std::filesystem::path(exePath).parent_path();

		// Set the relative directory
		std::filesystem::path relativeDir = exeDir / "Assets";

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = owner;
		ofn.lpstrFilter = filter;
		ofn.lpstrFile = fileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrInitialDir = relativeDir.string().c_str();
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = extension;

		if (GetOpenFileNameA(&ofn))
		{
			return fileName;
		}
		return "";
	}

	std::string FileDialog::SaveFile(const char* filter, const char* extension, HWND owner)
	{
		OPENFILENAMEA ofn;
		char fileName[MAX_PATH] = "";
		ZeroMemory(&ofn, sizeof(ofn));

		// Get the path of the executable
		wchar_t exePath[MAX_PATH];
		GetModuleFileName(NULL, exePath, MAX_PATH);

		// Get the directory of the executable
		std::filesystem::path exeDir = std::filesystem::path(exePath).parent_path();

		// Set the relative directory
		std::filesystem::path relativeDir = exeDir / "Assets";

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = owner;
		ofn.lpstrFilter = filter;
		ofn.lpstrFile = fileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrInitialDir = relativeDir.string().c_str();
		ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
		ofn.lpstrDefExt = extension;

		if (GetSaveFileNameA(&ofn))
		{
			return fileName;
		}
		return "";
	}
}