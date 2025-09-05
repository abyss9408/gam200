/******************************************************************************/
/*!
\file   Rainbow.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        BassNKick | C# file for rainbow feedback

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
using PopplioMath = Team_Popplio.Extensions.PopplioMath;
using PopplioRandom = Team_Popplio.Extensions.PopplioRandom;
using System.Collections.Generic;
using Mono.CSharp;
using System.Security.Policy;

namespace BassNKick
{
	public class Rainbow : Team_Popplio.PopplioMonoScript
	{
		//[Header("Fever Feedback Settings")]
		[Inspector("Duration")]
		float duration = .8f;
		//[Inspector("Variation")]
		//float variation = .5f;
		[Inspector("Alpha Modifier")]
		float alphaModifier = .7f;

		// when to play feedback
		bool playFeedback = false;

		// timer for animation
		float timer = 0f;

		Render2D? sprite; // sprite

		//float r = .7f, g = .7f, b = .7f, a = 0f; // color properties

		float speed = 1f; // speed of animation

		//float rInit = .2f, gInit = .6f, bInit = .9f; // initial color

		int rainbowChannel = 0;

		public override void Setup()
		{
			sprite = This.GetComponent<Render2D>();
		}

		// Start is called before the first frame update
		public override void Init()
		{
			// initialize
			//r = g = b = .7f;
			//a = 0f;
		}

		// Update is called once per frame
		public override void Update()
		{
			if (sprite == null) return; // return if no sprite

			if (!playFeedback) return; // don't play if not activated

			timer += Time.DeltaTimeF; // timer

			//r += Time.DeltaTimeF * speed;
			//g += Time.DeltaTimeF * speed;
			//b += Time.DeltaTimeF * speed;

			//r = r % 1f;
			//g = g % 1f;
			//b = b % 1f;

			sprite.Color = Lib.UILibrary.RainbowEffect(sprite.Color, rainbowChannel, speed, Time.DeltaTimeF, out int tmp);
			rainbowChannel = tmp;

			sprite.Color.A = PopplioMath.Clamp(
				(-4f * PopplioMath.Pow(timer / duration, 2f)) + (4 * (timer / duration)) * alphaModifier,
				0f, 1f);

			//sprite.Color = new Color(r, g, b, a);

			if (timer > duration) // end animation
			{
				//r = g = b = .7f;
				//a = 0f;

				sprite.Color.A = 0f;

				//sprite.Color = new Color(r, g, b, a);

				timer = 0f;

				playFeedback = false;
			}
		}

		public void Feedback() // external function
		{
			// overlay

			//return; // temp

			playFeedback = true;

			// randomise color
			Color col = Lib.UILibrary.RandRainbow(out int tmp);

			rainbowChannel = tmp;

			//r = col.R;
			//g = col.G;
			//b = col.B;
			

			//r = PopplioMath.Clamp(
			//	PopplioRandom.Range(rInit - variation, rInit + variation), 0f, 1f);
			//g = PopplioMath.Clamp(
			//	PopplioRandom.Range(gInit - variation, gInit + variation), 0f, 1f);
			//b = PopplioMath.Clamp(
			//	PopplioRandom.Range(bInit - variation, bInit + variation), 0f, 1f);
		}
	}
}