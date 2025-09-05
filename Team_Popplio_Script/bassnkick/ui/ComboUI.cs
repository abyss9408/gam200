/******************************************************************************/
/*!
\file   ComboUI.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for implementing the game camera

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using BassNKick.Lib;

namespace BassNKick
{
	public class ComboUI : PopplioMonoScript
	{
		[Inspector("Text Obj")]
		Entity textObj = Entity.EntityField();
		[InspectorIgnore]
		Text2D? text;
		[Inspector("Score Obj")]
		Entity scoreObj = Entity.EntityField();
		[InspectorIgnore]
		Score? score;

		///Color color;

		float animTimer = 0f;
		float animSpeed = 1.5f;

		[Inspector("Curr Combo")]
		int currCombo = 0;

		float fontSize = 1f;

		float animModScale = 2f;
		float animModTranslate = 100f;

		Vector2<float> initialTextPos = new Vector2<float>();

		public override void Setup()
		{
			text = textObj.GetComponent<Text2D>();
			score = scoreObj.GetScript<Score>();
			initialTextPos = textObj.Transform2D.Position;
		}

		// Start is called before the first frame update
		public override void Init()
		{
			if (text != null) fontSize = (float)text.FontSize;
			animModScale = fontSize * 0.7f;
		}

		public override void Update()
		{
			if (score == null)
			{
				Logger.Error("Score is null");
				return;
			}
			if (text == null)
			{
				Logger.Error("Text is null");
				return;
			}

			if (score.currCombo == 0)
			{
				if (score.currCombo != currCombo)
				{
					currCombo = score.currCombo;
					text.Text = "0";
				}
				return;
			}

			if (score.currCombo == currCombo && animTimer == 0f)
			{
				return;
			}

			if (score.currCombo != currCombo)
			{
				currCombo = score.currCombo;
				if (currCombo > 0) text.Text = currCombo.ToString();
				else text.Text = "";
			}

			// timer
			animTimer += Time.DeltaTimeF * animSpeed;

			// text animation
			// to animate feedback
			text.FontSize = (uint)(fontSize + (animModScale * UILibrary.EaseOutElastic(animTimer) - animModScale));
			textObj.Transform2D.Position = 
				new Vector2<float>(initialTextPos.X, 
				initialTextPos.Y + (animModTranslate * UILibrary.EaseOutElastic(animTimer) - animModTranslate));

			if (animTimer >= 1f)
			{
				animTimer = 0f;
			}
		}
	}
}
