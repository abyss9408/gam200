/******************************************************************************/
/*!
\file   LevelSelect.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\co-author Rio Shannon Yvon Leonardo, Pearl Goh Jia Tong
\contribution Shawn - 90% | Shannon - 5% | Pearl - 5%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/04/05 08:55:22 PM (Last Edit)
\brief
        C# file for level select menu

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
using RabbitMQ.Client.Framing.Impl.v0_8;
using Mono.WebServer.Options.Settings;

namespace BassNKick
{
    public class LevelSelect : PopplioMonoScript
    {
        public Entity[] levelButtons = { };

        const float levelButtonScaleX = 463f;
        const float levelButtonScaleY = 462f;
        const float hoverScale = 1.1f;
        const float pressScale = 0.6f;

        public Entity back = Entity.EntityField();
        public Entity howtoplay = Entity.EntityField();
        public Entity tutorial = Entity.EntityField();

        //sound effect entity reference
        [Inspector("Button SFX")]
        public Entity buttonSFX = Entity.EntityField();

        //scene transistion delay to allow for button sound
        private bool nextScene = false;
        private float countdown = 0.2f;
        private string nextSceneName = "";
        private string nextCutscene = "";

        bool isTutorialOn = false;

        public override void Setup()
        {
            //validate for button sfx entity
            if (!buttonSFX.Exists() || !buttonSFX.HasComponent<Audio>())
            {
                Logger.Warning("Button SFX entity not assigned or missing Audio Component");
            }
        }

        public override void Init()
        {
            foreach (Entity levelButton in levelButtons)
            {
                levelButton.Transform2D.Scale = new Vector2<float>(levelButtonScaleX, levelButtonScaleY);
                UI button = levelButton.GetComponent<UI>();
                button?.RegisterOnClickCallback("Select", Select);
                button?.AddOnClickAction("Select");
            }

            howtoplay.GetComponent<UI>().RegisterOnClickCallback("Tut", Tut);
            howtoplay.GetComponent<UI>().AddOnClickAction("Tut");

            //back.GetComponent<UI>().RegisterOnClickCallback("back", Back);
            //back.GetComponent<UI>().AddOnClickAction("back");
        }

        public override void Update()
        {
            //handle scene transistion with delay
            if (nextScene)
            {
                countdown -= Time.DeltaTimeF;

                if (countdown < 0.0f)
                {
                    Logger.Info($"Countdown finished, loading the next scene: {nextSceneName}");

                    if (nextSceneName == "Cutscene" && !string.IsNullOrEmpty(nextCutscene))
                    {
                        StaticCutsceneData.PlayThisCutscene = nextCutscene;
                        SceneManager.LoadScene("Cutscene");
                    } 
                    else
                    {
                        SceneManager.LoadScene(nextSceneName);
                    }

                    TempInputFix.ResetKeyCooldown();

                    //reset the variables
                    nextScene = false;
                    countdown = 0.2f;
                    nextSceneName = "";
                    nextCutscene = "";
                }
            }

            for (int i = 0; i < levelButtons.Length; ++i)
            {
                if (levelButtons[i].GetComponent<UI>().IsHovered)
                {
                    levelButtons[i].Transform2D.Scale = new Vector2<float>(
                        levelButtonScaleX * hoverScale, levelButtonScaleY * hoverScale);
                    if (Input.IsMouseButtonDown(Input.MouseButton.Left))
                    {
                        levelButtons[i].Transform2D.Scale = new Vector2<float>(
                        levelButtonScaleX * pressScale, levelButtonScaleY * pressScale);

                        List<string> actions = new List<string>(levelButtons[i].GetComponent<UI>().GetOnClickActions());
                        Logger.Info($"OnClick Actions in Update: {string.Join(", ", actions)}");
                        
                        // Play button sound
                        PlayButtonSound();


                        // Check button text/content
                        if (actions.Contains("Select"))
                        {
                            Select(i);
                        }
                    }
                }
                else
                {
                    levelButtons[i].Transform2D.Scale = new Vector2<float>(levelButtonScaleX, levelButtonScaleY);
                }
            }

            if (Input.IsMouseButtonDown(Input.MouseButton.Left) &&
                   back.GetComponent<UI>().IsHovered && !isTutorialOn)
            {
                // SceneManager.LoadScene("MainMenu");
                PlayButtonSound();

                TransitionToScene("MainMenu");
            }

            if (Input.IsMouseButtonDown(Input.MouseButton.Left) &&
                   howtoplay.GetComponent<UI>().IsHovered && !isTutorialOn)
            {
                PlayButtonSound();

                //call tutorial function
                Tut(0);
            }
            //else if (Input.IsMouseButtonDown(Input.MouseButton.Left) &&
            //       howtoplay.GetComponent<UI>().IsHovered)
            //{
            //   // TODO: insert tutorial here
            //   tutorial.Active = true;
            //}

            if (!tutorial.Active && isTutorialOn) isTutorialOn = false;
        }

        public override void FixedUpdate()
        {
            if (Input.IsKeyTriggered(Lib.Keybinds.back) || Input.IsKeyTriggered(Lib.Keybinds.pause))
            {
                //SceneManager.LoadScene("MainMenu");
                //TempInputFix.ResetKeyCooldown();

                TransitionToScene("MainMenu");
                TempInputFix.ResetKeyCooldown();
            }
        }

        //play button sound effect
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

        //create a scene transition with delay for sound effect
        private void TransitionToScene(string sceneName)
        {
            nextScene = true;
            countdown = 0.2f;
            nextSceneName = sceneName;
            Logger.Info($"Set up transition to scene: {sceneName}");
        }

        void Tut(int ent)
        {
            tutorial.Active = true;
            isTutorialOn = true;
        }

        void Select(int level)
        {
            if (isTutorialOn) return;

            switch (level)
            {
                case 0:
                    // Load level 1
                    //SceneManager.Load("Track1");
                    //StaticCutsceneData.PlayThisCutscene = "Cutscene_1_Prolouge_1";
                    nextCutscene = "Cutscene_1_Prolouge_1";
                    TransitionToScene("Cutscene");
                    break;
                case 1:
                    // Load level 2
                    //SceneManager.Load("Track2");
                    //StaticCutsceneData.PlayThisCutscene = "Cutscene_3_Level2start_1";
                    nextCutscene = "Cutscene_3_Level2start_1";
                    TransitionToScene("Cutscene");
                    break;
                case 2:
                    // Load level 3
                    //SceneManager.Load("Track3");    // delete this when lvl3 has cutscene
                    //StaticCutsceneData.PlayThisCutscene = "Cutscene_5_Level3start_1";
                    nextCutscene = "Cutscene_5_Level3start_1";
                    TransitionToScene("Cutscene");
                    break;
            }

            //SceneManager.LoadScene("Cutscene");
            //TempInputFix.ResetKeyCooldown();
        }
    }
}


