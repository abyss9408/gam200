/******************************************************************************/
/*!
\file   PhasedBG.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        BassNKick | C# file for backgrounds with parts

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
	public class PhasedBG : PopplioMonoScript
	{
		public Entity[] parts = { };
		bool prepared = false;

		int currentPart = 0;
		List<float> partTimings = new List<float>();

		public override void Setup()
		{
			
		}

		public override void Init()
		{
			if (AudioManager.audioManagerInstance == null) return;

			for (int i = 0; i < parts.Length; i++)
			{
				partTimings.Add(
					AudioManager.audioManagerInstance.musicObj[0].GetComponent<Audio>().LengthF /
					parts.Length * (i + 1));
			}
		}

		public override void Update()
		{
			if (!prepared)
			{
				for (int i = 1; i < parts.Length; i++)
				{
					parts[i].Active = false;
				}
				prepared = true;
			}

			if (currentPart >= parts.Length) return;
			if (AudioManager.audioManagerInstance == null) return;

			if (AudioManager.audioManagerInstance.musicObj[0].GetComponent<Audio>().CurrentTimeF >= 
				partTimings[currentPart])
			{
				Transition();
			}
		}

		const float transitionSpeed = 0.5f;
		bool transitioning = false;

		void Transition()
		{
			if (currentPart >= parts.Length - 1) return;

			parts[currentPart + 1].Active = true;

			foreach (Entity strip in parts[currentPart].Hierarchy.Children)
			{
				foreach (Entity child in strip.Hierarchy.Children)
				{
					if (child.HasComponent<Render2D>())
					{
						child.GetComponent<Render2D>().Color.A -= transitionSpeed * Time.DeltaTimeF;
						if (child.GetComponent<Render2D>().Color.A <= 0f)
						{
							child.GetComponent<Render2D>().Color.A = 0f;
							transitioning = false;
						}
						else transitioning = true;
					}
				}
			}

			if (!transitioning)
			{
				parts[currentPart].Active = false;
				++currentPart;
			}
		}
	}
}
