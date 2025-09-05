/******************************************************************************/
/*!
\file   PerformanceTesting.cpp
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/05
\brief
    This file tests the reliability and performance of the editor & engine
    These include:
    - Stress test via many entities
    - 

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>
#include "Editor.h"

#include <random>

namespace Popplio
{
    void Editor::AddManyInstancingEntities()
    {
        // Create a random number generator
        std::random_device rd;  // Used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()

        // Create distributions for our random values
        std::uniform_real_distribution<float> positionDist(-500.f, 500.f);  // Random position between -500 and 500
        std::uniform_real_distribution<float> scaleDist(50.f, 100.f);     // Random scale between 50 and 100

        // Ensure "Game" layer exists
        if (~layerManager.GetLayerOrder("Game") >= 0)
        {
            layerManager.AddLayer("Game", true);
        }

        for (int i = 0; i < 2500; ++i)
        {
            Entity entity = reg.CreateEntity();

            // Add TransformComponent
            float x = positionDist(gen);
            float y = positionDist(gen);
            float scale = scaleDist(gen);
            entity.AddComponent<ActiveComponent>();
            entity.AddComponent<TransformComponent>(x, y, scale, scale);

            // Add RenderComponent with instancing enabled
            entity.AddComponent<RenderComponent>(MeshType::Quad, "instanced_shader", "main-character", true);

            entity.AddComponent<RigidBodyComponent>(1.f, true, 1.f, 1.f);

            // Add entity to "Game" layer
            layerManager.AddEntityToLayer(entity, "Game");

            // Group the entity for easy removal later
            entity.Group("instanced_entity");
        }

        Logger::Info("Added 2500 instanced entities");
    }

    //void Editor::ClearInstancingEntities()
    //{
    //    std::vector<Entity> entitiesToRemove;

    //    for (const auto& entity : reg.GetAllEntities())
    //    {
    //        if (entity.BelongsToGroup("instanced_entity"))
    //        {
    //            entitiesToRemove.push_back(entity);
    //        }
    //    }

    //    for (const auto& entity : entitiesToRemove)
    //    {
    //        reg.KillEntity(entity);
    //    }

    //    Logger::Info("Cleared " + std::to_string(entitiesToRemove.size()) + " instanced entities");
    //}
}