/******************************************************************************/
/*!
\file   Mono.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for implementing the C# API for mono in C++

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System.Runtime.CompilerServices;

namespace Team_Popplio
{
    public static class Mono
    {
		// bridges // -----------------------------------------------------

		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void AddGC(object obj);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void RemoveGC(object obj);
	}
}
