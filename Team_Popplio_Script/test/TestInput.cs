/******************************************************************************/
/*!
\file   TestInput.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/04 08:55:22 PM (Last Edit)
\brief
        C# file to test Input functions for Team_Popplio engine

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
using Team_Popplio.Extern;

namespace BassNKick
{
	public class TestInput : PopplioMonoScript
	{
		public override void Setup()
		{
			
		}

		public override void Init()
		{
			Logger.Info("TestInput.cs: Entity Input");
		}

		public override void Update()
		{
			if (Input.IsKeyDown(GLFW.KEY_W)) This.Transform2D.Position.Y += 10f;
			if (Input.IsKeyDown(GLFW.KEY_S)) This.Transform2D.Position.Y -= 10f;
			if (Input.IsKeyDown(GLFW.KEY_A)) This.Transform2D.Position.X -= 10f;
			if (Input.IsKeyDown(GLFW.KEY_D)) This.Transform2D.Position.X += 10f;
		}

		public override void FixedUpdate()
		{

		}

		public override void End()
		{
			
		}
	}
}
