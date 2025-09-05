/******************************************************************************/
/*!
\file   Misc.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for miscellaneous game library functions for scripts

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using Team_Popplio;
using Team_Popplio.Libraries;

namespace BassNKick.Lib
{
	public static class Misc
	{
		// Metronome based on program time

		static bool isInThreshold = false;
		const int beatThreshold = 50;

		public static bool IsOnBPMActualTime(float bpm) // return if on bpm for audio provided
		{
			if (Time.ElapsedTimeF <= 0f) return false;

			// Expensive ?
			if (!isInThreshold && beatThreshold > GetOffActualTimeMs(bpm))
			{
				isInThreshold = true;
				return true;
			}

			if (isInThreshold && beatThreshold <= GetOffActualTimeMs(bpm))
				isInThreshold = false;

			return false;
		}

		// seconds
		public static float GetOffActualTimeS(float bpm)
		{
			float beatInterval = 60f / bpm;
			float currTime = Time.ElapsedTimeF;

			float offset = (float)((int)(currTime * 1000f) % (int)(beatInterval * 1000f)) / 1000f;

			if (offset > beatInterval / 2f) offset -= beatInterval;

			return offset;
		}

		// milliseconds
		public static int GetOffActualTimeMs(float bpm)
		{
			float beatInterval = 60f / bpm;
			float currTime = Time.ElapsedTimeF;

			int offset = (int)(currTime * 1000f) % (int)(beatInterval * 1000f);

			if (offset > beatInterval * 500f) offset -= (int)(beatInterval * 1000f);

			return offset;
		}
	}
}
