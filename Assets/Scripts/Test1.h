/******************************************************************************/
/*!
\file   Test1.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
        This is the source file of a Logic System script
        Tests Logic System functionality

        Use this as a C++ test1 for game scripts

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

// Includes

#pragma once

// Logic System // DO NOT REMOVE //
#include "../src/Script/LogicSystem.h"
// Logic System // DO NOT REMOVE //

using namespace Popplio;

namespace BassNKick
{
    class Test1 : public ILogicScript // Script here (Similar to Unity Monobehaviour)
    {
        private:

        public:

        Test1(Entity* e) : ILogicScript(e) {}

        //void Deserialize(std::string const& s) override;
        //void Serialize() override;

        //Test1& GetScript() override;

        // On Initialize
        void Init(Entity const& obj) override;

        // On Update
        void Update(Entity const& obj) override;

        // On Terminate
        void End(Entity const& obj) override;

        void OnCollision(CollisionEvent& event) override;

        //void OnKeyPressed(KeyPressedEvent& event) override;

        //void OnKeyHeld(KeyRepeatedEvent& event) override;
        //void OnKeyReleased(KeyReleasedEvent& event) override;
    };
}