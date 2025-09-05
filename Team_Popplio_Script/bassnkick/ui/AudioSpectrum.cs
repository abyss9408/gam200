/******************************************************************************/
/*!
\file   AudioSpectrum.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        BassNKick | C# file for audio spectrum ui

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BassNKick
{
	public class AudioSpectrum : PopplioMonoScript
	{
		[Inspector("Audio Manager Obj")]
		public Entity audioManagerObj = Entity.EntityField();
		AudioManager? audioManager;

		public Entity[] bars = { Entity.EntityField() };

		public float magMultiplier = 20f;

		Vector2<float>? initialPos;

		public override void Setup()
		{
			audioManager = audioManagerObj.GetScript<AudioManager>();
			initialPos = This.Transform2D.Position;
			This.Active = false;
		}

		public override void Update()
		{
			if (audioManager == null) return;
			if (initialPos == null) return;

			if (!audioManager.musicObj[0].GetComponent<Audio>().IsPlaying) return;

			// Get audio spectrum data
			float[] spectrumData = audioManager.GetSpectrumData(1);

			
			if (bars.Length != spectrumData.Length)
			{
				Logger.Error("AudioSpectrum: Number of bars does not match spectrum data length");
				return;
			}

			// Update bar transform
			for (int i = 0; i < bars.Length / 2; ++i)
			{
				bars[2 * i].Transform2D.Position = 
					new Vector2<float>(bars[2 * i].Transform2D.Position.X, 
					initialPos.Y + magMultiplier * spectrumData[i]);
				if (1 < bars.Length / 2 - 1)
				{
					bars[2 * i + 1].Transform2D.Position =
						new Vector2<float>(bars[2 * i + 1].Transform2D.Position.X,
						initialPos.Y + magMultiplier * (spectrumData[i] - (spectrumData[i] - spectrumData[i + 1]) / 2f));
				}
				else
				{
					bars[2 * i + 1].Transform2D.Position =
						new Vector2<float>(bars[2 * i + 1].Transform2D.Position.X,
						initialPos.Y + magMultiplier * (spectrumData[i] / 2f));
				}
			}
		}
	}
}
