/******************************************************************************/
/*!
\file   Cutscene.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        C# file for cutscenes

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

namespace BassNKick
{
	public class  Cutscene : PopplioMonoScript
	{
		float cutsceneTimer = 1f;
		float cutSceneMod = 1.5f;

		float delay = 2f;

		public Entity openCutsceneObj = Entity.EntityField();
		Render2D? openCutscene;

		Vector2<float> openCutsceneInitialScale = new Vector2<float>(1f, 1f);

		public override void Setup()
		{
			if (openCutsceneObj == null) return;

			openCutscene = openCutsceneObj.GetComponent<Render2D>();
			openCutsceneInitialScale = openCutsceneObj.Transform2D.Scale;

			openCutscene.Color = new Color(1f, 1f, 1f, 0f);
		}

		public override void Update()
		{
			if (delay > 0f) delay -= Time.DeltaTimeF;
			else if (MainMenuUI.gameFirstRun) OpenCutscene();
		}

		void OpenCutscene()
		{
			if (openCutscene == null) return;

			if (cutsceneTimer > 0f) cutsceneTimer -= Time.DeltaTimeF;
			else if (cutsceneTimer < 0f) cutsceneTimer = 0f;

			openCutscene.Color = new Color(1f, 1f, 1f, 1f - cutsceneTimer);
			openCutsceneObj.Transform2D.Scale = openCutsceneInitialScale * (1f + (cutSceneMod - 1f) * cutsceneTimer);

			if (Lib.TempInputFix.IsKeyTriggered(Lib.Keybinds.press))
			{
				MainMenuUI.gameFirstRun = false;
				openCutsceneObj.Active = false;
			}
		}
	}
}
