/******************************************************************************/
/*!
\file   ResultsCodeB.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/04/06 08:55:22 PM (Last Edit)
\brief
        BassNKick | C# file for animating Code B in the Results

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

namespace BassNKick
{
	public class ResultsCodeB : PopplioMonoScript
	{
		public Entity eyeL = Entity.EntityField();
		public Entity eyeR = Entity.EntityField();
		public Entity mouth = Entity.EntityField();

		Vector2<float> eyeLInit = new Vector2<float>();
		Vector2<float> eyeRInit = new Vector2<float>();
		Vector2<float> mouthInit = new Vector2<float>();

		Vector2<float> eyeLVel = new Vector2<float>();
		Vector2<float> eyeRVel = new Vector2<float>();
		Vector2<float> mouthVel = new Vector2<float>();

		const float radius = 15f;
		const float speed = 10f;

		public override void Setup()
		{
			eyeLInit = eyeL.Transform2D.Position;
			eyeRInit = eyeR.Transform2D.Position;
			mouthInit = mouth.Transform2D.Position;

			eyeLVel = new Vector2<float>((float)Math.Cos((double)Rand.Gen(0f, 2f * (float)Math.PI)),
				(float)Math.Sin((double)Rand.Gen(0f, 2f * (float)Math.PI)));
			eyeRVel = new Vector2<float>((float)Math.Cos((double)Rand.Gen(0f, 2f * (float)Math.PI)),
				(float)Math.Sin((double)Rand.Gen(0f, 2f * (float)Math.PI)));
			mouthVel = new Vector2<float>((float)Math.Cos((double)Rand.Gen(0f, 2f * (float)Math.PI)),
				(float)Math.Sin((double)Rand.Gen(0f, 2f * (float)Math.PI)));
		}

		public override void Update()
		{
			Vector2<float> temp = eyeL.Transform2D.Position;
			while (Mathematics.Distance(temp, eyeLInit) > radius)
			{
				eyeLVel = new Vector2<float>((float)Math.Cos((double)Rand.Gen(0f, 2f * (float)Math.PI)),
					(float)Math.Sin((double)Rand.Gen(0f, 2f * (float)Math.PI)));
				temp = eyeL.Transform2D.Position + eyeLVel;
			}
			temp = eyeR.Transform2D.Position;
			while (Mathematics.Distance(temp, eyeRInit) > radius)
			{
				eyeRVel = new Vector2<float>((float)Math.Cos((double)Rand.Gen(0f, 2f * (float)Math.PI)),
					(float)Math.Sin((double)Rand.Gen(0f, 2f * (float)Math.PI)));
				temp = eyeR.Transform2D.Position + eyeRVel;
			}
			temp = mouth.Transform2D.Position;
			while (Mathematics.Distance(temp, mouthInit) > radius)
			{
				mouthVel = new Vector2<float>((float)Math.Cos((double)Rand.Gen(0f, 2f * (float)Math.PI)),
					(float)Math.Sin((double)Rand.Gen(0f, 2f * (float)Math.PI)));
				temp = mouth.Transform2D.Position + mouthVel;
			}
			eyeL.Transform2D.Position += eyeLVel * speed * Time.DeltaTimeF;
			eyeR.Transform2D.Position += eyeRVel * speed * Time.DeltaTimeF;
			mouth.Transform2D.Position += mouthVel * speed * Time.DeltaTimeF;
		}
	}
}
