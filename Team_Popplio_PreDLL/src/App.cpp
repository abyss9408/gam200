/******************************************************************************/
/*!
\file           App.cpp
\author         Team Popplio
\par            Course : CSD2401 / CSD2400
\par            Section : A
\date           2024/11/06
\brief
    This is the source file for the main application entry point into the main DLL.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

// Enable run-time memory check for debug builds.
#include <pch.h>

#include "App.h"

extern "C" DLL_EXPORT int App()
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    Popplio::Engine engine;

    engine.Initialize();
    engine.Run();
    engine.Destroy();

	return 0;
}