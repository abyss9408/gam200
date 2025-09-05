/******************************************************************************/
/*!
\file   AudioComponent.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the header file for engine Audio System component
        Attaches audio objects to game objects

        Enables ability to play audio files during game runtime via scripting

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "AudioSystem.h"

#include <memory>

namespace Popplio
{
    /*
    *   Component for attaching audio to entities at runtime
    */
    struct AudioComponent
    {
        bool isActive;

        std::string audio; // name

        // ++++++++++++++++ //

        // Audio info
        float volume;					
        bool mute;

        bool loop;						

        float bpm;						

        std::pair<int, int> timeSig; // first (top) = beats, second (bot) = sub notes

        bool playOnAwake;				

        int grp;

        // ++++++++++++++++ //

        int data;// do not serialize

        AudioComponent() : isActive(true), audio(""),
            volume(1.f), mute(false), loop(false), bpm(0.f), timeSig(), playOnAwake(false), grp(0), data(-1) {}
        /*
        *   AudioComponent argument constructor
        *	Properly Initializes Audio Component
        *	To only be used by ECS & Editor
        *
        *   @param s | string id of audio
        */
        AudioComponent(std::string const& s) : isActive(true), audio(s),
            volume(1.f), mute(false), loop(false), bpm(0.f), timeSig(), playOnAwake(false), grp(0), data(-1) {}
        AudioComponent(AudioComponent const& rhs) = default;
        AudioComponent& operator=(AudioComponent const& rhs) = default;
        //{
        //    isActive = rhs.isActive;
        //    audio = rhs.audio;
        //    volume = rhs.volume;
        //    loop = rhs.loop;
        //    bpm = rhs.bpm;
        //    timeSig = rhs.timeSig;
        //    playOnAwake = rhs.playOnAwake;
        //    grp = rhs.grp;
        //    data = rhs.data;
        //    return *this;
        //}
        ~AudioComponent() = default;

        void Update(AudioSystem& sys) // Component > Audio
        {
            try
            {
                if (audio != "" && data == -1)
                {
                    data = sys.InstAudio(audio);
                }
                else if (audio != "" && data != -1 && audio != sys.GetAudioInst(data).GetID())
                {
                    //if (data != nullptr)
                    //{
                    //    sys.RemoveAudioInst(data->GetIndex());
                    //    //audio.data.reset();
                    //}

                    data = sys.InstAudio(audio);
                }
                //else if (audio == "" && data != nullptr)
                //{
                //    sys.RemoveAudioInst(data->GetIndex());
                //}

                if (data != -1)
                {
                    Audio& a = sys.GetAudioInst(data);
                    if (a.GetPlaying())
                    {
                        sys.SetResult(a.UseChannel()->setVolume(volume));
                        sys.ErrorCheck();
                    }
                    a.SetVolume(volume);
                    a.SetMute(mute);
                    a.SetLoop(loop);
                    a.SetBPM(bpm);
                    a.SetTimeSig(timeSig);
                    a.playOnAwake = playOnAwake;
                    a.SetGroup(sys.GetCGroup(grp));
                }
            }
            catch (const std::exception& e)
            {
                std::stringstream ss{ "" };
                ss << "AudioComponent: " << e.what();
                Logger::Error(ss.str());
            }
        }

        void Sync(AudioSystem& sys) // Audio > Component
        {
            if (data != -1)
            {
                Audio& a = sys.GetAudioInst(data);

                audio = a.GetID();
                volume = a.GetVolume();
                mute = a.GetMute();
                loop = a.GetLoop();
                bpm = a.GetBPM();
                timeSig = a.GetTimeSig();
                playOnAwake = a.playOnAwake;
                grp = a.GetGroupByIndex();
            }
        }
    };
}