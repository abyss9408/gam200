/******************************************************************************/
/*!
\file   Audio.cpp
\author Team Popplio
\author Rio Shannon Yvon Leonardo
\contribution Shannon - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/30 (Last Edit)
\brief
    This is the source file for the audio system functionality
    for the game.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

// Includes
#include <pch.h>
#include "Audio.h" // header

#include "../src/Logging/Logger.h"
#include "../src/Audio/AudioSystem.h"

//REGISTER_SCRIPT(BassNKick, TemplateScript);

namespace BassNKick
{
    // Script here (Similar to Unity Monobehaviour)

    //std::string TemplateScript::ScriptName() { return "BassNKick::TemplateScript"; }

    void Audio::Deserialize(std::string const& s)
    {
		static_cast<void>(s);
    }

    void Audio::Serialize()
    {

    }

    Audio& Audio::GetScript()
    {
        return *this;
    }

    // On Initialize
    void Audio::Init(Entity const& obj)
    {
        if (!obj.HasComponent<Popplio::AudioComponent>()) return;

        //get audio system from registry
        //auto& audioSystem = obj.registry->GetSystem<Popplio::AudioSystem>();

        //get the audio component attached to the entity
        auto& audioComponent = obj.GetComponent<Popplio::AudioComponent>();

        //set the audio track
        audioComponent.data->SetID("track");

        //play the background track
        audioComponent.data->Play();

        isInitialzed = true;



        Logger::Debug("Audio is initializing!", LogDestination::IMGUI_CONSOLE);
    }

    // On Update
    void Audio::Update(Entity const& obj)
    {
        static_cast<void>(obj);
        Logger::Debug("Audio is updating!", LogDestination::IMGUI_CONSOLE);
    }

    // On Terminate
    void Audio::End(Entity const& obj)
    {

        //if (!isInitialzed) return;

        //stop audio when script ends
        //auto& audioSystem = obj->registry->GetSystem<Popplio::AudioSystem>();
        //auto& track = audioSystem.GetAudio("track");
        //track.Stop();


        static_cast<void>(obj);
        Logger::Debug("Audio is ending!", LogDestination::IMGUI_CONSOLE);
    }

    void Audio::OnCollision(CollisionEvent& event)
    {
        static_cast<void>(event);
        Logger::Debug("Audio is colliding!", LogDestination::IMGUI_CONSOLE);
    }
}