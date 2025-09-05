/******************************************************************************/
/*!
\file   Runtime.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the source file for engine runtime library

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contentsv
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>

#include "RuntimeSystem.h"

#include "../Cloning/CloneSystem.h"

namespace Popplio
{
    RuntimeSystem::RuntimeSystem(Registry& r, EventBus& eBus, PopplioScriptAPI::MonoAPI& mono)
        : reg(r), eventBus(eBus), monoAPI(mono) {}

    namespace
    {
        // l-value required
        static CloneEntityEvent cloneEvent = CloneEntityEvent(-1);
    }

    //void RuntimeSystem::InstantiateEntity(int const& id)
    //{
    //    eventBus.EmitEvent<CloneEntityEvent>(id);
    //}
    int RuntimeSystem::InstantiateEntity(int const& id)
    {
        return reg.GetSystem<CloneSystem>().CloneEntity(cloneEvent = CloneEntityEvent(id)); // TODO test
    }
}