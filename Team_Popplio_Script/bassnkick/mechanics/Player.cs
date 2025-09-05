/******************************************************************************/
/*!
\file   Player.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        BassNKick | C# file for player mechanics

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System.Collections.Generic;

namespace BassNKick
{
	public class Player : Team_Popplio.PopplioMonoScript
	{
		#region variables

		public enum State // Track player state
		{
			idle = 0,
			lane1,
			lane2,
			lane3,
			hurt,
			fall
		}

		public enum Anim // Track animation state
		{
			run = 0,
			slide,
			cJump,
			jump,
			kick,
			hold_blend,
			hold_loop,
			hurt,
			fall
		}

		[InspectorIgnore]
		public Dictionary<Anim, string> anims = new Dictionary<Anim, string>();

		Dictionary<string, int> aIndex = new Dictionary<string, int>(); // map anim name to anim index within animator

		Dictionary<Anim, State> aToS = new Dictionary<Anim, State>();

		[InspectorIgnore]
		public State state = State.idle;

		[InspectorIgnore]
		public Anim aStatePrev = Anim.run;
		[InspectorIgnore]
		public Anim aState = Anim.run;

		private RigidBody2D? rb;

		private Queue<Anim> aQueue = new Queue<Anim>();

		[Inspector("Lane 1")]
		Entity lane1 = Entity.EntityField();
		[Inspector("Lane 2")]
		Entity lane2 = Entity.EntityField();
		[Inspector("Lane 3")]
		Entity lane3 = Entity.EntityField();

		[Inspector("Note Grp")]
		string noteGrp = "";
		[Inspector("Ground Grp")]
		string groundGrp = "";

		private Animation2D? anim;

		[Inspector("Scream Obj")]
		public Entity screamObj = Entity.EntityField();

		public bool screaming = false;

		[Inspector("Skid Obj")]
		public Entity skidObj = Entity.EntityField();
		Animation2D? skidAnim;
		bool skidding = false;

		[Inspector("Jump Obj")]
		public Entity jumpObj = Entity.EntityField();
		Animation2D? jumpAnim;
		bool jumping = false;

		[Inspector("Up To Down Obj")]
		public Entity upToDownObj = Entity.EntityField();
		Animation2D? upToDownAnim;

		[Inspector("Down To Up Obj")]
		public Entity downToUpObj = Entity.EntityField();
		Animation2D? downToUpAnim;

		Vector2<float> prevPos = new Vector2<float>(); // for vfx
		[InspectorIgnore]
		public float heightBeforeMove = 0f; // for directional note

		int currLane = 0;
		int currJudge = -1;

		#endregion

		#region Loop
		public override void Setup()
		{
			rb = This.GetComponent<RigidBody2D>();
			anim = This.GetComponent<Animation2D>();
			skidAnim = skidObj.GetComponent<Animation2D>();
			jumpAnim = jumpObj.GetComponent<Animation2D>();
			upToDownAnim = upToDownObj.GetComponent<Animation2D>();
			downToUpAnim = downToUpObj.GetComponent<Animation2D>();
		}

		public override void Init()
		{
			anims.Add(Anim.run, "idle");
			anims.Add(Anim.slide, "lane3");
			anims.Add(Anim.cJump, "lane2");
			anims.Add(Anim.jump, "lane1");
			anims.Add(Anim.kick, "kick");
			anims.Add(Anim.hold_blend, "hold_blend");
			//anims.Add(Anim.hold_loop, "hold_loop");
			anims.Add(Anim.hold_loop, "attack");
			anims.Add(Anim.hurt, "hurt");
			anims.Add(Anim.fall, "fall");

			aToS.Add(Anim.run, State.idle);
			aToS.Add(Anim.slide, State.lane3);
			aToS.Add(Anim.cJump, State.lane2);
			aToS.Add(Anim.jump, State.lane1);

			state = State.idle;

			// map anim name to anim index within animator
			for (int i = 0; i < anim?.Animations.Length; i++)
			{
				aIndex.Add(anim.Animations[i], i);
			}

			This.Hierarchy.InheritTransform = false;
			
			//rb.UseGravity = true;

			prevPos = This.Transform2D.Position;
			heightBeforeMove = This.Transform2D.Position.Y;
		}

		public override void Update()
		{
			if (rb == null || anim == null) return;

			if (GameManager.state != GameManager.TrackStart.PreTrack) 
				rb.UseGravity = true;

			//UpdateAnim();
			if (jumping)
			{
				if (!jumpObj.Active) jumpObj.Active = true;

				if (jumpAnim?.CurrentFrame.X == 3 && jumpAnim.CurrentFrame.Y == 0)
				{
					jumpAnim.CurrentFrame = new Vector2<int>(0, 4);
					jumping = false;
					jumpObj.Active = false;
				}
			}
			if (skidding)
			{
				if (!skidObj.Active) skidObj.Active = true;
			}
			else if (skidObj.Active) skidObj.Active = false;

			MoveVFX();
		}

		public override void FixedUpdate()
		{

		}

		public override void End()
		{
			
		}
		#endregion

		#region Events
		public override void OnTrigger2DEnter(Entity other, double time) // when getting hit by note
		{
			if (other.BelongsToGroup(noteGrp)) Hurt();
			//if (other.BelongsToGroup(groundGrp))
			//	rb.UseGravity = false;
			if (other.BelongsToGroup(groundGrp) && prevPos != This.Transform2D.Position)
				prevPos = This.Transform2D.Position;
		}

		public override void OnTrigger2DStay(Entity other, double time) 
		{
			if (other.BelongsToGroup(groundGrp))
			{
				if (state == State.fall)
				{
					state = State.idle;
					UpdateA(Anim.run);
				}
			}
		}

		public override void OnTrigger2DExit(Entity other, double time)
		{
			if (other.BelongsToGroup(groundGrp))
			{
				jumping = true;
				//rb.UseGravity = true;
			}
		}

		//public override void OnCollision2DEnter(Entity other, double time)
		//{
		//	if (other.BelongsToGroup(groundGrp))
		//		rb.UseGravity = false;
		//}
		#endregion

		#region Functions
		void Hurt() //todo: hurt player
		{
			state = State.hurt;
		}

		void Scream(bool toggle)
		{
			if (AudioManager.audioManagerInstance == null) return;
			if (GameManager.staticInstance == null) return;
			if (GameManager.staticInstance.settings == null) return;

			screaming = toggle;
			screamObj.Active = toggle;

			if (toggle)
			{
				if (currJudge > GameManager.staticInstance.settings.sfxHighestMistake)
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

				// placeholder
				//if (AudioManager.audioManagerInstance.soundObj[1].GetComponent<Audio>().IsPlaying)
				//	AudioManager.audioManagerInstance.soundObj[1].GetComponent<Audio>().Stop();
				//AudioManager.audioManagerInstance?.soundObj[1].GetComponent<Audio>().Play(); // correct sfx

				//switch (currLane)
				//{
				//	case 1:
				//		switch (currJudge)
				//		{
				//			case 0:
				//				GameManager.staticInstance.track.notesLane1[0].holdFeedback.GetScript<NoteFeedback>().judge = "miss";
				//				GameManager.staticInstance.track.notesLane1[0].holdFeedback.Active = true;
				//				break;
				//			case 1:
				//				GameManager.staticInstance.track.notesLane1[0].holdFeedback.GetScript<NoteFeedback>().judge = "meh";
				//				GameManager.staticInstance.track.notesLane1[0].holdFeedback.Active = true;
				//				break;
				//			case 2:
				//				GameManager.staticInstance.track.notesLane1[0].holdFeedback.GetScript<NoteFeedback>().judge = "great";
				//				GameManager.staticInstance.track.notesLane1[0].holdFeedback.Active = true;
				//				break;
				//			case 3:
				//				GameManager.staticInstance.track.notesLane1[0].holdFeedback.GetScript<NoteFeedback>().judge = "perfect";
				//				GameManager.staticInstance.track.notesLane1[0].holdFeedback.Active = true;
				//				break;
				//		}
				//		break;
				//	case 2:
				//		switch (currJudge)
				//		{
				//			case 0:
				//				GameManager.staticInstance.track.notesLane2[0].holdFeedback.GetScript<NoteFeedback>().judge = "miss";
				//				GameManager.staticInstance.track.notesLane2[0].holdFeedback.Active = true;
				//				break;
				//			case 1:
				//				GameManager.staticInstance.track.notesLane2[0].holdFeedback.GetScript<NoteFeedback>().judge = "meh";
				//				GameManager.staticInstance.track.notesLane2[0].holdFeedback.Active = true;
				//				break;
				//			case 2:
				//				GameManager.staticInstance.track.notesLane2[0].holdFeedback.GetScript<NoteFeedback>().judge = "great";
				//				GameManager.staticInstance.track.notesLane2[0].holdFeedback.Active = true;
				//				break;
				//			case 3:
				//				GameManager.staticInstance.track.notesLane2[0].holdFeedback.GetScript<NoteFeedback>().judge = "perfect";
				//				GameManager.staticInstance.track.notesLane2[0].holdFeedback.Active = true;
				//				break;
				//		}
				//		break;
				//	case 3:
				//		switch (currJudge)
				//		{
				//			case 0:
				//				GameManager.staticInstance.track.notesLane3[0].holdFeedback.GetScript<NoteFeedback>().judge = "miss";
				//				GameManager.staticInstance.track.notesLane3[0].holdFeedback.Active = true;
				//				break;
				//			case 1:
				//				GameManager.staticInstance.track.notesLane3[0].holdFeedback.GetScript<NoteFeedback>().judge = "meh";
				//				GameManager.staticInstance.track.notesLane3[0].holdFeedback.Active = true;
				//				break;
				//			case 2:
				//				GameManager.staticInstance.track.notesLane3[0].holdFeedback.GetScript<NoteFeedback>().judge = "great";
				//				GameManager.staticInstance.track.notesLane3[0].holdFeedback.Active = true;
				//				break;
				//			case 3:
				//				GameManager.staticInstance.track.notesLane3[0].holdFeedback.GetScript<NoteFeedback>().judge = "perfect";
				//				GameManager.staticInstance.track.notesLane3[0].holdFeedback.Active = true;
				//				break;
				//		}
				//		break;
				//}
			}
		}

		void MoveVFX()
		{
			if (upToDownAnim == null || downToUpAnim == null) return;

			if (upToDownAnim.CurrentFrame.X == 2 && upToDownAnim.CurrentFrame.Y == 0)
			{
				upToDownAnim.CurrentFrame = new Vector2<int>(0, 2);
				upToDownObj.Active = false;
			}
			if (downToUpAnim.CurrentFrame.X == 2 && downToUpAnim.CurrentFrame.Y == 0)
			{
				downToUpAnim.CurrentFrame = new Vector2<int>(0, 2);
				downToUpObj.Active = false;
			}
		}

		void UpdateMoveVFX()
		{
			float diff = This.Transform2D.Position.Y - prevPos.Y;

			if (diff > 0)
			{
				downToUpObj.Active = true;
				downToUpObj.Transform2D.Position = new Vector2<float>(
					This.Transform2D.Position.X, This.Transform2D.Position.Y - diff / 2f);
			}
			else if (diff < 0)
			{
				upToDownObj.Active = true;
				upToDownObj.Transform2D.Position = new Vector2<float>(
					This.Transform2D.Position.X, This.Transform2D.Position.Y - diff / 2f);
			}
			else return;

			prevPos = This.Transform2D.Position;
		}

		void UpdateA(Anim a)
		{
			switch (a)
			{
				case Anim.hold_loop:
					if (!screaming) Scream(true);
					break;
				default:
					if (screaming) Scream(false);
					break;
			}

			switch (aState)
			{
				case Anim.run:
				case Anim.kick:
				case Anim.hold_blend:

					break;

				default:
					if (aState != a) aStatePrev = aState;
					break;
			}

			aState = a;

			UpdateAnim();
		}

		void UpdateAnim()
		{
			if (anim == null) return;

			// Reactionary / Blend Animations

			if (anim.TimeElapsedSinceLastResume >= 1f && aQueue.Count > 0)
			{
				UpdateA(aQueue.Dequeue());
			}

			switch (aState) // todo: fix (kick and hold animations not running)
			{
				case Anim.kick:
					if (anim.CurrentAnimation == "kick" && anim.TimeElapsedSinceLastResume >= 1f)
					{
						state = State.fall;
						UpdateA(aStatePrev);
					}
					break;
				case Anim.hold_blend:
					if (anim.CurrentAnimation == "hold_blend" && anim.TimeElapsedSinceLastResume >= 1f) UpdateA(Anim.hold_loop);
					break;
			}

			//anim.Pause();
			anim.SetCurrentAnimation(anims[aState], false);
			//anim.Resume();
			anim.Play();
		}

		void Jump(int hit) // jump to lane 1
		{
			if (anim == null) return;

			state = State.lane1;
			UpdateA(Anim.jump);
			switch (hit)
			{
				case 0:
					anim.PlayOnce = true;
					break;
				case 1:
					anim.PlayOnce = false;
					//aQueue.Enqueue(Anim.kick);
					break;
				case 2:
					anim.PlayOnce = false;
					//aQueue.Enqueue(Anim.hold_blend);
					UpdateA(Anim.hold_loop);
					break;
			}

			//This.Transform2D.Position.Y = lane1.Transform2D.Position.Y; //print("1");
			This.Transform2D.Position = new Vector2<float>(This.Transform2D.Position.X, lane1.Transform2D.Position.Y);

			UpdateMoveVFX();
		}

		void CrouchJump(int hit) // jump to lane 2
		{
			if (anim == null) return;

			state = State.lane2;
			UpdateA(Anim.cJump);
			switch (hit)
			{
				case 0:
					anim.PlayOnce = true;
					break;
				case 1:
					anim.PlayOnce = false;
					//aQueue.Enqueue(Anim.kick);
					break;
				case 2:
					anim.PlayOnce = false;
					//aQueue.Enqueue(Anim.hold_blend);
					UpdateA(Anim.hold_loop);
					break;
			}

			//This.Transform2D.Position.Y = lane2.Transform2D.Position.Y; //print("2");
			This.Transform2D.Position = new Vector2<float>(This.Transform2D.Position.X, lane2.Transform2D.Position.Y);

			UpdateMoveVFX();
		}

		void Slide(int hit)  // slide to lane 3
		{
			if (anim == null) return;

			state = State.lane3;
			UpdateA(Anim.slide);

			switch (hit)
			{
				case 0:
					anim.PlayOnce = true;
					break;
				case 1:
					anim.PlayOnce = false;
					///anim.runtimeAnimatorController.animationClips[aIndex[anims[Anim.slide]]].wrapMode = WrapMode.Once;
					///updateA(Anim.kick);
					break;
				case 2:
					anim.PlayOnce = false;
					///anim.runtimeAnimatorController.animationClips[aIndex[anims[Anim.slide]]].wrapMode = WrapMode.Once;
					///updateA(Anim.hold_blend);
					UpdateA(Anim.hold_loop);
					break;
			}

			//This.Transform2D.Position.Y = lane3.Transform2D.Position.Y; //print("3");
			This.Transform2D.Position = new Vector2<float>(This.Transform2D.Position.X, lane3.Transform2D.Position.Y);

			skidding = true;

			UpdateMoveVFX();
		}

		void Reset() // reset sprite + scale
		{
			// Reset to normal state
			state = State.fall;
			//UpdateA(aStatePrev);
			UpdateA(Anim.fall);

			if (skidding) skidding = false;
		}

		public void MoveTo(int i, int hit) // public interface (to move to lanes)
		{
			if (GameManager.staticInstance == null) return;
			if (GameManager.staticInstance.track == null) return;

			heightBeforeMove = This.Transform2D.Position.Y;

			currLane = i;

			switch (i)
			{
				case 0:
					Reset();
					break;
				case 1:
					if (GameManager.staticInstance.track.notesLane1.Count > 0) 
						currJudge = GameManager.staticInstance.track.notesLane1[0].judgement;
					else currJudge = -1;
					Jump(hit);
					break;
				case 2:
					if (GameManager.staticInstance.track.notesLane2.Count > 0)
						currJudge = GameManager.staticInstance.track.notesLane2[0].judgement;
					else currJudge = -1;
					CrouchJump(hit);
					break;
				case 3:
					if (GameManager.staticInstance.track.notesLane3.Count > 0)
						currJudge = GameManager.staticInstance.track.notesLane3[0].judgement;
					else currJudge = -1;
					Slide(hit);
					break;
			}

			//UpdateAnim();
		}
		#endregion
	}
}
