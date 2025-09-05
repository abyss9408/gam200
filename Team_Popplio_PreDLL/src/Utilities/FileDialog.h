/******************************************************************************/
/*!
\file           FileDialog.h
\author         Team Popplio
\author         Bryan Ang Wei Ze
\contribution   Bryan - 100%
\par            Course : CSD2401 / CSD2400
\par            Section : A
\date           2024/11/30
\brief
	This file contains the declaration of the FileDialog class, which provides
	functionality for opening and saving files using the Windows file dialog.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include <Windows.h>
#include <string>

namespace Popplio
{
	class FileDialog
	{
	public:

        /**
         * @brief Shows a Save File dialog and returns the selected file path.
         * 
         * @param filter The file type filter for the dialog (e.g., "Text Files\0*.txt\0").
         * @param extension The default file extension (e.g., "txt").
         * @param owner The handle to the owner window (optional).
         * @return std::string The path of the file selected by the user.
         */
        static std::string SaveFile(const char* filter, const char* extension, HWND owner = nullptr);
		
        /**
         * @brief Shows an Open File dialog and returns the selected file path.
         * 
         * @param filter The file type filter for the dialog (e.g., "Text Files\0*.txt\0").
         * @param extension The default file extension (e.g., "txt").
         * @param owner The handle to the owner window (optional).
         * @return std::string The path of the file selected by the user.
         */
        static std::string OpenFile(const char* filter, const char* extension, HWND owner = nullptr);
	};
}