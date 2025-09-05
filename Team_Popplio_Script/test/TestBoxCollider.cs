/******************************************************************************/
/*!
\file   TestBoxCollider.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/04 08:55:22 PM (Last Edit)
\brief
        C# file to test BoxCollider functions for Team_Popplio engine

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
	public class TestBoxCollider : PopplioMonoScript
	{
		public override void Setup()
		{
			
		}

		public override void Init()
		{
			Logger.Info("TestBoxCollider.cs: Entity BoxCollider");

			Logger.Info(This.GetComponent<BoxCollider2D>().Size.X.ToString());
			Logger.Info(This.GetComponent<BoxCollider2D>().Size.Y.ToString());
			Logger.Info(This.GetComponent<BoxCollider2D>().Offset.X.ToString());
			Logger.Info(This.GetComponent<BoxCollider2D>().Offset.Y.ToString());
			Logger.Info(This.GetComponent<BoxCollider2D>().IsActive.ToString());
			Logger.Info(This.GetComponent<BoxCollider2D>().IsTrigger.ToString());
			Logger.Info(This.GetComponent<BoxCollider2D>().IsEnabled.ToString());
		}

		public override void Update()
		{
			
		}

		public override void FixedUpdate()
		{

		}

		public override void End()
		{
			
		}

		public override void OnCollision2DEnter(Entity other, double time)
		{
			Logger.Info("TestBoxCollider.cs: OnCollision2DEnter");
		}

		public override void OnCollision2DStay(Entity other, double time)
		{
			Logger.Info("TestBoxCollider.cs: OnCollision2DStay");
		}

		public override void OnCollision2DExit(Entity other, double time)
		{
			Logger.Info("TestBoxCollider.cs: OnCollision2DExit");
		}

		public override void OnTrigger2DEnter(Entity other, double time)
		{
			Logger.Info("TestBoxCollider.cs: OnTrigger2DEnter");
		}

		public override void OnTrigger2DStay(Entity other, double time)
		{
			Logger.Info("TestBoxCollider.cs: OnTrigger2DStay");
		}

		public override void OnTrigger2DExit(Entity other, double time)
		{
			Logger.Info("TestBoxCollider.cs: OnTrigger2DExit");
		}
	}
}
