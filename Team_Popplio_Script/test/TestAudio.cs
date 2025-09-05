/******************************************************************************/
/*!
\file   TestAudio.cs
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/04 08:55:22 PM (Last Edit)
\brief
        C# file to test Audio functions for Team_Popplio engine

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using System;
using System.Runtime.CompilerServices;

using Team_Popplio;
using Team_Popplio.ECS;
using Team_Popplio.Components;
using Team_Popplio.Libraries;

namespace BassNKick
{
	public class TestAudio : PopplioMonoScript
	{
		public override void Setup()
		{
			
		}

		public override void Init()
		{
			Logger.Info("TestAudio.cs: Entity Audio");

			Logger.Info(This.GetComponent<Team_Popplio.Components.Audio>().IsActive.ToString());
			Logger.Info(This.GetComponent<Team_Popplio.Components.Audio>().Name);
			Logger.Info(This.GetComponent<Team_Popplio.Components.Audio>().Volume.ToString());
			Logger.Info(This.GetComponent<Team_Popplio.Components.Audio>().BPM.ToString());
			Logger.Info(This.GetComponent<Team_Popplio.Components.Audio>().ChannelGroup.ToString());
			Logger.Info(This.GetComponent<Team_Popplio.Components.Audio>().Loop.ToString());
			Logger.Info(This.GetComponent<Team_Popplio.Components.Audio>().PlayOnAwake.ToString());
			Logger.Info(This.GetComponent<Team_Popplio.Components.Audio>().TimeSignature.ToString());

			Logger.Info("TestAudio.cs: Library Audio");

			This.GetComponent<Team_Popplio.Components.Audio>().Play();
			Logger.Info(This.GetComponent<Team_Popplio.Components.Audio>().IsPlaying.ToString());
			Logger.Info(This.GetComponent<Team_Popplio.Components.Audio>().Length.ToString());
			Logger.Info(This.GetComponent<Team_Popplio.Components.Audio>().CurrentTime.ToString());
		}

		public override void Update()
		{
			if (!This.GetComponent<Team_Popplio.Components.Audio>().GetSpectrumData(out float[] spectrum, 0, 1f)) return;

			Logger.Info(spectrum[0].ToString());
			This.Transform2D.Scale.Y = 1000f * spectrum[0];
		}

		public override void FixedUpdate()
		{

		}

		public override void End()
		{
			
		}
	}
}
