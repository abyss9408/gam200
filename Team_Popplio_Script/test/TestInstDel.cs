/******************************************************************************/
/*!
\file   TestInstDel.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/04 08:55:22 PM (Last Edit)
\brief
        C# file to test Instantiation and Destroy functions for Team_Popplio engine

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
	public class TestInstDel : PopplioMonoScript
	{
		public override void Setup()
		{
			
		}

		public override void Init()
		{
			Logger.Info("TestInstDel.cs: Entity Instantiation / Deletion");
		}

		public override void Update()
		{
			if (Input.IsKeyDown(GLFW.KEY_1))
			{
				Logger.Info("TestInstDel.cs: Deleting");
				Entity.Destroy(This);
			}
			if (Input.IsKeyDown(GLFW.KEY_2))
			{
				Logger.Info("TestInstDel.cs: Instantiating");
				Entity.Instantiate(This);
			}
		}

		public override void FixedUpdate()
		{

		}

		public override void End()
		{
			
		}
	}
}
