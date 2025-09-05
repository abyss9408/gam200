/******************************************************************************/
/*!
\file   BGMovingEntity.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        BassNKick | C# file to move background entities

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

namespace BassNKick
{
	public class BGMovingEntity : PopplioMonoScript
	{
		[Inspector("Track Obj")]
		public Entity trackObj = Entity.EntityField();

		[Inspector("Speed X")]
		public float speedX = 1.0f;
		[Inspector("Speed Y")]
		public float speedY = 0.0f;

		[Inspector("Loop Time")]
		public float loopTime = 0.0f;

		[Inspector("Random Loop")]
		public float randomLoop = 0f;

		[Inspector("Entities")]
		public Entity[] entities = { };

		Vector2<float> initialPos = new Vector2<float>();
		float currentLoopTime = 0f;
		int currentEntity = 0;
		Track? track;

		public override void Setup()
		{
			track = trackObj.GetScript<Track>();
			initialPos = This.Transform2D.Position;
			if (entities.Length > 1)
			{
				//entities[0].Transform2D.Position = initialPos;

				for (int i = 1; i < entities.Length - 1; ++i)
				{
					entities[i].Active = true;
				}
			}
			else
			{
				//This.Hierarchy.InheritTransform = false;
				//initialPos = This.Transform2D.Position;
			}
		}

		public override void Update()
		{
			if (GameManager.IsPaused) return;

			if (track == null)
			{
				Logger.Error("Track is null");
				return;
			}

			This.Transform2D.Position += new Vector2<float>(
				speedX * Time.DeltaTimeF * track.speed, speedY * Time.DeltaTimeF * track.speed);

			if (currentLoopTime >= loopTime + Rand.Gen(-randomLoop, randomLoop)) // loop
			{
				This.Transform2D.Position = initialPos;
				currentLoopTime = 0f;

				if (entities.Length > 1) // randomize entity
				{
					entities[currentEntity].Active = false;
					currentEntity = Rand.Gen(0, entities.Length - 1);
					entities[currentEntity].Active = true;
				}
			}

			currentLoopTime += Time.DeltaTimeF;
		}
	}
}
