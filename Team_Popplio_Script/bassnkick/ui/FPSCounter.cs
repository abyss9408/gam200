/******************************************************************************/
/*!
\file   FPSCounter.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        BassNKick | C# file for fps counter ui

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

namespace BassNKick
{
	public class FPSCounter : PopplioMonoScript
	{
		//public Entity fpsObj = Entity.EntityField();
		Text2D? fpsText;

		public override void Setup()
		{
			fpsText = This.GetComponent<Text2D>();
		}

		public override void Update()
		{
			if (fpsText == null) return;
			if (!Lib.StaticSettings.showFPS)
			{
				if (fpsText.IsActive) fpsText.IsActive = false;
				return;
			}
			else
			{
				if (!fpsText.IsActive) fpsText.IsActive = true;
			}

			fpsText.Text = "FPS: " + ((int)Engine.GetFPS()).ToString();
		}
	}
}
