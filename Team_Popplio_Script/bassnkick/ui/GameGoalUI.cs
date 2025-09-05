/******************************************************************************/
/*!
\file   GameGoalUI.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        BassNKick | C# file for goal bar UI

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using BassNKick.Lib;

namespace BassNKick
{
	public class GameGoalUI : PopplioMonoScript
	{
		public Entity gameManagerObj = Entity.EntityField();
		GameManager? gameManager;
		//Score? score;

		[Inspector("Marker")]
		Entity marker = Entity.EntityField();

		[Inspector("End")]
		Entity end = Entity.EntityField();

		[Inspector("Back")]
		Entity back = Entity.EntityField();
		Render2D? backRender;

		[Inspector("Fill")]
		Entity fill = Entity.EntityField();
		Render2D? fillRender;

		[Inspector("Border")]
		Entity border = Entity.EntityField();
		Render2D? borderRender;

		//Vector2<float> startPos;
		//Vector2<float> endPos;

		float startPosX = 0f;
		float endPosX = 0f;

		bool animMarker = false;

        Vector2<float> markerScale = new Vector2<float>(1f, 1f);

        public override void Setup()
		{
			gameManager = gameManagerObj.GetScript<GameManager>();
			backRender = back.GetComponent<Render2D>();
			fillRender = fill.GetComponent<Render2D>();
			borderRender = border.GetComponent<Render2D>();
			if (marker != null)
			{
				startPosX = marker.Transform2D.Position.X;
				markerScale = marker.Transform2D.Scale;
			}
			if (end != null) endPosX = end.Transform2D.Position.X;
		}

		int barChannel = 0;
		Color? temp;

		const float rainbowDuration = 0.3f;
		float rainbowTimer = 0f;
		const float rainbowSpeed = 8f;

		public override void Update()
		{
			//score = gameManager.track.score;

			if (backRender == null || fillRender == null || borderRender == null)
			{
				Logger.Error("GameGoalUI: Missing render component");
				return;
			}

			if (animMarker) AnimateMarker();

			if (rainbowTimer > 0f)
			{
				if (backRender.Color == Color.White && temp != null)
				{
					backRender.Color = temp;
					fillRender.Color = temp;
					borderRender.Color = temp;
				}
				AnimateBar();
				rainbowTimer -= Time.DeltaTimeF;
			}
			else
			{
				if (backRender.Color != Color.White)
				{
					backRender.Color = Color.White;
					fillRender.Color = Color.White;
					borderRender.Color = Color.White;
				}
				if (rainbowTimer < 0f) rainbowTimer = 0f;
			}
		}

		public void UpdateMarker(float percentage)
		{
			marker.Transform2D.Position = 
				new Vector2<float>(startPosX + ((endPosX - startPosX) * percentage),
				marker.Transform2D.Position.Y);
			rainbowTimer = rainbowDuration;

			markerTimer = 0f;
			animMarker = true;
		}

		void AnimateBar()
		{
			if (backRender == null || fillRender == null || borderRender == null)
			{
				Logger.Error("GameGoalUI: Missing render component");
				return;
			}

			temp = new Color(backRender.Color.R, backRender.Color.G, backRender.Color.B, backRender.Color.A);
			backRender.Color = UILibrary.RainbowEffect(temp, barChannel, rainbowSpeed, Time.DeltaTimeF, out int currChannel);
			temp = new Color(fillRender.Color.R, fillRender.Color.G, fillRender.Color.B, fillRender.Color.A);
			fillRender.Color = UILibrary.RainbowEffect(temp, barChannel, rainbowSpeed, Time.DeltaTimeF, out currChannel);
			temp = new Color(borderRender.Color.R, borderRender.Color.G, borderRender.Color.B, borderRender.Color.A);
			borderRender.Color = UILibrary.RainbowEffect(temp, barChannel, rainbowSpeed, Time.DeltaTimeF, out currChannel);
			barChannel = currChannel;
		}

		float markerTimer = 0f;
		const float markerSpeed = 1f;

		void AnimateMarker()
		{
			marker.Transform2D.Scale = markerScale * UILibrary.EaseOutElastic(markerTimer);
			markerTimer += Time.DeltaTimeF * markerSpeed;

			if (markerTimer >= 1f)
			{
				marker.Transform2D.Scale = markerScale;
				markerTimer = 0f;
				animMarker = false;
			}
		}
	}
}
