/******************************************************************************/
/*!
\file   TestTrans.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/04 08:55:22 PM (Last Edit)
\brief
        C# file to test Transform functions for Team_Popplio engine

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
	public class TestTrans : PopplioMonoScript
	{
		public override void Setup()
		{
			
		}

		public override void Init()
		{
			Logger.Info("TestTrans.cs: Entity Transform");
		}

		public override void Update()
		{
			This.Transform2D.Position.X += 1f;
			This.Transform2D.Position.Y += 1f;
			This.Transform2D.Scale.X += 1f;
			This.Transform2D.Scale.Y += 1f;
			This.Transform2D.Rotation += 10.0;
		}

		public override void FixedUpdate()
		{

		}

		public override void End()
		{
			
		}
	}
}
