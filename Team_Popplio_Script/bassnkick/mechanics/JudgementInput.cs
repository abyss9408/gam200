/******************************************************************************/
/*!
\file   JudgementInput.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for implementing the judgement of the notes from player input

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System.Collections;
using System.Collections.Generic;

using Team_Popplio;
using Team_Popplio.Attributes;
using Team_Popplio.Libraries;
using Team_Popplio.Extern;
using Team_Popplio.Components;
using Team_Popplio.ECS;
using PopplioMath = Team_Popplio.Extensions.PopplioMath;
using BassNKick.Lib;

namespace BassNKick
{
	public class JudgementInput : PopplioMonoScript
	{
		#region variables

		//[Header("Fields")]
		[InspectorIgnore]
		public Track? track;
		[InspectorIgnore]
		public Player? player;

		[Inspector("Track Obj")]
		public Entity trackObj = Entity.EntityField();
		[Inspector("Player Obj")]
		public Entity playerObj = Entity.EntityField();

		[Inspector("Removed Notes")]
		public Entity removedNotes = Entity.EntityField();

		// Particle System
		//[Inspector("P Sys 1")]
		//public Entity pSys1 = Entity.EntityField();
		//[Inspector("P Sys 2")]
		//public Entity pSys2 = Entity.EntityField();
		//[Inspector("P Sys 3")]
		//public Entity pSys3 = Entity.EntityField();

		// lane 1 -> top
		// lane 2 -> mid
		// lane 3 -> bot

		// input states
		[InspectorIgnore]
		public Dictionary<int, bool> lanePress = new Dictionary<int, bool>();
		[InspectorIgnore]
		public Dictionary<int, bool> laneHold = new Dictionary<int, bool>();

		// keys
		//[Header("Settings")]
		//[SerializeField]
		//GLFW lane1Key = Keybinds.lane1; // GLFW.KEY_R;
		//[SerializeField]
		//GLFW lane2Key = Keybinds.lane2; // GLFW.KEY_F;
		//[SerializeField]
		//GLFW lane3Key = Keybinds.lane3; // GLFW.KEY_V;

		//[Header("Judgement")]
		[Inspector("Perfect Threshold")]
		public float perfectThreshold = .08f;
		[Inspector("Great Threshold")]
		public float greatThreshold = .18f;
		[Inspector("Meh Threshold")]
		public float mehThreshold = .3f;

		//[Header("Update Buffer")]
		[Inspector("Press Threshold")]
		float pressThreshold = .1f;

		//[Header("States")]

		float currTime; // current time

		// timer for press state
		float keyPressTimer1, keyPressTimer2, keyPressTimer3;

		//bool isInThreshold = false;

		// hold note check
		[InspectorIgnore]
		public bool[] onHoldNote = new bool[3];

		[Inspector("Lanes Target UI")]
		public Entity[] laneTargetUI = { Entity.EntityField(), Entity.EntityField(), Entity.EntityField() };

		[Inspector("Lanes Target Aura UI")]
		public Entity[] laneTargetAuraUI = { Entity.EntityField(), Entity.EntityField(), Entity.EntityField() };
		#endregion

		public override void Setup()
		{
			track = trackObj.GetScript<Track>();
			player = playerObj.GetScript<Player>();
		}

		// Start is called before the first frame update
		public override void Init()
		{
			// initialize

			//lanePress = new Dictionary<int, bool>();
			//laneHold = new Dictionary<int, bool>();

			lanePress.Add(1, false);
			lanePress.Add(2, false);
			lanePress.Add(3, false);

			laneHold.Add(1, false);
			laneHold.Add(2, false);
			laneHold.Add(3, false);

			keyPressTimer1 = 0f;
			keyPressTimer2 = 0f;
			keyPressTimer3 = 0f;

			//onHoldNote = new bool[3];

			perfectThreshold = StaticSettings.perfectThreshold;
			greatThreshold = StaticSettings.greatThreshold;
			mehThreshold = StaticSettings.mehThreshold;
			pressThreshold = StaticSettings.pressThreshold;

			Logger.Critical("Judgement Input Initialized");
			Logger.Critical("Perfect Threshold: " + perfectThreshold);
			Logger.Critical("Great Threshold: " + greatThreshold);
			Logger.Critical("Meh Threshold: " + mehThreshold);
			Logger.Critical("Press Threshold: " + pressThreshold);
		}

		void DestroyHoldNote(int lane, int mod) // todo: fix back hold note being detected while front hold note being held
		{
			if (track == null)
			{
				Logger.Error("Track not found");
				return;
			}
			
			Entity obj = Entity.EntityField();

			switch (lane)
			{
				case 1:
					if (track.notesLane1.Count <= 0) return; // todo: error
					track.notesLane1[0].judgement -= mod;
					if (track.notesLane1[0].judgement < 1) track.notesLane1[0].judgement = 1;
					obj = track.notesLane1[0].This;
					track.notesLane1.RemoveAt(0);

					break;

				case 2:
					if (track.notesLane2.Count <= 0) return; // todo: error
					track.notesLane2[0].judgement -= mod;
					if (track.notesLane2[0].judgement < 1) track.notesLane2[0].judgement = 1;
					obj = track.notesLane2[0].This;
					track.notesLane2.RemoveAt(0);

					break;

				case 3:
					if (track.notesLane3.Count <= 0) return; // todo: error
					track.notesLane3[0].judgement -= mod;
					if (track.notesLane3[0].judgement < 1) track.notesLane3[0].judgement = 1;
					obj = track.notesLane3[0].This;
					track.notesLane3.RemoveAt(0);

					break;
			}

			//Entity.Destroy(obj);
			//obj.GetScript<NoteProperties>().HitNote();
			//RemoveNote(obj);

			obj.GetScript<NoteProperties>().HitNote();
			//DestroyNote(obj, lane);

			onHoldNote[lane - 1] = false;

			laneHold[lane] = false;
		}

		//public void DestroyNote(Entity obj, int lane)
		//{
		//	//Entity.Destroy(obj);
		//	NoteProperties n = obj.GetScript<NoteProperties>();
		//	n.HitNote();
		//	//RemoveNote(obj);
		//	n.RemoveNote();
		//}

		//void RemoveNote(Entity obj)
		//{
		//	//obj.Hierarchy.Parent = removedNotes;
		//	//obj.Hierarchy.InheritActive = true;

		//	obj.GetScript<NoteProperties>().RemoveNote();
		//}

		void DiffNote(int lane, int type)
		{
			if (track == null)
			{
				Logger.Error("Track not found");
				return;
			}
			if (player == null)
			{
				Logger.Error("Player not found");
				return;
			}
			
			switch (lane)
			{
				case 1:
					switch (type)
					{
						case 0: // atomic / tap
							Entity obj = track.notesLane1[0].This;

							track.notesLane1.RemoveAt(0);
							obj.GetScript<NoteProperties>().HitNote();
							//DestroyNote(obj, 1);

							player.MoveTo(1, 1);
							break;
						case 1: // non-atomic / hold
							onHoldNote[0] = true;

							player.MoveTo(1, 2);

							track.notesLane1[0].HoldFeedback();
							break; 
						case 2:
						case 3: // atomic / directional
							Entity obj2 = track.notesLane1[0].This;

							// bad for wrong direction
							if ((type == 2 && player.heightBeforeMove > track.lane1.Transform2D.Position.Y) ||
								(type == 3 && player.heightBeforeMove < track.lane1.Transform2D.Position.Y)) 
								obj2.GetScript<NoteProperties>().judgement = 1;

							track.notesLane1.RemoveAt(0);
							obj2.GetScript<NoteProperties>().HitNote();
							break;
					}
					break;

				case 2:
					switch (type)
					{
						case 0: // atomic / tap
							Entity obj = track.notesLane2[0].This;

							track.notesLane2.RemoveAt(0);
							obj.GetScript<NoteProperties>().HitNote();
							//DestroyNote(obj, 2);

							player.MoveTo(2, 1);
							break;
						case 1: // non-atomic / hold
							onHoldNote[1] = true;

							player.MoveTo(2, 2);

							track.notesLane2[0].HoldFeedback();
							break;
						case 2:
						case 3: // atomic / directional
							Entity obj2 = track.notesLane2[0].This;

							// bad for wrong direction
							if ((type == 2 && player.heightBeforeMove > track.lane2.Transform2D.Position.Y) ||
								(type == 3 && player.heightBeforeMove < track.lane2.Transform2D.Position.Y))
								obj2.GetScript<NoteProperties>().judgement = 1;

							track.notesLane2.RemoveAt(0);
							obj2.GetScript<NoteProperties>().HitNote();
							break;
					}
					break;

				case 3:
					switch (type)
					{
						case 0: // atomic / tap
							Entity obj = track.notesLane3[0].This;

							track.notesLane3.RemoveAt(0);
							obj.GetScript<NoteProperties>().HitNote();
							//DestroyNote(obj, 3);

							player.MoveTo(3, 1);
							break;
						case 1: // non-atomic / hold
							onHoldNote[2] = true;

							player.MoveTo(3, 2);

							track.notesLane3[0].HoldFeedback();
							break;
						case 2:
						case 3: // atomic / directional
							Entity obj2 = track.notesLane3[0].This;

							// bad for wrong direction
							if ((type == 2 && player.heightBeforeMove > track.lane3.Transform2D.Position.Y) ||
								(type == 3 && player.heightBeforeMove < track.lane3.Transform2D.Position.Y))
								obj2.GetScript<NoteProperties>().judgement = 1;

							track.notesLane3.RemoveAt(0);
							obj2.GetScript<NoteProperties>().HitNote();
							break;
					}
					break;
			}
		}

		void Judge()
		{
			if (track == null)
			{
				Logger.Error("Track not found");
				return;
			}

			// judgement

			// atomic (tap type) =================================================================

			if (track.notesLane1.Count > 0 && (lanePress[1]))
			{
				if (track.notesLane1[0].trackPosCorrected - currTime < mehThreshold)
				{
					track.notesLane1[0].judgement = 1;
					if (track.notesLane1[0].trackPosCorrected - currTime < greatThreshold &&
						track.notesLane1[0].trackPosCorrected - currTime > -greatThreshold)
					{
						track.notesLane1[0].judgement = 2;
						if (track.notesLane1[0].trackPosCorrected - currTime < perfectThreshold &&
							track.notesLane1[0].trackPosCorrected - currTime > -perfectThreshold)
						{
							track.notesLane1[0].judgement = 3;
						}
					}

					DiffNote(1, track.notesLane1[0].type);
				}

				lanePress[1] = !lanePress[1];
			}
			if (track.notesLane2.Count > 0 && (lanePress[2])) // for now press only
			{
				if (track.notesLane2[0].trackPosCorrected - currTime < mehThreshold)
				{
					track.notesLane2[0].judgement = 1;
					if (track.notesLane2[0].trackPosCorrected - currTime < greatThreshold &&
						track.notesLane2[0].trackPosCorrected - currTime > -greatThreshold)
					{
						track.notesLane2[0].judgement = 2;
						if (track.notesLane2[0].trackPosCorrected - currTime < perfectThreshold &&
							track.notesLane2[0].trackPosCorrected - currTime > -perfectThreshold)
						{
							track.notesLane2[0].judgement = 3;
						}
					}

					DiffNote(2, track.notesLane2[0].type);
				}

				lanePress[2] = !lanePress[2];
			}
			if (track.notesLane3.Count > 0 && (lanePress[3])) // for now press only
			{
				if (track.notesLane3[0].trackPosCorrected - currTime < mehThreshold)
				{
					track.notesLane3[0].judgement = 1;
					if (track.notesLane3[0].trackPosCorrected - currTime < greatThreshold &&
						track.notesLane3[0].trackPosCorrected - currTime > -greatThreshold)
					{
						track.notesLane3[0].judgement = 2;
						if (track.notesLane3[0].trackPosCorrected - currTime < perfectThreshold &&
							track.notesLane3[0].trackPosCorrected - currTime > -perfectThreshold)
						{
							track.notesLane3[0].judgement = 3;
						}
					}

					DiffNote(3, track.notesLane3[0].type);
				}

				lanePress[3] = !lanePress[3];
			}

			// atomic (tap type) =================================================================

			// non-atomic (hold type) ==================================================================

			if (track.audioManager == null)
			{
				Logger.Error("Audio Manager not found");
				return;
			}

			for (int i = 1; i <= 3; i++)
			{
				switch (i)
				{
					case 1:
						if (track.notesLane1.Count <= 0 || track.notesLane1[0].type != 1) continue;
						break;

					case 2:
						if (track.notesLane2.Count <= 0 || track.notesLane2[0].type != 1) continue;
						break;

					case 3:
						if (track.notesLane3.Count <= 0 || track.notesLane3[0].type != 1) continue;
						break;
				}

				if (onHoldNote[i - 1])
				{
					if (laneHold[i]) 
					{
						switch (i)
						{
							case 1:
								// front sprite stays at judgement line
								track.notesLane1[0].HoldShorten(track.audioManager.musicObj[0].GetComponent<Audio>().CurrentTimeF);
								
								if (track.notesLane1[0].trackPosCorrected + track.notesLane1[0].trackLength - currTime
									< -perfectThreshold)
									DestroyHoldNote(i, 0);
								break;

							case 2:
								// front sprite stays at judgement line
								track.notesLane2[0].HoldShorten(track.audioManager.musicObj[0].GetComponent<Audio>().CurrentTimeF);

								if (track.notesLane2[0].trackPosCorrected + track.notesLane2[0].trackLength - currTime
									< -perfectThreshold)
									DestroyHoldNote(i, 0);
								break;

							case 3:
								// front sprite stays at judgement line
								track.notesLane3[0].HoldShorten(track.audioManager.musicObj[0].GetComponent<Audio>().CurrentTimeF);

								if (track.notesLane3[0].trackPosCorrected + track.notesLane3[0].trackLength - currTime
									< -perfectThreshold)
									DestroyHoldNote(i, 0);
								break;
						} //Logger.Critical("Holding");
					}
					else
					{
						int judgeModify = 3;
						onHoldNote[i - 1] = false;
						//Logger.Critical("Hit");
						switch (i)
						{
							case 1:
								if (track.notesLane1[0].trackPosCorrected + track.notesLane1[0].trackLength - currTime < mehThreshold)
								{
									judgeModify = 2;
									if (track.notesLane1[0].trackPosCorrected + track.notesLane1[0].trackLength - currTime < greatThreshold)
									{
										judgeModify = 1;
										if (track.notesLane1[0].trackPosCorrected + track.notesLane1[0].trackLength - currTime
											< perfectThreshold)
										{
											judgeModify = 0;
										}
									}
								}

								DestroyHoldNote(1, judgeModify);

								break;
							case 2:
								if (track.notesLane2[0].trackPosCorrected + track.notesLane2[0].trackLength - currTime < mehThreshold)
								{
									judgeModify = 2;
									if (track.notesLane2[0].trackPosCorrected + track.notesLane2[0].trackLength - currTime < greatThreshold)
									{
										judgeModify = 1;
										if (track.notesLane2[0].trackPosCorrected + track.notesLane2[0].trackLength - currTime
											< perfectThreshold)
										{
											judgeModify = 0;
										}
									}
								}

								DestroyHoldNote(2, judgeModify);

								break;
							case 3:
								if (track.notesLane3[0].trackPosCorrected + track.notesLane3[0].trackLength - currTime < mehThreshold)
								{
									judgeModify = 2;
									if (track.notesLane3[0].trackPosCorrected + track.notesLane3[0].trackLength - currTime < greatThreshold)
									{
										judgeModify = 1;
										if (track.notesLane3[0].trackPosCorrected + track.notesLane3[0].trackLength - currTime
											< perfectThreshold)
										{
											judgeModify = 0;
										}
									}
								}

								DestroyHoldNote(3, judgeModify);

								break;
						}
					}
				}
			}

			// non-atomic (hold type) ==================================================================
		}

		// Update is called once per frame
		public override void Update()
		{
			if (GameManager.IsPaused) return;

			if (track == null)
			{
				Logger.Error("Track not found");
				return;
			}
			if (track.audioManager == null)
			{
				Logger.Error("Audio Manager not found");
				return;
			}

			currTime = track.audioManager.musicObj[0].GetComponent<Audio>().CurrentTimeF; // current time

			if (!track.audioManager.musicObj[0].GetComponent<Audio>().IsPlaying) return;
			
			Detect();

			Judge();

			//if ((lanePress[1] || laneHold[1]) && !pSys1.Active) pSys1.Active = true;
			//else if (!(lanePress[1] || laneHold[1]) && pSys1.Active) pSys1.Active = false;

			//if ((lanePress[2] || laneHold[2]) && !pSys2.Active) pSys2.Active = true;
			//else if (!(lanePress[2] || laneHold[2]) && pSys2.Active) pSys2.Active = false;

			//if ((lanePress[3] || laneHold[3]) && !pSys3.Active) pSys3.Active = true;
			//else if (!(lanePress[3] || laneHold[3]) && pSys3.Active) pSys3.Active = false;
		}

		void Detect()
		{
			if (track == null)
			{
				Logger.Error("Track is null");
				return;
			}
			if (player == null)
			{
				Logger.Error("Player not found");
				return;
			}

			// inputs

			// lane 1 ========================================
			if (TempInputFix.IsKeyTriggered(Keybinds.lane1))
			{
				keyPressTimer1 = pressThreshold;

				lanePress[1] = true;
			}

			if (Input.IsKeyDown(Keybinds.lane1)) laneHold[1] = true;
			else
			{
				laneHold[1] = false;
				//onHoldNote[0] = false;
			}

			// ===============================================

			// lane 2 ========================================
			if (TempInputFix.IsKeyTriggered(Keybinds.lane2))
			{
				keyPressTimer2 = pressThreshold;

				lanePress[2] = true;
			}

			if (Input.IsKeyDown(Keybinds.lane2)) laneHold[2] = true;
			else
			{
				laneHold[2] = false;
				//onHoldNote[1] = false;
			}

			// ===============================================

			// lane 3 ========================================
			if (TempInputFix.IsKeyTriggered(Keybinds.lane3))
			{
				keyPressTimer3 = pressThreshold;

				lanePress[3] = true;
			}

			if (Input.IsKeyDown(Keybinds.lane3)) laneHold[3] = true;
			else
			{
				laneHold[3] = false;
				//onHoldNote[2] = false;
			}


			// timer for key press buffer
			if (lanePress[1]) // lane 1
			{
				keyPressTimer1 -= Time.DeltaTimeF;
				if (keyPressTimer1 <= 0f) lanePress[1] = false;
			}

			if (lanePress[2]) // lane 2
			{
				keyPressTimer2 -= Time.DeltaTimeF;
				if (keyPressTimer2 <= 0f) lanePress[2] = false;
			}

			if (lanePress[3]) // lane 3
			{
				keyPressTimer3 -= Time.DeltaTimeF;
				if (keyPressTimer3 <= 0f) lanePress[3] = false;
			}

			// ===============================================

			// movement

			for (int i = 1; i < 4; i++)
			{
				if (lanePress[i])
				{
					player.MoveTo(i, 0); Logger.Debug("Press: " + i);

					switch (i) // debug
					{
						case 1:
							if (track.notesLane1.Count <= 0) break;
							Logger.Info("Lane 1 Next note: " + track.notesLane1[0].id);
							break;
						case 2:
							if (track.notesLane2.Count <= 0) break;
							Logger.Info("Lane 2 Next note: " + track.notesLane2[0].id);
							break;
						case 3:
							if (track.notesLane3.Count <= 0) break;
							Logger.Info("Lane 3 Next note: " + track.notesLane3[0].id);
							break;
					}
				}

				if (laneHold[i])
				{
					// constraints
					playerObj.GetComponent<RigidBody2D>().UseGravity = false;
					playerObj.GetComponent<RigidBody2D>().Velocity = new Vector2<float>(0, 0);
					Logger.Debug("Hold: " + i);
					break;
				}
			}

			// todo: fix (constraints stuck)
			if (!laneHold[1] && !laneHold[2] && !laneHold[3] &&
				player.state != Player.State.idle && player.state != Player.State.fall)
			{
				// constraints
				playerObj.GetComponent<RigidBody2D>().UseGravity = true;
				player.MoveTo(0, 0);
			}

			// UI
			for (int i = 1; i < 4; i++)
			{
				if (lanePress[i] || laneHold[i])
					laneTargetUI[i - 1].GetComponent<Render2D>().Color = Color.Magenta;
				else laneTargetUI[i - 1].GetComponent<Render2D>().Color = Color.White;

				if (laneHold[i]) laneTargetAuraUI[i - 1].Active = true;
				else laneTargetAuraUI[i - 1].Active = false;
			}
			//if (laneHold[1] || laneHold[2] || laneHold[3])
			//{
			//	player.Scream(true);
			//}
			//else
			//{
			//	player.Scream(false);
			//}
		}
	}
}
