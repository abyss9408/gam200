/******************************************************************************/
/*!
\file   NoteFeedback.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for implementing the note feedback

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System;
using System.Numerics;

using Team_Popplio;
using Team_Popplio.Attributes;
using Team_Popplio.Libraries;
using Team_Popplio.Components;
using Team_Popplio.ECS;
using PopplioMath = Team_Popplio.Extensions.PopplioMath;

namespace BassNKick
{
	public class NoteFeedback : PopplioMonoScript
	{
		//[Header("Fields")]

		[Inspector("Note")]
		public Entity note = Entity.EntityField();

		[Inspector("Canvas")]
		public Entity canvas = Entity.EntityField();

		[Inspector("Miss")]
		public Entity miss = Entity.EntityField();
		[Inspector("Bad")]
		public Entity bad = Entity.EntityField();
		[Inspector("Nice")]
		public Entity nice = Entity.EntityField();
		[Inspector("Perfect")]
		public Entity perfect = Entity.EntityField();
		[Inspector("Text")]
		public Entity text = Entity.EntityField();
		[Inspector("VFX")]
		public Entity vfx = Entity.EntityField();

		[Inspector("Game Manager Obj")]
		public Entity gameManagerObj = Entity.EntityField();

		[Inspector("Is Final Feedback")]
		public bool isFinalFeedback = false;

		//string missTexture = "";
		//string badTexture = "";
		//string niceTexture = "";
		//string perfectTexture = "";

		//[Header("Settings")]

		//[Inspector("Anim Speed")]
		static float animSpeed = 1f;
		//[Inspector("Text Speed")]
		static float textSpeed = 500f;

#pragma warning disable CS0414
		//[Inspector("Modifier")]
		static float modifier = 2f;
#pragma warning restore CS0414 // The field 'NoteFeedback.modifier' is assigned but its value is never used

		//[Header("State")]
		[Inspector("Judge")]
		public string judge = ""; // todo use enum

		public int vfxLastFrame = 5;

		public int textLastStartFrame = 0;
		public int textLastEndFrame = 0;

		public float vfxFrameRate = 12f;
		public float textFrameRate = 24f;

		public bool isRainbow = false;


		//Vector2<float> initialText = new Vector2<float>();
		//Vector2<float> initialVFX = new Vector2<float>();


		float animTimer = 0f;

		//Coroutine destroyEntities = new Coroutine();

		// Update is called once per frame
		public override void Update()
		{
			if (text == null || vfx == null || 
				judge == "")
			{
				Logger.Error("feedback error");
				return;
			}

			// temporary fix
			if (text.Hierarchy.InheritTransform)
			{
				//text.Hierarchy.Parent = Entity.EntityField(); // set no parent
				
				// color assign
				if (judge == "miss")
				{
					//text.GetComponent<Render2D>().Texture = miss.GetComponent<Render2D>().Texture;
					//text.Transform2D.Scale = miss.Transform2D.Scale;

					//Vector2<float> pos = text.Transform2D.Position;
					//text = Entity.Instantiate(miss);
					//text.Transform2D.Position = pos;
					text = miss;
					text.GetComponent<Animation2D>().SetCurrentAnimation("start", true);
					textLastStartFrame = 7;
					textLastEndFrame = 3;
				}
				else if (judge == "meh")
				{
					//text.GetComponent<Render2D>().Texture = bad.GetComponent<Render2D>().Texture;
					//text.Transform2D.Scale = bad.Transform2D.Scale;

					//Vector2<float> pos = text.Transform2D.Position;
					//text = Entity.Instantiate(bad);
					//text.Transform2D.Position = pos;
					text = bad;
					text.GetComponent<Animation2D>().SetCurrentAnimation("start", true);
					textLastStartFrame = 7;
					textLastEndFrame = 3;
				}
				else if (judge == "great")
				{
					//text.GetComponent<Render2D>().Texture = nice.GetComponent<Render2D>().Texture;
					//text.Transform2D.Scale = nice.Transform2D.Scale;

					//Vector2<float> pos = text.Transform2D.Position;
					//text = Entity.Instantiate(nice);
					//text.Transform2D.Position = pos;
					text = nice;
					text.GetComponent<Animation2D>().SetCurrentAnimation("start", true);
					textLastStartFrame = 7;
					textLastEndFrame = 3;
				}
				else if (judge == "perfect")
				{
					//text.GetComponent<Render2D>().Texture = perfect.GetComponent<Render2D>().Texture;
					//text.Transform2D.Scale = perfect.Transform2D.Scale;

					//Vector2<float> pos = text.Transform2D.Position;
					//text = Entity.Instantiate(perfect);
					//text.Transform2D.Position = pos;
					text = perfect;
					text.GetComponent<Animation2D>().SetCurrentAnimation("start", true);
					text.Hierarchy.Children[0].GetComponent<Animation2D>().SetCurrentAnimation("sparkle", false);
					textLastStartFrame = 7;
					textLastEndFrame = 3;
				}

				//text.Hierarchy.Parent = Entity.EntityField(); // set no parent

				//initialText = text.Transform2D.Scale;
				//initialVFX = vfx.Transform2D.Scale;

				//vfx.GetComponent<Animation2D>().PlayOnce = true;
				//vfx.GetComponent<Animation2D>().Play();
				//Logger.Warning(perfect.ToString());

				if (!isRainbow)
				{
					vfx.Active = true;
				}
				else
				{
					Logger.Critical("rainbow");
					vfx.Hierarchy.InheritActive = false;
					vfx.Active = false;
				}

				vfx.Hierarchy.LocalPosition2D = new Vector2<float>(vfx.Hierarchy.LocalPosition2D.X, 0f);
				vfx.Hierarchy.InheritTransform = false;

				text.Active = true;
				text.Hierarchy.LocalPosition2D = new Vector2<float>(text.Hierarchy.LocalPosition2D.X, 0f);
				text.Hierarchy.InheritTransform = false;
			}
			
			// timer
			animTimer += Time.DeltaTimeF * animSpeed;

			// text animation
			//text.Transform2D.Scale = initialText * Lib.UILibrary.EaseOutElastic(animTimer);
			//text.Transform2D.Translate(Vector2<float>.Up * textSpeed);

			//text.Transform2D.Position.Y += textSpeed;

			// sprite animation
			//vfx.Transform2D.Scale = initialVFX * animTimer * modifier;

			// color animation
			//Color temp = vfx.GetComponent<Render2D>().Color;
			//temp.A = 1f - animTimer;
			//vfx.GetComponent<Render2D>().Color = temp;

			//Color temp = text.GetComponent<Render2D>().Color;
			//temp.A = 1f - animTimer;
			//text.GetComponent<Render2D>().Color = temp;

			//if (animTimer >= 1f / vfxFrameRate * (float)vfxLastFrame)
			//if (!isRainbow)
			if (vfx.GetComponent<Animation2D>().CurrentFrame.X == 0 &&
				vfx.GetComponent<Animation2D>().CurrentFrame.Y == 0)
			{
				//vfx.GetComponent<Render2D>().Color.A = 0;
				//destroyEntities.AddStep(async () =>
				//{
				//	Entity.Destroy(vfx);
				//	//vfx.Hierarchy.InheritActive = true;
				//	//vfx.Hierarchy.Parent = gameManagerObj.GetScript<GameManager>().destroyed;
				//});
				vfx.Active = false;
				vfx.GetComponent<Animation2D>().CurrentFrame = new Vector2<int>(0, 3);
			}

			if (judge == "perfect")
			{
				Color temp = text.Hierarchy.Children[0].GetComponent<Render2D>().Color;
				temp.A = 1f - animTimer;
				text.Hierarchy.Children[0].GetComponent<Render2D>().Color = temp;

				//text.Hierarchy.Children[0].Transform2D.Scale = initialText * Lib.UILibrary.EaseOutElastic(animTimer);
			}

			//text?.Transform2D.Translate(Vector2<float>.Up * textSpeed * Time.DeltaTimeF);
			text.Transform2D.Position += new Vector2<float>(0f, textSpeed * Time.DeltaTimeF);

			// destroy when animation ends
			//if (vfx.GetComponent<Render2D>().Color.A <= 0

			if (text.GetComponent<Animation2D>().CurrentAnimation == "start" && 
				animTimer >= 1f / textFrameRate * (float)textLastStartFrame)
			{
				if (judge == "great")
				{
					text?.GetComponent<Animation2D>().SetCurrentAnimation("linger", true);
				}
				else text?.GetComponent<Animation2D>().SetCurrentAnimation("end", true);
			}

			if (text?.GetComponent<Animation2D>().CurrentAnimation == "linger" &&
				animTimer >= 1f / textFrameRate * (float)textLastStartFrame + 1f / textFrameRate * 2f)
			{
				text?.GetComponent<Animation2D>().SetCurrentAnimation("end", true);
			}

			if (judge != "great")
			{
				if (text?.GetComponent<Animation2D>().CurrentAnimation == "end" &&
					animTimer >= 1f / textFrameRate * (float)textLastStartFrame + 1f / textFrameRate * (float)textLastEndFrame)
				{
					text.Active = false;
					//destroyEntities.AddStep(async () =>
					//{
					//	Entity.Destroy(text);
					//	//text.Hierarchy.InheritActive = true;
					//	//text.Hierarchy.Parent = gameManagerObj.GetScript<GameManager>().destroyed;
					//});
					//Logger.Warning("destroyed");
				}
			}
			else
			{
				if (text?.GetComponent<Animation2D>().CurrentAnimation == "end" &&
					animTimer >= 1f / textFrameRate * (float)textLastStartFrame + 1f / textFrameRate * 2f + 1f / textFrameRate * (float)textLastEndFrame)
				{
					text.Active = false;
				}
			}

			if (text == null || vfx == null) return;

			if (!vfx.Active && !text.Active) // reset
			{
				Reset();
			}

			//RunDestroy();
		}

		void Reset()
		{
			animTimer = 0f;

			text.Transform2D.Position = This.Transform2D.Position;
			vfx.Transform2D.Position = This.Transform2D.Position;

			text.Hierarchy.InheritTransform = true;
			vfx.Hierarchy.InheritTransform = true;

			isRainbow = false;

			if (isFinalFeedback)
			{
				note.GetScript<NoteProperties>().Recycle();

				//This.Transform2D.Position = note.Transform2D.Position;
			}

			vfx.Hierarchy.InheritActive = true;
			vfx.Active = false;

			This.Active = false;

			if (isFinalFeedback)
				note.Active = false;
		}

		// to animate feedback
		//float EaseOutElastic(float x)
		//{
		//	if (x > 1f) return 1f;
		//	else if (x < 0f) return 0f;

		//	float c4 = (2f * (float)Math.PI) / 3f;

		//	return PopplioMath.Pow(2f, -10f * x) * (float)Math.Sin((x * 10f - 0.75f) * c4) + 1f;
		//}

		//async void RunDestroy()
		//{
		//	await destroyEntities.Run();
		//}
	}
}
