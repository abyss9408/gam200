/******************************************************************************/
/*!
\file   TutorialUI.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\co-author Rio Shannon Yvon Leonardo
\contribution Shawn - 100% | Shannon - 5%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/04/01 08:55:22 PM (Last Edit)
\brief
        BassNKick | C# file for Tutorial ui

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using BassNKick.Lib;
using Mac;
using System.Collections.Generic;

namespace BassNKick
{
	public class TutorialUI : PopplioMonoScript
	{
		public bool isTutorialScene = false;
		public string nextScene = "";

		public Entity tutorial = Entity.EntityField();
		public Entity button1 = Entity.EntityField();
		public Entity button2 = Entity.EntityField();
		public Entity button3 = Entity.EntityField();
		public Entity buttonLeft = Entity.EntityField();
		public Entity buttonRight = Entity.EntityField();
		public Entity buttonBack = Entity.EntityField();

        // Sound effect entity reference
        [Inspector("Button SFX")]
        public Entity buttonSFX = Entity.EntityField();

        // Scene transition delay to allow for button sound
        private bool transitioning = false;
        private float countdown = 0.2f;
        private string targetScene = "";

        List<string> pages = new List<string> { 
			"UI_HowToPlay_1", "UI_HowToPlay_2", "UI_HowToPlay_3", "UI_HowToPlay_4",
			"UI_HowToPlay_5", "UI_HowToPlay_6"};

		int currPage = -1;

		public override void Setup()
		{

            // Validate for button SFX entity
            if (!buttonSFX.Exists() || !buttonSFX.HasComponent<Audio>())
            {
                Logger.Warning("Button SFX entity not assigned or missing Audio Component");
            }

            if (buttonLeft.HasComponent<UI>())
			{
				buttonLeft.GetComponent<UI>().RegisterOnClickCallback("Left", Left);
			}
			if (buttonRight.HasComponent<UI>())
			{
				buttonRight.GetComponent<UI>().RegisterOnClickCallback("Right", Right);
			}
			if (buttonBack.HasComponent<UI>())
			{
				buttonBack.GetComponent<UI>().RegisterOnClickCallback("Back", Back);
			}

			currPage = 0;
		}

        public override void Update()
        {
            // Handle scene transition with delay
            if (transitioning)
            {
                countdown -= Time.DeltaTimeF;

                if (countdown < 0.0f)
                {
                    Logger.Info($"Countdown finished, loading the next scene: {targetScene}");
                    SceneManager.LoadScene(targetScene, true);
                    TempInputFix.ResetKeyCooldown();

                    // Reset the variables
                    transitioning = false;
                    countdown = 0.2f;
                    targetScene = "";
                }
            }

            // Check for button interactions
            CheckButtonInteractions();
        }

        private void CheckButtonInteractions()
        {
            // Check left button
            if (buttonLeft.Exists() && buttonLeft.HasComponent<UI>())
            {
                UI leftUI = buttonLeft.GetComponent<UI>();
                if (Input.IsMouseButtonDown(Input.MouseButton.Left) && leftUI.IsHovered)
                {
                    PlayButtonSound();
                }
            }

            // Check right button
            if (buttonRight.Exists() && buttonRight.HasComponent<UI>())
            {
                UI rightUI = buttonRight.GetComponent<UI>();
                if (Input.IsMouseButtonDown(Input.MouseButton.Left) && rightUI.IsHovered)
                {
                    PlayButtonSound();
                }
            }

            // Check back button
            if (buttonBack.Exists() && buttonBack.HasComponent<UI>())
            {
                UI backUI = buttonBack.GetComponent<UI>();
                if (Input.IsMouseButtonDown(Input.MouseButton.Left) && backUI.IsHovered)
                {
                    PlayButtonSound();
                }
            }
        }

        public override void Init()
		{
			string[] act = new string[] { "Left" };
			if (buttonLeft.HasComponent<UI>())
			{
				buttonLeft.GetComponent<UI>().SetOnClickActions(act);
			}
			act = new string[] { "Right" };
			if (buttonRight.HasComponent<UI>())
			{
				buttonRight.GetComponent<UI>().SetOnClickActions(act);
			}
			act = new string[] { "Back" };
			if (buttonBack.HasComponent<UI>())
			{
				buttonBack.GetComponent<UI>().SetOnClickActions(act);
			}

			if (!isTutorialScene) This.Active = false;
		}

		public override void OnEnable()
		{
			currPage = 0;

			button1.Hierarchy.InheritActive = false;
			button2.Hierarchy.InheritActive = false;
			button3.Hierarchy.InheritActive = false;

			button1.Active = true;
			button2.Active = true;
			button3.Active = true;
		}

		public override void OnDisable()
		{
			button1.Hierarchy.InheritActive = true;
			button2.Hierarchy.InheritActive = true;
			button3.Hierarchy.InheritActive = true;

			//button1.Active = false;
			//button2.Active = false;
			//button3.Active = false;
		}

		void UpdatePage(bool next)
		{
			if (next)
			{
				++currPage;
				if (currPage >= pages.Count)
				{
					currPage = 0;
				}
			}
			else
			{
				--currPage;
				if (currPage < 0)
				{
					currPage = pages.Count - 1;
				}
			}

			if (tutorial.HasComponent<Render2D>())
			{
				tutorial.GetComponent<Render2D>().Texture = pages[currPage];
			}

			if (currPage == 0)
			{
				button1.Active = true;
				button2.Active = true;
				button3.Active = true;
			}
			else
			{
				button1.Active = false;
				button2.Active = false;
				button3.Active = false;
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

        // Create a scene transition with delay for sound effect
        private void TransitionToScene(string sceneName)
        {
            transitioning = true;
            countdown = 0.2f;
            targetScene = sceneName;
            Logger.Info($"Set up transition to scene: {sceneName}");
        }

        void Left(int entityID)
		{
			Logger.Info($"{entityID} Prev Page");
            PlayButtonSound();
            UpdatePage(false);
		}

		void Right(int entityID)
		{
			Logger.Info($"{entityID} Next Page");
            PlayButtonSound();
            UpdatePage(true);
		}

		void Back(int entityID)
		{

            PlayButtonSound();

            if (isTutorialScene)
			{
				Logger.Info($"{entityID} Confirm");
                //SceneManager.LoadScene(nextScene);
                TransitionToScene(nextScene);
            }
			else
			{
				Logger.Info($"{entityID} Back");
				This.Active = false;
			}
		}
	}
}
