#pragma once
#include "../EventBus/Event.h"

#ifndef IMGUI_DISABLE
namespace Popplio
{
    class EntitySelectedEvent : public Event
    {
    public:
        Entity selectedEntity;

        EntitySelectedEvent(Entity id) : selectedEntity(id) {}
    };

    class EntityDeselectedEvent : public Event
    {

    };
}

#endif // !#ifndef IMGUI_DISABLE


