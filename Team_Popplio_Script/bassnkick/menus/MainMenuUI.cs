/******************************************************************************/
/*!
\file   MainMenu.cs
\author Team Popplio
\author Tang Xian Hui
\contribution XH - 3% | Hafa - 84% | Shawn - 8% | Shannon - 5%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/11  (Last Edit)
\brief
        BassNKick | Main Menu UI with Hover Effect

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
using System.Collections.Generic;
using System.Data.Linq.Mapping;
using BassNKick.Lib;

namespace BassNKick
{
    public class MainMenuUI : PopplioMonoScript
    {
        //variables for button sound and delayed scene transitions
        private bool nextScene = false;
        private float countdown = 0.2f;
        private string nextSceneName = "";

        public static bool gameFirstRun = true;

        private UI? uiComponent = null;

        //adding reference to button sound effect entity
        [Inspector("Button SFX")]
        public Entity buttonSFX = Entity.EntityField();

        private float initX = 0f;
        private float initY = 0f;

        public override void Setup()
        {
            Logger.Info("mainMenuUI.cs | Setup()");

            if (This.HasComponent<UI>())
            {
                uiComponent = This.GetComponent<UI>();
                Logger.Info("UIComponent Found!");
            }
            else
            {
                Logger.Error("UIComponent NOT Found on Entity!");
            }
        }

        public override void Init()
        {
            // Logger.Info("TestUI.cs | Init()");

            if (uiComponent == null)
            {
                Logger.Error("UIComponent is NULL, cannot initialize.");
                return;
            }

            if (uiComponent.Type == UIType.BUTTON)
            {
                Logger.Info("Registering UI Callbacks from C#");

                if (This.HasComponent<UI>())
                {
                    UI uiComponent = This.GetComponent<UI>();

                    initX = This.Transform2D.Scale.X;
                    initY = This.Transform2D.Scale.Y;

                    // Correct registration (fixes delegate issue)
                    uiComponent.RegisterOnClickCallback("Start_Game", StartFunction);
                    uiComponent.RegisterOnClickCallback("OPEN_SETTINGS", OptionsFunction);
                    uiComponent.RegisterOnClickCallback("BACK_TO_MAINMENU", MainMenuFunction);
                    uiComponent.RegisterOnClickCallback("CREDITS", CreditsFunction);


                    Logger.Info("Registered Callbacks Successfully.");
                }
                else
                {
                    Logger.Error("No UIComponent found on entity!");
                }

            }
        }

        public override void Update()
        {
            if (uiComponent == null) return;

            //handle scene transition with delay to allow for sound to play first
            if (nextScene)
            {
                //logging countdown value for testing
                Logger.Info($"Countdown: {countdown}");

                countdown -= Time.DeltaTimeF;

                //time concludes, load the next scene 
                if (countdown <= 0.0f)
                {
                    Logger.Info($"Countdown finished, loading the next scene: {nextSceneName}");

                    if (Lib.Profile.playerNoob && nextSceneName == "Start")
                    {
                        Logger.Info("comes here");

                        // now first time is false
                        //Lib.Profile.playerNoob = false;
                        //Lib.Profile.Save();

                        // go to level 1 as first time
                        //SceneManager.Load("Track1");
                        //SceneManager.LoadScene("Calibration");
                        //            TempInputFix.ResetKeyCooldown();

                        StaticCutsceneData.PlayThisCutscene = "Cutscene_1_Prolouge_1";
                        //nextSceneName = "Cutscene";
                        SceneManager.LoadScene("Cutscene");
                        TempInputFix.ResetKeyCooldown();

                    }
                    else if (nextSceneName == "Start")
                    {
                        Logger.Info("comes here instead");
                        SceneManager.LoadScene("LevelSelect");
                        TempInputFix.ResetKeyCooldown();
                        //nextSceneName = "LevelSelect";
                    }

                    //if (nextSceneName == "Cutscene" && Lib.Profile.playerNoob)
                    //{
                    //    StaticCutsceneData.PlayThisCutscene = "Cutscene_1_Prolouge_1";
                    //    Logger.Info("Setting cutscene data: Cutscene_1_Prolouge_1");
                    //}
                    else
                    {
                        SceneManager.LoadScene(nextSceneName);
                        TempInputFix.ResetKeyCooldown();
                    }

                    //reset variables
                    nextScene = false;
                    countdown = 0.2f;
                    nextSceneName = "";
                }
            }

            if (uiComponent.Type == UIType.BUTTON && uiComponent.IsHovered)
            {
				string[] actions = uiComponent.GetOnClickActions();

                if (actions[0] == "CREDITS")
                {
                    //Logger.Critical("HELP");
					This.Transform2D.Scale = new Vector2<float>(initX * 1.1f, initY * 1.1f);
				}

				if (Input.IsMouseButtonDown(Input.MouseButton.Left))
                {
                    Logger.Info($"OnClick Actions in Update: {string.Join(", ", actions)}");

                    int entityID = uiComponent.GetEntityID(); //

                    //play button sound effect
                    PlayButtonSound();

                    if (actions[0] == "Start_Game")
                    {
                        StartFunction(entityID);
                    }
                    if (actions[0] == "OPEN_SETTINGS")
                    {
                        OptionsFunction(entityID);
                    }
                    if (actions[0] == "BACK_TO_MAINMENU")
                    {
                        MainMenuFunction(entityID);
                    }
                    if (actions[0] == "CREDITS")
                        CreditsFunction(entityID);
                }
            }
            else if (uiComponent.Type == UIType.BUTTON && !uiComponent.IsHovered)
            {
				string[] actions = uiComponent.GetOnClickActions();
				if (actions[0] == "CREDITS")
                {
					This.Transform2D.Scale = new Vector2<float>(initX, initY);
				}
			}

			if (Lib.TempInputFix.IsKeyTriggered(Lib.Keybinds.exit))
                Engine.AppQuit();
        }

        public override void End()
        {
            Logger.Info("mainMenuUI.cs | End()");
        }

        //method to play button sound
        private void PlayButtonSound()
        {
            if (buttonSFX != null && buttonSFX.HasComponent<Audio>())
            {
                Audio audio = buttonSFX.GetComponent<Audio>();
                audio.Play();
                Logger.Info("Play button sound effect");
            }
            else
            {
                Logger.Warning("Button SFX entity not assigned or missing Audio Component!");
            }
        }


        public void StartFunction(int entityID)
        {
            Logger.Info($"button_clicked function triggered for entity ID: {entityID}!");

            nextSceneName = "Start";

            //start countdown and set the next scene
            nextScene = true;
            countdown = 0.2f;
        }
        public void OptionsFunction(int entityID)
        {
            Logger.Info($"button_clicked function triggered for entity ID: {entityID}!");
            //SceneManager.LoadScene("OptionsMenu");
            //TempInputFix.ResetKeyCooldown();
            nextScene = true;
            countdown = 0.2f;
            nextSceneName = "OptionsMenu";

        }
        public void MainMenuFunction(int entityID)
        {
            Logger.Info($"button_clicked function triggered for entity ID: {entityID}!");
            //SceneManager.LoadScene("MainMenu");
            //TempInputFix.ResetKeyCooldown();
            nextScene = true;
            countdown = 0.2f;
            nextSceneName = "MainMenu";

        }
        public void CreditsFunction(int entityID)
        {
			Logger.Info($"button_clicked function triggered for entity ID: {entityID}!");

			This.Transform2D.Scale = new Vector2<float>(initX * 0.9f, initY * 0.9f);

			nextScene = true;
			countdown = 0.2f;
			nextSceneName = "Credits";
		}
    }
}


