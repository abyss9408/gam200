/******************************************************************************/
/*!
\file   EntitySelectionInViewport.cpp
\author			Bryan
\co-author		Pearl
\contribution	Pearl - 40%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/29
\brief
    This source file implements editor functionality for entity manipulation. 
    It handles entity selection with rainbow borders, ImGuizmo-based transform controls, and viewport-based entity selection with layer management.
    Main operations include selecting, deleting, and transforming (translate/rotate/scale) entities in the viewport.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>

#include "Editor.h"
#include "EntitySelectionEvents.h"

#include <ImGuizmo/ImGuizmo.h>

namespace Popplio
{
    void Editor::RenderSelectedEntity()
    {
        if (!selectedEntity.has_value())
            return;

        const auto& transform = selectedEntity.value().GetComponent<TransformComponent>();

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        if (!drawList) return;

        // Get view and projection matrices
        PopplioMath::M3x3f viewMatrix = cameraManager.GetViewMatrix();
        PopplioMath::M3x3f projMatrix = cameraManager.GetProjectionMatrix();

        // Create vertices for the selection rectangle in local space
        PopplioMath::Vec2f halfSize = transform.scale * 0.5f;
        std::vector<PopplioMath::Vec2f> localCorners = {
            {-halfSize.x, -halfSize.y},
            { halfSize.x, -halfSize.y},
            { halfSize.x,  halfSize.y},
            {-halfSize.x,  halfSize.y}
        };

        // Transform corners to world space
        std::vector<ImVec2> screenCorners;
        float angle = static_cast<float>(transform.rotation * PopplioMath::M_PI / 180.0f);

        for (const auto& corner : localCorners) {
            // Rotate point around origin
            PopplioMath::Vec2f rotated = {
                corner.x * std::cos(angle) - corner.y * std::sin(angle),
                corner.x * std::sin(angle) + corner.y * std::cos(angle)
            };

            // Translate to world position
            PopplioMath::Vec2f worldPos = rotated + transform.position;

            // Transform to view space
            PopplioMath::Vec2f viewPos = viewMatrix * worldPos;

            // Transform to screen space
            PopplioMath::Vec2f projPos = projMatrix * viewPos;

            // Convert to screen coordinates
            float screenX = (projPos.x + 1.0f) * 0.5f * (viewportBounds[1].x - viewportBounds[0].x) + viewportBounds[0].x;
            float screenY = (1.0f - (projPos.y + 1.0f) * 0.5f) * (viewportBounds[1].y - viewportBounds[0].y) + viewportBounds[0].y;

            screenCorners.push_back(ImVec2(screenX, screenY));
        }

        // Draw rainbow borders
        rainbowTime += ImGui::GetIO().DeltaTime * RAINBOW_SPEED;

        for (size_t i = 0; i < screenCorners.size(); ++i) {
            float t = static_cast<float>(i) / screenCorners.size() + rainbowTime;
            ImVec4 color = GetRainbowColor(t);
            ImU32 col = ImGui::ColorConvertFloat4ToU32(color);

            const auto& p1 = screenCorners[i];
            const auto& p2 = screenCorners[(i + 1) % screenCorners.size()];
            drawList->AddLine(p1, p2, col, BORDER_THICKNESS);
        }
    }

    ImVec4 Editor::GetRainbowColor(float offset) const
    {
        float hue = fmodf(rainbowTime + offset, 1.0f);
        ImVec4 color;
        ImGui::ColorConvertHSVtoRGB(hue, 1.0f, 1.0f,
            color.x, color.y, color.z);
        color.w = 1.0f;
        return color;
    }

	void Editor::HandleEntitySelection()
	{
        if (isViewportHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            ImVec2 mousePos = ImGui::GetMousePos();

            PopplioMath::Vec2f worldPos = ScreenToWorldPosition(mousePos);

            int closestEntityId = FindClosestEntity(worldPos);

            if (closestEntityId != -1)
            {
                selectedEntity = reg.GetEntityById(closestEntityId);
				eventBus.EmitEvent<EntitySelectedEvent>(selectedEntity.value());
            }
            else
            {
                selectedEntity.reset();
                eventBus.EmitEvent<EntityDeselectedEvent>();
            }
        }
	}

    void Editor::HandleEntityDeletion()
    {
        if (selectedEntity.has_value() && ImGui::IsKeyPressed(ImGuiKey_Delete))
        {
            Entity entity = selectedEntity.value();

            // Collect all entities that will be destroyed (the entity and all its children)
            std::vector<Entity> entitiesToDestroy;
            if (entity.HasComponent<ParentComponent>())
            {
                // Recursive function to collect all children
                std::function<void(const Entity&)> collectChildren = [&](const Entity& e) {
                    if (e.HasComponent<ParentComponent>())
                    {
                        for (const auto child : e.GetComponent<ParentComponent>().children)
                        {
                            entitiesToDestroy.push_back(child);
                            collectChildren(child);
                        }
                    }
                };

                collectChildren(entity);
            }
            // Add the parent entity itself
            entitiesToDestroy.push_back(entity);

            // Remove all collected entities from their layers and then destroy them
            for (const auto& entityToDestroy : entitiesToDestroy)
            {
                layerManager.RemoveEntityFromCurrentLayer(entityToDestroy);

                if (lastNewEntity == entityToDestroy)
                    lastNewEntity.reset();
                if (latestNewEntity == entityToDestroy)
                    latestNewEntity.reset();

                reg.KillEntity(entityToDestroy);

                if (selectedEntity.has_value() && selectedEntity.value() == entityToDestroy)
                {
                    selectedEntity.reset();
                    eventBus.EmitEvent<EntityDeselectedEvent>();
                }
            }
        }
    }

    int Editor::FindClosestEntity(const PopplioMath::Vec2f& worldPos)
    {
		//float closestDistance = std::numeric_limits<float>::max();
        //int closestEntityId = -1;
        std::vector<std::pair<int, float>> intersectingEntities;

        // Get all entities sorted by layer to maintain proper selection order
        auto sortedEntities = layerManager.GetAllEntitiesSortedByLayer();

        for (const auto& entity : sortedEntities)
        {
            if (!entity.GetComponent<ActiveComponent>().isActive) continue;

            const auto& transform = entity.GetComponent<TransformComponent>();

            // If entity uses instanced rendering, make sure it has a render component
            bool isInstanced = false;
            if (entity.HasComponent<RenderComponent>())
            {
                const auto& render = entity.GetComponent<RenderComponent>();
                isInstanced = render.useInstancing;
            }

            // Calculate the bounds of the entity
            float halfWidth = transform.scale.x * 0.5f;
            float halfHeight = transform.scale.y * 0.5f;
			float left = transform.position.x - halfWidth;
			float right = transform.position.x + halfWidth;
            float top = transform.position.y + halfHeight;
            float bottom = transform.position.y - halfHeight;

            // Check if worldPos is within the bounds of the entity
            if (worldPos.x >= left && worldPos.x <= right &&
                worldPos.y >= bottom && worldPos.y <= top)
            {
                // Calculate distance to entity center
                float dx = worldPos.x - transform.position.x;
                float dy = worldPos.y - transform.position.y;
                float distance = std::sqrt(dx * dx + dy * dy);

                // Store intersecting entity and its distance
                // Only store non-instanced entities or if we explicitly want to select instanced ones
                if (!isInstanced)
                {
                    intersectingEntities.push_back({ entity.GetId(), distance });
                }
                
            }
        }

        // If we found intersecting entities, find the closest one
        if (!intersectingEntities.empty())
        {
            // Sort by distance
            std::sort(intersectingEntities.begin(), intersectingEntities.end(),
                [](const auto& a, const auto& b) {
                return a.second < b.second;
            });

            // Return the ID of the closest entity
            return intersectingEntities[0].first;
        }

        return -1; // No valid entity found
    }

    void Editor::HandleGuizmo()
    {
        if (currentTool != GuizmoTool::None)
        {
            ImGuizmo::SetOrthographic(true);
            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(viewportBounds[0].x, viewportBounds[0].y,
                viewportBounds[1].x - viewportBounds[0].x,
                viewportBounds[1].y - viewportBounds[0].y);

            auto& transform = selectedEntity->GetComponent<TransformComponent>();

            PopplioMath::M3x3f viewMat = cameraManager.GetEditorCamera().GetViewMatrix();
            PopplioMath::M3x3f projMat = cameraManager.GetEditorCamera().GetProjectionMatrix();

            float view[16] = 
            {
                viewMat[0][0], viewMat[1][0], 0, viewMat[2][0],
                viewMat[0][1], viewMat[1][1], 0, viewMat[2][1],
                0, 0, 1, 0,
                viewMat[0][2], viewMat[1][2], 0, viewMat[2][2]
            };


            float projection[16] = 
            {
                projMat[0][0], projMat[1][0], 0, projMat[2][0],
                projMat[1][0], projMat[1][1], 0, projMat[2][1],
                0, 0, 1, 0,
				projMat[0][2], projMat[1][2], 0, projMat[2][2]
            };

            // Create 4x4 model matrix
            float radians = static_cast<float>(transform.rotation * PopplioMath::M_PI / 180.0f);
            float model[16] = {
                transform.scale.x * cosf(radians),  transform.scale.x * -sinf(radians), 0, 0,
                transform.scale.y * sinf(radians),  transform.scale.y * cosf(radians),  0, 0,
                0,                                    0,                                     1, 0,
                transform.position.x,               transform.position.y,                0, 1
            };

            // Store the original position
            float deltaMatrix[16];
            ImGuizmo::Manipulate(
                view, 
                projection, 
                static_cast<ImGuizmo::OPERATION>(currentTool), 
                ImGuizmo::LOCAL,
                model, 
                deltaMatrix
            );

            static bool isUsing = false;
            static float translation[3], rotation[3], scale[3];
            static TransformComponent oldState;

            if (ImGuizmo::IsUsing())
            {
                if (!isUsing) oldState = transform; // to be used in change event

                ImGuizmo::DecomposeMatrixToComponents(model, translation, rotation, scale);
                transform.position.x = translation[0];
                transform.position.y = translation[1];

                float deltaRotation = atan2(deltaMatrix[4], deltaMatrix[0]) * 180.0f / PopplioMath::M_PI;

                // Apply the rotation
                transform.rotation -= deltaRotation;

                // Normalize the angle
                while (transform.rotation < 0) transform.rotation += 360.0f;
                while (transform.rotation >= 360) transform.rotation -= 360.0f;

               //transform.rotation = rotation[2];
                transform.scale.x = scale[0];
                transform.scale.y = scale[1];

                isUsing = true;
            }
            else
            {
                if (isUsing)
                {
                    // emit change event
                    eventBus.EmitEvent<SceneChangeEvent<std::any>>(*selectedEntity, transform, oldState, ChangeAction::MODIFY_COMPONENT); 
                    isUsing = false;
                }
            }
        }
    }

    //void Editor::ImGuizmoLogic()
    //{
    //    if (selectedEntity && ImGuizmoActive == 1)
    //    {
    //        ImGuizmo::SetDrawlist();

    //        auto rectMin = ImGui::GetItemRectMin();
    //        auto rectMax = ImGui::GetItemRectMax();

    //        ImGuizmo::SetRect(
    //            rectMin.x, rectMin.y, rectMax.x - rectMin.x, rectMax.y - rectMin.y
    //        );

    //        // Show matrix values
    //        auto& transform = selectedEntity.value().GetComponent<TransformComponent>();
    //        EditorCamera& editorCamera = cameraManager.GetEditorCamera();

    //        PopplioMath::Vec2f cameraPos = editorCamera.GetPosition();
    //        float zoom = editorCamera.GetZoom();

    //        // Create view matrix that accounts for camera position and zoom
    //        PopplioMath::M3x3f viewMat = editorCamera.GetViewMatrix();
    //        float view[16] = {
    //            viewMat[0][0], viewMat[0][1], 0, viewMat[0][2],
    //            viewMat[1][0], viewMat[1][1], 0, viewMat[1][2],
    //            0,            0,            1, 0,
    //            viewMat[2][0], viewMat[2][1], 0, 1
    //        };

    //        // Create projection matrix that accounts for zoom
    //        PopplioMath::M3x3f projMat = editorCamera.GetProjectionMatrix();
    //        float projection[16] = {
    //            projMat[0][0], projMat[0][1], 0, projMat[0][2],
    //            projMat[1][0], projMat[1][1], 0, projMat[1][2],
    //            0,            0,            1, 0,
    //            projMat[2][0], projMat[2][1], 0, 1
    //        };

    //        float rotationRad = static_cast<float>(transform.rotation * PopplioMath::M_PI / 180.0f);
    //        float cosRot = cos(rotationRad);
    //        float sinRot = sin(rotationRad);

    //        // Create model matrix that properly combines scale, rotation, and position
    //        float model[16] = {
    //            transform.scale.X() * cosRot, -transform.scale.X() * sinRot, 0, 0,
    //            transform.scale.Y() * sinRot,  transform.scale.Y() * cosRot, 0, 0,
    //            0,                             0,                            1, 0,
    //            transform.position.X(),        transform.position.Y(),       0, 1
    //        };

    //        // Store the original position
    //        float deltaMatrix[16];
    //        float originalPosition[3] = { transform.position.X(), transform.position.Y(), 0 };

    //        if (ImGuizmo::Manipulate(
    //            view,
    //            projection,
    //            currentGizmoOperation,
    //            ImGuizmo::LOCAL,
    //            model,
    //            deltaMatrix,
    //            nullptr
    //        ))
    //        {
    //            switch (currentGizmoOperation)
    //            {
    //            case ImGuizmo::TRANSLATE:
    //                transform.position.X() = model[12];
    //                transform.position.Y() = model[13];
    //                break;

    //            case ImGuizmo::ROTATE:
    //            {
    //                float deltaRotation = atan2(deltaMatrix[4], deltaMatrix[0]) * 180.0f / PopplioMath::M_PI;

    //                // Apply the rotation
    //                transform.rotation += deltaRotation;

    //                // Normalize the angle
    //                while (transform.rotation < 0) transform.rotation += 360.0f;
    //                while (transform.rotation >= 360) transform.rotation -= 360.0f;
    //                break;
    //            }
    //            case ImGuizmo::SCALE:
    //                // Extract scale while preserving sign
    //                transform.scale.X() = sqrt(model[0] * model[0] + model[1] * model[1]);
    //                transform.scale.Y() = sqrt(model[4] * model[4] + model[5] * model[5]);

    //                // Preserve original scale sign
    //                if (model[0] < 0) transform.scale.X() *= -1;
    //                if (model[5] < 0) transform.scale.Y() *= -1;
    //                break;
    //            }
    //        }

    //    }
    //}
}