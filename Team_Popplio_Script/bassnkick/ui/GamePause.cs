/******************************************************************************/
/*!
\file   GamePause.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\co-author Rio Shannon Yvon Leonardo, Pearl Goh Jia Tong
\contribution Shawn - 90% | Shannon - 5% | Pearl - 5%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/04/05 08:55:22 PM (Last Edit)
\brief
        BassNKick | C# file for game pause ui

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using BassNKick.Lib;
using Mono.Unix;

namespace BassNKick
{
	public class GamePause : PopplioMonoScript
	{
		float bgWidth = 1920f;

		float bgAnimTimer = 0f;

		float bgAnimDuration = 0.2f;

		public Entity bg = Entity.EntityField();
		Render2D? bgRender;

		public Entity resume = Entity.EntityField();
		public Entity restart = Entity.EntityField();
		public Entity quit = Entity.EntityField();
		public Entity options = Entity.EntityField();
        public Entity howtoplay = Entity.EntityField();

		public Entity tutorial = Entity.EntityField();

        // Sound effect entity reference
        [Inspector("Button SFX")]
        public Entity buttonSFX = Entity.EntityField();

        // Scene transition delay to allow for button sound
        private bool transitioning = false;
        private float countdown = 0.2f;
        private string targetScene = "";

        public override void Setup()
		{
			bgRender = bg.GetComponent<Render2D>();

            // validate for button SFX entity
            if (!buttonSFX.Exists() || !buttonSFX.HasComponent<Audio>())
            {
                Logger.Warning("Button SFX entity not assigned or missing Audio Component");
            }
        }

		public override void Update()
		{
			if (bgRender == null) return;

            // Handle scene transition with delay
            if (transitioning)
            {
                countdown -= Time.DeltaTimeF;

                if (countdown < 0.0f)
                {
                    Logger.Info($"Countdown finished, loading the next scene: {targetScene}");
                    if (targetScene == "Reload")
                    {
                        SceneManager.ReloadScene();
                    }
                    else
                    {
                        SceneManager.LoadScene(targetScene);
                    }
                    TempInputFix.ResetKeyCooldown();

                    // Reset the variables
                    transitioning = false;
                    countdown = 0.2f;
                    targetScene = "";
                }
            }

            if (GameManager.IsPaused)
			{
				if (!bg.Active)
				{
					bgAnimTimer = -bgAnimDuration;
					bg.Active = true;
				}

				if (bgAnimTimer < 0f)
				{
					bg.Transform2D.Position = new Vector2<float>(bgWidth * (bgAnimTimer / bgAnimDuration), 0f);
					bgRender.Color = new Color(1f, 1f, 1f, 1f + (bgAnimTimer / bgAnimDuration));
					bgAnimTimer += Time.DeltaTimeF;
				}
				else if (bgAnimTimer != 0f)
				{
					bg.Transform2D.Position = new Vector2<float>(0f, 0f);
					bgRender.Color = Color.White;
					bgAnimTimer = 0f;
				}

				if (tutorial.Active) return;

				if (Input.IsMouseButtonDown(Input.MouseButton.Left) && 
					resume.GetComponent<UI>().IsHovered)
				{
                    PlayButtonSound();
                    GameManager.TogglePause();
				}
				if (Input.IsMouseButtonDown(Input.MouseButton.Left) &&
					restart.GetComponent<UI>().IsHovered)
				{
                    PlayButtonSound();
                    //SceneManager.ReloadScene();
                    TransitionToScene("Reload");
                }
				if (Input.IsMouseButtonDown(Input.MouseButton.Left) &&
					quit.GetComponent<UI>().IsHovered)
				{
                    PlayButtonSound();
                    //SceneManager.LoadScene("LevelSelect");
                    //TransitionToScene("LevelSelect");
                    Team_Popplio.Libraries.Engine.AppQuit();
                }
                if (Input.IsMouseButtonDown(Input.MouseButton.Left) &&
                    options.GetComponent<UI>().IsHovered)
				{
                    PlayButtonSound();
                    //SceneManager.LoadScene("OptionsMenu");
                    TransitionToScene("OptionsMenu");
                }
                if (Input.IsMouseButtonDown(Input.MouseButton.Left) &&
                    howtoplay.GetComponent<UI>().IsHovered)
                {
                    PlayButtonSound();
                    tutorial.Active = true;
				}
            }
			else
			{
				if (!bg.Active) return;

				if (bgAnimTimer < bgAnimDuration)
				{
					bg.Transform2D.Position = new Vector2<float>(bgWidth * (bgAnimTimer / bgAnimDuration), 0f);
					bgRender.Color = new Color(1f, 1f, 1f, 1f - (bgAnimTimer / bgAnimDuration));
					bgAnimTimer += Time.DeltaTimeF;
				}
				else 
				{
					bg.Transform2D.Position = new Vector2<float>(bgWidth, 0f);
					bgRender.Color = Color.Transparent;
					bg.Active = false;
					bgAnimTimer = -bgAnimDuration;
				}
			}
		}

        // Play button sound effect
        private void PlayButtonSound()
        {
            if (buttonSFX.Exists() && buttonSFX.HasComponent<Audio>())
            {
                Audio audio = buttonSFX.GetComponent<Audio>();
                audio.Play();
                Logger.Info("Played button sound effect");
            }
            else
            {
                Logger.Warning("Button SFX entity not assigned or missing Audio Component!");
            }
        }

        private void TransitionToScene(string sceneName)
        {
            transitioning = true;
            countdown = 0.2f;
            targetScene = sceneName;
            Logger.Info($"Set up transition to scene: {sceneName}");
        }

    }
}
