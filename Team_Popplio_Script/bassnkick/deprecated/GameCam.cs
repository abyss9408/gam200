/******************************************************************************/
/*!
\file   GameCam.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for implementing the game camera

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

namespace BassNKick
{
	public class GameCam : PopplioMonoScript
	{
		[Inspector("Player")]
		Entity player = Entity.EntityField(); // Reference to the player's transform
		//public float scrollSpeed = 5f; // Speed at which the background scrolls

		[Inspector("Offset")]
		float offset = 10f;

		public override void Update()
		{
			// Keep the camera at the player's position
			Vector2<float> cameraPosition = new Vector2<float>(
				player.Transform2D.Position.X + offset, This.Transform2D.Position.Y);
			This.Transform2D.Position = cameraPosition;
		}
	}
}
