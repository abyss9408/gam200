/******************************************************************************/
/*!
\file   UILibrary.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for UI functions for scripts

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

namespace BassNKick.Lib
{
	public static class UILibrary
	{
		public static float EaseOutElastic(float x)
		{
			if (x > 1f) return 1f;
			else if (x < 0f) return 0f;

			float c4 = (2f * (float)Math.PI) / 3f;

			return (float)Math.Pow(2f, -10f * x) * (float)Math.Sin((double)((x * 10f - 0.75f) * c4)) + 1f;
		}

		public static Color RainbowEffect(Color color, int startChannel, float speed, float dt, out int currChannel)
		{
			Color col = new Color(color);

			currChannel = startChannel;
			if (col.R == 1f && col.G == 1f && col.B == 1f)
			{
				switch (startChannel)
				{
					case 0:
						col.G = 0f;
						col.B = 0f;
						break;
					case 1:
						col.R = 0f;
						col.B = 0f;
						break;
					case 2:
						col.R = 0f;
						col.G = 0f;
						break;
				}
				currChannel = (startChannel + 1) % 3 + 1;
			}
			else
			{
				switch (startChannel)
				{
					case 1:
						col.R += speed * dt;
						if (col.R >= 1f)
						{
							col.R = 1f;
							if (col.B == 0f) currChannel = 3;
							else currChannel = -3;
						}
						break;
					case -1:
						col.R -= speed * dt;
						if (col.R <= 0f)
						{
							col.R = 0f;
							if (col.B == 0f) currChannel = 3;
							else currChannel = -3;
						}
						break;
					case 2:
						col.G += speed * dt;
						if (col.G >= 1f)
						{
							col.G = 1f;
							if (col.R == 0f) currChannel = 1;
							else currChannel = -1;
						}
						break;
					case -2:
						col.G -= speed * dt;
						if (col.G <= 0f)
						{
							col.G = 0f;
							if (col.R == 0f) currChannel = 1;
							else currChannel = -1;
						}
						break;
					case 3:
						col.B += speed * dt;
						if (col.B >= 1f)
						{
							col.B = 1f;
							if (col.G == 0f) currChannel = 2;
							else currChannel = -2;
						}
						break;
					case -3:
						col.B -= speed * dt;
						if (col.B <= 0f)
						{
							col.B = 0f;
							if (col.G == 0f) currChannel = 2;
							else currChannel = -2;
						}
						break;
				}
			}


			return col;
		}

		public static Color RandRainbow(out int channel)
		{
			Color col = RainbowEffect(Color.White, Rand.Gen(0, 2), 1f, 0f, out int ch);
			col = RainbowEffect(col, ch, 1f, Rand.Gen(0f, 1f), out int tmp);
			channel = tmp;
			return col;
		}
	}
}
