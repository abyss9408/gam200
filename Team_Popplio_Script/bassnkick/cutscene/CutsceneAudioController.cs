/******************************************************************************/
/*!
\file   CutsceneAudioController.cs
\author Team Popplio
\author Val Tay Yun Ying
\contribution Val - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/04/06 09:18:00 AM (Last Edit)
\brief
        C# file for cutscene audio controller

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using BassNKick.Lib;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Team_Popplio.Extensions;
using Team_Popplio.Extern;

namespace BassNKick
{
    public class CutsceneAudioController : PopplioMonoScript
    {
        [Inspector("AudioObj")]
        public Entity[] AudioObj = { };
        private List<Audio> audios = new List<Audio>();

        [Inspector("0 - Band, 1 - Guitar, 2 - Drums")]          // 0 - Band, 1 - Guitar, 2 - Drums
        public string[] specialBGM = { };

        //private bool isSpecialTime = true;
        private Audio[] specialBGMAudios = new Audio[3];

        public float volumeTweenTime = 0.3f;
        public float softenedVol = 0.0f;
        public float softenedVolGuitar = 0.0f;
        public float softenedVolDrums = 0.0f;

        private float maxVol = 1.0f;

        public void PlayBackingTrack()
        {
            if (specialBGM == null) return;
			if (specialBGM.Length < 3) return;
			if (specialBGM[0] == null || specialBGM[1] == null || specialBGM[2] == null) return;

#pragma warning disable 
            //isSpecialTime = true;
            specialBGMAudios[0] = PlayAudio(specialBGM[0]);   // band
            specialBGMAudios[1] = PlayAudio(specialBGM[1]);   // guitar
            specialBGMAudios[2] = PlayAudio(specialBGM[2]);   // drums
#pragma warning restore
		}

		public void PlayGuitarLouder()
        {
            //isSpecialTime = true;

            specialBGMAudios[0].Volume = softenedVol;             // band
            specialBGMAudios[1].Volume = maxVol;                 // guitar
            specialBGMAudios[2].Volume = softenedVol;           // drums
        }

        public void PlayDrumsLouder()
        {
            //isSpecialTime = true;
            specialBGMAudios[0].Volume = softenedVol;             // band
            specialBGMAudios[1].Volume = softenedVol;                 // guitar
            specialBGMAudios[2].Volume = maxVol;           // drums
        }

        public void EqualSpecial()
        {
            //isSpecialTime = true;
            specialBGMAudios[0].Volume = maxVol;             // band
            specialBGMAudios[1].Volume = maxVol;                 // guitar
            specialBGMAudios[2].Volume = maxVol;           // drums
        }

        public Audio? PlayAudio(string name, bool isLooping = false)
        {
            Logger.Warning($"Playing Audio!{name}");
            foreach (var audio in audios)
            {
                if (audio.Name != name) continue;

                audio.Loop = isLooping;
                audio.Play();

                return audio;
            }

            return audios[0];
        }

        public void RefreshAudio()
        {
            foreach (var clip in audios)
            {
                if (clip.IsPlaying)
                    clip.Stop();
            }
            //isSpecialTime = false;
        }

        public override void Setup()
        {
            foreach (var obj in AudioObj)
            {
                audios.Add(obj.GetComponent<Audio>());
            }
        }
    }
}
