/******************************************************************************/
/*!
\file   Player.h
\author Team Popplio
\author Rio Shannon Yvon Leonardo
\contribution Shannon - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/30 (Last Edit)
\brief
    This is the header file for the file Player.cpp

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

// Logic System // DO NOT REMOVE //
#include "../src/Script/LogicSystem.h"
// Logic System // DO NOT REMOVE //

using namespace Popplio;

namespace BassNKick
{
    class Player : public ILogicScript // Script here (Similar to Unity Monobehaviour)
    {
    private:
        //Entity const& m_entity = nullptr;

    public:

        Player(Entity* e) : ILogicScript(e) {}

        void Deserialize(std::string const& s) override;
        void Serialize() override;

        Player& GetScript() override;

        // On Initialize
        void Init(Entity const& obj) override;

        // On Update
        void Update(Entity const& obj) override;

        // On Terminate
        void End(Entity const& obj) override;

        void OnCollision(CollisionEvent& event) override;
    };
}