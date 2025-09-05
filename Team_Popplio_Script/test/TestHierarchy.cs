/******************************************************************************/
/*!
\file   TestHierarchy.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/04 08:55:22 PM (Last Edit)
\brief
        C# file to test Hierarchy functions for Team_Popplio engine

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
using Mono.CSharp;

namespace BassNKick
{
	public class TestHierarchy : PopplioMonoScript
	{
		public override void Setup()
		{

		}

		public override void Init()
		{
			Logger.Info("TestHierarchy.cs: Hierarchy");

			Logger.Info(This.Hierarchy.Parent.ToString());
			Logger.Info(This.Hierarchy.InheritActive.ToString());
			Logger.Info(This.Hierarchy.InheritTransform.ToString());

			This.Hierarchy.AddChild(Entity.GetEntity(4));

			//Logger.Info(Entity.Hierarchy.ChildrenId.Length.ToString());

			//Logger.Info((Entity.Hierarchy.ChildrenId[0]).ToString());
			//foreach (Entity c in Entity.Hierarchy.Children)
			//	Logger.Info(c.ToString());

			//Entity child = Entity.Hierarchy.Children[0];
			//Logger.Info(Entity.Hierarchy.ChildrenList[0].ToString());
			//Logger.Info(Entity.Hierarchy.ChildrenDictId[4].ToString());
			//Logger.Info(child.Name());
			//Logger.Info(Entity.Hierarchy.ChildrenDictName[child.Name()].ToString());
			//Logger.Info(child.Hierarchy.LocalPosition2D.ToString());
			//Logger.Info(child.Hierarchy.LocalScale2D.ToString());
			//Logger.Info(child.Hierarchy.LocalRotation2D.ToString());
			//Entity.Hierarchy.RemoveChild(child);

			//Logger.Info(Entity.Hierarchy.LocalPosition2D.ToString());
			//Logger.Info(Entity.Hierarchy.LocalScale2D.ToString());
			//Logger.Info(Entity.Hierarchy.LocalRotation2D.ToString());
		}

		public override void Update()
		{
			if (Input.IsKeyDown(Team_Popplio.Extern.GLFW.KEY_EQUAL))
			{
				Logger.Info(This.Hierarchy.Parent.Hierarchy.Parent.ToString());
				This.Hierarchy.Parent = This.Hierarchy.Parent.Hierarchy.Parent;
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