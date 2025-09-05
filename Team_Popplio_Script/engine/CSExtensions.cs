/******************************************************************************/
/*!
\file   CSExtensions.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for C# features that may not be available or broken in the engine

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System;

namespace Team_Popplio.Extensions
{
	public static class PopplioMath
	{
		public static T Clamp<T>(this T value, T min, T max) where T : IComparable<T>
		{
			if (value.CompareTo(min) < 0) return min;
			if (value.CompareTo(max) > 0) return max;
			return value;
		}

		public static T Pow<T>(this T value, T power) where T : IConvertible
		{
			return (T)Convert.ChangeType(Math.Pow(value.ToDouble(null), power.ToDouble(null)), typeof(T));
		}
	}

	public static class PopplioRandom
	{
		private static System.Random random = new System.Random();
		public static int Range(int min, int max)
		{
			return random.Next(min, max);
		}
		public static float Range(float min, float max)
		{
			return (float)random.NextDouble() * (max - min) + min;
		}
	}

	public static class PopplioArray
	{
		public static T[] Add<T>(T[] array, T value)
		{
			if (array == null) return new T[] { value };

			T[] newArray = new T[array.Length + 1];
			Array.Copy(array, newArray, array.Length);

			newArray[array.Length] = value;

			return newArray;
		}
	}
}
