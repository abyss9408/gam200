/******************************************************************************/
/*!
\file   GameBG.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        BassNKick | C# file for game background controller

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System;
using System.Collections.Generic;
using Team_Popplio;
using Team_Popplio.Libraries;
using Team_Popplio.Attributes;
using Team_Popplio.ECS;
using Team_Popplio.Libraries;
using Team_Popplio.Components;

namespace BassNKick
{
	public class GameBG : PopplioMonoScript
	{
		//[Header("Track Properties")]
		[Inspector("Track Obj")]
		public Entity trackObj = Entity.EntityField();
		Track? track;

		[Inspector("Scroll Speed")]
		public float scrollSpeed = 0f; // Speed at which the background scrolls

		// private
		private float startPosition = 0f; // Store the initial position
		[Inspector("Width")]
		private float width = 0f; // Width of the background

		float offset = 0f;

		public override void Setup()
		{
			track = trackObj.GetScript<Track>();
		}

		public override void Init()
		{
			startPosition = This.Transform2D.Position.X; // Store the initial position
			///width = GetComponent<SpriteRenderer>().bounds.size.x; // Get the width of the background
			//width = This.Transform2D.Scale.X; // Get the width of the background
			//Logger.Warning(offset.ToString());
			//This.Hierarchy.InheritTransform = false;
		}
		
		public override void Update()
		{
			if (GameManager.IsPaused) return;

			if (track == null)
			{
				Logger.Error("Track is null");
				return;
			}

			offset += Time.DeltaTimeF * scrollSpeed * track.speed;
			
			// Calculate the new position for the background
			//Logger.Warning(offset.ToString());
			//float newPositionX = offset + scrollSpeed * track.speed;

			// If the background has moved far enough, reset its position
			if (offset >= width)
			{
				//newPositionX -= width;
				//offset = (newPositionX / track.speed) / scrollSpeed;
				offset = 0f;
			}

			//This.Hierarchy.LocalPosition2D = Vector2<float>.Left;

			//This.Transform2D.Position = new Vector2<float>(startPosition - offset, This.Transform2D.Position.Y);

			This.Transform2D.Position.X = startPosition - offset;

			// this crashes // TODO fix
			//This.Hierarchy.LocalPosition2D = startPosition + Vector2<float>.Left * newPositionX;

			//Vector2<float> temp = startPosition + Vector2<float>.Left * newPositionX;
		}
	}
}
