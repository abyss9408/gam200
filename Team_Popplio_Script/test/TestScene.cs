/******************************************************************************/
/*!
\file   TestScene.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/04 08:55:22 PM (Last Edit)
\brief
        C# file to test Scene functions for Team_Popplio engine

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
	public class TestScene : PopplioMonoScript
	{
		public override void Setup()
		{

		}

		public override void Init()
		{
			Logger.Info("TestScene.cs: Scene");

			Logger.Info(SceneManager.GetCurrent());
		}

		public override void Update()
		{
			if (Input.IsKeyDown(Team_Popplio.Extern.GLFW.KEY_I))
			{
				Logger.Info("Attempting scene load...");
				SceneManager.LoadScene("Game1");
			}
			if (Input.IsKeyDown(Team_Popplio.Extern.GLFW.KEY_O))
			{
				Logger.Info("Attempting scene reload...");
				SceneManager.ReloadScene();
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