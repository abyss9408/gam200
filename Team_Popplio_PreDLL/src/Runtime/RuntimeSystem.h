/******************************************************************************/
/*!
\file   Runtime.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the header file for engine runtime library

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contentsv
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Mono/MonoAPI.h"

namespace Popplio
{
    class RuntimeSystem : public System
    {
    private:
        Registry& reg;
        EventBus& eventBus;
        PopplioScriptAPI::MonoAPI& monoAPI;
    public:
        RuntimeSystem(Registry& r, EventBus& eBus, PopplioScriptAPI::MonoAPI& mono);

        //void InstantiateEntity(int const& id);

        /*
        *   instantiate entity
        */
        int InstantiateEntity(int const& id);
    };
}