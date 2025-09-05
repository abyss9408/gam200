/******************************************************************************/
/*!
\file   OptionsManager.cs
\author Team Popplio
\author Rio Shannon Yvon Leonardo
\co-author Ng Jun Heng Shawn
\contribution Shannon - 90% | Shawn - 10%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13
\brief
        C# file for managing Options Buttons and savings

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using BassNKick.Lib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BassNKick
{
    public class OptionsManager : PopplioMonoScript
    {
        //reference to the Settings entity/script
        //[Inspector("Settings Obj")]
        //public Entity SettingsObj = Entity.EntityField();
        //public Settings? settings;

        //reference to back button
        [Inspector("Back Button")]
        public Entity backButton = Entity.EntityField();

        private UI? backButtonUI;

		[Inspector("Restore Button")]
		public Entity restoreButton = Entity.EntityField();

		[Inspector("Calibration Button")]
		public Entity calibrationButton = Entity.EntityField();

        //reference to button sound effect entity
        [Inspector("Button SFX")]
        public Entity buttonSFX = Entity.EntityField();

        //scene transistion delay for button sounds
        private bool nextScene = false;
        private float countdown = 0.2f;
        private string nextSceneName = "";


        // previous scene
        static string prev = "";

		public override void Setup()
        {

            //validate button SFX entity
            if (!buttonSFX.Exists() || !buttonSFX.HasComponent<Audio>())
            {
                Logger.Warning("Button SFX entity not assigned or missing Audio Component!-hi");
            }


            //get Settings script from SettingsObj
            //if (SettingsObj != null)
            //{
            //    settings = SettingsObj.GetScript<Settings>();

            //    if (settings != null)
            //    {
            //        //load settings
            //        settings.Load();
            //        Logger.Info("OptionsManager.cs | Settings loaded successfully");
            //    }
            //    else
            //    {
            //        Logger.Error("OptionsManager.cs | Failed to get Settings script");
            //    }
            //}
            //else
            //{
            //    Logger.Error("OptionsManager.cs | SettingsObj is not assigned");
            //}

            Lib.StaticSettings.Load();

            //get UI component for back button
            if(backButton != null && backButton.HasComponent<UI>())
            {
                backButtonUI = backButton.GetComponent<UI>();
                backButtonUI.RegisterOnClickCallback("BACK_TO_MAINMENU", ReturnToMainMenu);
                Logger.Info("Back button callback registered.");
            } else
            {
                Logger.Error("OptionsManager.cs | Back button not assigned or missing UI component");
            }

			if (restoreButton != null && restoreButton.HasComponent<UI>())
			{
				restoreButton.GetComponent<UI>().RegisterOnClickCallback("RESTORE", Restore);
				Logger.Info("Restore button callback registered.");
			}
			else
			{
				Logger.Error("OptionsManager.cs | Restore button not assigned or missing UI component");
			}

			if (calibrationButton != null && calibrationButton.HasComponent<UI>())
			{
				calibrationButton.GetComponent<UI>().RegisterOnClickCallback("CALIBRATE", Calibrate);
				Logger.Info("Calibrate button callback registered.");
			}
			else
			{
				Logger.Error("OptionsManager.cs | Calibrate button not assigned or missing UI component");
			}

            // prevent loop
            if (SceneManager.GetPrevious() != "Calibration") prev = SceneManager.GetPrevious();
		}

        public override void Update()
        {
            //handle scene transistion with delay
            if (nextScene)
            {
                countdown -= Time.DeltaTimeF;

                if (countdown <= 0.0f)
                {
                    Logger.Info($"Countdown finished, loading the next scene: {nextSceneName}");
                    SceneManager.LoadScene(nextSceneName);
                    Lib.TempInputFix.ResetKeyCooldown();

                    // reset variables for next scene
                    nextScene = false;
                    countdown = 0.2f;
                    nextSceneName = "";
                }
            }

            if (backButtonUI == null)
            {
                return;
            }

            //detects a click on Back Button
            if (TempInputFix.IsMouseTriggered(Input.MouseButton.Left) && backButtonUI.IsHovered)
            {
                PlayButtonSound();
                ReturnToMainMenu(0);
            }
			if (TempInputFix.IsMouseTriggered(Input.MouseButton.Left) && restoreButton.GetComponent<UI>().IsHovered)
			{
                PlayButtonSound();
                Restore(0);
			}
			if (TempInputFix.IsMouseTriggered(Input.MouseButton.Left) && calibrationButton.GetComponent<UI>().IsHovered)
			{
                PlayButtonSound();
                Calibrate(0);
			}
		}

        //play button sound effect
        public void PlayButtonSound()
        {
            if (buttonSFX.Exists() && buttonSFX.HasComponent<Audio>())
            {
                Audio audio = buttonSFX.GetComponent<Audio>();
                audio.Play();
                Logger.Info("Played button sound effect");
            } else
            {
                Logger.Warning("Button SFX entity not assigned or missing Audio Component! - hellloooo");
            }
        }

        //set up a transistion to scene with delay for sound effect
        public void TransitionToScene(string sceneName)
        {
            nextScene = true;
            countdown = 0.2f;
            nextSceneName = sceneName;
            Logger.Info($"Set up transition to scene: {sceneName}");
        }

        private void ReturnToMainMenu(int id)
        {
            Logger.Info("Back Button is clicked!");
            Lib.StaticSettings.Save();
            //SceneManager.Load("MainMenu");
            //string prev = SceneManager.GetPrevious();
            //if (prev != "" && prev != SceneManager.GetCurrent()) SceneManager.LoadScene(prev);
            // else SceneManager.LoadScene("MainMenu");

            if (prev != "" && prev != SceneManager.GetCurrent())
                TransitionToScene(prev);
            else
                TransitionToScene("MainMenu");

        }

        void Restore(int id)
		{
			Logger.Info("Restore Button is clicked!");
			Lib.StaticSettings.Restore();
		}

        void Calibrate(int id)
        {
			Logger.Info("Calibrate Button is clicked!");
            //SceneManager.LoadScene("Calibration");
            TransitionToScene("Calibration");
        }
	}
}
