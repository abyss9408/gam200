/******************************************************************************/
/*!
\file   Track.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        BassNKick | C# file for track system

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System.Linq;
using System.Collections.Generic;
using System.Numerics;
using Team_Popplio;
using Team_Popplio.Components;
using Team_Popplio.Libraries;
using Team_Popplio.Attributes;
using Team_Popplio.ECS;
using System.Security.Policy;
using System.Diagnostics;

namespace BassNKick
{
	public class Track : PopplioMonoScript
	{
		#region variables

		//[Header("Fields")]
		[Inspector("Audio Manager Obj")]
		Entity audioManagerObj = Entity.EntityField();
		[InspectorIgnore]
		public AudioManager? audioManager;
		[Inspector("Game Manager Obj")]
		Entity gameManagerObj = Entity.EntityField();
		[InspectorIgnore]
		public GameManager? gameManager;
		[Inspector("Judgement Obj")]
		Entity judgementObj = Entity.EntityField();
		[InspectorIgnore]
		public JudgementInput? judgement;
		[Inspector("Score Obj")]
		Entity scoreObj = Entity.EntityField();
		[InspectorIgnore]
		public Score? score;
		[Inspector("Note Track")]
		Entity noteTrack = Entity.EntityField();

		//[Inspector("Note Prefab")]
		Prefab notePrefab = Prefab.PrefabField(); // ToDo Implement

		[Inspector("Note Temp")]
		public Entity noteTemp = Entity.EntityField();

		[Inspector("Lane 1")]
		public Entity lane1 = Entity.EntityField();
		[Inspector("Lane 2")]
		public Entity lane2 = Entity.EntityField();
		[Inspector("Lane 3")]
		public Entity lane3 = Entity.EntityField();


		public Entity start = Entity.EntityField();
		public Entity end = Entity.EntityField();

		[Inspector("Note Pool")]
		public Entity notePool = Entity.EntityField();

		[Inspector("Notes")]
		public Entity[] notes = { };

		//[Inspector("Destroying Notes")]
		//public Entity destroyingNotes = Entity.EntityField();

		[Inspector("Chart Speed (CHANGE THIS PER SCENE)")]
		public float chartSpeed = 1f;

		//[Header("Notes")]
		[InspectorIgnore]
		public List<NoteProperties> notesLane1 = new List<NoteProperties>();
		[InspectorIgnore]
		public List<NoteProperties> notesLane2 = new List<NoteProperties>();
		[InspectorIgnore]
		public List<NoteProperties> notesLane3 = new List<NoteProperties>();

		//[Header("Notes")]
		[InspectorIgnore]
		public List<NoteProperties> lane1NotesToRemove = new List<NoteProperties>();
		[InspectorIgnore]
		public List<NoteProperties> lane2NotesToRemove = new List<NoteProperties>();
		[InspectorIgnore]
		public List<NoteProperties> lane3NotesToRemove = new List<NoteProperties>();

		// vars

		[InspectorIgnore]
		public float speed; // speed
		
		Vector2<float> noteStart // note parent start pos
			= new Vector2<float>(0f, 0f);

		[InspectorIgnore]
		public float length; // length

		[InspectorIgnore]
		public Vector2<float> absoluteJudgementPos // judgement pos relative to track (0, 0)
			= new Vector2<float>(0f, 0f);
		[InspectorIgnore]
		public Vector2<float> judgementPos // judgement pos relative to track (0, 0) + custom offset
			= new Vector2<float>(0f, 0f);

		float absoluteOffsetLen = 0f; // length from judgement line to start pos of track
		float musicDelayLen = 0f; // length from music delay
		float startTimeLen = 0f; // length from start timestamp of audio
		float customOffsetLen = 0f; // length from custom offset setting
		float baseOffsetLen = 0f; // length from base offset setting

		// UI

		//[Inspector("Note Feedback")]
		//public Entity noteFeedback = Entity.EntityField();
		//Prefab? noteFeedback;

		[Inspector("Canvas")]
		public Entity canvas = Entity.EntityField();
		[Inspector("Rainbow Obj")]
		Entity rainbowObj = Entity.EntityField();
		[InspectorIgnore]
		public Rainbow? rainbow;

		[Inspector("Mute Time")]
		public float muteTime = 3f;

		[Inspector("Bar Marker")]
		public Entity barMarker = Entity.EntityField();
		[Inspector("Bar Marker Holder")]
		public Entity barMarkerHolder = Entity.EntityField();

		// temporary
		//[InspectorIgnore]
		//public List<Entity> feedbackObjs = new List<Entity>();

		public int barCount = 0;
		public float barCountF = 0f;

		public double beatTimeLength = 0.0;

#pragma warning disable CS0414
		double currTrackTime = 0.0;
		float tempX = 0f;
		double lastSystemTime = 0.0;
#pragma warning restore CS0414 // The field ___ is assigned but its value is never used
#pragma warning disable CS0169
		Stopwatch? trackTime;
#pragma warning restore CS0169 // The field ___ is never used

		#endregion

		//todo: optimize track (conveyor belt system with set number of objects)

		public void SetupTrack()
		{
			// initialize
			//foreach (KeyValuePair<int, Entity> pair in Entity.GetEntities())
			//{
			//	Logger.Warning(pair.Key.ToString());
			//	Logger.Warning(pair.Value.Name());
			//}
			audioManager = audioManagerObj.GetScript<AudioManager>();
			gameManager = gameManagerObj.GetScript<GameManager>();
			judgement = judgementObj.GetScript<JudgementInput>();
			score = scoreObj.GetScript<Score>();
			rainbow = rainbowObj.GetScript<Rainbow>();

			if (gameManager.settings == null)
			{
				Logger.Error("Settings not found");
				return;
			}

			//speed = gameManager.settings.customSpeed * gameManager.settings.speedConst;
			speed = Lib.StaticSettings.customSpeed * Lib.StaticSettings.speedConst * chartSpeed;

			length = speed * gameManager.settings.duration; //* audioManager.musicObj[0].GetComponent<Audio>().LengthF;

			//noteTrack.Hierarchy.InheritTransform = true;
			//noteStart = new Vector2<float>(noteTrack.Transform2D.Position.X + customOffset * speed,
			//	noteTrack.Transform2D.Position.Y);
			//judgementPos = noteTrack.Hierarchy.LocalPosition2D;

			absoluteJudgementPos = new Vector2<float>(noteTrack.Transform2D.Position.X, 0f);

			judgementPos = new Vector2<float>(absoluteJudgementPos.X +
				gameManager.settings.baseOffset * speed, 0f);

			//judgementPos = new Vector2<float>(absoluteJudgementPos.X + 
			//	(gameManager.settings.baseOffset + gameManager.settings.customOffset) * speed, 0f);

			//finalStartPosX =
			//	noteTrack.Hierarchy.LocalPosition2D.X + (((-gameManager.startTime + gameManager.musicDelay + customOffset) /
			//	audioManager.musicObj[0].GetComponent<Audio>().LengthF) * length);

			// lanes

			//notesLane1 = new List<NoteProperties>();
			//notesLane2 = new List<NoteProperties>();
			//notesLane3 = new List<NoteProperties>();


			//start = Entity.Instantiate();
			start.Transform2D.Position = new Vector2<float>(noteTrack.Transform2D.Position.X, 0f);
			//start.Hierarchy.Parent = noteTrack; //start.AddComponent<Hierarchy>();
			//start.Tag("start");
			//end = Entity.Instantiate();
			end.Transform2D.Position = new Vector2<float>(noteTrack.Transform2D.Position.X + length, 0f);
			//end.Hierarchy.Parent = noteTrack;
			//end.Tag("end");
		}

		public void InitializeTrack()
		{
			if (gameManager == null)
			{
				Logger.Error("Game Manager not found");
				return;
			}
			if (audioManager == null)
			{
				Logger.Error("Audio Manager not found");
				return;
			}
			if (gameManager.settings == null)
			{
				Logger.Error("Settings not found");
				return;
			}


			//noteStart = new Vector2<float>(absoluteJudgementPos.X +
			//	(((-gameManager.settings.startTime + gameManager.settings.musicDelay + 
			//	gameManager.settings.baseOffset + gameManager.settings.customOffset) /
			//	 gameManager.settings.duration) * length), 0f);

			noteStart = new Vector2<float>(absoluteJudgementPos.X +
				(((-gameManager.settings.startTime + gameManager.settings.musicDelay +
				gameManager.settings.baseOffset) /
				 gameManager.settings.duration) * length), 0f);

			musicDelayLen = gameManager.settings.musicDelay / gameManager.settings.duration * length;
			startTimeLen = -gameManager.settings.startTime / gameManager.settings.duration * length;
			baseOffsetLen = gameManager.settings.baseOffset / gameManager.settings.duration * length;

			customOffsetLen = gameManager.settings.customOffset / gameManager.settings.duration * length;

			absoluteOffsetLen = noteStart.X - judgementPos.X;
			
			noteTrack.Transform2D.Position = noteStart;


			// bar markers
			barCountF = gameManager.settings.bpm * gameManager.settings.duration /
				60f / (float)gameManager.settings.timeSigTop;
			barCount = (int)(barCountF);

			beatTimeLength = 60.0 / (double)gameManager.settings.bpm;
				//(double)((double)gameManager.settings.duration / 
				//(double)(barCountF) / (double)gameManager.settings.timeSigTop);

			//Logger.Critical(beatTimeLength.ToString());
			//Logger.Critical(gameManager.settings.timeSigTop.ToString());
			//Logger.Critical(barCount.ToString());
			//Logger.Critical(audioManager.musicObj[0].GetComponent<Audio>().LengthF.ToString());

			for (int i = 0; i <= barCount; i++)
			{
				Entity bar = Entity.Instantiate(barMarker);
				bar.Hierarchy.Parent = barMarkerHolder;
				bar.Hierarchy.LocalPosition2D = 
					new Vector2<float>((float)i * length / (float)barCountF, 400f);
			}

			currTrackTime = 0.0;
		}

		public void InitializeNotes()
		{
			foreach (Entity note in notes)
			{
				note.GetScript<NoteProperties>().InitNote();
			}

			// sort
			SortLanes();
		}

		public override void Update()
		{
			try
			{
				if (audioManager == null)
				{
					Logger.Error("Audio Manager not found");
					return;
				}

				MoveTrack();
			}
			catch (System.Exception e)
			{
				Logger.Error(e.Message);
			}
		}

		void MoveTrack()
		{
			if (gameManager == null)
			{
				Logger.Error("Game Manager not found");
				return;
			}
			if (audioManager == null)
			{
				Logger.Error("Audio Manager not found");
				return;
			}
			if (gameManager.settings == null)
			{
				Logger.Error("Settings not found");
				return;
			}

			// translate notes
			if (!audioManager.musicObj[0].GetComponent<Audio>().IsPlaying ||
				audioManager.preTrackTimer > 0f) // if not playing
			{
				//noteTrack.Transform2D.Translate(Vector2<float>.Left * speed * Time.DeltaTimeF);
				// setting the x and y for local transform directly is not working

				//noteTrack.Hierarchy.LocalPosition2D = new Vector2<float>(
				//	noteTrack.Hierarchy.LocalPosition2D.X - speed * Time.DeltaTimeF,
				//	noteTrack.Hierarchy.LocalPosition2D.Y);

				noteTrack.Transform2D.Position = new Vector2<float>(
					judgementPos.X +
					(audioManager.preTrackTimer / gameManager.settings.musicDelay * musicDelayLen), 0f);
			}
			else
			{
				//if (lastSystemTime == 0.0)
				//{
				//	trackTime = Stopwatch.StartNew();
				//}

				noteTrack.Transform2D.Position = new Vector2<float>((float)(
					(double)(judgementPos.X) - (audioManager.musicObj[0].GetComponent<Audio>().CurrentTimeD /
					audioManager.musicObj[0].GetComponent<Audio>().LengthD * (double)(length))), 0f);
					//(double)(gameManager.settings.duration) * (double)(length))), 0f);

				//noteTrack.Transform2D.Position = new Vector2<float>((float)(
				//	(double)(judgementPos.X) - ((audioManager.musicObj[0].GetComponent<Audio>().CurrentTimeD /
				//	(double)(gameManager.settings.duration)) * (double)(length))), 0f);

				//noteTrack.Transform2D.Position = new Vector2<float>((float)(
				//	(double)(judgementPos.X) - ((currTrackTime /
				//	(double)(gameManager.settings.duration)) * (double)(length))), 0f);

				//lastSystemTime = trackTime.Elapsed.TotalSeconds;

				//Logger.Critical(audioManager.musicObj[0].GetComponent<Audio>().LengthF.ToString());
			}
		}

		public override void FixedUpdate()
		{
			//if (currTrackTime <= 0)
			//	tempX = (float)audioManager.musicObj[0].GetComponent<Audio>().CurrentTimeD;

			//currTrackTime += Time.FixedDeltaTime;

			//if (currTrackTime >= 1)
			//{
			//	Logger.Error(audioManager.musicObj[0].GetComponent<Audio>().CurrentTimeD.ToString());
			//	Logger.Error((tempX - audioManager.musicObj[0].GetComponent<Audio>().CurrentTimeD).ToString());

			//	if (tempX - audioManager.musicObj[0].GetComponent<Audio>().CurrentTimeD > 0.01)
			//	{
			//		int newTime = (int)audioManager.musicObj[0].GetComponent<Audio>().CurrentTimeD;
			//		audioManager.musicObj[0].GetComponent<Audio>().CurrentTimeD = (double)newTime;
			//	}

			//	currTrackTime -= 1;
			//}

			//if (audioManager.musicObj[0].GetComponent<Audio>().IsPlaying ||
			//	audioManager.preTrackTimer <= 0f)
			//{
			//	noteTrack.Transform2D.Position = new Vector2<float>((float)(
			//		(double)(judgementPos.X) - (((double)currTrackTime /
			//		(double)(gameManager.settings.duration)) * (double)(length))), 0f);
			//	currTrackTime += Time.FixedDeltaTime;
			//}
			//if (!(!audioManager.musicObj[0].GetComponent<Audio>().IsPlaying ||
			//	audioManager.preTrackTimer > 0f)) // if playing
			//{
			//	noteTrack.Transform2D.Position = new Vector2<float>((float)(
			//	(double)(judgementPos.X) - (((double)currTrackTime /
			//	(double)(gameManager.settings.duration)) * (double)(length))), 0f);
			//	currTrackTime += Time.FixedDeltaTime;
			//}
			//Logger.Critical(((double)(gameManager.settings.duration) * (double)(length)).ToString());

			if (audioManager == null)
			{
				Logger.Error("Audio Manager not found");
				return;
			}

			ConveyorBelt();

			if (!audioManager.musicObj[0].GetComponent<Audio>().IsPlaying) return; // if not playing, do nothing

			MissNotes();

			//ConveyorBelt();
		}

		void MissNotes()
		{
			if (gameManager == null)
			{
				Logger.Error("Game Manager not found");
				return;
			}
			if (audioManager == null)
			{
				Logger.Error("Audio Manager not found");
				return;
			}
			if (judgement == null)
			{
				Logger.Error("Judgement not found");
				return;
			}
			if (score == null)
			{
				Logger.Error("Score not found");
				return;
			}
			if (gameManager.settings == null)
			{
				Logger.Error("Settings not found");
				return;
			}

			//foreach(Entity note in notes)
			//{
			//	NoteProperties prop = note.GetScript<NoteProperties>();
			//	if (prop.trackPos + judgement.mehThreshold <
			//		audioManager.musicObj[0].GetComponent<Audio>().CurrentTimeF)
			//	{
			//		if (!(prop.type == 1 && judgement.onHoldNote[0])) // don't miss hold notes being held
			//		{
			//			if (prop.trackPos <= gameManager.settings.startTime) prop.judgement = 0;

			//			Entity obj = note;

			//			//judgement.DestroyNote(obj, 1);

			//			//switch (prop.lane)
			//			//{
			//			//	case 1:
			//			//		notesLane1.Remove(prop);
			//			//		break;
			//			//	case 2:
			//			//		notesLane2.Remove(prop);
			//			//		break;
			//			//	case 3:
			//			//		notesLane3.Remove(prop);
			//			//		break;
			//			//}
			//			Logger.Critical(obj.GetScript<NoteProperties>().id.ToString());
			//			Logger.Critical(obj.GetScript<NoteProperties>().type.ToString());
			//			obj.GetScript<NoteProperties>().HitNote();
			//			score.UpdateScore(0);
			//		}
			//	}
			//}

			foreach (NoteProperties note in notesLane1)
			{
				if (note.trackPosCorrected + judgement.mehThreshold <
					audioManager.musicObj[0].GetComponent<Audio>().CurrentTimeF)
				{
					if (!(note.type == 1 && judgement.onHoldNote[0])) // don't miss hold notes being held
					{
						if (note.trackPosCorrected <= gameManager.settings.startTime) note.judgement = 0;

						note.HitNote();
						//score.UpdateScore(0);
					}
				}
			}
			foreach (NoteProperties note in notesLane2)
			{
				if (note.trackPosCorrected + judgement.mehThreshold <
					audioManager.musicObj[0].GetComponent<Audio>().CurrentTimeF)
				{
					if (!(note.type == 1 && judgement.onHoldNote[1])) // don't miss hold notes being held
					{
						if (note.trackPosCorrected <= gameManager.settings.startTime) note.judgement = 0;

						note.HitNote();
						//score.UpdateScore(0);
					}
				}
			}
			foreach (NoteProperties note in notesLane3)
			{
				if (note.trackPosCorrected + judgement.mehThreshold <
					audioManager.musicObj[0].GetComponent<Audio>().CurrentTimeF)
				{
					if (!(note.type == 1 && judgement.onHoldNote[2])) // don't miss hold notes being held
					{
						if (note.trackPosCorrected <= gameManager.settings.startTime) note.judgement = 0;

						note.HitNote();
						//score.UpdateScore(0);
					}
				}
			}

			// remove notes
			foreach (NoteProperties note in lane1NotesToRemove)
			{
				notesLane1.Remove(note);
			}
			foreach (NoteProperties note in lane2NotesToRemove)
			{
				notesLane2.Remove(note);
			}
			foreach (NoteProperties note in lane3NotesToRemove)
			{
				notesLane3.Remove(note);
			}
			lane1NotesToRemove.Clear();
			lane2NotesToRemove.Clear();
			lane3NotesToRemove.Clear();
		}

		void ConveyorBelt()
		{
			if (gameManager == null)
			{
				Logger.Error("Game Manager not found");
				return;
			}
			if (audioManager == null)
			{
				Logger.Error("Audio Manager not found");
				return;
			}
			if (gameManager.settings == null)
			{
				Logger.Error("Settings not found");
				return;
			}

			foreach (Entity note in notes)
			{
				NoteProperties n = note.GetScript<NoteProperties>();
				if (!n.isHit &&
					n.trackPosCorrected - audioManager.musicObj[0].GetComponent<Audio>().CurrentTimeF <
					gameManager.settings.noteActivateTime)
				{
					if (note.Active) continue;
					//note.GetScript<NoteProperties>().ReAlignNote();
					//if (!note.Active) 
					note.Active = true;

					//if (!note.GetComponent<Render2D>().IsActive) 
					note.GetComponent<Render2D>().IsActive = true;

					//n.AlignNote();
				}
			}
		}

		public void SortLanes()
		{
			notesLane1 = notesLane1.OrderBy(o => o.trackPos).ToList();
			notesLane2 = notesLane2.OrderBy(o => o.trackPos).ToList();
			notesLane3 = notesLane3.OrderBy(o => o.trackPos).ToList();
		}
	}
}
