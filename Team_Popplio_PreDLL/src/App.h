/******************************************************************************/
/*!
\file           App.h
\author         Team Popplio
\par            Course : CSD2401 / CSD2400
\par            Section : A
\date           2024/11/06
\brief
    This is the header file for the main application entry point into the main DLL.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "Engine/Engine.h"

#define DLL_EXPORT __declspec(dllexport)

/*
*   This is the main entry point for the Team_Popplio_PreDLL.
*   @returns int | The exit code of the core engine DLL.
*/
extern "C" DLL_EXPORT int App(); 
