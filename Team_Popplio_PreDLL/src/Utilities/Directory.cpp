/******************************************************************************/
/*!
\file   Directory.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the source file for filesystem utilities

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>

#include "Directory.h"

#include "../Engine/Engine.h"

#include <filesystem>

namespace PopplioUtil
{
    void ResetCurrentDirectory()
    {
        std::filesystem::current_path(std::filesystem::path(Popplio::Engine::programPath));
    }
}