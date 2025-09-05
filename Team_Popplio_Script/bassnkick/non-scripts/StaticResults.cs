/******************************************************************************/
/*!
\file   StaticResults.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for cross scene static results

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

namespace BassNKick.Lib
{
	public static class StaticResults
	{
		public static Score? score;

		public static int currScore = 0;
		public static int currMaxCombo = 0;
		public static int perfect = 0;
		public static int nice = 0;
		public static int bad = 0;
		public static int miss = 0;

		public static int feverScore = 0;
		public static int rainbowScore = 0;

		public static float scorePercent = 0f;
		
		public static int rank = -1;
	}
}
