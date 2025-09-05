/******************************************************************************/
/*!
\file   GameUI.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        BassNKick | C# file for game UI

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System.Collections.Generic;
using System.Numerics;
using Team_Popplio;
using Team_Popplio.Components;
using Team_Popplio.Libraries;
using Team_Popplio.Attributes;
using Team_Popplio.ECS;
using BassNKick.Lib;
using Team_Popplio.Extern;

namespace BassNKick
{
	public class GameUI : PopplioMonoScript
	{
		[Inspector("Game Manager Obj")]
		public Entity gameManagerObj = Entity.EntityField();

		[Inspector("Instrument Sprite Objs")]
		public Entity[] instrumentSpriteObjs = { };
		[InspectorIgnore]
		public Render2D[] instrumentSprites = { };

		public Entity missObj = Entity.EntityField();
		public Entity badObj = Entity.EntityField();
		public Entity niceObj = Entity.EntityField();
		public Entity perfectObj = Entity.EntityField();

		public Entity overlayObj = Entity.EntityField();

		public Entity darkenObj = Entity.EntityField();

		float fadeInDuration = 0f;
		float fadeInTimer = 0f;

		[Inspector("JParticles Obj")]
		Entity jParticlesObj = Entity.EntityField();
		public JParticles? jParticles;

		[Inspector("Pause Menu Obj")]
		Entity pauseMenuObj = Entity.EntityField();

		public Entity rainbow1 = Entity.EntityField();
		public Entity rainbow2 = Entity.EntityField();
		public Entity rainbow3 = Entity.EntityField();

		public Entity audioSpectrum = Entity.EntityField();

		public Entity fullCombo = Entity.EntityField();

		float initialCamZoom = 0f;
		//float pauseCamZoom = 0.95f;
		float pauseOverlayAlpha = 0.5f;

		public override void Setup()
		{
			instrumentSprites = new Render2D[instrumentSpriteObjs.Length];
			for (int i = 0; i < instrumentSpriteObjs.Length; ++i)
			{
				instrumentSprites[i] = instrumentSpriteObjs[i].GetComponent<Render2D>();
			}
			jParticles = jParticlesObj.GetScript<JParticles>();

			initialCamZoom = Camera.Zoom;
		}

		public override void Init()
		{
			overlayObj.GetComponent<Render2D>().Color = Color.Black;
			fadeInDuration = gameManagerObj.GetScript<GameManager>().SettingsObj.GetScript<Settings>().levelFadeInTime;

			darkenObj.GetComponent<Render2D>().Color = new Color(0, 0, 0,
				gameManagerObj.GetScript<GameManager>().SettingsObj.GetScript<Settings>().customOverlayOpacity);

			rainbow1.GetComponent<Animation2D>().PlayOnce = true;
			rainbow2.GetComponent<Animation2D>().PlayOnce = true;
			rainbow3.GetComponent<Animation2D>().PlayOnce = true;
		}

		public override void Update()
		{
			if (fadeInTimer < fadeInDuration)
			{
				fadeInTimer += Time.DeltaTimeF;
				overlayObj.GetComponent<Render2D>().Color =
					new Color(0, 0, 0, 1 - fadeInTimer / fadeInDuration);
			}
			else if (!GameManager.IsPaused && overlayObj.GetComponent<Render2D>().Color.A != 0f)
				overlayObj.GetComponent<Render2D>().Color = Color.Transparent;

			UpdateRainbow();
			UpdateFC(fullCombo);

			if (TempInputFix.IsKeyTriggered(Keybinds.pause)) GameManager.TogglePause();

			Pause();
		}

		void Pause()
		{
			if (GameManager.IsPaused)
			{
				//if (Time.FixedDeltaTime == 0.0) return;
				//if (Camera.Zoom != pauseCamZoom)

				//Camera.Zoom = pauseCamZoom * initialCamZoom;

				//if (overlayObj.GetComponent<Render2D>().Color.A != pauseOverlayAlpha)
				overlayObj.GetComponent<Render2D>().Color = new Color(0, 0, 0, pauseOverlayAlpha);

				//Time.SetFixedDeltaTime(0.0);
				//pauseMenuObj.Active = true;
			}
			else
			{
				//if (Time.FixedDeltaTime > 0.0) return;

				//if (Camera.Zoom != initialCamZoom)
				
				//Camera.Zoom = initialCamZoom;
				
				//if (overlayObj.GetComponent<Render2D>().Color.A != 0f)
				//	overlayObj.GetComponent<Render2D>().Color = Color.Transparent;

				//Time.ResetFixedDeltaTime();
				//pauseMenuObj.Active = false;
			}
		}

		public void RainbowVFX(int lane)
		{
			switch (lane)
			{
				case 1:
					if (rainbow1.Active) ResetRainbow(1);
					rainbow1.Active = true;
					rainbow1.GetComponent<Animation2D>().Play();
					break;
				case 2:
					if (rainbow2.Active) ResetRainbow(2);
					rainbow2.Active = true;
					rainbow2.GetComponent<Animation2D>().Play();
					break;
				case 3:
					if (rainbow3.Active) ResetRainbow(3);
					rainbow3.Active = true;
					rainbow3.GetComponent<Animation2D>().Play();
					break;
			}
		}

		void ResetRainbow(int lane)
		{
			switch (lane)
			{
				case 1:
					rainbow1.GetComponent<Animation2D>().Stop();
					rainbow1.GetComponent<Animation2D>().CurrentFrame = new Vector2<int>(0, 3);
					rainbow1.Active = false;
					break;
				case 2:
					rainbow2.GetComponent<Animation2D>().Stop();
					rainbow2.GetComponent<Animation2D>().CurrentFrame = new Vector2<int>(0, 3);
					rainbow2.Active = false;
					break;
				case 3:
					rainbow3.GetComponent<Animation2D>().Stop();
					rainbow3.GetComponent<Animation2D>().CurrentFrame = new Vector2<int>(0, 3);
					rainbow3.Active = false;
					break;
			}
		}

		void UpdateRainbow()
		{
			if (rainbow1.Active)
			{
				if (!rainbow1.GetComponent<Animation2D>().IsPlaying)
					ResetRainbow(1);
			}
			if (rainbow2.Active)
			{
				if (!rainbow2.GetComponent<Animation2D>().IsPlaying)
					ResetRainbow(2);
			}
			if (rainbow3.Active)
			{
				if (!rainbow3.GetComponent<Animation2D>().IsPlaying)
					ResetRainbow(3);
			}
		}

		public void ToggleSpectrum()
		{
			audioSpectrum.Active = !audioSpectrum.Active;
		}

		public void FC()
		{
			StaticFC(fullCombo);
		}

		const float fcSheetBWidth = 3072f;
		const float fcSheetBHeight = 2880f;

		public static void StaticFC(Entity fc)
		{
			fc.Active = true;
			fc.GetComponent<Animation2D>().AnimSpeed("A", 1f / 48f);
			fc.GetComponent<Animation2D>().AnimSpeed("B", 1f / 48f);
			fc.GetComponent<Animation2D>().PlayOnce = true;
			fc.GetComponent<Animation2D>().Play();
		}

		public static void UpdateFC(Entity fc)
		{
			if (fc.Active)
			{
				if (!fc.GetComponent<Animation2D>().IsPlaying)
				{
					if (fc.GetComponent<Render2D>().Texture == "UI_Animation_FullCombo_spritesheet_01")
					{
						fc.GetComponent<Render2D>().Texture = "UI_Animation_FullCombo_spritesheet_02";

						fc.GetComponent<Animation2D>().SheetWidth = fcSheetBWidth;
						fc.GetComponent<Animation2D>().SheetHeight = fcSheetBHeight;
						fc.GetComponent<Animation2D>().SetCurrentAnimation("B", true);
						fc.GetComponent<Animation2D>().CurrentFrame = new Vector2<int>(0, 5);
						fc.GetComponent<Animation2D>().Play();
					}
					else
					{
						//fc.Active = false;
						fc.GetComponent<Animation2D>().CurrentFrame = new Vector2<int>(5, 0);
					}
				}
			}
		}
	}
}
