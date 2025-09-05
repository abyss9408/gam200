/******************************************************************************/
/*!
\file   Background.h
\author Team Popplio
\author Rio Shannon Yvon Leonardo
\contribution Shannon - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/30 (Last Edit)
\brief


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
    class Background : public ILogicScript // Script here (Similar to Unity Monobehaviour)
    {
    private:
        const float MOVE_SPEED = 50.0f;     // Pixels per second
        const float BACKGROUND_WIDTH = 2007.0f;  // Width from start of BG1 to start of BG2 (1880 + 127)
        const float INITIAL_X1 = -127.0f;    // Initial X for Background-Between
        const float INITIAL_X2 = 1880.0f;    // Initial X for Background-Between2
        const float RESET_Y = -99.0f;        // Y position remains constant

    public:

        Background(Entity* e) : ILogicScript(e) {}

        void Deserialize(std::string const& s) override;
        void Serialize() override;

        Background& GetScript() override;

        // On Initialize
        void Init(Entity const& obj) override;

        // On Update
        void Update(Entity const& obj) override;

        // On Terminate
        void End(Entity const& obj) override;

        void OnCollision(CollisionEvent& event) override;
    };
}