/******************************************************************************/
/*!
\file   StaticSettings.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for static player / game settings in BassNKick
		Contains player settings and game settings
		Settings get deserialized from JSON file here

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using Team_Popplio.Extern;

namespace BassNKick.Lib
{
	public static class StaticSettings
	{
		//public static Entity gameManagerObj = Entity.EntityField();
		//public static GameManager? gameManager;

		public static string settingsFile = "%POPPLIO%\\BassNKick\\settings.json"; //"settings.json";
		public static string settingsDefaultFile = "settings_default.json";

		public static string gameSettingsFile = "game.json";

		public static string debugSettingsFile = "%POPPLIO%\\BassNKick\\debug.json"; //"debug.json";
		public static string debugSettingsDefaultFile = "debug_default.json";

		#region Player
		// sfx
		public static int sfxHighestMistake = 1; // when to play mistake sfx (0 = miss, 1 = bad, 2 = nice, 3 = perfect)

		// volume
		public static float volumeMaster = 1f;
		public static float volumeMusic = 1f;
		public static float volumeAllSFX = 1f;

		// gameplay
		public static float customSpeed = 1f;
		public static float customOffset = 0f;
		public static float customAudioOffset = 0f;

		public static float customOverlayOpacity = 0.5f;

		// keybinds
		public static GLFW keyTopL = GLFW.KEY_R;
		public static GLFW keyMidL = GLFW.KEY_F;
		public static GLFW keyBotL = GLFW.KEY_V;
		#endregion

		#region Game
		// game settings
		public static float levelFadeInTime = 1f;
		public static float resultDelay = 3f;
		public static float speedConst = 1f;

		public static float noteActivateTime = 3f; // time to activate note before it reaches the hit zone

		// Developer settings

		public static float baseOffset = 0.32f; // base offset set

		public static float feverProgMod = 0.2f; // fever progress modifier


		public static float perfectThreshold = .08f;

		public static float greatThreshold = .18f;

		public static float mehThreshold = .3f;

		public static float pressThreshold = .1f;
		#endregion

		// set by level select / level
		public static string levelJson = "croptop_test_noSub.grookey";

		// level settings
		public static float musicDelay = 3f;
		public static float startTime = 0f; // for testing, otherwise set to 0

		#region Level
		public static float duration = 0f;
		public static float bpm = 0f;
		public static int timeSigTop = 0;
		public static int timeSigBottom = 0;
		#endregion

		#region Debug
		public static bool showFPS = false;
		#endregion

		// have yet to be read from json
		public static float noteFeverMod = 1f;
		public static float noteRainbowMod = 2f;

		public const float visualOffRange = 3f; // -1.5 <-> 1.5
		public const float audioOffRange = 3f; // -1.5 <-> 1.5
		public const float speedRange = 30f; // 0.0 <-> 30.0
		public const float overlayRange = 1f; // 0.0 <-> 1.0

		// done after Setup()
		public static void LoadLevel(GameManager manager)
		{
			if (manager.level == null)
			{
				Logger.Error("Level is null");
				return;
			}
			if (manager.audioManager == null)
			{
				Logger.Error("audioManager is null");
				return;
			}

			bpm = manager.level.bpm;
			//duration = manager.level.duration;
			duration = manager.audioManager.musicObj[0].GetComponent<Audio>().LengthF; // temp
			timeSigTop = manager.level.timeSignatureTop;
			timeSigBottom = manager.level.timeSignatureBottom;
		}

		public static void LoadGame()
		{
			try
			{
				if (Json.LoadJson(gameSettingsFile) == IntPtr.Zero)
					throw new Exception("Failed to get Json file");

				Logger.Critical("StaticSettings.cs | Loading Game Settings " + gameSettingsFile);

				levelFadeInTime = Json.LoadFloat("level fade in time");
				resultDelay = Json.LoadFloat("result delay");
				speedConst = Json.LoadFloat("speed const");
				noteActivateTime = Json.LoadFloat("note activate time");
				baseOffset = Json.LoadFloat("base offset");
				feverProgMod = Json.LoadFloat("fever progress modifier");

				musicDelay = Json.LoadFloat("music delay");
				startTime = Json.LoadFloat("start time");

				perfectThreshold = Json.LoadFloat("perfect threshold");
				greatThreshold = Json.LoadFloat("great threshold");
				mehThreshold = Json.LoadFloat("meh threshold");
				pressThreshold = Json.LoadFloat("press threshold");

				// Close json file
				Json.Close();
				Json.ClearObjs();

				Logger.Critical("StaticSettings.cs | Loaded Game Settings");
			}
			catch (Exception e)
			{
				Logger.Error("Error loading game settings: " + e.Message);
			}
		}

		public static void LoadDebug()
		{
			try
			{
				if (Json.LoadJson(debugSettingsFile) == IntPtr.Zero)
				{
					Logger.Info("StaticSettings.cs | Debug Settings not found, attempting to restore from defaults");
					RestoreDebug();
					if (Json.LoadJson(debugSettingsFile) == IntPtr.Zero)
						throw new Exception("Failed to get Json file");
				}
				Logger.Critical("StaticSettings.cs | Loading Debug Settings " + debugSettingsFile);
				showFPS = Json.LoadBool("show fps");
				// Close json file
				Json.Close();
				Json.ClearObjs();
				Logger.Critical("StaticSettings.cs | Loaded Debug Settings");
			}
			catch (Exception e)
			{
				Logger.Error("Error loading debug settings: " + e.Message);
			}
		}

		public static void SaveDebug()
		{
			try
			{
				Json.SaveJson1();
				Json.SaveBool("show fps", showFPS, IntPtr.Zero);
				Json.SaveJson2(debugSettingsFile);
				Logger.Critical("StaticSettings.cs | Loaded Debug Settings");
			}
			catch (Exception e)
			{
				Logger.Error("Error loading debug settings: " + e.Message);
			}
		}

		public static void RestoreDebug()
		{
			try
			{
				if (Json.LoadJson(debugSettingsDefaultFile) == IntPtr.Zero)
				{
					throw new Exception("Failed to get default debug settings file");
				}
				Logger.Critical("StaticSettings.cs | Loading Default Debug Settings " + 
					debugSettingsDefaultFile + " to " + debugSettingsFile);

				showFPS = Json.LoadBool("show fps");

				// Close json file
				Json.Close();
				Json.ClearObjs();

				SaveDebug();

				//Load();

				Logger.Critical("StaticSettings.cs | Loaded Default Debug Settings");
			}
			catch (Exception e)
			{
				Logger.Error("Error saving debug settings: " + e.Message);
			}
		}

		public static void Load()
		{
			try
			{
				if (Json.LoadJson(settingsFile) == IntPtr.Zero)
				{
					Logger.Info("StaticSettings.cs | Settings not found, attempting to restore from defaults");
					Restore();
					if (Json.LoadJson(settingsFile) == IntPtr.Zero)
					{
						Logger.Critical("StaticSettings.cs | Restore failed, attempting to initialize to zero");
						Save();
						if (Json.LoadJson(settingsFile) == IntPtr.Zero)
							throw new Exception("Failed to get Json file");
					}
				}
				Logger.Critical("StaticSettings.cs | Loading Settings " + settingsFile);
				volumeMaster = Json.LoadFloat("volume master");
				volumeMusic = Json.LoadFloat("volume music");
				volumeAllSFX = Json.LoadFloat("volume all sfx");
				customSpeed = Json.LoadFloat("custom speed");
				customOffset = Json.LoadFloat("custom offset");
				customAudioOffset = Json.LoadFloat("custom audio offset");
				customOverlayOpacity = Json.LoadFloat("custom overlay opacity");
				keyTopL = (GLFW)Json.LoadInt("custom top button");
				keyMidL = (GLFW)Json.LoadInt("custom mid button");
				keyBotL = (GLFW)Json.LoadInt("custom bot button");
				// Close json file
				Json.Close();
				Json.ClearObjs();

				// load to all the various systems
				Audio.ChannelMasterVolume = volumeMaster;
				Audio.ChannelBGMVolume = volumeMusic;
				Audio.ChannelSFXVolume = volumeAllSFX;

				Keybinds.lane1 = keyTopL;
				Keybinds.lane2 = keyMidL;
				Keybinds.lane3 = keyBotL;

				Logger.Critical("StaticSettings.cs | Loaded Settings");
			}
			catch (Exception e)
			{
				Logger.Error("Error loading settings: " + e.Message);
			}
		}

		public static void Save()
		{
			try
			{
				Logger.Critical("StaticSettings.cs | Saving Settings " + settingsFile);
				Json.SaveJson1();
				Json.SaveFloat("volume master", volumeMaster, IntPtr.Zero);
				Json.SaveFloat("volume music", volumeMusic, IntPtr.Zero);
				Json.SaveFloat("volume all sfx", volumeAllSFX, IntPtr.Zero);
				Json.SaveFloat("custom speed", customSpeed, IntPtr.Zero);
				Json.SaveFloat("custom offset", customOffset, IntPtr.Zero);
				Json.SaveFloat("custom audio offset", customAudioOffset, IntPtr.Zero);
				Json.SaveFloat("custom overlay opacity", customOverlayOpacity, IntPtr.Zero);
				Json.SaveInt("custom top button", (int)keyTopL, IntPtr.Zero);
				Json.SaveInt("custom mid button", (int)keyMidL, IntPtr.Zero);
				Json.SaveInt("custom bot button", (int)keyBotL, IntPtr.Zero);
				Json.SaveJson2(settingsFile);
				Logger.Critical("StaticSettings.cs | Saved Settings");
			}
			catch (Exception e)
			{
				Logger.Error("Error saving settings: " + e.Message);
			}
		}

		public static void Restore()
		{
			try
			{
				//Json.SaveJson1();
				//Json.SaveFloat("volume master", 1f, IntPtr.Zero);
				//Json.SaveFloat("volume music", 0.3f, IntPtr.Zero);
				//Json.SaveFloat("volume all sfx", 1f, IntPtr.Zero);
				//Json.SaveFloat("custom speed", 20f, IntPtr.Zero);
				//Json.SaveFloat("custom offset", 0.0f, IntPtr.Zero);
				//Json.SaveFloat("custom audio offset", 0.0f, IntPtr.Zero);
				//Json.SaveFloat("custom overlay opacity", 0.5f, IntPtr.Zero);
				//Json.SaveJson2(settingsFile);

				if (Json.LoadJson(settingsDefaultFile) == IntPtr.Zero)
				{
					throw new Exception("Failed to get default settings file");
				}
				Logger.Critical("StaticSettings.cs | Loading Default Settings " + settingsDefaultFile + " to " + settingsFile);
				volumeMaster = Json.LoadFloat("volume master");
				volumeMusic = Json.LoadFloat("volume music");
				volumeAllSFX = Json.LoadFloat("volume all sfx");
				customSpeed = Json.LoadFloat("custom speed");
				customOffset = Json.LoadFloat("custom offset");
				customAudioOffset = Json.LoadFloat("custom audio offset");
				customOverlayOpacity = Json.LoadFloat("custom overlay opacity");
				keyTopL = (GLFW)Json.LoadInt("custom top button");
				keyMidL = (GLFW)Json.LoadInt("custom mid button");
				keyBotL = (GLFW)Json.LoadInt("custom bot button");
				// Close json file
				Json.Close();
				Json.ClearObjs();

				Save();

				//Load();

				Logger.Critical("StaticSettings.cs | Loaded Default Settings");
			}
			catch (Exception e)
			{
				Logger.Error("Error saving settings: " + e.Message);
			}
		}
	}
}
