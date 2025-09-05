/******************************************************************************/
/*!
\file   Fever.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        BassNKick | C# file for fever mode

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using BassNKick.Lib;
using Team_Popplio.Extensions;

namespace BassNKick
{
	public class Fever : PopplioMonoScript
	{
		[Inspector("BG")]
		public Entity bg = Entity.EntityField();
		[Inspector("Top")]
		public Entity top = Entity.EntityField();
		[Inspector("Bot")]
		public Entity bot = Entity.EntityField();
		[Inspector("Mana")]
		public Entity mana = Entity.EntityField();
		[Inspector("Text")]
		public Entity text = Entity.EntityField();
		[Inspector("Sparkles")]
		public Entity sparkles = Entity.EntityField();

		[Inspector("Bar")]
		public Entity bar = Entity.EntityField();

		Render2D? bgRender, topRender, botRender, manaRender, textRender, sparklesRender, barRender;


		float barWidth = 1920f;

		Vector2<float>? rot;
		float rotation = -5f;

		//float animTimer = 0f;
		//float animDuration = 0.5f;

		Vector2<float> initTop = new Vector2<float>();
		Vector2<float> initBot = new Vector2<float>();

		// actual fever mechanic
		public bool feverMode = false;
		public float progress = 0f;

		// settings
		float progPerNote = 0.1f;
		float feverModeDuration = 10f;

		public override void Setup()
		{
			bgRender = bg.GetComponent<Render2D>();
			topRender = top.GetComponent<Render2D>();
			botRender = bot.GetComponent<Render2D>();
			manaRender = mana.GetComponent<Render2D>();
			textRender = text.GetComponent<Render2D>();
			sparklesRender = sparkles.GetComponent<Render2D>();
			barRender = bar.GetComponent<Render2D>();

			rot = new Vector2<float>((float)Math.Cos(rotation * Math.PI / 180f), (float)Math.Sin(rotation * Math.PI / 180f));

			initTop = top.Transform2D.Position;
			initBot = bot.Transform2D.Position;

			Reset();
		}

		public override void Update()
		{
			DoFeverMode();
			FeverAnimation();

			if (barRender == null) return;
			if (feverMode) AnimateBar();
			else if (barRender.Color != Color.White) barRender.Color = Color.White;
		}

		public void SetProgPerNote(int notesPerFever)
		{
			progPerNote = 1f / (float)notesPerFever;
		}

		public void UpdateFever()
		{
			if (feverMode) return;

			progress += progPerNote;

			// update bar
			bar.Transform2D.Position = new Vector2<float>(
				(progress * barWidth / 2f) - (barWidth / 2f), bar.Transform2D.Position.Y);
			bar.Transform2D.Scale = new Vector2<float>(progress * barWidth, bar.Transform2D.Scale.Y);

			if (progress >= 1f)
			{
				Activate();
			}
		}

		void DoFeverMode()
		{
			if (!feverMode) return;

			if (progress <= 0f)
			{
				Deactivate();
				return;
			}

			// update bar
			bar.Transform2D.Position = new Vector2<float>(
				(progress * barWidth / 2f) - (barWidth / 2f), bar.Transform2D.Position.Y);
			bar.Transform2D.Scale = new Vector2<float>(progress * barWidth, bar.Transform2D.Scale.Y);

			// do fever mode stuff



			//Logger.Critical("Fever mode in progress");

			progress -= Time.DeltaTimeF / feverModeDuration;
		}

		void Activate()
		{
			feverMode = true;
			progress = 1f;

			bg.Active = true;
			top.Active = true;
			bot.Active = true;
			mana.Active = true;
			text.Active = true;
			sparkles.Active = true;

			GameManager.ui?.ToggleSpectrum();

			Logger.Critical("Fever mode activated");
		}

		void Deactivate()
		{
			progress = 0f;
			feverMode = false;

			// update bar
			bar.Transform2D.Position = new Vector2<float>(0f, bar.Transform2D.Position.Y);
			bar.Transform2D.Scale = new Vector2<float>(0f, bar.Transform2D.Scale.Y);

			bg.Active = false;
			top.Active = false;
			bot.Active = false;
			mana.Active = false;
			text.Active = false;
			sparkles.Active = false;

			GameManager.ui?.ToggleSpectrum();

			Reset();

			Logger.Critical("Fever mode deactivated");
		}

		void FeverAnimation()
		{
			if (!feverMode) return;

			AnimateBorder();
			AnimateBG();
			AnimateMana();
			AnimateText();
			AnimateSparkle();
		}

		int barChannel = 0;

		void AnimateBar()
		{
			//float animTimer = feverModeDuration - progress * feverModeDuration;
			if (barRender == null) return;
			Color temp = new Color(barRender.Color.R, barRender.Color.G, barRender.Color.B, barRender.Color.A); 
			barRender.Color = UILibrary.RainbowEffect(temp, barChannel, 0.5f, Time.DeltaTimeF, out int currChannel);
			barChannel = currChannel;
		}

		const float borderDist = 2000f;
		const float borderAnimDuration = 1.5f;

		void AnimateBorder()
		{
			//top.Active = false;
			//bot.Active = false;

			if (rot == null) return;

			float animTimer = feverModeDuration - progress * feverModeDuration;

			if (progress * feverModeDuration > feverModeDuration - borderAnimDuration)
			{
				// position over time
				float pos = 0.5f * PopplioMath.Pow(2f * ((animTimer / borderAnimDuration) - 0.5f), 3) + 0.5f;

				top.Transform2D.Position = borderDist * rot + initTop - borderDist * rot * (1f - pos);
				bot.Transform2D.Position = -borderDist * rot + initBot + borderDist * rot * (1f - pos);
			}
		}

		const float bgWidth = 1920f;
		const float bgAnimDuration = 0.2f;
		const float breathingAnimScale = 0.1f;

		void AnimateBG()
		{
			if (bgRender == null) return;

			float bgAnimTimer = feverModeDuration - progress * feverModeDuration;
			if (progress * feverModeDuration > feverModeDuration - bgAnimDuration)
			{
				bg.Transform2D.Position = new Vector2<float>(bgWidth - bgWidth * (bgAnimTimer / bgAnimDuration), 0f);
				bgRender.Color = new Color(1f, 1f, 1f, (bgAnimTimer / bgAnimDuration));
			}
			else if (progress * feverModeDuration > bgAnimDuration)
			{
				if (bg.Transform2D.Position != Vector2<float>.Zero) bg.Transform2D.Position = Vector2<float>.Zero;
				bgRender.Color = new Color(1f, 1f, 1f, 1f - (breathingAnimScale * (float)Math.Sin(bgAnimTimer) + breathingAnimScale));
			}
			else if (progress * feverModeDuration > 0f)
			{
				bgRender.Color = new Color(1f, 1f, 1f, (progress * feverModeDuration / bgAnimDuration));
			}
		}

		const float manaDist = 2500f;
		const float manaAnimDuration = 1.5f;

		void AnimateMana()
		{
			float animTimer = feverModeDuration - progress * feverModeDuration;

			if (progress * feverModeDuration > feverModeDuration - manaAnimDuration)
			{
				// position over time
				float pos = 0.5f * PopplioMath.Pow(2f * ((animTimer / manaAnimDuration) - 0.5f), 3) + 0.5f;

				mana.Transform2D.Position = new Vector2<float>(-manaDist + manaDist * pos * 2f, 0f);
			}
		}

		const float textDist = 1500f;
		const float textAnimDuration = 1.5f;

		void AnimateText()
		{
			float animTimer = feverModeDuration - progress * feverModeDuration;

			if (progress * feverModeDuration > feverModeDuration - textAnimDuration)
			{
				// position over time
				float pos = 0.5f * PopplioMath.Pow(2f * ((animTimer / textAnimDuration) - 0.5f), 3) + 0.5f;

				text.Transform2D.Position = new Vector2<float>(0f, -textDist + textDist * pos * 2f);
			}
		}

		void AnimateSparkle()
		{
			if (sparklesRender == null) return;

			float animTimer = feverModeDuration - progress * feverModeDuration;
			sparklesRender.Color = new Color(1f, 1f, 1f, 1f - (breathingAnimScale * (float)Math.Sin(animTimer) + breathingAnimScale));
		}

		void Reset()
		{
			if (bgRender == null || rot == null) return;

			bgRender.Color = Color.Transparent;
			bg.Transform2D.Position = new Vector2<float>(bgWidth, 0f);

			bot.Transform2D.Position = initBot + borderDist * rot;
			top.Transform2D.Position = initTop - borderDist * rot;

			mana.Transform2D.Position = new Vector2<float>(0f, -manaDist);
			text.Transform2D.Position = new Vector2<float>(0f, -textDist);
		}
	}
}