/******************************************************************************/
/*!
\file   GameSystem.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for implementing the game system

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System;
using Team_Popplio;
using Team_Popplio.Libraries;
using Team_Popplio.Extern;
using BassNKick.Lib;

namespace BassNKick
{
	public class GameSystem : PopplioMonoScript
	{
		[Inspector("Setting Obj")]
		public Entity settingObj = Entity.EntityField();
		public Settings? settings;

		public override void Setup()
		{
			StaticSettings.LoadGame();
			StaticSettings.Load();
			StaticSettings.LoadDebug();
			Profile.Load();

			if (!settingObj.Exists())
			{
				Logger.Info("Scene has no settings object");
				return;
			}

			settings = settingObj.GetScript<Settings>();
			settings.Load();
			settings.LoadDebug();
		}

		public override void FixedUpdate()
		{
			//Lib.TempInputFix.Update();

			if (Input.IsKeyTriggered(Keybinds.options)) SceneManager.LoadScene("OptionsMenu");

			// debug
			if (Input.IsKeyTriggered(Keybinds.mainMenu)) SceneManager.LoadScene("MainMenu");
			if (Input.IsKeyTriggered(Keybinds.levelSelect)) SceneManager.LoadScene("LevelSelect");
			if (Input.IsKeyTriggered(Keybinds.results)) SceneManager.LoadScene("Results");

			if (Input.IsKeyTriggered(Keybinds.track1)) SceneManager.LoadScene("Track1", true);
			if (Input.IsKeyTriggered(Keybinds.track2)) SceneManager.LoadScene("Track2", true);
			if (Input.IsKeyTriggered(Keybinds.track3)) SceneManager.LoadScene("Track3", true);
			if (Input.IsKeyTriggered(Keybinds.reload))
			{
				if (SceneManager.GetCurrent() == "Track1" ||
					SceneManager.GetCurrent() == "Track2" ||
					SceneManager.GetCurrent() == "Track3") 
					SceneManager.ReloadScene(true);
				else SceneManager.ReloadScene();
			}

			if (Input.IsKeyTriggered(Keybinds.showFPS))
			{
				Logger.Critical("FPS Counter Toggled");
				StaticSettings.showFPS = !StaticSettings.showFPS;
				StaticSettings.SaveDebug();
			}
		}

		public override void Update()
		{
			Lib.TempInputFix.Update();
		}

		public override void End()
		{
			// set to null to enable GC
			Profile.playerTracks = null;
		}
	}
}
