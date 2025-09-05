/******************************************************************************/
/*!
\file	GameManager.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        BassNKick | C# file for managing game system

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System;
using Team_Popplio;
using Team_Popplio.Libraries;
using Team_Popplio.Attributes;
using Team_Popplio.ECS;
using Team_Popplio.Extern;
using Team_Popplio.Components;
using BassNKick.Lib;
using System.Collections.Generic;

namespace BassNKick
{
	public class GameManager : PopplioMonoScript
	{
		public static GameManager? staticInstance;

		//[Header("Fields")]
		[Inspector("Audio Manager Obj")]
		public Entity AudioManagerObj = Entity.EntityField();
		[InspectorIgnore]
		public AudioManager? audioManager;
		[Inspector("Track Obj")]
		public Entity TrackObj = Entity.EntityField();
		[InspectorIgnore]
		public Track? track;

		[Inspector("Settings Obj")]
		public Entity SettingsObj = Entity.EntityField();
		[InspectorIgnore]
		public Settings? settings;

		[Inspector("Fever Obj")]
		public Entity feverObj = Entity.EntityField();
		[InspectorIgnore]
		public static Fever? fever;

		[Inspector("UI Obj")]
		public Entity uiObj = Entity.EntityField();
		public static GameUI? ui;

		public Entity destroyingNotes = Entity.EntityField();

		[Inspector("FPS Counter")]
		public Entity fpsCounter = Entity.EntityField();
		Text2D? fpsText;

		float timer = 0f;

		[InspectorIgnore]
		public Chart? level;

		//List<Entity>? notesToInit;

		bool levelEnded = false;

		static bool isPaused = false;

		static float pauseCooldown = 0.1f;
		static float pauseTimer = 0f;
		static float unPauseTimer = 0f;

		//[InspectorIgnore]
		public enum TrackStart
		{
			PreTrack,
			PreAudio,
			Playing,
			Ended
		}

		//[InspectorIgnore]
		public static TrackStart state = TrackStart.PreTrack;


		public static void TogglePause()
		{
			if (isPaused && pauseTimer < pauseCooldown) return;
			if (!isPaused && unPauseTimer < pauseCooldown) return;
			isPaused = !isPaused;
			//if (!isPaused) pauseTimer = 0f;
			if (isPaused) Animation.PauseAll();
			else Animation.ResumeAll();
		}

		public static bool IsPaused
		{
			get => isPaused;
		}

		public override void Setup()
		{
			staticInstance = this;

			audioManager = AudioManagerObj.GetScript<AudioManager>();
			track = TrackObj.GetScript<Track>();
			settings = SettingsObj.GetScript<Settings>();
			fever = feverObj.GetScript<Fever>();
			ui = uiObj.GetScript<GameUI>();

			state = TrackStart.PreTrack;

			settings.musicDelay = audioManager.GetBeatInterval(audioManager.musicObj, 0) *
				(float)Math.Floor(settings.musicDelay / audioManager.GetBeatInterval(audioManager.musicObj, 0));
			timer = 0f;
			level = LevelJson.LoadChart("Assets/Charts/" + settings.levelJson);

			audioManager.musicObj[0].GetComponent<Audio>().Name = level.audioFile;
			audioManager.musicObj[1].GetComponent<Audio>().Name = level.audioFile;

			// load game settings
			settings.LoadGame();

			settings.LoadLevel(this);

			track.SetupTrack();

			//if (level != null) notesToInit = 
			//	LevelJson.BuildChart(level, track.lane1, track.lane2, track.lane3, track.noteTemp, this);

			fpsText = fpsCounter.GetComponent<Text2D>();

			isPaused = false;
		}

		//async void RunInitNotes() // init notes after setup to avoid wrong order
		//{
		//	await LevelJson.initNotes.Run();
		//}

		public override void Init()
		{
			if (level == null) throw new Exception("Level is null");

			try
			{
				//track?.AlignNotes();

				track?.score?.InitScore();

				track?.InitializeTrack();

				LevelJson.InitChart(level, this);

				track?.InitializeNotes();
			}
			catch (Exception e)
			{
				Logger.Critical("Failed to initialize game | " + e.Message);
			}

			levelEnded = false;
		}

		// Update is called once per frame
		public override void Update()
		{
			///if (Input.GetKey(KeyCode.Escape)) Application.Quit();
			//if (TempInputFix.IsKeyTriggered(GLFW.KEY_TAB)) // reset scene
			//{
			//	string scene = SceneManager.GetCurrent();
			//	SceneManager.Load(scene);
			//}

			GameState();

			//Logger.Info("FPS: " + Engine.GetFPS().ToString());
			ShowFPS();

			if (IsPaused)
			{
				pauseTimer += Time.DeltaTimeF;
				if (unPauseTimer > 0f) unPauseTimer = 0f;
			}
			else
			{
				unPauseTimer += Time.DeltaTimeF;
				if (pauseTimer > 0f) pauseTimer = 0f;
			}
		}

		//public override void FixedUpdate()
		//{
		//	GameState();
		//}

		public override void End()
		{
			//state = TrackStart.PreTrack;

			// set to null to enable GC
			staticInstance = null;
			fever = null;
			ui = null;
		}

		void ShowFPS()
		{
			if (fpsText == null) return;

			if (!StaticSettings.showFPS)
			{
				if (fpsText.IsActive) fpsText.IsActive = false;
				return;
			}
			else
			{
				if (!fpsText.IsActive) fpsText.IsActive = true;
			}

			fpsText.Text = "FPS: " + ((int)Engine.GetFPS()).ToString();
		}

		//float endLevelTimer = 0f;

		void GameState()
		{
			if (state == TrackStart.Ended)
			{
				if (!levelEnded)
				{
					//if (track.score.currCombo == track.score.maxCombo)
					if (track?.score?.miss == 0)
					{
						ui?.FC();
						Logger.Critical("Full combo achieved");
					}

					Logger.Critical("Level ended");
					levelEnded = true;
				}
				timer += Time.DeltaTimeF;
				//endLevelTimer += Time.DeltaTimeF;
				//if (endLevelTimer >= settings?.resultDelay)
				if (timer >= audioManager?.musicObj[0].GetComponent<Audio>().LengthF +
					StaticSettings.musicDelay + StaticSettings.resultDelay)
				{
					Logger.Critical("Loading Results...");
					SceneManager.LoadScene("Results");
				}
			}

			if (settings == null)
			{
				Logger.Error("Settings is null");
				return;
			}

			if (audioManager == null)
			{
				Logger.Error("Audio Manager is null");
				return;
			}

			switch (state)
			{
				case TrackStart.PreTrack: // delay music
					timer += Time.DeltaTimeF;
					if (timer >= settings.musicDelay)
					{
						audioManager.Play(audioManager.musicObj, 0); // base anchor

						if (audioManager != null) 
							audioManager.musicObj[0].GetComponent<Audio>().CurrentTimeF = settings.startTime;

						if (settings.customAudioOffset == 0f)
						{
							audioManager?.PlayMusic(); // actual audio
							state = TrackStart.Playing;
						}
						else state = TrackStart.PreAudio;

						Logger.Critical("Track started");
					}
					break;
				case TrackStart.PreAudio:
					timer += Time.DeltaTimeF;
					if (timer >= settings.musicDelay + settings.customAudioOffset)
					{
						audioManager.PlayMusic(); // actual audio
						state = TrackStart.Playing;
						Logger.Critical("Audio started");
					}
					break;
				case TrackStart.Playing:
					timer += Time.DeltaTimeF;
					if (audioManager.musicObj[0].GetComponent<Audio>().CurrentTimeF >=
						audioManager.musicObj[0].GetComponent<Audio>().LengthF ||
						!audioManager.musicObj[0].GetComponent<Audio>().IsPlaying || 
						timer >= audioManager.musicObj[0].GetComponent<Audio>().LengthF)
					{
						// todo: end current level

						//timer = 0f;
						//endLevelTimer = 0f;
						state = TrackStart.Ended;
					}
					break;
				//case TrackStart.Ended:
				//	if (!levelEnded)
				//	{
				//		//if (track.score.currCombo == track.score.maxCombo)
				//		if (track?.score?.miss == 0)
				//		{
				//			ui?.FC();
				//			Logger.Critical("Full combo achieved");
				//		}

				//		Logger.Critical("Level ended");
				//		levelEnded = true;
				//	}
				//	endLevelTimer += Time.DeltaTimeF;
				//	if (endLevelTimer >= settings.resultDelay) SceneManager.Load("Results");
				//	break;
			}
		}
	}
}
