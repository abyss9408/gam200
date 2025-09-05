/******************************************************************************/
/*!
\file   NoteProperties.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        BassNKick | C# file for note properties

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System.Collections.Generic;
using System.Numerics;
using Team_Popplio;
using Team_Popplio.Components;
using Team_Popplio.Libraries;
using Team_Popplio.Attributes;
using Team_Popplio.ECS;

namespace BassNKick
{
	public class NoteProperties : PopplioMonoScript
	{
		#region variables
		//[Header("Fields")]
		[Inspector("Track Obj")]
		public Entity trackObj = Entity.EntityField();
		[InspectorIgnore]
		Track? track;
		[Inspector("Game UI Obj")]
		public Entity gameUIObj = Entity.EntityField();
		[InspectorIgnore]
		GameUI? gameUI;
		[Inspector("Feedback")]
		public Entity feedback = Entity.EntityField();
		[Inspector("Hold End")]
		public Entity holdEnd = Entity.EntityField(); // end pos if this note is hold
		[Inspector("Hold Feedback")]
		public Entity holdFeedback = Entity.EntityField();
		//[Inspector("Sprite")]
		//Entity spriteObj = Entity.EntityField();
		Render2D? sprite;
		[Inspector("Instrument Obj")]
		public Entity instrumentObj = Entity.EntityField();
		//[InspectorIgnore]
		//Render2D instrument;
		[Inspector("Hold")]
		public Entity hold = Entity.EntityField();
		//[Inspector("Hold Sprite End")]
		//Entity holdSpriteEnd = Entity.EntityField();

		[Inspector("Direction")]
		public Entity direction = Entity.EntityField();

		//[Header("Runtime Properties")]

		public int id = -1;

		public double trackPos = 0.0; // track pos in seconds (true)
		public double trackPosCorrected = 0.0; // track pos in seconds (with offset correction)
		public double trackLength = 0.0; // hold length in seconds

		public int judgement; // 0 = miss, 1 = meh, 2 = good, 3 = perfect

		//[Header("Properties")]
		//public int barStart = 0; // temporary for convenience (for repeated sections)
		public int bar = 0;
		public int beat = 0;
		public int beatFractionTop = 0;
		public int beatFractionBot = 0;
		public int beatLength = 0;
		public int beatLengthFractionTop = 0;
		public int beatLengthFractionBot = 0;

		public int lane = 0; // 1 = top, 2 = middle, 3 = bot

		public int type = 0; // 0 = tap, 1 = hold, 2 = directional up, 3 = directional down

		public bool rainbow = false;

		public int musicLayer = 0; // which layer / instrument this note is linked to ( 0 is none )

		public bool isHit = false;

		// testing
		public bool testMode = false;

		// private
		Lib.Note? currNote;
		bool appQuit = false;

		const string normalSheet = "Animation_IP_Enemy_Robot_Spritesheet";
		const string upSheet = "Animation_IP_Enemy_Robot_Directional_UP_Spritesheet";
		const string downSheet = "Animation_IP_Enemy_Robot_Directional_DOWN_Spritesheet";
		const string holdSheet = "Animation_IP_Enemy_Robot_HoldNote_Spritesheet";

		const string holdVFXOn = "UI_HoldNote";
		const string holdVFX = "UI_Holdnote_new";

		#endregion

		public override void Init()
		{
			if (testMode) InitNote();
		}

		public override void OnAppQuit()
		{
			appQuit = true;
		}

		int rainbowChannel = 0;

		public override void Update()
		{
			if (sprite == null) return;

			if (rainbow)
			{
				sprite.Color = Lib.UILibrary.RainbowEffect(sprite.Color, rainbowChannel, 1f, Time.DeltaTimeF, out int curr);
				rainbowChannel = curr;
			}
		}

		void BuildNote()
		{
			if (track == null)
			{
				Logger.Error("Track is null");
				return;
			}
			if (track.gameManager == null)
			{
				Logger.Error("track.gameManager is null");
				return;
			}
			if (track.gameManager.level == null)
			{
				Logger.Error("track.gameManager.level is null");
				return;
			}
			if (track.gameManager.settings == null)
			{
				Logger.Error("track.gameManager.settings is null");
				return;
			}
			if (sprite == null)
			{
				Logger.Error("Sprite is null");
				return;
			}

			judgement = 0;
			isHit = false;

			if (track.gameManager.level.unspawnedNotes?.Count > 0)
			{
				currNote = track.gameManager.level.unspawnedNotes[0];
				Lib.LevelJson.SetNote(this, currNote);

				trackPos = track.beatTimeLength *
					(double)((float)(bar * track.gameManager.settings.timeSigTop + beat));
				if (beatFractionBot > 0 && beatFractionTop > 0)
					trackPos += track.beatTimeLength *
						(double)((float)beatFractionTop / (float)beatFractionBot);

				trackPosCorrected = trackPos + track.gameManager.settings.customOffset;

				AddToLane(lane);

				Logger.Info("Set note " + id + " at lane " + lane.ToString() +
					" at position " + trackPos.ToString() + " secs, corrected to " + 
					trackPosCorrected.ToString() + " secs");

				track.gameManager.level.unspawnedNotes.RemoveAt(0);
			}
			else
			{
				Logger.Info("No more notes to spawn at initialization");
				isHit = true;
				This.Hierarchy.InheritActive = true;
				This.Hierarchy.Parent = track.notePool;
				This.Active = false;
			}

			sprite.Color = Color.White;
			sprite.IsActive = true;
			//instrumentObj.Hierarchy.InheritActive = true;
			//instrumentObj.Active = true;

			//direction.Hierarchy.InheritActive = false;
			//direction.Active = false;

			switch (type)
			{
				case 1: // hold
					//sprite.Texture = holdSheet;
					sprite.Texture = normalSheet;
					This.GetComponent<Animation2D>().SetCurrentAnimation("robotB", false);
					//holdEnd.GetComponent<Render2D>().Texture = holdSheet;
					holdEnd.GetComponent<Render2D>().Texture = normalSheet;
					holdEnd.GetComponent<Animation2D>().SetCurrentAnimation("robotB", false);
					hold.GetComponent<Render2D>().Texture = holdVFX;
					break;
				case 2: // dir up
					sprite.Texture = upSheet;
					This.GetComponent<Animation2D>().SetCurrentAnimation("robotC", false);
					//holdEnd.GetComponent<Animation2D>().SetCurrentAnimation("robotC", false);
					break;
				case 3: // dir down
					sprite.Texture = downSheet;
					This.GetComponent<Animation2D>().SetCurrentAnimation("robotC", false);
					//holdEnd.GetComponent<Animation2D>().SetCurrentAnimation("robotC", false);
					break;
				case 0: // tap
					sprite.Texture = normalSheet;
					This.GetComponent<Animation2D>().SetCurrentAnimation("robotA", false);
					//holdEnd.GetComponent<Animation2D>().SetCurrentAnimation("robotA", false);
					break;
			}
		}

		public void InitNote()
		{
			// initialize

			track = trackObj.GetScript<Track>();
			gameUI = gameUIObj.GetScript<GameUI>();
			//sprite = spriteObj.GetComponent<Render2D>();

			if (track.audioManager == null)
			{
				Logger.Error("Audio Manager not found");
				return;
			}

			sprite = This.GetComponent<Render2D>();

			Logger.Info("Initialized note " + This.ToString());

			//switch (musicLayer)
			//{
			//	case 1:
			//		instrument = gameUI.instrumentSprites[0];
			//		break;
			//	case 2:
			//		instrument = gameUI.instrumentSprites[1];
			//		break;
			//	case 3:
			//		instrument = gameUI.instrumentSprites[3];
			//		break;
			//	case 4:
			//		instrument = gameUI.instrumentSprites[2];
			//		break;
			//	case 5:
			//		instrument = gameUI.instrumentSprites[3];
			//		break;
			//}

			BuildNote();

			instrumentObj.Hierarchy.InheritActive = false;
			instrumentObj.Active = false;
		}

		const float targetUIGap = 250f;

		public void AlignNote()
		{
			if (track == null)
			{
				Logger.Error("Track not found");
				return;
			}
			if (track.gameManager == null)
			{
				Logger.Error("Game Manager not found");
				return;
			}
			if (track.gameManager.settings == null)
			{
				Logger.Error("Settings not found");
				return;
			}

			This.Hierarchy.LocalPosition2D =
				new Vector2<float>((float)(trackPos / (double)track.gameManager.settings.duration *
				(double)track.length), 0f); //This.Hierarchy.LocalPosition2D.Y);

			// update pos to new length if custom speed != 1f

			//feedback.GetScript<NoteFeedback>().canvas = track.canvas;

			switch (type)
			{
				case 0:
					//direction.Hierarchy.InheritActive = false;
					//direction.Active = false;
					break;

				case 1: // set up hold ui
					//direction.Hierarchy.InheritActive = false;
					//direction.Active = false;

					trackLength = track.beatTimeLength *
						(double)((float)(beatLength));
					if (beatLengthFractionBot > 0 && beatLengthFractionTop > 0)
						trackLength += track.beatTimeLength *
							(double)((float)beatLengthFractionTop / (float)beatLengthFractionBot);

					holdEnd.Hierarchy.InheritActive = true;

					holdEnd.Hierarchy.LocalPosition2D = new Vector2<float>((float)(trackLength * (double)track.speed), 0f);

					//hold.Active = true;
					hold.Hierarchy.InheritActive = true;

					hold.Transform2D.Scale =
						new Vector2<float>((float)(trackLength * (double)track.speed), This.Transform2D.Scale.Y / 2f);

					hold.Hierarchy.LocalPosition2D =
						new Vector2<float>((float)((trackLength * (double)track.speed) / 2.0), 0f);
					break;

				case 2: // set up dir up ui
					direction.Hierarchy.InheritActive = true;
					direction.GetComponent<Render2D>().Texture = "VFX_DirectionalArrow_green_UP";
					direction.Hierarchy.LocalPosition2D = new Vector2<float>(0f, -targetUIGap);
					//direction.Transform2D.Position = new Vector2<float>(This.Transform2D.Position.X, - 100f);
					break;

				case 3: // set up dir down ui
					direction.Hierarchy.InheritActive = true;
					direction.GetComponent<Render2D>().Texture = "VFX_DirectionalArrow_red_DOWN";
					direction.Hierarchy.LocalPosition2D = new Vector2<float>(0f, targetUIGap);
					//direction.Transform2D.Position = new Vector2<float>(This.Transform2D.Position.X, 100f);
					break;
			}

			//if (type == 1) 
			//{

			//}
			//else
			//{
			//	holdEnd.Hierarchy.InheritActive = false;
			//	holdEnd.Active = false;
			//	hold.Hierarchy.InheritActive = false;
			//	hold.Active = false;
			//}

			feedback.Hierarchy.LocalPosition2D = new Vector2<float>(0f, 0f);

			feedback.GetScript<NoteFeedback>().vfx.Hierarchy.InheritTransform = true;
			//feedback.GetScript<NoteFeedback>().text.Hierarchy.InheritTransform = true;
			feedback.GetScript<NoteFeedback>().miss.Hierarchy.InheritTransform = true;
			feedback.GetScript<NoteFeedback>().bad.Hierarchy.InheritTransform = true;
			feedback.GetScript<NoteFeedback>().nice.Hierarchy.InheritTransform = true;
			feedback.GetScript<NoteFeedback>().perfect.Hierarchy.InheritTransform = true;

			feedback.GetScript<NoteFeedback>().vfx.Hierarchy.LocalPosition2D = new Vector2<float>(0f, 0f);
			feedback.GetScript<NoteFeedback>().miss.Hierarchy.LocalPosition2D = new Vector2<float>(0f, 0f);
			feedback.GetScript<NoteFeedback>().bad.Hierarchy.LocalPosition2D = new Vector2<float>(0f, 0f);
			feedback.GetScript<NoteFeedback>().nice.Hierarchy.LocalPosition2D = new Vector2<float>(0f, 0f);
			feedback.GetScript<NoteFeedback>().perfect.Hierarchy.LocalPosition2D = new Vector2<float>(0f, 0f);

			holdFeedback.Hierarchy.LocalPosition2D = new Vector2<float>(0f, 0f);

			holdFeedback.GetScript<NoteFeedback>().vfx.Hierarchy.InheritTransform = true;
			holdFeedback.GetScript<NoteFeedback>().miss.Hierarchy.InheritTransform = true;
			holdFeedback.GetScript<NoteFeedback>().bad.Hierarchy.InheritTransform = true;
			holdFeedback.GetScript<NoteFeedback>().nice.Hierarchy.InheritTransform = true;
			holdFeedback.GetScript<NoteFeedback>().perfect.Hierarchy.InheritTransform = true;

			holdFeedback.GetScript<NoteFeedback>().vfx.Hierarchy.LocalPosition2D = new Vector2<float>(0f, 0f);
			holdFeedback.GetScript<NoteFeedback>().miss.Hierarchy.LocalPosition2D = new Vector2<float>(0f, 0f);
			holdFeedback.GetScript<NoteFeedback>().bad.Hierarchy.LocalPosition2D = new Vector2<float>(0f, 0f);
			holdFeedback.GetScript<NoteFeedback>().nice.Hierarchy.LocalPosition2D = new Vector2<float>(0f, 0f);
			holdFeedback.GetScript<NoteFeedback>().perfect.Hierarchy.LocalPosition2D = new Vector2<float>(0f, 0f);

			//feedback.GetScript<NoteFeedback>().vfx.Hierarchy.InheritTransform = false;
			////feedback.GetScript<NoteFeedback>().text.Hierarchy.InheritTransform = false;
			//feedback.GetScript<NoteFeedback>().miss.Hierarchy.InheritTransform = false;
			//feedback.GetScript<NoteFeedback>().bad.Hierarchy.InheritTransform = false;
			//feedback.GetScript<NoteFeedback>().nice.Hierarchy.InheritTransform = false;
			//feedback.GetScript<NoteFeedback>().perfect.Hierarchy.InheritTransform = false;

			//feedback.GetScript<NoteFeedback>().vfx.Transform2D.Position = This.Transform2D.Position;
			////feedback.GetScript<NoteFeedback>().text.Transform2D.Position = This.Transform2D.Position;
			//feedback.GetScript<NoteFeedback>().miss.Transform2D.Position = This.Transform2D.Position;
			//feedback.GetScript<NoteFeedback>().bad.Transform2D.Position = This.Transform2D.Position;
			//feedback.GetScript<NoteFeedback>().nice.Transform2D.Position = This.Transform2D.Position;
			//feedback.GetScript<NoteFeedback>().perfect.Transform2D.Position = This.Transform2D.Position;

			//feedback.GetScript<NoteFeedback>().vfx.Hierarchy.InheritTransform = true;
			////feedback.GetScript<NoteFeedback>().text.Hierarchy.InheritTransform = true;
			//feedback.GetScript<NoteFeedback>().miss.Hierarchy.InheritTransform = true;
			//feedback.GetScript<NoteFeedback>().bad.Hierarchy.InheritTransform = true;
			//feedback.GetScript<NoteFeedback>().nice.Hierarchy.InheritTransform = true;
			//feedback.GetScript<NoteFeedback>().perfect.Hierarchy.InheritTransform = true;
		}

		public void HitNote()
		{
			if (isHit) return;

			if (appQuit || AudioManager.audioManagerInstance == null) return; // if app quit, do nothing
																			  //if (appQuit || track.audioManager == null) return; // if app quit, do nothing

			// useless boilerplate checks to remove warnings
			if (track == null)
			{
				Logger.Error("Track not found");
				return;
			}
			if (track.score == null)
			{
				Logger.Error("Score not found");
				return;
			}
			if (track.rainbow == null)
			{
				Logger.Error("Fever not found");
				return;
			}
			if (track.gameManager == null)
			{
				Logger.Error("Game Manager not found");
				return;
			}
			if (track.judgement == null)
			{
				Logger.Error("Judgement not found");
				return;
			}
			if (track.gameManager.settings == null)
			{
				Logger.Error("Settings not found");
				return;
			}
			if (GameManager.ui == null)
			{
				Logger.Error("UI not found");
				return;
			}
			if (GameManager.ui.jParticles == null)
			{
				Logger.Error("jParticles not found");
				return;
			}
			if (sprite == null)
			{
				Logger.Error("Sprite not found");
				return;
			}

			//todo: score

			Logger.Critical("Hit note " + id.ToString() + " | Type " + type.ToString() +
				" | Judgement: " + judgement.ToString());

			// remove from lane
			switch (lane)
			{
				case 1:
					if (track.notesLane1.Contains(this)) track.lane1NotesToRemove.Add(this);
					break;
				case 2:
					if (track.notesLane2.Contains(this)) track.lane2NotesToRemove.Add(this);
					break;
				case 3:
					if (track.notesLane3.Contains(this)) track.lane3NotesToRemove.Add(this);
					break;
			}

			// ===================================== //

			//feedback

			if (judgement > track.gameManager.settings.sfxHighestMistake)
			{
				if (AudioManager.audioManagerInstance.soundObj[1].GetComponent<Audio>().IsPlaying)
					AudioManager.audioManagerInstance.soundObj[1].GetComponent<Audio>().Stop();
				AudioManager.audioManagerInstance?.soundObj[1].GetComponent<Audio>().Play(); // correct sfx
			}
			else
			{
				if (AudioManager.audioManagerInstance.soundObj[2].GetComponent<Audio>().IsPlaying)
					AudioManager.audioManagerInstance.soundObj[2].GetComponent<Audio>().Stop();
				AudioManager.audioManagerInstance?.soundObj[2].GetComponent<Audio>().Play(); // mistake sfx
			}

			switch (judgement)
			{
				case 0:
					feedback.GetScript<NoteFeedback>().judge = "miss";
					track.score.UpdateScore(0, rainbow);
					break;
				case 1:
					feedback.GetScript<NoteFeedback>().judge = "meh";
					track.score.UpdateScore(1, rainbow);
					break;
				case 2:
					feedback.GetScript<NoteFeedback>().judge = "great";
					track.score.UpdateScore(2, rainbow);
					break;
				case 3:
					feedback.GetScript<NoteFeedback>().judge = "perfect";
					track.score.UpdateScore(3, rainbow);
					break;
			}


			// prevent feedback from getting destroyed with note
			//feedback.Hierarchy.Parent = Entity.EntityField();
			// should set the position to be kept in the same place

			This.Hierarchy.Parent = track.gameManager.destroyingNotes;

			switch (type)
			{
				case 1: // hold
					feedback.Transform2D.Position =
						new Vector2<float>(track.judgement.This.Transform2D.Position.X, This.Transform2D.Position.Y);
					//Logger.Warning(feedback.Transform2D.Position.ToString());

					holdEnd.Hierarchy.InheritActive = false;
					holdEnd.Active = false;
					hold.Hierarchy.InheritActive = false;
					hold.Active = false;
					break;

				case 2:
				case 3:
					direction.Hierarchy.InheritActive = false;
					direction.Active = false;
					break;
			}


			//feedback.GetComponent<Logic>().IsActive = true; // enable NoteFeedback script in the component

			// fever feedback
			//if (fever && feedback.GetScript<NoteFeedback>().judge != "miss") 

			// rainbow
			if (rainbow && judgement != 0)
			{
				feedback.GetScript<NoteFeedback>().isRainbow = true;
				track.rainbow.Feedback();
				gameUI?.RainbowVFX(lane);
			}

			feedback.Active = true;

			// temporary
			//track.feedbackObjs.Add(feedback);

			// mute when miss, unmute when hit
			//if (musicLayer != 0)
			//{
			//	if (judgement < 2) track.audioManager.mLayerTimer[musicLayer - 1] = track.muteTime;
			//	else track.audioManager.mLayerTimer[musicLayer - 1] = -1f;
			//}

			sprite.IsActive = false;
			//instrumentObj.Hierarchy.InheritActive = false;
			//instrumentObj.Active = false;

			//holdEnd.Hierarchy.InheritActive = false;
			//holdEnd.Active = false;
			//hold.Hierarchy.InheritActive = false;
			//hold.Active = false;

			if (judgement > 0)
				GameManager.ui.jParticles.SpawnParticle(
					new Vector2<float>(track.judgementPos.X, This.Transform2D.Position.Y));

			isHit = true;
		}

		void RemoveFromLane(int lane)
		{
			if (track == null)
			{
				Logger.Error("Track not found");
				return;
			}

			if (lane == 0) return;

			Vector2<float> pos = This.Transform2D.Position;
			switch (lane)
			{
				case 1:
					This.Hierarchy.Parent = Entity.GetEntity(-1);
					if (track.notesLane1.Contains(this)) track.notesLane1.Remove(this);
					break;
				case 2:
					This.Hierarchy.Parent = Entity.GetEntity(-1);
					if (track.notesLane2.Contains(this)) track.notesLane2.Remove(this);
					break;
				case 3:
					This.Hierarchy.Parent = Entity.GetEntity(-1);
					if (track.notesLane3.Contains(this)) track.notesLane3.Remove(this);
					break;
			}
			This.Transform2D.Position = pos;
		}

		void AddToLane(int lane)
		{
			if (track == null)
			{
				Logger.Error("Track not found");
				return;
			}

			if (lane == 0)
			{
				Logger.Warning("Lane not set");
				return;
			}

			switch (lane)
			{
				case 1:
					This.Hierarchy.Parent = track.lane1;
					track.notesLane1.Add(this);
					break;
				case 2:
					This.Hierarchy.Parent = track.lane2;
					track.notesLane2.Add(this);
					break;
				case 3:
					This.Hierarchy.Parent = track.lane3;
					track.notesLane3.Add(this);
					break;
			}

			// debug
			string ids = "";
			foreach (NoteProperties n in track.notesLane1)
			{
				ids += n.id.ToString() + " ";
			}
			Logger.Info("Lane 1 Order: " + ids);
			ids = "";
			foreach (NoteProperties n in track.notesLane2)
			{
				ids += n.id.ToString() + " ";
			}
			Logger.Info("Lane 2 Order: " + ids);
			ids = "";
			foreach (NoteProperties n in track.notesLane3)
			{
				ids += n.id.ToString() + " ";
			}
			Logger.Info("Lane 3 Order: " + ids);

			AlignNote();
		}

		public void Recycle()
		{
			if (track == null)
			{
				Logger.Error("Track not found");
				return;
			}
			if (track.gameManager == null)
			{
				Logger.Error("track.gameManager not found");
				return;
			}
			if (track.gameManager.level == null)
			{
				Logger.Error("track.gameManager.level not found");
				return;
			}
			if (currNote == null)
			{
				Logger.Error("currNote not found");
				return;
			}

			RemoveFromLane(lane);
			track.gameManager.level.remainingNotes?.Remove(currNote);
			BuildNote();
			//track.SortLanes();
		}

		public void HoldFeedback()
		{
			if (holdFeedback == null)
			{
				Logger.Error("Hold Feedback not found");
				return;
			}

            if (sprite == null)
            {
                Logger.Error("Sprite not found in HoldFeedback()");
                return;
            }
            //holdFeedback.Hierarchy.InheritTransform = false;
            //holdFeedback.Transform2D.Position = This.Transform2D.Position;

            switch (judgement)
			{
				case 0:
					holdFeedback.GetScript<NoteFeedback>().judge = "miss";
					holdFeedback.Active = true;
					break;
				case 1:
					holdFeedback.GetScript<NoteFeedback>().judge = "meh";
					holdFeedback.Active = true;
					break;
				case 2:
					holdFeedback.GetScript<NoteFeedback>().judge = "great";
					holdFeedback.Active = true;
					break;
				case 3:
					holdFeedback.GetScript<NoteFeedback>().judge = "perfect";
					holdFeedback.Active = true;
					break;
			}

			sprite.Texture = holdSheet;
			holdEnd.GetComponent<Render2D>().Texture = holdSheet;
			hold.GetComponent<Render2D>().Texture = holdVFXOn;
		}

		public void HoldShorten(float currentTrackPos)
		{
			if (track == null)
			{
				Logger.Error("Track not found");
				return;
			}

			This.Transform2D.Position = new Vector2<float>(
				track.absoluteJudgementPos.X, This.Transform2D.Position.Y);

			//holdEnd.Hierarchy.InheritTransform = false;
			//hold.Hierarchy.InheritTransform = false;

			float len = (float)((double)track.speed * (trackLength +
				trackPosCorrected - (double)currentTrackPos));

			//Logger.Error(trackLength.ToString());
			//Logger.Error(track.speed.ToString());
			//Logger.Error(trackPosCorrected.ToString());
			//Logger.Error(currentTrackPos.ToString());

			holdEnd.Transform2D.Position = new Vector2<float>(
				This.Transform2D.Position.X + len, This.Transform2D.Position.Y);

			hold.Transform2D.Scale =
				new Vector2<float>(len, hold.Transform2D.Scale.Y);

			hold.Transform2D.Position = new Vector2<float>(
				This.Transform2D.Position.X + len / 2f, This.Transform2D.Position.Y);

			//holdEnd.Hierarchy.InheritTransform = true;
			//hold.Hierarchy.InheritTransform = true;
		}
	}
}
