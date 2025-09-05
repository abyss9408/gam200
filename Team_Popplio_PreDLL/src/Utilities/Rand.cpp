/******************************************************************************/
/*!
\file   Rand.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        Source file for Utility functions for Random Number Generation

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>

#include "Rand.h"

namespace PopplioUtil
{
    void InitRand()
    {
        srand(static_cast<unsigned int>(time(0)));
    }

    void SetRand(unsigned int seed)
    {
        srand(seed);
    }

    int Rand()
    {
        return rand();
    }

    int RandInt(int min, int max)
    {
        return (rand() % (max - min + 1)) + min;
    }

    float RandFloat(float min, float max)
    {
        return (max - min) * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) + min;
    }
}