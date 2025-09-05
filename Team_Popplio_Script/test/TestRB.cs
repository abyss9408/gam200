/******************************************************************************/
/*!
\file   TestRB.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/04 08:55:22 PM (Last Edit)
\brief
        C# file to test RigidBody functions for Team_Popplio engine

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
	public class TestRB : PopplioMonoScript
	{
		public override void Setup()
		{
			
		}

		public override void Init()
		{
			Logger.Info("TestRB.cs: Entity RigidBody");
		}

		public override void Update()
		{
			This.GetComponent<RigidBody2D>().Velocity.X += 1f;
			This.GetComponent<RigidBody2D>().Velocity.Y += 1f;
			This.GetComponent<RigidBody2D>().Acceleration.X += 1f;
			This.GetComponent<RigidBody2D>().Acceleration.Y += 1f;
			Logger.Info(This.GetComponent<RigidBody2D>().Force.X.ToString());
			Logger.Info(This.GetComponent<RigidBody2D>().Force.Y.ToString());
			Logger.Info(This.GetComponent<RigidBody2D>().IsActive.ToString());
			Logger.Info(This.GetComponent<RigidBody2D>().UseGravity.ToString());
			Logger.Info(This.GetComponent<RigidBody2D>().Mass.ToString());
			Logger.Info(This.GetComponent<RigidBody2D>().DragCoefficient.ToString());
		}

		public override void FixedUpdate()
		{

		}

		public override void End()
		{
			
		}
	}
}
