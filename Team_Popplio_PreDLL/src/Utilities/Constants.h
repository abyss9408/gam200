/******************************************************************************/
/*!
\file   Constants.h
\author Team Popplio
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the header file for engine constant values

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

// System //
#define POPPLIO_MAX_MEMORY 2.0 // memory allocated to program [GB]

// File System //
#define POPPLIO_MAX_PATH 256 // max path length
#define POPPLIO_MAX_STRING 256 // max string length
#define POPPLIO_MAX_JSON_NAME 256 // max json member names length

// Particle System //
#define PARTICLE_SYSTEM_PARTICLE_MAX 256
#define PARTICLE_SYSTEM_EMISSION_MAX 8

// Scene //
#define POPPLIO_SCENE_LOAD_COOLDOWN 1.f // scene load cooldown

// Logic System / Mono //
#define POPPLIO_GC_CYCLE 60.0 // garbage collection cycle
#define POPPLIO_GC_HANDLES 50000 // garbage collection handles
#define POPPLIO_GC_HANDLES_WARNING 40000 // garbage collection handles warning
#define POPPLIO_MONO_HEAP_LIMIT 0 // mono heap limit | currently disabled
