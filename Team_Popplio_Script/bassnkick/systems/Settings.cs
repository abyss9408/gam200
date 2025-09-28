/******************************************************************************/
/*!
\file   Settings.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for player / game settings in BassNKick
		Contains player settings and game settings
		Settings get deserialized from JSON file here

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

////////////////////
// NOT BEING USED //
////////////////////

using BassNKick.Lib;
using Team_Popplio.Extern;

namespace BassNKick
{
	public class Settings : PopplioMonoScript
	{
		public static Settings? staticInstance;

		[Inspector("Game Manager Obj")]
		public Entity gameManagerObj = Entity.EntityField();
		GameManager? gameManager;

		[Inspector("Settings File")]
		public string settingsFile = "%POPPLIO%\\BassNKick\\settings.json"; //"settings.json";
		public string settingsDefaultFile = "settings_default.json";

		[Inspector("Game Settings File")]
		public string gameSettingsFile = "game.json";

		[Inspector("Debug Settings File")]
		public string debugSettingsFile = "%POPPLIO%\\BassNKick\\debug.json"; //"debug.json";
		public string debugSettingsDefaultFile = "debug_default.json";

		#region Player
		// sfx
		public int sfxHighestMistake = 1; // when to play mistake sfx (0 = miss, 1 = bad, 2 = nice, 3 = perfect)

		// volume
		public float volumeMaster = 1f;
		public float volumeMusic = 1f;
		public float volumeAllSFX = 1f;

		// gameplay
		[Inspector("Custom Speed")]
		public float customSpeed = 1f;
		[Inspector("Custom Offset")]
		public float customOffset = 0f;
		[Inspector("Custom Audio Offset")]
		public float customAudioOffset = 0f;

		[Inspector("Custom Overlay Opacity")]
		public float customOverlayOpacity = 0.5f;

		// keybinds
		[InspectorIgnore]
		public GLFW keyTopL = GLFW.KEY_R;
		[InspectorIgnore]
		public GLFW keyMidL = GLFW.KEY_F;
		[InspectorIgnore]
		public GLFW keyBotL = GLFW.KEY_V;
		#endregion

		#region Game
		// game settings
		[Inspector("UI Fade In Time")]
		public float levelFadeInTime = 1f;

		[Inspector("Result Delay")]
		public float resultDelay = 3f;

		[Inspector("Speed Const")]
		public float speedConst = 1f;

		[Inspector("Note Activate Time")]
		public float noteActivateTime = 3f; // time to activate note before it reaches the hit zone

		// Developer settings

		[Inspector("Base Offset")]
		public float baseOffset = 0.32f; // base offset set

		[Inspector("Fever Progress Modifier")]
		public float feverProgMod = 0.2f; // fever progress modifier

		[Inspector("Perfect Threshold")]
		public float perfectThreshold = .08f;
		[Inspector("Great Threshold")]
		public float greatThreshold = .18f;
		[Inspector("Meh Threshold")]
		public float mehThreshold = .3f;
		[Inspector("Press Threshold")]
		float pressThreshold = .1f;
		#endregion

		// set by level select / level
		[Inspector("Level JSON")]
		public string levelJson = "croptop_test_noSub.grookey";

		// level settings
		[Inspector("Music Delay")]
		public float musicDelay = 3f;
		[Inspector("Start Time")]
		public float startTime = 0f; // for testing, otherwise set to 0

		#region Level
		[Inspector("Duration")]
		public float duration = 0f;
		[Inspector("BPM")]
		public float bpm = 0f;
		[Inspector("Time Signature Top")]
		public int timeSigTop = 0;
		[Inspector("Time Signature Bottom")]
		public int timeSigBottom = 0;
		#endregion

		#region Debug
		public static bool showFPS = false;
		#endregion

		// have yet to be read from json
		[InspectorIgnore]
		public float noteFeverMod = 1f;
		[InspectorIgnore]
		public float noteRainbowMod = 2f;

		public const float visualOffRange = 3f; // -1.5 <-> 1.5
		public const float audioOffRange = 3f; // -1.5 <-> 1.5
		public const float speedRange = 30f; // 0.0 <-> 30.0
		public const float overlayRange = 1f; // 0.0 <-> 1.0

		public override void Setup()
		{
			staticInstance = this;

			gameManager = gameManagerObj.GetScript<GameManager>();
		}

		public override void Init()
		{
			if (gameManager == null)
			{
				Logger.Error("GameManager is null");
				return;
			}

			// load to all the various systems
			//Audio.ChannelMasterVolume = volumeMaster;
			//Audio.ChannelBGMVolume = volumeMusic;
			//Audio.ChannelSFXVolume = volumeAllSFX;
		}

		public override void End()
		{
			staticInstance = null;
			//gameManager = null;
		}

		// done after Setup()
		public void LoadLevel(GameManager manager)
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

		public void LoadGame()
		{
			try
			{
				if (Json.LoadJson(gameSettingsFile) == IntPtr.Zero)
					throw new Exception("Failed to get Json file");

				Logger.Critical("Settings.cs | Loading Game Settings " + gameSettingsFile);

				levelFadeInTime = Json.LoadFloat("level fade in time");
				resultDelay = Json.LoadFloat("result delay");
				speedConst = Json.LoadFloat("speed const");
				noteActivateTime = Json.LoadFloat("note activate time");
				baseOffset = Json.LoadFloat("base offset");
				baseOffset = customOffset; // set base offset to custom offset to correct to judgement line
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

				Logger.Critical("Settings.cs | Loaded Game Settings");
			}
			catch (Exception e)
			{
				Logger.Error("Error loading game settings: " + e.Message);
			}
		}

		public void LoadDebug()
		{
			try
			{
				if (Json.LoadJson(debugSettingsFile) == IntPtr.Zero)
					throw new Exception("Failed to get Json file");
				Logger.Critical("Settings.cs | Loading Debug Settings " + debugSettingsFile);
				showFPS = Json.LoadBool("show fps");
				// Close json file
				Json.Close();
				Json.ClearObjs();
				Logger.Critical("Settings.cs | Loaded Debug Settings");
			}
			catch (Exception e)
			{
				Logger.Error("Error loading debug settings: " + e.Message);
			}
		}

		public void SaveDebug()
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

		public void Load()
		{
			try
			{
				if (Json.LoadJson(settingsFile) == IntPtr.Zero)
				{
					Logger.Critical("Settings.cs | Settings not found, attempting to initialize defaults");
					Save();
					if (Json.LoadJson(settingsFile) == IntPtr.Zero) 
						throw new Exception("Failed to get Json file");
				}
				Logger.Critical("Settings.cs | Loading Settings " + settingsFile);
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

				Logger.Critical("Settings.cs | Loaded Settings");
			}
			catch (Exception e)
			{
				Logger.Error("Error loading settings: " + e.Message);
			}
		}

		public void Save()
		{
			try
			{
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
			}
			catch (Exception e)
			{
				Logger.Error("Error saving settings: " + e.Message);
			}
		}

		public void Restore()
		{
			try
			{
				if (Json.LoadJson(settingsDefaultFile) == IntPtr.Zero)
				{
					throw new Exception("Failed to get default settings file");
				}
				Logger.Critical("StaticSettings.cs | Loading Default Settings " + settingsFile);
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
