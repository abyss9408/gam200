/******************************************************************************/
/*!
\file   CSConstants.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for C# constants / C++ constants

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System;

namespace Team_Popplio
{
	public static class Constants
	{
		// System //
		public const double		MAX_MEMORY = 2.0; // memory allocated to program [GB]
		// File System //
		public const int		MAX_PATH = 256; // max path length
		public const int		MAX_STRING = 256; // max string length
		public const int		MAX_JSON_NAME = 256; // max json member names length
		// Particle System //
		public const int		PARTICLE_SYSTEM_PARTICLE_MAX = 256;
		public const int		PARTICLE_SYSTEM_EMISSION_MAX = 8;
		// Scene //
		public const float		SCENE_LOAD_COOLDOWN = 1.0f;
		// Logic System / Mono //
		public const double		GC_CYCLE = 60.0;
		public const int		GC_HANDLES = 10000;
		public const int		GC_HANDLES_WARNING = 8000;
		public const int		MONO_HEAP_LIMIT = 0;
	}
}