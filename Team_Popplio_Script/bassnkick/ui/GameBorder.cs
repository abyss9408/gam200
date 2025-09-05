/******************************************************************************/
/*!
\file   GameBorder.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        BassNKick | C# file for border animation

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BassNKick
{
	public class GameBorder : PopplioMonoScript
	{
		[Inspector("Rotation")]
		public float rotation = 0f;

		[Inspector("Score")]
		public Entity scoreObj = Entity.EntityField();
		Score? score;

		// width of border
		public float width = 0f;

		public float minspeed = 5f;
		public float maxspeed = 100f;

		// min max for speed
		public int minCombo = 0;
		public int maxCombo = 50;

		public float acceleration = 5f;
		public float deceleration = 0.4f;

		float currentSpeed = 0f;
		float currentLowerSpeed = 0f;
		float currentUpperSpeed = 0f;

		// used to detect change in combo in Score script
		int currentCombo = 0;

		Vector2<float> rot = new Vector2<float>(); // unit vector for direction of movement

		Vector2<float> initialPos = new Vector2<float>();

		Render2D? renderA;
		Render2D? renderB;
		float minAlpha = 0f;
		const float alphaDec = 0.1f;
		const float alphaInc = 0.1f;

		Text2D? text0;
		Text2D? text1;

		const string textP = "PERFECTPERFECTPERFECTPERFECTPERFECTPERFECTPERFECTPERFECTPERFECT";
		const string textN = "NICENICENICENICENICENICENICENICENICENICENICENICENICENICENICENICENICENICENI";
		const string textB = "BADBADBADBADBADBADBADBADBADBADBADBADBADBADBADBADBADBADBADBA";
		const string textM = "MISSMISSMISSMISSMISSMISSMISSMISSMISSMISSMISSMISSMISSMISSMISSMISSMISS";
		//const string textF = "FEVERFEVERFEVERFEVERFEVERFEVERFEVERFEVERFEVERFEVERFEVERFEVERFE";
		const string textG = "GOGOGOGOGOGOGOGOGOGOGOGOGOGOGOGOGOGOGOGOGOGOGOGOGOGOGOGOGOGO";

		const float textAlpha = 0.2f;

		public override void Setup()
		{
			if (scoreObj.Exists()) score = scoreObj.GetScript<Score>();
			rot = new Vector2<float>((float)Math.Cos(rotation * Math.PI / 180f), (float)Math.Sin(rotation * Math.PI / 180f));
			initialPos = This.Transform2D.Position;
			if (This.Hierarchy.Children[0].HasComponent<Render2D>()) renderA = This.Hierarchy.Children[0].GetComponent<Render2D>();
			if (This.Hierarchy.Children[1].HasComponent<Render2D>()) renderB = This.Hierarchy.Children[1].GetComponent<Render2D>();
			if (renderA != null) minAlpha = renderA.Color.A;
			if (This.Hierarchy.Children[0].HasComponent<Text2D>())
			{
				text0 = This.Hierarchy.Children[0].GetComponent<Text2D>();
				text0.Text = textG;
				text0.Color = new Color(0f, 1f, 0f, textAlpha); // Color.Green;
				//text0.Color = Color.Green;
				//text0.Color.A = textAlpha;
			}
			if (This.Hierarchy.Children[1].HasComponent<Text2D>())
			{
				text1 = This.Hierarchy.Children[1].GetComponent<Text2D>();
				text1.Text = textG;
				text1.Color = new Color(0f, 1f, 0f, textAlpha); // Color.Green;
				//text1.Color = Color.Green;
				//text1.Color.A = textAlpha;
			}
		}

		public override void Update()
		{
			if (GameManager.IsPaused) return;

			MoveBorder();
			Accelerate();

			TextBorder();
		}

		int currentJudgement = -1;

		void TextBorder()
		{
			if (text0 == null || text1 == null) return;

			
				//case 4:
				//	text0.Text = textF;
				//	text1.Text = textF;
				//	break;

			if (currentJudgement == score?.currJudgement) return;

			switch (score?.currJudgement)
			{
				case 0:
					text0.Text = textM;
					text1.Text = textM;
					text0.Color = new Color(0.5f, 0.5f, 0.5f, textAlpha); 
					text1.Color = new Color(0.5f, 0.5f, 0.5f, textAlpha); 
					//text0.Color = Color.Gray;
					//text0.Color.A = textAlpha;
					//text1.Color = Color.Gray;
					//text1.Color.A = textAlpha;
					break;
				case 1:
					text0.Text = textB;
					text1.Text = textB;
					text0.Color = new Color(1f, 1f, 1f, textAlpha);
					text1.Color = new Color(1f, 1f, 1f, textAlpha);
					//text0.Color = Color.White;
					//text0.Color.A = textAlpha;
					//text1.Color = Color.White;
					//text1.Color.A = textAlpha;
					break;
				case 2:
					text0.Text = textN;
					text1.Text = textN;
					text0.Color = new Color(0f, 0f, 1f, textAlpha);
					text1.Color = new Color(0f, 0f, 1f, textAlpha);
					//text0.Color = Color.Blue;
					//text0.Color.A = textAlpha;
					//text1.Color = Color.Blue;
					//text1.Color.A = textAlpha;
					break;
				case 3:
					text0.Text = textP;
					text1.Text = textP;
					text0.Color = new Color(1f, 0f, 1f, textAlpha);
					text1.Color = new Color(1f, 0f, 1f, textAlpha);
					//text0.Color = Color.Magenta;
					//text0.Color.A = textAlpha;
					//text1.Color = Color.Magenta;
					//text1.Color.A = textAlpha;
					break;
				default:
					text0.Text = textG;
					text1.Text = textG;
					text0.Color = new Color(0f, 1f, 0f, textAlpha); // Color.Green;
					text1.Color = new Color(0f, 1f, 0f, textAlpha); // Color.Green;
					//text0.Color = Color.Green;
					//text0.Color.A = textAlpha;
					//text1.Color = Color.Green;
					//text1.Color.A = textAlpha;
					break;
			}
		}

		void MoveBorder()
		{
			if (currentSpeed < minspeed) currentSpeed = minspeed;

			Vector2<float> move = new Vector2<float>(rot.X * currentSpeed * Time.DeltaTimeF, rot.Y * currentSpeed * Time.DeltaTimeF);

			This.Transform2D.Position += move;

			if (Mathematics.Distance(This.Transform2D.Position, initialPos) >= width)
				This.Transform2D.Position = initialPos;

		}

		public void Accelerate()
		{
			if (score == null || renderA == null || renderB == null) return;

			if (currentCombo != score.currCombo)
			{
				currentCombo = score.currCombo;
				currentSpeed += acceleration;
				currentUpperSpeed = currentSpeed;

				if (currentCombo >= minCombo && currentCombo <= maxCombo)
					currentLowerSpeed = minspeed + (maxspeed - minspeed) * (currentCombo - minCombo) / (maxCombo - minCombo);

				if (currentSpeed < currentLowerSpeed) currentSpeed = currentLowerSpeed;
				if (currentSpeed > maxspeed) currentSpeed = maxspeed;

				if (currentUpperSpeed < currentSpeed)
				{
					currentSpeed = currentUpperSpeed;
				}

				// alpha
				if (renderA.Color.A < 1f)
				{
					Color col = new Color(renderA.Color.R, renderA.Color.G, renderA.Color.B, renderA.Color.A + alphaInc);
					renderA.Color = col;
					renderB.Color = col;
					//Logger.Error("1");
				}
				else if (renderA.Color.A > 1f)
				{
					renderA.Color = Color.White;
					renderB.Color = Color.White;
					//Logger.Error("2");
				}
			}
			else
			{
				currentSpeed -= deceleration * Time.DeltaTimeF;
				currentUpperSpeed -= deceleration * Time.DeltaTimeF;

				// alpha
				if (renderA.Color.A > minAlpha)
				{
					Color col = new Color(renderA.Color.R, renderA.Color.G, renderA.Color.B, 
						renderA.Color.A - alphaDec * Time.DeltaTimeF);
					renderA.Color = col;
					renderB.Color = col;
					//Logger.Error("3");
				}
				else if (renderA.Color.A < minAlpha)
				{
					Color col = new Color(renderA.Color.R, renderA.Color.G, renderA.Color.B, minAlpha);
					renderA.Color = col;
					renderB.Color = col;
					//Logger.Error("4");
				}
			}
		}
	}
}
