/******************************************************************************/
/*!
\file   GameTargetUI.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for implementing the target UI in game

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System.Numerics;

namespace BassNKick
{
	public class GameTargetUI : PopplioMonoScript
	{
		float timer = 0f;

		// Update is called once per frame
		public override void Update()
		{
			if (GameManager.IsPaused) return;

			This.Transform2D.Rotation = (double)(timer += Time.DeltaTimeF * 20f);
		}
	}
}
