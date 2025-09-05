/******************************************************************************/
/*!
\file   Test.h
\author Team Popplio
\author Ng Jun Heng Shawn
\co-author  Bryan
\contribution Shawn - 40% | Bryan - 20%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
        This is the header file for engine tests

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "../src/Audio/AudioSystem.h"
#include "../src/EventBus/EventBus.h"
#include "../src/PrefabManagement/Prefab.h"
#include "../src/PrefabManagement/PrefabManager.h"
#include "../src/Utilities/Components.h"
#include "../src/Serialization/Serialization.h"

namespace PopplioTest
{
    // Shawn ----------------------------------------------- //
    void fmodGuide();
    void LoadAudio(Popplio::AudioSystem& audio);

    void InitTestAudio(Popplio::AudioSystem& audio);

    void TestAudio(Popplio::AudioSystem& audio);

    void InputLagTestInit(std::unique_ptr<Popplio::Registry>& reg, std::unique_ptr<Popplio::EventBus>& eventbus);
    //void InputLagTest(Popplio::KeyPressedEvent& event);

    class Test
    {
    public:
    };
    // Shawn ----------------------------------------------- //

    void TestPrefabManager(Popplio::PrefabManager& prefabManager, Popplio::Registry& registry);

    void TestSerialization(Popplio::Registry& registry, Popplio::Serializer& serializer);
}