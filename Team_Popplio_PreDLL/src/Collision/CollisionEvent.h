/******************************************************************************/
/*!
\file   CollisionEvent.h
\author Team Popplio
\author Rio Shannon Yvon Leonardo - 100%
\par    Course : CSD2400/CSD2401
\par    Section : A
\date   2024/11/08
\brief
        This file defines a constructor that references to the components of the
    colliding entities.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include "../EventBus/Event.h"

#include "../Transformation/TransformComponent.h"
#include "../Physics/RigidBodyComponent.h"

namespace Popplio
{
    class CollisionEvent : public Event
    {
    public:
        enum EventID
        {
            ENTER = -1,
            STAY = 0,
            EXIT = 1
        };

        int entity1Id;
        int entity2Id;
        double collisionTime;

        EventID eventID;

        //TransformComponent& e1Transform, e2Transform;
        //RigidBodyComponent& e1Rigidbody, e2Rigidbody;

        CollisionEvent(int e1, int e2, double time, EventID id)
            : entity1Id(e1), entity2Id(e2), collisionTime(time), eventID(id) {}

        //CollisionEvent(TransformComponent& e1T, TransformComponent& e2T, RigidBodyComponent& e1R, RigidBodyComponent& e2R) :
        //    e1Transform(e1T), e2Transform(e2T), e1Rigidbody(e1R), e2Rigidbody(e2R) {}
    };

    class TriggerEvent : public Event
    {
    public:
        enum EventID
        {
            ENTER = -1,
            STAY = 0,
            EXIT = 1
        };

        int entity1Id;
        int entity2Id;
        double triggerTime;

        EventID eventID;

        TriggerEvent(int e1, int e2, double time, EventID id)
            : entity1Id(e1), entity2Id(e2), triggerTime(time), eventID(id) {}
    };
}