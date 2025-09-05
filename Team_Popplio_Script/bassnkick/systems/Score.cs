/******************************************************************************/
/*!
\file   Score.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        BassNKick | C# file for score system

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
using Team_Popplio.Libraries;
using Team_Popplio.Extern;
using Team_Popplio.Attributes;
using System.Collections.Generic;

namespace BassNKick
{
	public class Score : Team_Popplio.PopplioMonoScript
	{
		[Inspector("Track Obj")]
		Entity trackObj = Entity.EntityField();
		[InspectorIgnore]
		Track? track;

		[Inspector("MaxCombo")]
		public int maxCombo = 0; // theoretical max
		[Inspector("CurrCombo")]
		public int currCombo = 0;
		[Inspector("CurrMaxCombo")]
		public int currMaxCombo = 0;

		[Inspector("MaxScore")]
		public int maxScore = 0; // theoretical max
		[Inspector("MaxRainbow")]
		public int maxRainbow = 0;
		[Inspector("CurrScore")]
		public int currScore = 0;

		[Inspector("Miss")]
		public int miss = 0;
		[Inspector("Bad")]
		public int bad = 0;
		[Inspector("Nice")]
		public int nice = 0;
		[Inspector("Perfect")]
		public int perfect = 0;

		[Inspector("Fever Score")]
		public int feverScore = 0;
		[Inspector("Rainbow Score")]
		public int rainbowScore = 0;

		[Inspector("Goal Bar Obj")]
		public Entity goalBarObj = Entity.EntityField();
		GameGoalUI? goalBar;

		[Inspector("Fever Obj")]
		public Entity feverObj = Entity.EntityField();
		Fever? fever;

		public int currJudgement = -1;

		const int missScore = 0;
		const int badScore = 1;
		const int niceScore = 2;
		const int perfectScore = 3;

		public override void Setup()
		{
			//MenuResults.score = this;
			Lib.StaticResults.score = this;

			if (trackObj != null && trackObj.Exists()) 
				if (trackObj.HasScript<Track>()) track = trackObj.GetScript<Track>();
			if (goalBarObj != null && goalBarObj.Exists()) 
				if (goalBarObj.HasScript<GameGoalUI>()) goalBar = goalBarObj.GetScript<GameGoalUI>();
			if (feverObj != null && feverObj.Exists()) 
				if (feverObj.HasScript<Fever>()) fever = feverObj.GetScript<Fever>();
		}

		public void InitScore()
		{
			if (track == null)
			{
				Logger.Error("Track is null");
				return;
			}
			if (track.gameManager == null)
			{
				Logger.Error("GameManager is null");
				return;
			}
			if (track.gameManager.level == null)
			{
				Logger.Error("Level is null");
				return;
			}

			//This.Persistent = true;
			This.Persistent = false;

			currCombo = 0;
			currMaxCombo = 0;

			currScore = 0;

			// TODO: take into account rainbow notes' score modifier

			///maxCombo = (uint)(track.notesLane1.Count + track.notesLane2.Count + track.notesLane3.Count);
			maxCombo = track.gameManager.level.notes.Count; //track.notesLane1.Count + track.notesLane2.Count + track.notesLane3.Count;
			maxScore = maxCombo * 3;

			foreach (Lib.Note note in track.gameManager.level.notes)
			{
				if (note.rainbow) ++maxRainbow;
			}
		}

		// Update is called once per frame
		public override void Update()
		{
			if (SceneManager.GetCurrent() == "Results")
			{

			}
		}

		public void UpdateCombo(bool b)
		{
			if (b) ++currCombo;
			else currCombo = 0;

			if (currCombo > currMaxCombo) currMaxCombo = currCombo;
		}

		public void UpdateScore(uint s, bool isRainbow)
		{
			if (goalBar == null)
			{
				Logger.Error("Goal Bar is null");
				return;
			}
			if (GameManager.fever == null)
			{
				Logger.Error("Fever is null");
				return;
			}

			int prevScore = currScore;

			currJudgement = (int)s;

			switch (s)
			{
				case 0:
					++miss;
					currScore += missScore;
					UpdateCombo(false);
					break;

				case 1:
					++bad;
					//++currScore;
					currScore += badScore;
					if (isRainbow) rainbowScore += badScore; //++rainbowScore;
					if (GameManager.fever.feverMode) feverScore += badScore; //++feverScore;
					UpdateCombo(true);
					GameManager.fever?.UpdateFever();
					break;
				case 2:
					++nice;
					//currScore += 2;
					currScore += niceScore;
					if (isRainbow) rainbowScore += niceScore; //2;
					if (GameManager.fever.feverMode) feverScore += niceScore; //2;
					UpdateCombo(true);
					GameManager.fever?.UpdateFever();
					break;
				case 3:
					++perfect;
					currScore += perfectScore; //3;
					if (isRainbow) rainbowScore += perfectScore; //3;
					if (GameManager.fever.feverMode) feverScore += perfectScore; //3;
					UpdateCombo(true);
					GameManager.fever?.UpdateFever();
					break;
			}

			if (prevScore != currScore) goalBar.UpdateMarker((float)currScore / (float)maxScore);
		}

		public override void End()
		{
			//MenuResults.currScore = currScore;
			//MenuResults.currMaxCombo = currMaxCombo;
			//MenuResults.perfect = perfect;
			//MenuResults.nice = nice;
			//MenuResults.bad = bad;
			//MenuResults.miss = miss;

			Lib.StaticResults.currScore = currScore;
			Lib.StaticResults.currMaxCombo = currMaxCombo;
			Lib.StaticResults.feverScore = feverScore;
			Lib.StaticResults.rainbowScore = rainbowScore;
			Lib.StaticResults.perfect = perfect;
			Lib.StaticResults.nice = nice;
			Lib.StaticResults.bad = bad;
			Lib.StaticResults.miss = miss;
			Lib.StaticResults.scorePercent = GetScorePercentage();
			Lib.StaticResults.rank = GetRank();

			Lib.StaticResults.score = null;
		}

		public float GetScorePercentage()
		{
			if (GameManager.staticInstance == null) return -1f;
			if (GameManager.staticInstance.settings == null) return -1f;

			return ((float)currScore + (feverScore * GameManager.staticInstance.settings.noteFeverMod) + 
				(rainbowScore * GameManager.staticInstance.settings.noteRainbowMod)) / 
				((float)maxScore + (maxRainbow * GameManager.staticInstance.settings.noteRainbowMod));
		}

		const float sRank = 1f;
		const float aRank = 0.9f;
		const float bRank = 0.7f;

		// 0 = S, 1 = A, 2 = B, 3 = F
		public int GetRank()
		{
			return GetScorePercentage() >= sRank ? 0 : GetScorePercentage() >= aRank ? 1 : GetScorePercentage() >= bRank ? 2 : 3;
		}
	}
}