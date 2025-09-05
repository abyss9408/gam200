/******************************************************************************/
/*!
\file   AudioManager.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        BassNKick | C# file for managing audio

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
using PopplioLibraries = Team_Popplio.Libraries;
using Team_Popplio.Extern;
using Team_Popplio.Attributes;
using Team_Popplio.Extensions;
using System.Collections.Generic;
using System.Diagnostics;
using vbnc;

namespace BassNKick
{
	public class AudioManager : Team_Popplio.PopplioMonoScript
	{
		[Inspector("Game Manager Obj")]
		public Entity gameManagerObj = Entity.EntityField();
		GameManager? gameManager;

		[Inspector("Music Obj")]
		public Entity[] musicObj = { };
		//[InspectorIgnore]
		//public Audio[] music = { };
		[Inspector("Sound Obj")]
		public Entity[] soundObj = { };
		//[InspectorIgnore]
		//public Audio[] sound = { };

		public static AudioManager? audioManagerInstance; //AudioManagerScript instance;

		//[Inspector("BPM")]
		//public float bpm = 170f;
		//[Inspector("Time Signature Top")]
		//public int timeSigTop = 4;
		//[Inspector("Time Signature Bottom")]
		//public int timeSigBottom = 4;

		public float[] mLayerTimer = { };

		[InspectorIgnore]
		public float preTrackTimer = 0f;

		const float fadeInDuration = 0.1f;

		float[][] spectrumData = new float[2][];

		public override void Setup()
		{
			audioManagerInstance = this;

			//if (audioManagerInstance == null)
			//{
			//	audioManagerInstance = this;
			//}
			//else
			//{
			//	Entity.Destroy(This);
			//	return;
			//}

			mLayerTimer = new float[musicObj.Length - 1];

			gameManager = gameManagerObj.GetScript<GameManager>();

			// 32 is bin count
			spectrumData[0] = new float[32];
			spectrumData[1] = new float[32];

			//Logger.Warning(mLayerTimer.Length.ToString());

			//foreach (Entity m in musicObj)
			//{
			//	PopplioArray.Add(music, m.GetComponent<Audio>());
			//}
			//foreach (Entity s in soundObj)
			//{
			//	PopplioArray.Add(sound, s.GetComponent<Audio>());
			//}
		}

		public override void Init()
		{
			if (gameManager != null)
			{
				if (gameManager.settings != null) preTrackTimer = gameManager.settings.musicDelay;
				if (gameManager.level != null)
				{
					musicObj[0].GetComponent<Audio>().Name = gameManager.level.audioFile;
					musicObj[1].GetComponent<Audio>().Name = gameManager.level.audioFile;
				}
			}
			else Logger.Error("Game Manager not found");
		}

		public override void Update()
		{
			//Logger.Warning(musicObj[0].GetComponent<Audio>().CurrentTimeF.ToString());
			//Logger.Warning(musicObj[0].GetComponent<Audio>().LengthF.ToString());

			if (GameManager.IsPaused)
			{
				if (!musicObj[0].GetComponent<Audio>().IsPaused &&
					GameManager.state != GameManager.TrackStart.Ended)
				{
					musicObj[0].GetComponent<Audio>().Pause();
					musicObj[1].GetComponent<Audio>().Pause();
				}
			}
			else
			{
				if (GameManager.state != GameManager.TrackStart.PreTrack &&
					GameManager.state != GameManager.TrackStart.Ended)
				{
					if (musicObj[0].GetComponent<Audio>().IsPaused)
					{
						musicObj[0].GetComponent<Audio>().Play();
					}
					if (GameManager.state != GameManager.TrackStart.PreAudio && musicObj[1].GetComponent<Audio>().IsPaused)
					{
						musicObj[1].GetComponent<Audio>().Play();
					}
				}
			}

			// metronome
			if (preTrackTimer >= 0f && !musicObj[0].GetComponent<Audio>().IsPlaying &&
				GameManager.state == GameManager.TrackStart.PreTrack) 
				//GameManager.state != GameManager.TrackStart.Ended) // metronome
			{
				preTrackTimer -= PopplioLibraries.Time.DeltaTimeF;
				// use audible audio instead of actual track time audio
				if (IsOnBPMActualTime(musicObj[1])) Play(soundObj, 0);
				return;
			}
			else if (preTrackTimer < 0f) preTrackTimer = 0f;

			// for music layers (deprecated)
			for (int i = 0; i < musicObj.Length - 1; ++i)
			{
				if (mLayerTimer[i] > 0f)
				{
					if (!musicObj[i + 1].GetComponent<Audio>().Mute)
						musicObj[i + 1].GetComponent<Audio>().Mute = true;
					mLayerTimer[i] -= PopplioLibraries.Time.DeltaTimeF;
				}
				if (mLayerTimer[i] < 0f)
				{
					musicObj[i + 1].GetComponent<Audio>().Mute = false;
					mLayerTimer[i] = 0f;
				}
			}

			if (fadeInTimer >= 0f) FadeIn(musicObj[1].GetComponent<Audio>());
		}

		public override void End()
		{
			audioManagerInstance = null;
		}

		public void PlayMusic() // public interface
		{
			Logger.Critical("Playing music");
			Logger.Critical($"Music length: {musicObj[0].GetComponent<Audio>().LengthF}");

			if (gameManager != null && gameManager.settings != null)
			{
				Play(musicObj, 1); 

				if (gameManager.settings.startTime != 0f) 
					musicObj[1].GetComponent<Audio>().CurrentTimeF = 
						gameManager.settings.startTime + gameManager.settings.customAudioOffset;
				else musicObj[1].GetComponent<Audio>().CurrentTimeF =
						gameManager.settings.customAudioOffset;
			}
			else Logger.Error("Game Manager not found");

			fadeInTimer = 0f;
			//Play(musicObj, 2); musicObj[2].GetComponent<Audio>().CurrentTimeF = gameManager.startTime;
			//Play(musicObj, 3); musicObj[3].GetComponent<Audio>().CurrentTimeF = gameManager.startTime;
			//Play(musicObj, 4); musicObj[4].GetComponent<Audio>().CurrentTimeF = gameManager.startTime;
			//Play(musicObj, 5); musicObj[5].GetComponent<Audio>().CurrentTimeF = gameManager.startTime;
		}

		// thresholds
		bool isInThreshold = false;
		int beatThreshold = 50;

		float beatInterval = 0f;
		float currTime = 0f;

		public float GetBeatInterval(Entity[] grp, int index)
		{
			if (gameManager == null || gameManager.settings == null) return 0f;

			beatInterval = grp[index].GetComponent<Audio>().LengthF /
				(gameManager.settings.bpm * (grp[index].GetComponent<Audio>().LengthF / 60f));
			return beatInterval;
		}

		bool IsOnBPM(Entity[] grp, int index) // return if on bpm for audio provided (based on track time)
		{
			if (gameManager == null || gameManager.settings == null) return false;

			if (gameManager.settings.bpm <= 0) return false;

			GetBeatInterval(grp, index);
			currTime = grp[index].GetComponent<Audio>().CurrentTimeF;

			// Expensive ?
			if (!isInThreshold && beatThreshold > (int)(currTime * 1000f) % (int)(beatInterval * 1000f))
			{
				isInThreshold = true;
				return true;
			}

			if (isInThreshold && beatThreshold <= (int)(currTime * 1000f) % (int)(beatInterval * 1000f))
				isInThreshold = false;

			return false;
		}

		bool IsOnBPMActualTime(Entity ent) // return if on bpm for audio provided (based on program time)
		{
			if (gameManager == null || gameManager.settings == null) return false;

			if (gameManager.settings.bpm <= 0) return false;
			if (PopplioLibraries.Time.ElapsedTimeF <= 0f) return false;

			Entity[] grp = { ent };

			GetBeatInterval(grp, 0);
			currTime = PopplioLibraries.Time.ElapsedTimeF;

			// Expensive ?
			if (!isInThreshold && beatThreshold > (int)(currTime * 1000f) % (int)(beatInterval * 1000f))
			{
				isInThreshold = true;
				return true;
			}

			if (isInThreshold && beatThreshold <= (int)(currTime * 1000f) % (int)(beatInterval * 1000f))
				isInThreshold = false;

			return false;
		}

		public void Play(Entity[] grp, int i) // play audio
		{
			if (i >= grp.Length || i < 0)
			{
				PopplioLibraries.Logger.Warning($"Sound {i} not found");
				return;
			}
			grp[i].GetComponent<Audio>().Play();
		}

		public void Play(Entity[] grp, string name) // play audio
		{
			Entity s = Array.Find(grp, sound => sound.GetComponent<Audio>().Name == name);
			if (s == null)
			{
				PopplioLibraries.Logger.Warning($"Sound {name} not found");
				return;
			}
			s.GetComponent<Audio>().Play();
		}

		float fadeInTimer = -fadeInDuration;

		void FadeIn(Audio audio)
		{
			//if (duration == 0f)
			//{
			//	return;
			//}
			if (fadeInTimer >= 0f)
			{
				audio.Volume = fadeInTimer / fadeInDuration;
				if (fadeInTimer >= fadeInDuration)
				{
					audio.Volume = 1f;
					fadeInTimer = -fadeInDuration;
					return;
				}
				fadeInTimer += Time.DeltaTimeF;
			}
		}

		public float[] GetSpectrumData(int i)
		{
			if (i >= musicObj.Length || i < 0)
			{
				Logger.Warning($"Sound {i} not found");
				return spectrumData[0];
			}

			if (!musicObj[i].GetComponent<Audio>().GetSpectrumData(out float[] data, 0, 500f))
				return spectrumData[0];

			spectrumData[0] = data;

			return spectrumData[0];
		}
	}
}