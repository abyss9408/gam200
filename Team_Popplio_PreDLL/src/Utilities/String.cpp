/******************************************************************************/
/*!
\file   String.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/09/22 08:55:22 PM (Last Edit)
\brief
        This is the source file for string utilities

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>

#include "String.h"

namespace PopplioUtil::String
{
    bool Contains(std::string const& a, std::string const& b)
    {
        return a.find(b) != std::string::npos;
    }

    void Replace(std::string& str, const std::string& from, const std::string& to) 
    {
        if (from.empty()) return; // avoid infinite loop

        size_t pos = 0;

        while ((pos = str.find(from, pos)) != std::string::npos) 
        {
            str.replace(pos, from.length(), to);
            pos += to.length(); // move past the replacement
        }
    }
}