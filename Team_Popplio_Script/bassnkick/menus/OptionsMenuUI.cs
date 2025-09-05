/******************************************************************************/
/*!
// File: OptionsMenuUI.cs
// Author: Team Popplio
// Description: Handles tab switching, volume sliders, and confirmation SFX.

Copyright(C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System;
using Team_Popplio;
using Team_Popplio.ECS;
using Team_Popplio.Components;
using Team_Popplio.Libraries;
using Team_Popplio.Extern;
using System.Collections.Generic;

namespace BassNKick
{
    public class OptionsMenuUI : PopplioMonoScript
    {
        [Inspector("Music Object")] public Entity musicObj = Entity.EntityField();
        [Inspector("Confirmation Sound")] public Entity confirmationSoundObj = Entity.EntityField();

        //reference to options manager for button sounds
        [Inspector("Options Manager")]
        public Entity optionsManagerEntity = Entity.EntityField();
        private OptionsManager? optionsManager;

        // Audio Settings
        // Sliders
        [Inspector("Master Slider")] public Entity masterSlider = Entity.EntityField();
        [Inspector("BGM Slider")] public Entity bgmSlider = Entity.EntityField();
        [Inspector("SFX Slider")] public Entity sfxSlider = Entity.EntityField();
        [Inspector("Speed Slider")] public Entity speedSlider = Entity.EntityField();
		[Inspector("Offset Slider")] public Entity offsetSlider = Entity.EntityField();
		[Inspector("Audio Offset Slider")] public Entity audioOffsetSlider = Entity.EntityField();
		[Inspector("Overlay Opacity Slider")] public Entity overlayOpacitySlider = Entity.EntityField();

		[Inspector("Key Top")] public Entity keyTop = Entity.EntityField();
		[Inspector("Key Mid")] public Entity keyMid = Entity.EntityField();
		[Inspector("Key Bottom")] public Entity keyBot = Entity.EntityField();
		[Inspector("Key Top Button")] public Entity keyTopButt = Entity.EntityField();
		[Inspector("Key Mid Button")] public Entity keyMidButt = Entity.EntityField();
		[Inspector("Key Bottom Button")] public Entity keyBotButt = Entity.EntityField();

		// Tab buttons
		[Inspector("Audio Tab Button")] public Entity audioTabButton = Entity.EntityField();
        [Inspector("Gameplay Tab Button")] public Entity gameplayTabButton = Entity.EntityField();
        [Inspector("Keybinds Tab Button")] public Entity keybindsTabButton = Entity.EntityField();

        // Tab contents
        [Inspector("Audio Tab")] public Entity audioTab = Entity.EntityField();
        [Inspector("Gameplay Tab")] public Entity gameplayTab = Entity.EntityField();
        [Inspector("Keybinds Tab")] public Entity keybindsTab = Entity.EntityField();

        private Audio? bgm;
        private Audio? confirmationSound;

        private float lastSpeed, lastOff, lastAudioOff, lastOverlayOpacity;
		private float lastMasterVol, lastBgmVol, lastSfxVol;
        private GLFW lastLane1, lastLane2, lastLane3;

        private DateTime lastSoundPlayTime;

		uint maxKeySize = 0;
		const float normalKeyTextSize = 0.6f;
		float initTopKeyPosY = 0f, initMidKeyPosY = 0f, initBotKeyPosY = 0f;

		int isEditingKey = -1;

		//bool changed = false;

		public override void Setup()
        {
            bgm = musicObj.GetComponent<Audio>();
            confirmationSound = confirmationSoundObj?.GetComponent<Audio>();

            //lastMasterVol = masterSlider.GetComponent<UI>().SliderValue;
            //lastBgmVol = bgmSlider.GetComponent<UI>().SliderValue;
            //lastSfxVol = sfxSlider.GetComponent<UI>().SliderValue;
            lastSoundPlayTime = DateTime.Now;

			//confirm reference to options manager for button sounds
			if (optionsManagerEntity.Exists())
			{
                optionsManager = optionsManagerEntity.GetScript<OptionsManager>();
                if (optionsManager == null)
                {
                    Logger.Error("OptionsManager script not found on the specified entity!");
                }
            } else
			{
                Logger.Error("Options Manager entity not assigned!");
            }

            // Register callbacks
            audioTabButton.GetComponent<UI>().RegisterOnClickCallback("TAB_AUDIO", ShowAudioTab);
            gameplayTabButton.GetComponent<UI>().RegisterOnClickCallback("TAB_GAMEPLAY", ShowGameplayTab);
            keybindsTabButton.GetComponent<UI>().RegisterOnClickCallback("TAB_KEYBINDS", ShowKeybindsTab);

			keyTopButt.GetComponent<UI>().RegisterOnClickCallback("KEY_TOP", EditKeyTop);
			keyMidButt.GetComponent<UI>().RegisterOnClickCallback("KEY_MID", EditKeyMid);
			keyBotButt.GetComponent<UI>().RegisterOnClickCallback("KEY_BOT", EditKeyBot);

			// Start with gameplay tab
			ShowGameplayTab(0);
        }

        public override void Init()
        {
            //string[] actions = { "TAB_AUDIO" };
            audioTabButton.GetComponent<UI>().AddOnClickAction("TAB_AUDIO");
            gameplayTabButton.GetComponent<UI>().AddOnClickAction("TAB_GAMEPLAY");
            keybindsTabButton.GetComponent<UI>().AddOnClickAction("TAB_KEYBINDS");
			//actions = { "TAB_GAMEPLAY" };
			keyTopButt.GetComponent<UI>().AddOnClickAction("KEY_TOP");
			keyMidButt.GetComponent<UI>().AddOnClickAction("KEY_MID");
			keyBotButt.GetComponent<UI>().AddOnClickAction("KEY_BOT");

			speedSlider.GetComponent<UI>().SliderValue = Lib.StaticSettings.customSpeed / Lib.StaticSettings.speedRange;
			offsetSlider.GetComponent<UI>().SliderValue = (Lib.StaticSettings.customOffset +
				(Lib.StaticSettings.visualOffRange / 2f)) / Lib.StaticSettings.visualOffRange;
			audioOffsetSlider.GetComponent<UI>().SliderValue = (Lib.StaticSettings.customAudioOffset +
				(Lib.StaticSettings.audioOffRange / 2f)) / Lib.StaticSettings.audioOffRange;
			overlayOpacitySlider.GetComponent<UI>().SliderValue = Lib.StaticSettings.customOverlayOpacity;

			//lastSpeed = Lib.StaticSettings.customSpeed;
			//lastOff = Lib.StaticSettings.customOffset;
			//lastAudioOff = Lib.StaticSettings.customAudioOffset;
			lastSpeed = speedSlider.GetComponent<UI>().SliderValue;
			lastOff = offsetSlider.GetComponent<UI>().SliderValue;
			lastAudioOff = audioOffsetSlider.GetComponent<UI>().SliderValue;
			lastOverlayOpacity = Lib.StaticSettings.customOverlayOpacity;

			//Logger.Critical($"{Lib.StaticSettings.volumeMaster}");
			// Set initial volumes from saved settings
			masterSlider.GetComponent<UI>().SliderValue = Lib.StaticSettings.volumeMaster;
			bgmSlider.GetComponent<UI>().SliderValue = Lib.StaticSettings.volumeMusic;
			sfxSlider.GetComponent<UI>().SliderValue = Lib.StaticSettings.volumeAllSFX;
			//Logger.Critical($"{masterSlider.GetComponent<UI>().SliderValue}");
			lastBgmVol = Lib.StaticSettings.volumeMusic;
			lastSfxVol = Lib.StaticSettings.volumeAllSFX;
			lastMasterVol = Lib.StaticSettings.volumeMaster;

			// Set initial keybinds from saved settings
			keyTop.GetComponent<Text2D>().Text = Lib.StaticSettings.keyTopL.ToString().Replace("KEY_", "");
			keyMid.GetComponent<Text2D>().Text = Lib.StaticSettings.keyMidL.ToString().Replace("KEY_", "");
			keyBot.GetComponent<Text2D>().Text = Lib.StaticSettings.keyBotL.ToString().Replace("KEY_", "");
			lastLane1 = Lib.StaticSettings.keyTopL;
			lastLane2 = Lib.StaticSettings.keyMidL;
			lastLane3 = Lib.StaticSettings.keyBotL;

			maxKeySize = keyTop.GetComponent<Text2D>().FontSize;
			keyTop.GetComponent<Text2D>().FontSize = (uint)(normalKeyTextSize * maxKeySize);
			keyMid.GetComponent<Text2D>().FontSize = (uint)(normalKeyTextSize * maxKeySize);
			keyBot.GetComponent<Text2D>().FontSize = (uint)(normalKeyTextSize * maxKeySize);

			initTopKeyPosY = keyTop.Transform2D.Position.Y;
			initMidKeyPosY = keyMid.Transform2D.Position.Y;
			initBotKeyPosY = keyBot.Transform2D.Position.Y;

			keyTop.Transform2D.Position = new Vector2<float>
				(keyTop.Transform2D.Position.X, initTopKeyPosY + 15f);
			keyMid.Transform2D.Position = new Vector2<float>
				(keyMid.Transform2D.Position.X, initMidKeyPosY + 15f);
			keyBot.Transform2D.Position = new Vector2<float>
				(keyBot.Transform2D.Position.X, initBotKeyPosY + 15f);
		}

        public override void Update()
        {
			//float master = masterSlider.GetComponent<UI>().SliderValue;
			//float bgmVal = bgmSlider.GetComponent<UI>().SliderValue * master;
			//float sfxVal = sfxSlider.GetComponent<UI>().SliderValue * master;

			//Lib.StaticSettings.volumeMaster = master;
			//Lib.StaticSettings.volumeMusic = bgmVal;
			//Lib.StaticSettings.volumeAllSFX = sfxVal;

			//Audio.ChannelMasterVolume = master;
			//Audio.ChannelSFXVolume = sfxVal;
			//if (bgm != null) bgm.Volume = bgmVal;

			CheckKey();

			if (isEditingKey >= 0) return;

			if ((DateTime.Now - lastSoundPlayTime).TotalSeconds > 1f)
            {
				//Logger.Critical($"1");
				if (HasChanged())
				{
					//Logger.Critical($"2");
					Lib.StaticSettings.Save();
                    //Lib.StaticSettings.Load();
					confirmationSound?.Play();
					lastSoundPlayTime = DateTime.Now;

					//changed = false;
				}
            }

            // Check for tab button clicks
            if (Input.IsMouseButtonDown(Input.MouseButton.Left))
            {
                if (audioTabButton.GetComponent<UI>().IsHovered)
                {
                    PlayButtonSound();
                    ShowAudioTab(0);
                }
                else if (gameplayTabButton.GetComponent<UI>().IsHovered)
                {
                    PlayButtonSound();
                    ShowGameplayTab(0);
                }
                else if (keybindsTabButton.GetComponent<UI>().IsHovered)
                {
                    PlayButtonSound();
                    ShowKeybindsTab(0);
                }
                else if (keyTopButt.GetComponent<UI>().IsHovered && keybindsTab.Active)
                {
                    PlayButtonSound();
                    EditKeyTop(0);
                }
                else if (keyMidButt.GetComponent<UI>().IsHovered && keybindsTab.Active)
                {
                    PlayButtonSound();
                    EditKeyMid(0);
                }
                else if (keyBotButt.GetComponent<UI>().IsHovered && keybindsTab.Active)
                {
                    PlayButtonSound();
                    EditKeyBot(0);
                }
            }

            //lastMasterVol = master;
            //lastBgmVol = bgmVal;
            //lastSfxVol = sfxVal;

            //if (Input.IsMouseButtonDown(Input.MouseButton.Left) && audioTabButton.GetComponent<UI>().IsHovered)
            //{
            //    //Logger.Critical("1");
            //    ShowAudioTab(0);
            //}
        }

		//use the optionsManager button sound effect
		private void PlayButtonSound()
		{
            if (optionsManager != null)
            {
                optionsManager.PlayButtonSound();
            }
            else
            {
                Logger.Warning("OptionsManager reference is null, cannot play button sound!");
            }
        }

		public override void End()
		{
			Lib.StaticSettings.Save();
		}

		private void CheckKey()
		{
			if (!keybindsTab.Active) return;

			if (isEditingKey >= 0)
			{
				List<GLFW> keys = Input.GetKeysTriggered();
				if (keys.Count == 0 || keys.Count > 1) return;
				switch (isEditingKey)
				{
					case 0:
						keyTop.GetComponent<Text2D>().Text = keys[0].ToString().Replace("KEY_", "");
						Lib.StaticSettings.keyTopL = 
							(GLFW)Enum.Parse(typeof(GLFW), "KEY_" + keyTop.GetComponent<Text2D>().Text);
						break;
					case 1:
						keyMid.GetComponent<Text2D>().Text = keys[0].ToString().Replace("KEY_", "");
						Lib.StaticSettings.keyMidL = 
							(GLFW)Enum.Parse(typeof(GLFW), "KEY_" + keyMid.GetComponent<Text2D>().Text);
						break;
					case 2:
						keyBot.GetComponent<Text2D>().Text = keys[0].ToString().Replace("KEY_", "");
						Lib.StaticSettings.keyBotL = 
							(GLFW)Enum.Parse(typeof(GLFW), "KEY_" + keyBot.GetComponent<Text2D>().Text);
						break;
				}
				isEditingKey = -1;
			}
			else
			{
				if (keyTopButt.GetComponent<UI>().IsHovered)
				{
					keyTop.GetComponent<Text2D>().FontSize = maxKeySize;
					keyTop.Transform2D.Position = new Vector2<float>
						(keyTop.Transform2D.Position.X, initTopKeyPosY);
				}
				else
				{
					keyTop.GetComponent<Text2D>().FontSize = (uint)(normalKeyTextSize * maxKeySize);
					keyTop.Transform2D.Position = new Vector2<float>
						(keyTop.Transform2D.Position.X, initTopKeyPosY + 15f);
				}
				if (keyMidButt.GetComponent<UI>().IsHovered)
				{
					keyMid.GetComponent<Text2D>().FontSize = maxKeySize;
					keyMid.Transform2D.Position = new Vector2<float>
						(keyMid.Transform2D.Position.X, initMidKeyPosY);
				}
				else
				{
					keyMid.GetComponent<Text2D>().FontSize = (uint)(normalKeyTextSize * maxKeySize);
					keyMid.Transform2D.Position = new Vector2<float>
						(keyMid.Transform2D.Position.X, initMidKeyPosY + 15f);
				}
				if (keyBotButt.GetComponent<UI>().IsHovered)
				{
					keyBot.GetComponent<Text2D>().FontSize = maxKeySize;
					keyBot.Transform2D.Position = new Vector2<float>
						(keyBot.Transform2D.Position.X, initBotKeyPosY);
				}
				else
				{
					keyBot.GetComponent<Text2D>().FontSize = (uint)(normalKeyTextSize * maxKeySize);
					keyBot.Transform2D.Position = new Vector2<float>
						(keyBot.Transform2D.Position.X, initBotKeyPosY + 15f);
				}
			}
		}

        private bool HasChanged()
        {
			bool changed = false;

			//         Logger.Critical($"{masterSlider.GetComponent<UI>().SliderValue}");
			//Logger.Critical($"{Lib.StaticSettings.volumeMaster}");

			if (lastSpeed != speedSlider.GetComponent<UI>().SliderValue)
            {
                Logger.Critical("Speed changed");
				Lib.StaticSettings.customSpeed = 
					speedSlider.GetComponent<UI>().SliderValue * Lib.StaticSettings.speedRange;
				lastSpeed = speedSlider.GetComponent<UI>().SliderValue;
				changed = true;
			}

			if (lastOff != offsetSlider.GetComponent<UI>().SliderValue)
			{
				Logger.Critical("Offset changed");
				Lib.StaticSettings.customOffset = 
					(offsetSlider.GetComponent<UI>().SliderValue * Lib.StaticSettings.visualOffRange) - 
					(Lib.StaticSettings.visualOffRange / 2f);
				lastOff = offsetSlider.GetComponent<UI>().SliderValue;
				changed = true;
			}

			if (lastAudioOff != audioOffsetSlider.GetComponent<UI>().SliderValue)
			{
				Logger.Critical("Audio Offset changed");
				Lib.StaticSettings.customAudioOffset = 
					(audioOffsetSlider.GetComponent<UI>().SliderValue * Lib.StaticSettings.audioOffRange) -
					(Lib.StaticSettings.audioOffRange / 2f);
				lastAudioOff = audioOffsetSlider.GetComponent<UI>().SliderValue;
				changed = true;
			}

			if (lastOverlayOpacity != overlayOpacitySlider.GetComponent<UI>().SliderValue)
			{
				Logger.Critical("Overlay Opacity changed");
				Lib.StaticSettings.customOverlayOpacity = overlayOpacitySlider.GetComponent<UI>().SliderValue;
				lastOverlayOpacity = Lib.StaticSettings.customOverlayOpacity;
				changed = true;
			}

			if (lastMasterVol != masterSlider.GetComponent<UI>().SliderValue)
			{
                Logger.Critical("Master volume changed");
				Lib.StaticSettings.volumeMaster = masterSlider.GetComponent<UI>().SliderValue;
				lastMasterVol = Lib.StaticSettings.volumeMaster;

				changed = true;
			}

			if (lastBgmVol != bgmSlider.GetComponent<UI>().SliderValue)
			{
				Logger.Critical("BGM volume changed");
				Lib.StaticSettings.volumeMusic = bgmSlider.GetComponent<UI>().SliderValue;
				lastBgmVol = Lib.StaticSettings.volumeMusic;
				changed = true;
			}

			if (lastSfxVol != sfxSlider.GetComponent<UI>().SliderValue)
			{
				Logger.Critical("SFX volume changed");
				Lib.StaticSettings.volumeAllSFX = sfxSlider.GetComponent<UI>().SliderValue;
				lastSfxVol = Lib.StaticSettings.volumeAllSFX;
				changed = true;
			}

			if (lastLane1 != (GLFW)Enum.Parse(typeof(GLFW), "KEY_" + keyTop.GetComponent<Text2D>().Text))
			{
				Logger.Critical("Lane 1 changed");
				Lib.StaticSettings.keyTopL = (GLFW)Enum.Parse(typeof(GLFW), "KEY_" + keyTop.GetComponent<Text2D>().Text);
				lastLane1 = Lib.StaticSettings.keyTopL;
				changed = true;
			}

			if (lastLane2 != (GLFW)Enum.Parse(typeof(GLFW), "KEY_" + keyMid.GetComponent<Text2D>().Text))
			{
				Logger.Critical("Lane 2 changed");
				Lib.StaticSettings.keyMidL = (GLFW)Enum.Parse(typeof(GLFW), "KEY_" + keyMid.GetComponent<Text2D>().Text);
				lastLane2 = Lib.StaticSettings.keyMidL;
				changed = true;
			}

			if (lastLane3 != (GLFW)Enum.Parse(typeof(GLFW), "KEY_" + keyBot.GetComponent<Text2D>().Text))
			{
				Logger.Critical("Lane 3 changed");
				Lib.StaticSettings.keyBotL = (GLFW)Enum.Parse(typeof(GLFW), "KEY_" + keyBot.GetComponent<Text2D>().Text);
				lastLane3 = Lib.StaticSettings.keyBotL;
				changed = true;
			}

			return changed;

			//return master != lastMasterVol || bgm != lastBgmVol || sfx != lastSfxVol;
		}

        private void ShowAudioTab(int entityID)
        {
            //Logger.Critical("HI");
            audioTab.Active = true;
            gameplayTab.Active = false;
            keybindsTab.Active = false;
        }

        private void ShowGameplayTab(int entityID)
        {
            audioTab.Active = false;
            gameplayTab.Active = true;
            keybindsTab.Active = false;
        }

        private void ShowKeybindsTab(int entityID)
        {
            audioTab.Active = false;
            gameplayTab.Active = false;
            keybindsTab.Active = true;
        }

		private void EditKeyTop(int entityID)
		{
			keyTop.GetComponent<Text2D>().Text = "?";
			keyTop.GetComponent<Text2D>().FontSize = maxKeySize;
			isEditingKey = 0;
		}

		private void EditKeyMid(int entityID)
		{
			keyMid.GetComponent<Text2D>().Text = "?";
			keyMid.GetComponent<Text2D>().FontSize = maxKeySize;
			isEditingKey = 1;
		}

		private void EditKeyBot(int entityID)
		{
			keyBot.GetComponent<Text2D>().Text = "?";
			keyBot.GetComponent<Text2D>().FontSize = maxKeySize;
			isEditingKey = 2;
		}
	}
}
