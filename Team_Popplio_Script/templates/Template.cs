/******************************************************************************/
/*!
\file   Template.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file to show users a template C# script for Team_Popplio engine

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System;

using Team_Popplio;
using Team_Popplio.ECS;
using Team_Popplio.Components;
using Team_Popplio.Libraries;
using Team_Popplio.Extern;
using Team_Popplio.Attributes;
using Team_Popplio.Extensions;

namespace BassNKick
{
    public class Template : Team_Popplio.PopplioMonoScript
	{
		#region Info
		/*
		 *	KNOWN ISSUES
		 *	
		 *	===================
		 *	
		 *	DO NOT INITIALIZE VARIABLES WITH 'new' MORE THAN ONCE
		 *	CRASHES THE ENGINE ON GARBAGE COLLECTION
		 *	
		 *	EXAMPLE:
		 *	
		 *	// variable declaration
		 *	Entity entity = new Entity();
		 *	
		 *	public override Setup()
		 *	{
		 *		entity = new Entity(); // CRASH
		 *	}
		 *	
		 *	===================
		 *	
		 *	ADDING TOO MANY ENTITY VARIABLE DECLARATIONS TO SCRIPTS
		 *	CRASHES THE ENGINE ON GARBAGE COLLECTION
		 *	
		 *	===================
		 *	
		 *	SETTING PARENT AND GETTING LOCAL POSITION IN SAME LOOP
		 *	DOES NOT GET LOCAL POSITION AFTER SETTING PARENT
		 *	
		 *	EXAMPLE:
		 *	
		 *	public override Setup()
		 *	{
		 *		This.Parent = entity;
		 *		Vector2<float> pos = This.Hierarchy.LocalPosition2D; 
		 *		// gives previous local position before setting parent
		 *	}
		 *	
		 *	===================
		 *	
		*/
		#endregion

		#region Variables

		#endregion

		#region Functions

		#endregion

		#region Loop
		public override void Setup()  
		{
			Logger.Info("Template.cs | Setup()");
		}

		public override void Init()
		{	
			Logger.Info("Template.cs | Init()");
		}

		public override void Update()
		{

		}

		public override void FixedUpdate()
		{

		}

		public override void End()
		{
			Logger.Info("Template.cs | End()");
		}
		#endregion

		#region Events
		public override void OnCollision2DEnter(Entity other, double time)
		{
			Logger.Info($"Template.cs | OnCollision2D({other}, {time})");
		}
		public override void OnCollision2DStay(Entity other, double time)
		{
			Logger.Info($"Template.cs | OnCollision2D({other}, {time})");
		}
		public override void OnCollision2DExit(Entity other, double time)
		{
			Logger.Info($"Template.cs | OnCollision2D({other}, {time})");
		}

		public override void OnTrigger2DEnter(Entity other, double time)
		{
			Logger.Info($"Template.cs | OnTrigger2D({other}, {time})");
		}
		public override void OnTrigger2DStay(Entity other, double time)
		{
			Logger.Info($"Template.cs | OnTrigger2D({other}, {time})");
		}
		public override void OnTrigger2DExit(Entity other, double time)
		{
			Logger.Info($"Template.cs | OnTrigger2D({other}, {time})");
		}

		public override void OnDestroy()
		{
			Logger.Info("Template.cs | OnDestroy()");
		}

		public override void OnEnable()
		{
			Logger.Info("Template.cs | OnEnable()");
		}

		public override void OnDisable()
		{
			Logger.Info("Template.cs | OnDisable()");
		}
		#endregion

		#region Functions

		#endregion
	}
}
