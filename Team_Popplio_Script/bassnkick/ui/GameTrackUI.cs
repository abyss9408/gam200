/******************************************************************************/
/*!
\file   GameTrackUI.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for implementing the track UI in game

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System.Numerics;
using vbnc;

namespace BassNKick
{
	public class GameTrackUI : PopplioMonoScript
	{
		[Inspector("Marker")]
		Entity marker = Entity.EntityField();
		[Inspector("Bar")]
		Entity bar = Entity.EntityField();

		[Inspector("Track Obj")]
		Entity trackObj = Entity.EntityField();
		Track? track;

		float animTimer = 0f;

		float animSpeed = 3f;
		float animScale = 15f;

		Transform2D? markerT;

		public float modifier = 0.7f;

		// Start is called before the first frame update
		public override void Setup()
		{
			markerT = marker.Transform2D;
			track = trackObj.GetScript<Track>();
		}

		// Update is called once per frame
		public override void Update()
		{
			if (track == null)
			{
				Logger.Error("Track is null");
				return;
			}
			if (track.audioManager == null)
			{
				Logger.Error("Audio Manager is null");
				return;
			}
			if (markerT == null)
			{
				Logger.Error("Marker Transform2D is null");
				return;
			}

			if (GameManager.IsPaused) return;

			// timer
			animTimer += Time.DeltaTimeF * animSpeed;

			// text animation
			markerT.Rotation = (double)animScale * Math.Cos(Math.PI * (double)animTimer);

			if (GameManager.state == GameManager.TrackStart.Ended) return;

			Audio temp = track.audioManager.musicObj[0].GetComponent<Audio>();

			markerT.Position = new Vector2<float>(
				bar.Transform2D.Position.X - (bar.Transform2D.Scale.X / 2f * modifier) +
				(bar.Transform2D.Scale.X * (temp.CurrentTimeF / temp.LengthF) * modifier), 
				markerT.Position.Y);
		}
	}
}
