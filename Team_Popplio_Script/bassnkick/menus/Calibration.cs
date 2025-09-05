/******************************************************************************/
/*!
\file   Calibration.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\co-author Pearl Goh, Hafawati Binte Mohd Ali, Rio Shannon Yvon Leonardo
\contribution Shawn - 78% | Pearl - 5% | Hafawati - 15% | Shannon - 2%
\par    Course : CSD2401
\par    Section : A
\date   2025/04/06 04:06:00 PM (Last Edit)
\brief	 The calibration system helps players synchronize their inputs with the game's timing.

    - Implements a visual calibration process using animated circles
    - Uses a metronome system with major and minor beats at 80 BPM
    - Calculates input timing offsets by measuring player responses to visual/audio cues
    - Allows players to save custom offset values or restore defaults
    - Includes settings for visual offset, audio offset, and game speed
    - Contains UI elements for user interaction (confirm button, restore defaults button)


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
using Mac;
using Mono.CSharp.Linq;
using System.IdentityModel.Claims;

namespace BassNKick
{
    public class Calibration : PopplioMonoScript
    {
        // References to UI elements
        //private UI? uiComponent = null;

        // Timer for automatic screen progression
        //private float screenTimer = 0.0f;

        // Time delay before moving to next screen (in seconds)
        //private const float SCREEN_DELAY = 10.0f;

        public Entity bg = Entity.EntityField();

		public Entity buttonConfirm = Entity.EntityField();
		public Entity buttonRestoreDefaults = Entity.EntityField();
        public Entity buttonBack = Entity.EntityField();
        public UI? confirm;
		public UI? restoreDefaults;
        public UI? back;

        [Inspector("Button SFX")]
        public Entity buttonSFX = Entity.EntityField();

        private bool transitioning = false;
        private float countdown = 0.2f;
        private string targetScene = "";


        [Inspector("press label")]
        public Entity pressLabel = Entity.EntityField();

        // For overlapping buttons issue
        private bool toggleConfirmOffNextFrame;
        private bool toggleBackOnNextFrame;

        int currentBgState = 0;
		string[] bgStates = { "UI_Calibration_bg_1_instructions",
			"UI_Calibration_bg_2_blank", "UI_Calibration_bg_3_results" };


		// Calibration =========================================================

		// Circle entities
		public Entity[] circles = { };

		float[] circleElasticTimer = { 0f, 0f, 0f, 0f };
		const float elasticSpeed = 2f;
		float initialScaleBig = 322f;
		float initialScaleSmall = 236f;
		bool[] circleActivated = { false, false, false, false };

		const string smallCircle = "UI_Calibration_circles_small_spritesheet";
		const string bigCircle = "UI_Calibration_circles_big_spritesheet";

		float[] offsets = { 0f, 0f, 0f, 0f };

		//[Inspector("Small Circle Left")]
		//      public Entity smallCircleLeft = Entity.EntityField();

		//      [Inspector("Small Circle Mid")]
		//      public Entity smallCircleMid = Entity.EntityField();

		//      [Inspector("Small Circle Right")]
		//      public Entity smallCircleRight = Entity.EntityField();

		//      [Inspector("Big Circle")]
		//      public Entity bigCircle = Entity.EntityField();

		// Animation timing
		//private float blinkInterval = 2.0f; // Time between each circle's blink (2 seconds)
		//private float elapsedTime = 0f;
		private int activeCircleIndex = 0; // first active circle initially

		// References to components
		//private Entity[] circles;
		//private Animation2D[] animations;


		public Entity sfxMinorBeat = Entity.EntityField();
		public Entity sfxMajorBeat = Entity.EntityField();

		const float bpm = 80f;
		const int beatsPerBar = 4;
		int currentBeat = 0;

		public Entity resultText = Entity.EntityField();

		float lastMajBeat = 0f;

		// Audio Calibration =========================================================

		float finalAudioOffset = 0f;

        bool calibratedAudio = false;

		// Visual Calibration =========================================================

        float finalVisualOffset = 0f;

		bool calibratedVisual = false;

        [Inspector("Visual Target")]
		Entity visualTarget = Entity.EntityField();

		[Inspector("Visual Note")]
		Entity visualNote = Entity.EntityField();

        // judgement
        float visualTargetPos = 0f;

        const float visualSpeed = 600f;

		// timer for note movement
		float visualTimer = 0f;

        // duration of 1 bar in current bpm
        float visualDuration = 0f;

        float visualTargetElasticTimer = 0f;

		const float initialTargetScaleBig = 236f;
		//const float initialTargetScaleSmall = 180f;

		// sliders =========================================================

		public Entity settingSliders = Entity.EntityField();
        public Entity visualOff = Entity.EntityField();
        public Entity audioOff = Entity.EntityField();
        public Entity speed = Entity.EntityField();

       // private bool ignoreInputUntilMouseRelease = true;

        public override void Setup()
        {
            Logger.Info("Calibration.cs | Setup()");

            //checking to make sure that buffonSFX entity does exist

            if (!buttonSFX.Exists())
            {
                Logger.Warning("Button SFX entity does not exist or was not assigned in the Inspector");
            }
            else
            {
                Logger.Info($"ButtonSFX entity exists with ID: {buttonSFX.GetId()}");

                if (!buttonSFX.HasComponent<Audio>())
                {
                    Logger.Warning("Button SFX entity exists but has no Audio component");
                }
                else
                {
                    Logger.Info("ButtonSFX has a valid Audio component");

                    Audio audio = buttonSFX.GetComponent<Audio>();
                    if (audio != null)
                    {
                        Logger.Info("Audio component successfully obtained from buttonSFX");
                    }
                    else
                    {
                        Logger.Warning("Failed to get Audio component from buttonSFX entity");
                    }
                }
            }

            //if (This.HasComponent<UI>())
            //{
            //    uiComponent = This.GetComponent<UI>();
            //    Logger.Info("UIComponent Found!");
            //}
            //else
            //{
            //    Logger.Error("UIComponent NOT Found on Entity!");
            //}

            if (buttonConfirm.HasComponent<UI>())
            {
                confirm = buttonConfirm.GetComponent<UI>();
                confirm.RegisterOnClickCallback("Confirm", ConfirmAction);
            }
            if (buttonRestoreDefaults.HasComponent<UI>())
            {
                restoreDefaults = buttonRestoreDefaults.GetComponent<UI>();
                restoreDefaults.RegisterOnClickCallback("RestoreDefaults", RestoreDefaultsAction);
            }
            if (buttonBack.HasComponent<UI>())
            {
                back = buttonBack.GetComponent<UI>();
                back.RegisterOnClickCallback("Back", BackAction);
            }

            //confirm = buttonConfirm.GetComponent<UI>();
            //restoreDefaults = buttonRestoreDefaults.GetComponent<UI>();
            //back = buttonBack.GetComponent<UI>();

            // do not animate
            //foreach (Entity circle in circles)
            //         {
            //             circle.GetComponent<Animation2D>().Stop();
            //}

            //// set first circle to glow
            //circles[0].GetComponent<Animation2D>().CurrentFrame = new Vector2<int>(1, 0);

            // Initialize the circle entities array
            //circles = new Entity[] {
            //    smallCircleRight,
            //    smallCircleMid,
            //    smallCircleLeft,
            //    bigCircle
            //};

            // Get animation components
            //animations = new Animation2D[circles.Length];
            //for (int i = 0; i < circles.Length; i++)
            //{
            //    if (circles[i] != null)
            //    {
            //        animations[i] = circles[i].GetComponent<Animation2D>();
            //        if (animations[i] == null)
            //        {
            //            Logger.Error("Failed to get Animation2D component for circle " + i);
            //        }
            //        else
            //        {
            //            // Make sure all animations are initially inactive/stopped
            //            animations[i].IsActive = true;
            //            animations[i].Stop();
            //            Logger.Info("Set up animation for circle " + i);
            //        }
            //    }
            //}

            //// Start with the first circle
            //elapsedTime = 0f;
            //activeCircleIndex = 0;
            //PlayCircleAnimation(activeCircleIndex);
            //Logger.Info("Initial animation played for circle 0");

            //// Initialize timer and screen state
            //screenTimer = 0.0f;
        }

        public override void Init()
        {
            // Logger.Info("Calibration.cs | Init()");

            //if (uiComponent == null)
            //{
            //    Logger.Error("UIComponent is NULL, cannot initialize.");
            //    return;
            //}

            //restoreDefaults?.RegisterOnClickCallback("RestoreDefaults", RestoreDefaultsAction);
            //restoreDefaults?.AddOnClickAction("RestoreDefaults");
            //confirm?.RegisterOnClickCallback("Confirm", ConfirmAction);
            //confirm?.AddOnClickAction("Confirm");
            //back?.RegisterOnClickCallback("Back", BackAction);
            // back?.AddOnClickAction("Back");
            //ignoreInputUntilMouseRelease = true;

            string[] act = new string[] { "Confirm" };
            if (buttonConfirm.HasComponent<UI>())
            {
                buttonConfirm.GetComponent<UI>().SetOnClickActions(act);
            }
            act = new string[] { "RestoreDefaults" };
            if (buttonRestoreDefaults.HasComponent<UI>())
            {
                buttonRestoreDefaults.GetComponent<UI>().SetOnClickActions(act);
            }
            act = new string[] { "Back" };
            if (buttonBack.HasComponent<UI>())
            {
                buttonBack.GetComponent<UI>().SetOnClickActions(act);
            }

            foreach (Entity circle in circles)
            {
                circle.GetComponent<Animation2D>().Stop();
            }
            // set first circle to glow
            circles[0].GetComponent<Animation2D>().CurrentFrame = new Vector2<int>(1, 0);

            lastMajBeat = -1f;

            //if (uiComponent.Type == UIType.BUTTON)
            //{
            //    Logger.Info("Registering UI Callbacks from C#");

            //    if (This.HasComponent<UI>())
            //    {
            //        UI uiComponent = This.GetComponent<UI>();

            //        // Register callbacks based on the button's text content
            //        uiComponent.RegisterOnClickCallback("RestoreDefaults", RestoreDefaultsAction);
            //        uiComponent.RegisterOnClickCallback("Back", BackAction);

            //        Logger.Info("Registered Callbacks Successfully.");
            //    }
            //    else
            //    {
            //        Logger.Error("No UIComponent found on entity!");
            //    }
            //}
        }
        
        public override void Update()
        {
            ////if (uiComponent == null) return;
            //if (ignoreInputUntilMouseRelease)
            //{
            //    TempInputFix.ResetKeyCooldown();
            //    if (!TempInputFix.IsMouseTriggered(Input.MouseButton.Left))
            //    {
            //        // Once the mouse is released, stop ignoring input.
            //        ignoreInputUntilMouseRelease = false;


            //        Logger.Critical("SKRT>>???>>??????????????????????????????");
            //    }
            //    else
            //    {
            //        // If the mouse is still down, skip UI processing this frame.
            //        return;
            //    }
            //}

            if (confirm == null || restoreDefaults == null || back == null)
            {
                Logger.Error("UI Components not found!");
                return;
            }

            if (transitioning)
            {
                if (countdown >= 0.2f)
                {
                    PlayButtonSound();
                }
                
                countdown -= Time.DeltaTimeF;

                if (countdown < 0.0f)
                {
                    Logger.Info($"Countdown finished, loading the next scene: {targetScene}");
                    if (targetScene == "Previous")
                    {
                        SceneManager.LoadScene(SceneManager.GetPrevious());
                    }
                    else if (targetScene == "Cutscene")
                    {
                        StaticCutsceneData.PlayThisCutscene = "Cutscene_1_Prolouge_1";
                        SceneManager.LoadScene("Cutscene");
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



            //bool clickUsed = false;

            //// Check for UI button clicks
            //if (!clickUsed && TempInputFix.IsMouseTriggered(Input.MouseButton.Left)
            //    && confirm.Active && confirm.IsHovered)
            //{
            //    //Input.IsMouseButtonTriggered(Input.MouseButton.Left)
            //    PlayButtonSound();

            //    ConfirmAction(buttonConfirm.GetId());
            //    //List<string> actions = new List<string>(confirm.GetOnClickActions());
            //    //Logger.Info($"OnClick Actions in Update: {string.Join(", ", actions)}");

            //    //int entityID = buttonConfirm.GetId();

            //    //// Check button text/content
            //    //if (actions.Contains("Confirm"))
            //    //{
            //    //    clickUsed = true;

            //    //    ConfirmAction(entityID);
            //    //}
            //}
            //if (!clickUsed && TempInputFix.IsMouseTriggered(Input.MouseButton.Left)
            //    && restoreDefaults.Active && restoreDefaults.IsHovered)
            //{
            //    PlayButtonSound();

            //    RestoreDefaultsAction(buttonRestoreDefaults.GetId());
            //    //List<string> actions = new List<string>(restoreDefaults.GetOnClickActions());
            //    //Logger.Info($"OnClick Actions in Update: {string.Join(", ", actions)}");

            //    //int entityID = buttonRestoreDefaults.GetId();

            //    //// Check button text/content
            //    //if (actions.Contains("RestoreDefaults"))
            //    //{
            //    //    clickUsed = true;

            //    //    RestoreDefaultsAction(entityID);
            //    //}
            //}

            //if (!clickUsed && TempInputFix.IsMouseTriggered(Input.MouseButton.Left)
            //    && back.Active && back.IsHovered)
            //{
            //    PlayButtonSound();

            //    BackAction(buttonBack.GetId());
            //    //List<string> actions = new List<string>(back.GetOnClickActions());
            //    //Logger.Info($"OnClick Actions in Update: {string.Join(", ", actions)}");

            //    //int entityID = buttonBack.GetId();

            //    //// Check button text/content
            //    //if (actions.Contains("Back"))
            //    //{
            //    //    clickUsed = true;

            //    //    BackAction(entityID);
            //    //}
            //}
            //if (uiComponent != null && uiComponent.Type == UIType.BUTTON && 
            //    Input.IsMouseButtonDown(Input.MouseButton.Left) && uiComponent.IsHovered)
            //{
            //    List<string> actions = new List<string>(uiComponent.GetOnClickActions());
            //    Logger.Info($"OnClick Actions in Update: {string.Join(", ", actions)}");

            //    int entityID = uiComponent.GetEntityID();

            //    // Check button text/content
            //    if (actions.Contains("RestoreDefaults"))
            //    {
            //        RestoreDefaultsAction(entityID);
            //    }
            //    else if (actions.Contains("Back"))
            //    {
            //        BackAction(entityID);
            //    }
            //}

            UpdateCirclesAnimation();

            UpdateTargetAnimation();

			//if (GameManager.IsPaused)
			//    return;

			//// Update elapsed time
			//elapsedTime += (float)Time.DeltaTime;

			//if (elapsedTime > 0 && elapsedTime < 0.1f)
			//{
			//    Logger.Debug($"Starting new animation cycle, elapsedTime: {elapsedTime}");
			//}

			//// Check if 2 seconds have passed since last change
			//if (elapsedTime >= blinkInterval)
			//{
			//    Logger.Info($"Animation change triggered at {elapsedTime} seconds");

			//    // Reset timer
			//    elapsedTime = 0f;

			//    // Stop current animation explicitly
			//    if (activeCircleIndex >= 0 && activeCircleIndex < animations.Length && animations[activeCircleIndex] != null)
			//    {
			//        animations[activeCircleIndex].Stop();
			//        Logger.Info($"Stopped animation on circle {activeCircleIndex}");
			//    }

			//    // Move to the next circle
			//    activeCircleIndex = (activeCircleIndex + 1) % circles.Length;

			//    // Play the animation for this circle only
			//    PlayCircleAnimation(activeCircleIndex);

			//    Logger.Info("Moved to circle " + activeCircleIndex);
			//}

			//// Update the screen timer
			//screenTimer += (float)Time.DeltaTime;

			//// Check if it's time to move to the next screen
			//if (screenTimer >= SCREEN_DELAY)
			//{
			//    SceneManager.Load("Calibration2");

			//    // Reset the timer
			//    screenTimer = 0.0f;
			//}

			Settings();

            if (toggleConfirmOffNextFrame)
            {
                buttonConfirm.Active = false;
                toggleConfirmOffNextFrame = false;
            }
            if (toggleBackOnNextFrame)
            {
                buttonBack.Active = true;
                toggleBackOnNextFrame = false;
            }

            if(calibratedAudio && calibratedVisual)
            {
                buttonBack.Active = false;
                toggleBackOnNextFrame = false;
                buttonConfirm.Active = true;
                toggleConfirmOffNextFrame = false;
            }
        }

        public override void FixedUpdate()
        {
            if (Input.IsKeyTriggered(Lib.Keybinds.calibrate))
                Press(Time.ElapsedTimeF);
            //Press(Lib.Misc.GetOffActualTimeS(bpm / beatsPerBar));

            if (!calibratedAudio && activeCircleIndex < circles.Length) Metronome();
			if (!calibratedVisual && activeCircleIndex < circles.Length) UpdateNote();
		}

        void UpdateCirclesAnimation()
        {
            for (int i = 0; i < circles.Length; i++)
            {
                if (!circles[i].Exists())
                    continue;

                if (circleActivated[i])
                {
                    if (circleElasticTimer[i] <= 0f)
                    {
                        circles[i].GetComponent<Render2D>().Texture = bigCircle;
                        circles[i].GetComponent<Animation2D>().SheetWidth = initialScaleBig * 2;
                        circles[i].GetComponent<Animation2D>().SheetHeight = initialScaleBig;
                        circles[i].GetComponent<Animation2D>().AnimFrameWidth("anim", initialScaleBig);
                        circles[i].GetComponent<Animation2D>().AnimFrameHeight("anim", initialScaleBig);

                        circles[i].Transform2D.Scale = new Vector2<float>(initialScaleBig, initialScaleBig) *
                            Lib.UILibrary.EaseOutElastic(circleElasticTimer[i]);
                        circleElasticTimer[i] += Time.DeltaTimeF * elasticSpeed;
                    }
                    else if (circleElasticTimer[i] < 1f)
                    {
                        circles[i].Transform2D.Scale = new Vector2<float>(initialScaleBig, initialScaleBig) *
                            Lib.UILibrary.EaseOutElastic(circleElasticTimer[i]);
                        circleElasticTimer[i] += Time.DeltaTimeF * elasticSpeed;
                    }
                    else if (circleElasticTimer[i] > 1f)
                    {
                        circleElasticTimer[i] = 1f;
                        circles[i].Transform2D.Scale = new Vector2<float>(initialScaleBig, initialScaleBig) *
                            Lib.UILibrary.EaseOutElastic(circleElasticTimer[i]);
                    }
                }
            }
        }

        void Press(float time)
        {
            float offset = 0f;

            if (!calibratedAudio)
            {
                if (lastMajBeat < 0f)
                {
                    offset = Lib.Misc.GetOffActualTimeS(bpm / beatsPerBar);
                }
                else
                {
                    offset = time - lastMajBeat;
                    float beatInterval = 60f / (bpm / beatsPerBar);
                    if (offset > beatInterval / 2f) offset -= beatInterval;
                }
            }
			else if (!calibratedVisual)
			{
				offset = (visualTargetPos - visualNote.Transform2D.Position.X) / visualSpeed;
			}

			if (activeCircleIndex >= circles.Length) return;

            circleActivated[activeCircleIndex] = true;
            offsets[activeCircleIndex] = offset;
            ++activeCircleIndex;

            if (activeCircleIndex < circles.Length)
                circles[activeCircleIndex].GetComponent<Animation2D>().CurrentFrame = new Vector2<int>(1, 0);

			if (!calibratedAudio) Logger.Critical($"audio offset {activeCircleIndex} : {offset}");
			else if (!calibratedVisual) Logger.Critical($"visual offset {activeCircleIndex} : {offset}");

            //buttonBack.Active = true;

            switch (activeCircleIndex)
            {
                case 1:
                    NextBg();
                    break;
                case 4:
                    NextBg();
                    if (!calibratedAudio) ResultAudio();
                    else if (!calibratedVisual) ResultVisual();
					break;
            }

            if (calibratedAudio && !calibratedVisual) visualTargetElasticTimer = 0f;

		}

        void NextBg()
        {
            ++currentBgState;
            bg.GetComponent<Render2D>().Texture = bgStates[currentBgState];

            if(currentBgState == 0) pressLabel.Active = true;
            else pressLabel.Active = false;
        }

        void Metronome()
        {
            if (Lib.Misc.IsOnBPMActualTime(bpm))
            {
                ++currentBeat;
                if (currentBeat % beatsPerBar == 0)
                {
                    lastMajBeat = Time.ElapsedTimeF;
                    sfxMajorBeat.GetComponent<Audio>().Play();
                }
                else
                {
                    sfxMinorBeat.GetComponent<Audio>().Play();
                }
            }
        }

        void ResultAudio()
        {
            //buttonBack.Active = false;

            foreach (float offset in offsets)
            {
                finalAudioOffset += offset;
            }
            finalAudioOffset /= offsets.Length;

			// flip
			finalAudioOffset = -finalAudioOffset;

			resultText.GetComponent<Text2D>().Text = finalAudioOffset.ToString("0.00");
            resultText.Active = true;

            Logger.Critical($"final audio offset : {finalAudioOffset}");
            
            buttonBack.Active = false;
            buttonConfirm.Active = true;
            //buttonBack.Active = !buttonConfirm.Active;

        }

        void ResultVisual()
		{
            //buttonBack.Active = false;
            visualNote.Active = false;
			visualTarget.Active = false;

			foreach (float offset in offsets)
			{
				finalVisualOffset += offset;
			}
			finalVisualOffset /= offsets.Length;

			// flip
			finalVisualOffset = -finalVisualOffset;

			resultText.GetComponent<Text2D>().Text = finalVisualOffset.ToString("0.00");
			resultText.Active = true;

			Logger.Critical($"final visual offset : {finalVisualOffset}");

            buttonBack.Active = false;
            buttonConfirm.Active = true;
            
        }

        //private void PlayCircleAnimation(int index)
        //      {
        //          if (index < 0 || index >= animations.Length || animations[index] == null)
        //              return;

        //          // First stop all animations to ensure only one plays
        //          for (int i = 0; i < animations.Length; i++)
        //          {
        //              if (animations[i] != null)
        //              {
        //                  animations[i].Stop();
        //                  Logger.Debug("Stopped animation " + i);
        //              }
        //          }

        //          // Play animation for the selected circle
        //          //if (circles[index] == smallCircleRight ||
        //          //    circles[index] == smallCircleMid ||
        //          //    circles[index] == smallCircleLeft)
        //          //{
        //          //    Logger.Info($"Playing small circle animation for circle {index}");
        //          //    animations[index].IsActive = true;  // Ensure the animation is active
        //          //    animations[index].SetCurrentAnimation("small circle", false);
        //          //}
        //          //else if (circles[index] == bigCircle)
        //          //{
        //          //    Logger.Info("Playing big circle animation");
        //          //    animations[index].IsActive = true;  // Ensure the animation is active
        //          //    animations[index].SetCurrentAnimation("animation_big", false);
        //          //}
        //      }

        const float visualCircleInitScaleBig = 150f;
		const float visualCircleInitScaleSmall = 100f;

		void StartVisualCalibration()
        {
            // reset

            currentBgState = -1;
			NextBg();

			resultText.Active = false;
            buttonConfirm.Active = false;

            // set circles

            activeCircleIndex = 0;

			initialScaleBig = visualCircleInitScaleBig;
			initialScaleSmall = visualCircleInitScaleSmall;

            for (int i = 0; i < circles.Length; i++)
			{
				circleElasticTimer[i] = 0f;
				circleActivated[i] = false;
                if (i > 0)
                {
                    circles[i].GetComponent<Animation2D>().CurrentFrame = new Vector2<int>(0, 0);
                    circles[i].GetComponent<Render2D>().Texture = smallCircle;
				}
                else
                {
                    circles[i].GetComponent<Animation2D>().CurrentFrame = new Vector2<int>(1, 0);
					circles[i].GetComponent<Render2D>().Texture = bigCircle;
				}
				circles[i].Transform2D.Scale = new Vector2<float>(initialScaleSmall, initialScaleSmall);
                circles[i].Transform2D.Position = new Vector2<float>(
                    circles[i].Transform2D.Position.X, circles[i].Transform2D.Position.Y + 100f);
			}

            // visual calibration

            visualTarget.Active = true;
			visualNote.Active = true;
            //buttonBack.Active = true;

            visualTarget.GetComponent<Animation2D>().Stop();

            visualTargetPos = visualTarget.Transform2D.Position.X;

            visualDuration = 60f / (bpm / beatsPerBar);
		}

		void UpdateNote()
        {
            visualNote.Transform2D.Position = new Vector2<float>(
				visualTargetPos + (visualDuration * visualSpeed) - (visualTimer * visualSpeed), visualNote.Transform2D.Position.Y);

			visualTimer += Time.FixedDeltaTimeF;

            if (visualTimer >= 2f * visualDuration)
			{
				visualTimer = 0f;
			}
		}

        void UpdateTargetAnimation()
        {
            if (visualTargetElasticTimer < 0f) return;

			if (visualTargetElasticTimer == 0f)
			{
				visualTarget.GetComponent<Render2D>().Texture = bigCircle;
				visualTarget.GetComponent<Animation2D>().SheetWidth = initialTargetScaleBig * 2;
				visualTarget.GetComponent<Animation2D>().SheetHeight = initialTargetScaleBig;
				visualTarget.GetComponent<Animation2D>().AnimFrameWidth("anim", initialTargetScaleBig);
				visualTarget.GetComponent<Animation2D>().AnimFrameHeight("anim", initialTargetScaleBig);

				visualTarget.Transform2D.Scale = new Vector2<float>(initialTargetScaleBig, initialTargetScaleBig) *
					Lib.UILibrary.EaseOutElastic(visualTargetElasticTimer);
				visualTargetElasticTimer += Time.DeltaTimeF * elasticSpeed;
			}
			else if (visualTargetElasticTimer < 1f)
			{
				visualTarget.Transform2D.Scale = new Vector2<float>(initialTargetScaleBig, initialTargetScaleBig) *
					Lib.UILibrary.EaseOutElastic(visualTargetElasticTimer);
				visualTargetElasticTimer += Time.DeltaTimeF * elasticSpeed;
			}
			else if (visualTargetElasticTimer > 1f)
			{
				visualTargetElasticTimer = -1f;
				visualTarget.Transform2D.Scale = new Vector2<float>(initialTargetScaleBig, initialTargetScaleBig) *
					Lib.UILibrary.EaseOutElastic(1f);
                visualTarget.GetComponent<Render2D>().Texture = smallCircle;
			}
		}

        private void PlayButtonSound()
        {
            //for debugging
            Logger.Info("PlayButtonSound called");

            if (!buttonSFX.Exists())
            {
                Logger.Warning("Button SFX entity does not exist!");
                return;
            }

            if (!buttonSFX.HasComponent<Audio>())
            {
                Logger.Warning("Button SFX entity exists but has no Audio component!");
                return;
            }

            Audio audio = buttonSFX.GetComponent<Audio>();

            if (audio == null)
            {
                Logger.Warning("Audio component reference is null!");
                return;
            }
            audio.Play();
            Logger.Info("Audio Play() method called successfully");
        }

        //scene transisiton for delay
        private void TransitionToScene(string sceneName)
        {
            // Don't allow new transitions if we're already transitioning
            if (transitioning)
            {
                Logger.Warning("Already transitioning, ignoring new transition request");
                return;
            }

            transitioning = true;
            countdown = 0.2f;
            targetScene = sceneName;
            Logger.Info($"Set up transition to scene: {sceneName}");
        }

        #region Buttons

        public void RestoreDefaultsAction(int entityID)
        {
            Logger.Info($"Restore Defaults button clicked for entity ID: {entityID}!");
            RestoreDefaults();

            // Return to main menu
            //SceneManager.Load("MainMenu");

            if (SceneManager.GetPrevious() == "MainMenu")
            {
                // now first time is false
                Lib.Profile.Load();
                if (Lib.Profile.playerNoob)
                {
                    Lib.Profile.playerNoob = false;
                    Lib.Profile.Save();
                }

                // go to level 1 as first time
                //SceneManager.Load("Track1");
                StaticCutsceneData.PlayThisCutscene = "Cutscene_1_Prolouge_1";
                //SceneManager.LoadScene("Cutscene");
                //TempInputFix.ResetKeyCooldown();
                TransitionToScene("Cutscene");

                //SceneManager.Load("LevelSelect");
            }
            else
            {
                // SceneManager.LoadScene(SceneManager.GetPrevious());
                // TempInputFix.ResetKeyCooldown();
                TransitionToScene("Previous");
            }
        }

        public void ConfirmAction(int entityID)
        {
            Logger.Info($"Confirm button clicked for entity ID: {entityID}!");


            if (!calibratedAudio || !calibratedVisual)
            {
                toggleConfirmOffNextFrame = true;
                toggleBackOnNextFrame = true;
            }

            if (!calibratedAudio && !calibratedVisual) // audio
            {
                // For Calibration 1, just go back to main menu
                //RestoreDefaults();

                //Save();

                //ActivateSettings();

                PlayButtonSound(); //play button sound immediatley as there is no need for delay

                StartVisualCalibration();

                calibratedAudio = true;
            }
            else if (!calibratedVisual) // visual
			{
                PlayButtonSound();

                Save();

				ActivateSettings();

				calibratedVisual = true;
			}
			else
            {
                // Return to main menu
                //SceneManager.Load("MainMenu");

                Lib.StaticSettings.Save();

                //SceneManager.LoadScene(SceneManager.GetPrevious());
                //TempInputFix.ResetKeyCooldown();
                TransitionToScene("Previous");

                //if (SceneManager.GetPrevious() == "MainMenu" ||
                //                SceneManager.GetPrevious() == SceneManager.GetCurrent() ||
                //                SceneManager.GetPrevious() == "")
                //            {
                //                // now first time is false
                //                Lib.Profile.Load();
                //                if (Lib.Profile.playerNoob)
                //                {
                //                    Lib.Profile.playerNoob = false;
                //                    Lib.Profile.Save();
                //                }

                //                // go to level 1 as first time
                //                //SceneManager.Load("Track1");
                //                StaticCutsceneData.PlayThisCutscene = "Cutscene_1_Prolouge_1";
                //                SceneManager.LoadScene("Cutscene");
                //                TempInputFix.ResetKeyCooldown();


                //                //SceneManager.Load("LevelSelect");
                //            }
                //            else
                //            {
                //                SceneManager.LoadScene(SceneManager.GetPrevious());
                //                TempInputFix.ResetKeyCooldown();
                //            }
            }
        }

        public void BackAction(int entityID)
        {
            Logger.Info($"Back button clicked for entity ID: {entityID}!");
            //SceneManager.LoadScene(SceneManager.GetPrevious());
            // TempInputFix.ResetKeyCooldown();
            TransitionToScene("Previous");
        }

        #endregion

        #region Settings

        private void RestoreDefaults()
        {
            //         Lib.StaticSettings.Load();
            //Lib.StaticSettings.customOffset = 0f;
            //Lib.StaticSettings.customAudioOffset = 0f;
            //         Lib.StaticSettings.Save();

            Lib.StaticSettings.Restore();

            Logger.Critical("Calibration values restored to defaults");
        }

        void Save()
        {
            Lib.StaticSettings.Load();
			Lib.StaticSettings.customOffset = finalVisualOffset;
			Lib.StaticSettings.customAudioOffset = finalAudioOffset;
            Lib.StaticSettings.Save();

            Logger.Critical($"Updated calibration: audio offset {finalAudioOffset} | visual offset {finalVisualOffset}");
        }

        //const float visualOffRange = 3f; // -1.5 <-> 1.5
        //const float audioOffRange = 3f; // -1.5 <-> 1.5
        //const float speedRange = 30f; // 0.0 <-> 30.0

        void ActivateSettings()
        {

            bg.GetComponent<Render2D>().Texture = bgStates[1];

            foreach (Entity circle in circles)
            {
                circle.Active = false;
            }
            resultText.Active = false;

            visualOff.GetComponent<UI>().SliderValue = (Lib.StaticSettings.customOffset +
                (Lib.StaticSettings.visualOffRange / 2f)) / Lib.StaticSettings.visualOffRange;
            audioOff.GetComponent<UI>().SliderValue = (Lib.StaticSettings.customAudioOffset +
                (Lib.StaticSettings.audioOffRange / 2f)) / Lib.StaticSettings.audioOffRange;
            speed.GetComponent<UI>().SliderValue = Lib.StaticSettings.customSpeed / Lib.StaticSettings.speedRange;

            settingSliders.Active = true;
        }

        void Settings()
        {
            if (!calibratedAudio) return;

            Lib.StaticSettings.customOffset =
                (visualOff.GetComponent<UI>().SliderValue * Lib.StaticSettings.visualOffRange) - 
                (Lib.StaticSettings.visualOffRange / 2f);
            Lib.StaticSettings.customAudioOffset =
                (audioOff.GetComponent<UI>().SliderValue * Lib.StaticSettings.audioOffRange) - 
                (Lib.StaticSettings.audioOffRange / 2f);
            Lib.StaticSettings.customSpeed = 
                speed.GetComponent<UI>().SliderValue * Lib.StaticSettings.speedRange;
        }

		#endregion
	}
}