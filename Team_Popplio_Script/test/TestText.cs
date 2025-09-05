/******************************************************************************/
/*!
\file   TestText.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/04 08:55:22 PM (Last Edit)
\brief
        C# file to test Text functions for Team_Popplio engine

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System;
using System.Runtime.CompilerServices;

using Team_Popplio;
using Team_Popplio.ECS;
using Team_Popplio.Components;
using Team_Popplio.Libraries;

namespace BassNKick
{
	public class TestText : PopplioMonoScript
	{
		public override void Setup()
		{
			
		}

		public override void Init()
		{
			Logger.Info("TestText.cs: Entity Text");

			Logger.Info(This.GetComponent<Text2D>().IsActive.ToString());
			Logger.Info(This.GetComponent<Text2D>().FontName);
			Logger.Info(This.GetComponent<Text2D>().Text);
		}

		public override void Update()
		{
			if (This.GetComponent<Text2D>().Color.R >= 1.0f) This.GetComponent<Text2D>().Color.R = 0.0f;
			if (This.GetComponent<Text2D>().Color.G >= 1.0f) This.GetComponent<Text2D>().Color.G = 0.0f;
			if (This.GetComponent<Text2D>().Color.B >= 1.0f) This.GetComponent<Text2D>().Color.B = 0.0f;
			if (This.GetComponent<Text2D>().Color.A >= 1.0f) This.GetComponent<Text2D>().Color.A = 0.0f; // warning

			This.GetComponent<Text2D>().Color.R += .01f;
			This.GetComponent<Text2D>().Color.G += .02f;
			This.GetComponent<Text2D>().Color.B += .005f;
			This.GetComponent<Text2D>().Color.A += .01f; // warning

			This.GetComponent<Text2D>().FontSize += 1;
		}

		public override void FixedUpdate()
		{

		}

		public override void End()
		{
			
		}
	}
}
