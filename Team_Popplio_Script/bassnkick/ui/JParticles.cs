/******************************************************************************/
/*!
\file   JParticles.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        BassNKick | C# file for judgement particles vfx feedback

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System.Collections.Generic;

namespace BassNKick
{
	public class JParticles : PopplioMonoScript
	{
		[Inspector("Particles")]
		Entity[] particles = { };

		[Inspector("Sprites")]
		string[] sprites = { };

		// Variables for animation

		//public float xVar1 = 0.0f;
		//public float xVar2 = 0.0f;

		//public float yVar1 = 0.0f;
		//public float yVar2 = 0.0f;

		//public float xVarRandom = 0.0f;
		//public float yVarRandom = 0.0f;

		public float tVar = 0.8f;
		public float tVarRandom = 0.2f;

		public float rotVar = 0f;
		public float rotVarRandom = 1f;

		float scale = 96f;

		List<float>? timers;
		List<float>? rotSpeeds;
		List<Vector2<float>>? sources;
		List<Vector2<float>>? controlPoints;

		public Entity destination = Entity.EntityField();

		int currSprite = 0;

		public override void Setup()
		{
			timers = new List<float>(particles.Length);
			rotSpeeds = new List<float>(particles.Length);
			sources = new List<Vector2<float>>(particles.Length);
			controlPoints = new List<Vector2<float>>(particles.Length);

			for (int i = 0; i < particles.Length; ++i)
			{
				//timers[i] = -1f;
				//rotSpeeds[i] = 0f;
				//sources[i] = new Vector2<float>();
				timers.Add(-1f);
				rotSpeeds.Add(0f);
				sources.Add(new Vector2<float>());
				controlPoints.Add(new Vector2<float>());
			}
		}

		public override void Update()
		{
			if (timers == null || rotSpeeds == null || sources == null || controlPoints == null)
			{
				Logger.Error("One or more lists are null");
				return;
			}

			for (int i = 0; i < particles.Length; ++i)
			{
				if (timers[i] <= 0f) continue; // skip inactive particles

				float t = 1f - (timers[i] / tVar); // calculate t

				particles[i].Transform2D.Position = BezierCalculation(sources[i], controlPoints[i], t); // update position

				particles[i].Transform2D.Rotation += rotSpeeds[i] * Time.DeltaTimeF; // update rotation

				timers[i] -= Time.DeltaTimeF; // update timer

				if (timers[i] <= 0f)
				{
					particles[i].Active = false;
					timers[i] = -1f;
				}
			}
		}

		public void SpawnParticle(Vector2<float> pos)
		{
			if (timers == null || rotSpeeds == null || sources == null || controlPoints == null)
			{
				Logger.Error("One or more lists are null");
				return;
			}

			for (int i = 0; i < particles.Length; ++i)
			{
				if (timers[i] > 0f) continue; // skip active particles

				particles[i].Transform2D.Position = pos;
				particles[i].Transform2D.Scale = new Vector2<float>(scale, scale);
				particles[i].GetComponent<Render2D>().Texture = sprites[currSprite];
				particles[i].Active = true;
				sources[i] = pos;

				rotSpeeds[i] = Rand.Gen(-rotVarRandom, rotVarRandom); // set rotation speed

				// set control point
				controlPoints[i] = RandPosBetween2Pts(pos, destination.Transform2D.Position);

				// set timer
				timers[i] = tVar + Rand.Gen(-tVarRandom, tVarRandom);

				// cycle through sprites
				++currSprite;
				if (currSprite >= sprites.Length) currSprite = 0;

				return;
			}

			Logger.Error("No available particles to spawn");
		}

		// B(t) = (1−t)^2 * P0 ​+ 2(1−t) * tP1 ​+ t^2 * P2​
		Vector2<float> BezierCalculation(Vector2<float> source, Vector2<float> ctrlPt, float t)
		{
			return ((1f - t) * (1f - t) * source) +
				(2f * (1f - t) * t * ctrlPt) +
				(t * t * destination.Transform2D.Position);
		}

		// Random position between 2 points' X and Y
		Vector2<float> RandPosBetween2Pts(Vector2<float> a, Vector2<float> b)
		{
			return new Vector2<float>(Rand.Gen(a.X, b.X), Rand.Gen(a.Y, b.Y));
		}
	}
}
