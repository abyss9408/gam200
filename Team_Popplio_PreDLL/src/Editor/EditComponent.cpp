/******************************************************************************/
/*!
\file       EditComponent.cpp
\author     Team Popplio
\author     Bryan Ang Wei Ze
\co-author  Hafawati Binte Mohd Ali | Ng Jun Heng Shawn
\contribution Bryan - 47.5%, Hafawati - 15%, Shawn - 37.5%
\par    Course : CSD2401 / CSD2450 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 10:01:45 AM (Last Edit)
\brief
    This source file defines various methods of the Editor class within the Popplio 
    namespace. These methods are responsible for adding entities and components to the 
    editor,  as well as providing UI elements to edit different types of components 
    using the ImGui library.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include "Editor.h"

#include "../Audio/AudioSystem.h" // temp for audiocomponent section // todo: remove after assetstore is in
#include "../Input/InputSystem.h" // detect mouse
#include "../Hierarchy/HierarchySystem.h" // for parent component
#include "../UI/FunctionRegistry.h"  
#include "../UI/UISystem.h"
#include "../Particles/ParticleSystem.h"

#include "../Mono/MonoUtilities.h"

#include "../PerformanceViewer/PerformanceViewer.h"

#include <mono/metadata/attrdefs.h>

namespace Popplio
{
    void Editor::AddEntityPopup()
    {
        if (ImGui::BeginPopup("Add Entity"))
        {
            if (ImGui::MenuItem("Empty Entity"))
            {
                latestNewEntity = reg.CreateEntity();
            }
            // Add more entity types as needed

            if (ImGui::Button("Add Slider"))
            {
                CreateSlider();  // Handles creating both track & handle
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }

    void Editor::CreateSlider()
    {
        // Create Track
        Entity track = reg.CreateEntity();
        track.AddComponent<ActiveComponent>(true);
        track.AddComponent<TransformComponent>(0.0f, 0.0f, 150.0f, 20.0f, 0.0f); // Default size
        track.AddComponent<UIComponent>(UIType::SLIDER_TRACK);
        //track.Tag("Slider_Track");

        // Create Handle
        Entity handle = reg.CreateEntity();
        handle.AddComponent<ActiveComponent>(true);
        handle.AddComponent<TransformComponent>(0.0f, 0.0f, 15.0f, 20.0f, 0.0f); // Smaller than track
        handle.AddComponent<UIComponent>(UIType::SLIDER_HANDLE);
        //handle.Tag("Slider_Handle");

        // Link Handle to Track
        auto& handleUI = handle.GetComponent<UIComponent>();
        handleUI.linkedEntityId = track.GetId();  // Handle knows its track

        auto& trackUI = track.GetComponent<UIComponent>();
        trackUI.linkedEntityId = handle.GetId();  // Track knows its handle

        // Position Handle Centered on Track
        auto& trackTransform = track.GetComponent<TransformComponent>();
        auto& handleTransform = handle.GetComponent<TransformComponent>();
        handleTransform.position.x = trackTransform.position.x;  // Center it
        handleTransform.position.y = trackTransform.position.y;
    }
    static bool isNineSliceEditorOpen = true; // Track if the window is open

    void Editor::ShowNineSliceEditor(RenderComponent& renderComp)
    {
        if (!isNineSliceEditorOpen) return; // Prevent redundant calls

        static bool isDragging = false;
        static int selectedBorder = -1;

        // Set constraints to prevent excessive resizing behavior
        ImGui::SetNextWindowSizeConstraints(ImVec2(300, 300), ImVec2(800, 800));

        if (ImGui::Begin("9-Slice Editor", &isNineSliceEditorOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
        {
            ImGui::Text("Editing Texture: %s", renderComp.textureName.c_str());
            ImGui::Text("Size: %d x %d", renderComp.textureWidth, renderComp.textureHeight);

            // Draw the preview
            DrawNineSlicePreview(renderComp);
        }

        ImGui::End();
    }

    void Editor::DrawNineSlicePreview(RenderComponent& renderComp)
    {
        static GLuint cachedTextureID = 0;
        static ImVec2 cachedImageSize = ImVec2(0, 0);

        // Load texture only if it has changed
        if (cachedTextureID != AssetStore::GetTexture(renderComp.textureName))
        {
            cachedTextureID = AssetStore::GetTexture(renderComp.textureName);
            cachedImageSize = ImVec2(static_cast<float>(renderComp.textureWidth), static_cast<float>(renderComp.textureHeight));
        }

        if (cachedTextureID == 0) return;

        // Scale preview properly without flickering
        ImVec2 windowSize = ImGui::GetContentRegionAvail();
        float scaleFactor = std::min(windowSize.x / cachedImageSize.x, windowSize.y / cachedImageSize.y);
        ImVec2 imageSize = ImVec2(cachedImageSize.x * scaleFactor, cachedImageSize.y * scaleFactor);

        // Render Image with correct UVs
        ImGui::Image((void*)(intptr_t)cachedTextureID, imageSize, ImVec2(0, 1), ImVec2(1, 0));

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetItemRectMin();
        ImVec2 size = ImGui::GetItemRectSize();

        // Y-coordinate mapping for top and bottom borders
        float left = pos.x + (renderComp.borderLeft / renderComp.textureWidth) * size.x;
        float right = pos.x + (1.0f - renderComp.borderRight / renderComp.textureWidth) * size.x;
        float top = pos.y + (renderComp.borderTop / renderComp.textureHeight) * size.y;
        float bottom = pos.y + (1.0f - renderComp.borderBottom / renderComp.textureHeight) * size.y;


        // Draw border lines
        drawList->AddLine(ImVec2(left, pos.y), ImVec2(left, pos.y + size.y), IM_COL32(255, 0, 0, 255), 2.0f);
        drawList->AddLine(ImVec2(right, pos.y), ImVec2(right, pos.y + size.y), IM_COL32(255, 0, 0, 255), 2.0f);
        drawList->AddLine(ImVec2(pos.x, top), ImVec2(pos.x + size.x, top), IM_COL32(255, 0, 0, 255), 2.0f);
        drawList->AddLine(ImVec2(pos.x, bottom), ImVec2(pos.x + size.x, bottom), IM_COL32(255, 0, 0, 255), 2.0f);

        // Ensure values stay within the valid range
        renderComp.borderLeft = std::clamp(renderComp.borderLeft, 0.0f, renderComp.textureWidth / 2.0f);
        renderComp.borderRight = std::clamp(renderComp.borderRight, 0.0f, renderComp.textureWidth / 2.0f);
        renderComp.borderTop = std::clamp(renderComp.borderTop, 0.0f, renderComp.textureHeight / 2.0f);
        renderComp.borderBottom = std::clamp(renderComp.borderBottom, 0.0f, renderComp.textureHeight / 2.0f);
        ////  Adjust border dragging logic for correct Y-axis behavior
        //if (ImGui::IsMouseDragging(0))
        //{
        //    ImVec2 mouse = ImGui::GetMousePos();

        //    if (abs(mouse.x - left) < 5.0f) { renderComp.borderLeft += ImGui::GetIO().MouseDelta.x; }
        //    if (abs(mouse.x - right) < 5.0f) { renderComp.borderRight -= ImGui::GetIO().MouseDelta.x; }

        //    // Adjust the top border, should move it up
        //    if (abs(mouse.y - top) < 5.0f) { renderComp.borderTop += ImGui::GetIO().MouseDelta.y; }
        //    if (abs(mouse.y - bottom) < 5.0f) { renderComp.borderBottom -= ImGui::GetIO().MouseDelta.y; }

        //    // Ensure values stay within the valid range
        //    renderComp.borderLeft = std::clamp(renderComp.borderLeft, 0.0f, renderComp.textureWidth / 2.0f);
        //    renderComp.borderRight = std::clamp(renderComp.borderRight, 0.0f, renderComp.textureWidth / 2.0f);
        //    renderComp.borderTop = std::clamp(renderComp.borderTop, 0.0f, renderComp.textureHeight / 2.0f);
        //    renderComp.borderBottom = std::clamp(renderComp.borderBottom, 0.0f, renderComp.textureHeight / 2.0f);
        //}
    }


    void Editor::RenderEntityInfoSection(Entity& entity)
    {
        // change detection
        static bool isChanged = false;

        static std::any newValue;
        static std::any oldValue;
        static std::string type = "";

        bool isInputting = false;

        ImGui::Text("Info");

        if (ImGui::IsItemHovered())
        {
            std::string str = "Cloned from: " + std::to_string(entity.GetClonedFrom().GetId());
            ImGui::BeginTooltip();
            ImGui::Text(str.c_str());
            ImGui::EndTooltip();
        }

        if (ImGui::CollapsingHeader("Tags & Groups", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // Tag editing
            static char tagBuffer[256] = "";
            strcpy_s(tagBuffer, sizeof(tagBuffer), "");
            bool hasTag = reg.EntityHasAnyTag(entity);

            if (hasTag)
            {
                std::string currentTag = reg.GetEntityTag(entity);
                strcpy_s(tagBuffer, sizeof(tagBuffer), currentTag.c_str());
            }

            ImGui::Text("Tag:");
            ImGui::SameLine();

            if (ImGui::InputText("##TagInput", tagBuffer, sizeof(tagBuffer),
                ImGuiInputTextFlags_EnterReturnsTrue))
            {
                std::string newTag(tagBuffer);

                if (!newTag.empty())
                {
                    if (!isChanged)
                    {
                        oldValue = reg.GetEntityTag(entity);
                        type = typeid(std::string).name();
                        type += "0";
                        isChanged = true;
                    }

                    // Remove any existing tag first
                    if (hasTag) {
                        reg.RemoveEntityTag(entity);
                    }
                    // Add new tag
                    entity.Tag(newTag);
                    Logger::Debug("Updated tag for Entity " +
                        std::to_string(entity.GetId()) + " to: " + newTag);

                    newValue = newTag;
                }
            }
            if (ImGui::IsItemActive()) isInputting = true;

            if (hasTag && ImGui::Button("Remove Tag"))
            {
                if (!isChanged)
                {
                    oldValue = reg.GetEntityTag(entity);
                    type = typeid(std::string).name();
                    type += "0";
                    isChanged = true;
                }

                reg.RemoveEntityTag(entity);
                memset(tagBuffer, 0, sizeof(tagBuffer));
                Logger::Debug("Removed tag from Entity " +
                    std::to_string(entity.GetId()));

                newValue = static_cast<std::string>("");
            }

            // Group editing
            static char groupBuffer[256] = "";
            strcpy_s(groupBuffer, sizeof(groupBuffer), "");
            bool hasGroup = reg.EntityBelongsToAnyGroup(entity);

            ImGui::Separator();

            if (hasGroup) {
                std::string currentGroup = reg.GetEntityGroup(entity);
                strcpy_s(groupBuffer, sizeof(groupBuffer), currentGroup.c_str());
            }

            ImGui::Text("Group:");
            ImGui::SameLine();

            if (ImGui::InputText("##GroupInput", groupBuffer, sizeof(groupBuffer),
                ImGuiInputTextFlags_EnterReturnsTrue))
            {
                std::string newGroup(groupBuffer);

                if (!newGroup.empty()) {
                    if (!isChanged)
                    {
                        oldValue = reg.GetEntityGroup(entity);
                        type = typeid(std::string).name();
                        type += "1";
                        isChanged = true;
                    }

                    // Remove from existing group first
                    if (hasGroup) {
                        reg.RemoveEntityGroup(entity);
                    }
                    // Add to new group
                    entity.Group(newGroup);
                    Logger::Debug("Updated group for Entity " +
                        std::to_string(entity.GetId()) + " to: " + newGroup);

                    newValue = newGroup;
                }
            }
            if (ImGui::IsItemActive()) isInputting = true;

            if (hasGroup && ImGui::Button("Remove Group")) {

                if (!isChanged)
                {
                    oldValue = reg.GetEntityGroup(entity);
                    type = typeid(std::string).name();
                    type += "1";
                    isChanged = true;
                }

                reg.RemoveEntityGroup(entity);
                memset(groupBuffer, 0, sizeof(groupBuffer));
                Logger::Debug("Removed group from Entity " +
                    std::to_string(entity.GetId()));

                newValue = static_cast<std::string>("");
            }

            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Remove entity from its current group");
            }
        }

        // emit change event
        if (isChanged && !isInputting && !InputSystem::IsMouseButtonDown(InputSystem::MouseButton::Left)) // change detection
        {
            eventBus.EmitEvent<SceneChangeEvent<std::any>>(*selectedEntity, newValue, oldValue, ChangeAction::MODIFY_ENTITY, type);
            isChanged = false;
        }
    }

    void Editor::AddComponentPopup(Entity entity)
    {
        if (ImGui::BeginPopup("Add Component"))
        {
            AddComponentOption<ActiveComponent>(entity, "Active");
            AddComponentOption<TransformComponent>(entity, "Transform");
            AddComponentOption<RenderComponent>(entity, "Render");
            AddComponentOption<RigidBodyComponent>(entity, "Rigid Body");
            AddCustomComponentOption<BoxColliderComponent>(entity, "Custom Box Collider");
            AddComponentOption<BoxColliderComponent>(entity, "Box Collider");
            AddComponentOption<TextComponent>(entity, "Text");
            AddComponentOption<LogicComponent>(entity, "Script");
            AddComponentOption<AudioComponent>(entity, "Audio");
            AddComponentOption<AnimationComponent>(entity, "Animation");
            AddComponentOption<ParentComponent>(entity, "Parent/Child");
            AddComponentOption<PersistOnLoadComponent>(entity, "Persist On Load");
            AddComponentOption<UIComponent>(entity, "UI");
            AddComponentOption<ParticleComponent>(entity, "Particles");
            // Add more component types as needed
            // CircleColliderComponent is missing
            ImGui::EndPopup();
        }
    }

    template <>
    void Editor::EditComponent(ActiveComponent& active)
    {
        // change detection
        static bool isChanged = false;
        static ActiveComponent oldState;

        bool a{active.isActive};
        if (ImGui::Checkbox("Is Active", &a))
        {
            if (!isChanged)
            {
                oldState = active;
                isChanged = true;
            }
            active.isActive = a;
        }

        if (isChanged) // change detection
        {
            eventBus.EmitEvent<SceneChangeEvent<std::any>>(*selectedEntity, active, oldState, ChangeAction::MODIFY_COMPONENT);
            isChanged = false;
        }
    }

    template <>
    void Editor::EditComponent(TransformComponent& transform)
    {
        // change detection
        static bool isChanged = false;
        static TransformComponent oldState;

        float temp[2] = { transform.position.x, transform.position.y };
        if (ImGui::DragFloat2("Position", temp))
        {
            if (!isChanged)
            {
                oldState = transform;
                isChanged = true;
            }
            transform.position.x = temp[0];
            transform.position.y = temp[1];
        }
        temp[0] = transform.scale.x;
        temp[1] = transform.scale.y;
        if (ImGui::DragFloat2("Scale", temp))
        {
            if (!isChanged)
            {
                oldState = transform;
                isChanged = true;
            }

            transform.scale.x = temp[0];
            transform.scale.y = temp[1];
        }

        // Handle rotation (already in degrees)
        temp[0] = static_cast<float>(transform.rotation);
        if (ImGui::DragFloat("Rotation", temp, 1.0f))
        {
            if (!isChanged)
            {
                oldState = transform;
                isChanged = true;
            }
            transform.rotation = temp[0];
        }

        const char* anchorLabels[] = {
            "TopLeft", "TopCenter", "TopRight",
            "MiddleLeft", "MiddleCenter", "MiddleRight",
            "BottomLeft", "BottomCenter", "BottomRight"
        };

        int currentAnchor = static_cast<int>(transform.anchor);
        if (ImGui::Combo("Anchor Point", &currentAnchor, anchorLabels, IM_ARRAYSIZE(anchorLabels)))
        {
            if (!isChanged)
            {
                oldState = transform;
                isChanged = true;
            }
            transform.anchor = static_cast<Anchor>(currentAnchor);
        }
        
        // emit change event
        if (isChanged && !InputSystem::IsMouseButtonDown(InputSystem::MouseButton::Left) && !ImGui::GetIO().WantTextInput) // change detection
        {
            eventBus.EmitEvent<SceneChangeEvent<std::any>>(*selectedEntity, transform, oldState, ChangeAction::MODIFY_COMPONENT);
            isChanged = false;
        }
    }

    template <>
    void Editor::EditComponent(RenderComponent& render)
    {
        // change detection
        static bool isChanged = false;
        static RenderComponent oldState;

        bool a{ render.isActive };
        if (ImGui::Checkbox("Active", &a))
        {
            if (!isChanged)
            {
                oldState = render;
                isChanged = true;
            }
            render.isActive = a;
        }

        const char* meshTypes[] = { "Triangle", "Quad", "Circle", "FullscreenQuad" };
        int currentMeshType = static_cast<int>(render.meshType);

        if (ImGui::Combo("Mesh Type", &currentMeshType, meshTypes, IM_ARRAYSIZE(meshTypes)))
        {
            if (!isChanged) oldState = render;
            render.meshType = static_cast<MeshType>(currentMeshType);
            if (!isChanged) isChanged = true;
        }

        // Shader selection
        ImGui::BeginGroup();
        ImGui::Text("Shader:");
        ImGui::SameLine();

		// When instancing is enabled, only instanced_shader is available
        bool temp = render.useInstancing;
        if (ImGui::Checkbox("Use Instancing", &temp))
        {
            if (!isChanged)
            {
                oldState = render;
                isChanged = true;
            }

            render.useInstancing = temp;

            if (render.useInstancing)
            {
                render.shaderName = "instanced_shader";
            }
            else
            {
                render.shaderName = "default_shader";
            }
        }

        if (render.useInstancing)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
            ImGui::Text("instanced_shader");
            ImGui::PopStyleVar();
        }
        else
        {
            if (ImGui::BeginCombo("##Shader", render.shaderName.c_str()))
            {
                for (const auto& [shaderName, shader] : AssetStore::GetAllShaders())
                {
                    if (shaderName != "instanced_shader")
                    {
                        bool isSelected = (render.shaderName == shaderName);
                        if (ImGui::Selectable(shaderName.c_str(), isSelected))
                        {
                            if (!isChanged) oldState = render;
                            render.shaderName = shaderName;
                            if (!isChanged) isChanged = true;
                        }
                        if (isSelected)
                        {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                }
                ImGui::EndCombo();
            }
        }
        ImGui::EndGroup();

        // Texture selection with search functionality
        if (ImGui::BeginCombo("Texture", render.textureName.c_str()))
        {
            // Add a search box at the top of the dropdown
            static char searchBuffer[128] = "";
            ImGui::PushItemWidth(-1);
            ImGui::InputText("##TextureSearch", searchBuffer, IM_ARRAYSIZE(searchBuffer));
            ImGui::Separator();

            // Filter textures based on search query
            std::string searchQuery = searchBuffer;
            std::transform(searchQuery.begin(), searchQuery.end(), searchQuery.begin(),
                [](unsigned char c) { return std::tolower(c); });

            // Display filtered textures
            for (const auto& [textureName, textureID] : AssetStore::GetAllTextures())
            {
                // Skip if doesn't match search
                std::string lowerTextureName = textureName;
                std::transform(lowerTextureName.begin(), lowerTextureName.end(), lowerTextureName.begin(),
                    [](unsigned char c) { return std::tolower(c); });

                if (!searchQuery.empty() && lowerTextureName.find(searchQuery) == std::string::npos)
                    continue;

                bool isSelected = (render.textureName == textureName);
                if (ImGui::Selectable(textureName.c_str(), isSelected))
                {
                    if (!isChanged) oldState = render;
                    render.textureName = textureName;
                    // Fetch texture size immediately when selection changes
                    std::tie(render.textureWidth, render.textureHeight) = Popplio::AssetStore::GetTextureSize(textureName);
                    if (!isChanged) isChanged = true;

                    // Clear search when selection is made
                    searchBuffer[0] = '\0';
                }

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }
        
        // Ensure texture size is set only if it's not already retrieved
        if ((render.textureWidth == 0 || render.textureHeight == 0) && !render.textureName.empty())
        {
            std::tie(render.textureWidth, render.textureHeight) = Popplio::AssetStore::GetTextureSize(render.textureName);
        }

        // Display selected texture name
        ImGui::Text("Texture: %s", render.textureName.c_str());

        // Display texture width and height (Even before clicking the combo box)
        ImGui::Text("Size: %d x %d", render.textureWidth, render.textureHeight);

        // Tooltip when hovered
        if (ImGui::IsItemHovered() && render.textureWidth > 0 && render.textureHeight > 0)
        {
            ImGui::BeginTooltip();
            ImGui::Text("Size: %d x %d", render.textureWidth, render.textureHeight);
            ImGui::EndTooltip();
        }

        float Alpha[1] = { render.alpha };
        if (ImGui::DragFloat("Alpha", Alpha, 0.05f))
        {
            if (!isChanged)
            {
                oldState = render;
                isChanged = true;
            }
            render.alpha = Alpha[0];
        }

        float colors[3] = { render.color.x, render.color.y, render.color.z };
        if (ImGui::ColorEdit3("Color", colors, ImGuiColorEditFlags_Float))
        {
            if (!isChanged)
            {
                oldState = render;
                isChanged = true;
            }

            render.color.x = colors[0];
            render.color.y = colors[1];
            render.color.z = colors[2];
        }

        // Enable 9-Slice
        bool enableNineSlice = render.enableNineSlice;
        if (ImGui::Checkbox("Enable 9-Slice", &enableNineSlice))
        {
            if (!isChanged) { oldState = render; isChanged = true; }
            render.enableNineSlice = enableNineSlice;
        }

        if (render.enableNineSlice)
        {
            // Adjust Borders
            // Left Border
            if (ImGui::DragFloat("Left Border", &render.borderLeft, 1.0f, 0.0f, static_cast<float>(render.textureWidth) / 2))
            {
                if (!isChanged)
                {
                    oldState = render;  // store the original RenderComponent
                    isChanged = true;
                }
            }

            // Right Border
            if (ImGui::DragFloat("Right Border", &render.borderRight, 1.0f, 0.0f, static_cast<float>(render.textureWidth) / 2))
            {
                if (!isChanged)
                {
                    oldState = render;
                    isChanged = true;
                }
            }

            // Top Border
            if (ImGui::DragFloat("Top Border", &render.borderTop, 1.0f, 0.0f, static_cast<float>(render.textureHeight) / 2))
            {
                if (!isChanged)
                {
                    oldState = render;
                    isChanged = true;
                }
            }

            // Bottom Border
            if (ImGui::DragFloat("Bottom Border", &render.borderBottom, 1.0f, 0.0f, static_cast<float>(render.textureHeight) / 2))
            {
                if (!isChanged)
                {
                    oldState = render;
                    isChanged = true;
                }
            }

            // Show a separate ImGui window for previewing the texture
            ShowNineSliceEditor(render);
        }

        // emit change event
        if (isChanged && !InputSystem::IsMouseButtonDown(InputSystem::MouseButton::Left) && !ImGui::GetIO().WantTextInput) // change detection
        {
            eventBus.EmitEvent<SceneChangeEvent<std::any>>(*selectedEntity, render, oldState, ChangeAction::MODIFY_COMPONENT);
            isChanged = false;
        }
    }

    template <>
    void Editor::EditComponent(RigidBodyComponent& rigidBody)
    {
        // change detection
        static bool isChanged = false;
        static RigidBodyComponent oldState;

        bool isInputting = false;

        bool a{ rigidBody.isActive };
        if (ImGui::Checkbox("Active", &a))
        {
            if (!isChanged)
            {
                oldState = rigidBody;
                isChanged = true;
            }
            rigidBody.isActive = a;
        }

        // Velocity
        float temp[2] = { rigidBody.velocity.x, rigidBody.velocity.y };
		if (ImGui::DragFloat2("Velocity", temp))
        {
            if (!isChanged)
            {
                oldState = rigidBody;
                isChanged = true;
            }
            rigidBody.velocity.x = temp[0];
            rigidBody.velocity.y = temp[1];
        }
        if (ImGui::IsItemActive()) isInputting = true;
        // Force
        temp[0] = rigidBody.force.x;
        temp[1] = rigidBody.force.y;
		if (ImGui::DragFloat2("Force", temp))
        {
            if (!isChanged)
            {
                oldState = rigidBody;
                isChanged = true;
            }
            rigidBody.force.x = temp[0];
            rigidBody.force.y = temp[1];
        }
        if (ImGui::IsItemActive()) isInputting = true;
        // Mass
        temp[0] = rigidBody.mass;
        if (ImGui::InputFloat("Mass", temp))
        {
            if (!isChanged)
            {
                oldState = rigidBody;
                isChanged = true;
            }
            rigidBody.mass = temp[0];
        }
        if (ImGui::IsItemActive()) isInputting = true;

        // Gravity settings
        bool temp2 = rigidBody.useGravity;
        if (ImGui::Checkbox("Use Gravity", &temp2))
        {
            if (!isChanged)
            {
                oldState = rigidBody;
                isChanged = true;
            }

            rigidBody.useGravity = temp2;
        }

        if (rigidBody.useGravity)
        {
            temp[0] = rigidBody.gravityScale;
            if (ImGui::InputFloat("Gravity Scale", temp))
            {
                if (!isChanged)
                {
                    oldState = rigidBody;
                    isChanged = true;
                }
                rigidBody.gravityScale = temp[0];
            }
            if (ImGui::IsItemActive()) isInputting = true;

            temp[0] = rigidBody.dragCoefficient;
            if (ImGui::InputFloat("Drag Coefficient", temp))
            {
                if (!isChanged)
                {
                    oldState = rigidBody;
                    isChanged = true;
                }
                rigidBody.dragCoefficient = temp[0];
            }
            if (ImGui::IsItemActive()) isInputting = true;
        }

        // Read-only displays for dynamic values
        ImGui::Separator();
        ImGui::TextDisabled("Dynamic Values (Read-only)");

        // Acceleration (read-only)
        ImGui::BeginGroup();
        ImGui::Text("Acceleration:");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f),
            "X: %.2f, Y: %.2f", rigidBody.acceleration.x, rigidBody.acceleration.y);
        ImGui::EndGroup();

        // emit change event
        if (isChanged && !InputSystem::IsMouseButtonDown(InputSystem::MouseButton::Left) && !isInputting) // change detection
        {
            eventBus.EmitEvent<SceneChangeEvent<std::any>>(*selectedEntity, rigidBody, oldState, ChangeAction::MODIFY_COMPONENT);
            isChanged = false;
        }
    }

    template <>
    void Editor::EditComponent(BoxColliderComponent& boxCollider)
    {
        // change detection
        static bool isChanged = false;
        static BoxColliderComponent oldState;

        bool a{ boxCollider.isActive };
        if (ImGui::Checkbox("Active", &a))
        {
            if (!isChanged)
            {
                oldState = boxCollider;
                isChanged = true;
            }
            boxCollider.isActive = a;
        }

        bool isEnabled = boxCollider.isEnabled;
        if (ImGui::Checkbox("Enabled", &isEnabled))
        {
            if (!isChanged)
            {
                oldState = boxCollider;
                isChanged = true;
            }
            boxCollider.isEnabled = isEnabled;
        }

        // if (!boxCollider.isEnabled) return;
        float temp[2] = { boxCollider.size.x, boxCollider.size.y };
        if (ImGui::DragFloat2("Size", temp))
        {
            if (!isChanged)
            {
                oldState = boxCollider;
                isChanged = true;
            }
            boxCollider.size.x = temp[0];
            boxCollider.size.y = temp[1];
        }
        temp[0] = boxCollider.offset.x;
        temp[1] = boxCollider.offset.y;
        if (ImGui::DragFloat2("Offset", temp))
        {
            if (!isChanged)
            {
                oldState = boxCollider;
                isChanged = true;
            }
            boxCollider.offset.x = temp[0];
            boxCollider.offset.y = temp[1];
        }

        bool isTrigger = boxCollider.isTrigger;
        if (ImGui::Checkbox("Trigger Collider", &isTrigger))
        {
            if (!isChanged)
            {
                oldState = boxCollider;
                isChanged = true;
            }
            boxCollider.isTrigger = isTrigger;
        }

        // emit change event
        if (isChanged && !InputSystem::IsMouseButtonDown(InputSystem::MouseButton::Left) && !ImGui::GetIO().WantTextInput) // change detection
        {
            eventBus.EmitEvent<SceneChangeEvent<std::any>>(*selectedEntity, boxCollider, oldState, ChangeAction::MODIFY_COMPONENT);
            isChanged = false;
        }
    }

    template <>
    void Editor::EditComponent(TextComponent& textComp)
    {
        // change detection
        static bool isChanged = false;
        static TextComponent oldState;

        bool isInputting = false;

        bool a{ textComp.isActive };
        if (ImGui::Checkbox("Active", &a))
        {
            if (!isChanged)
            {
                oldState = textComp;
                isChanged = true;
            }
            textComp.isActive = a;
        }

        // Text Content Section
        ImGui::SeparatorText("Text Content");
        {
            char buffer[1024];
            strcpy_s(buffer, sizeof(buffer), textComp.text.c_str());
            if (ImGui::InputTextMultiline("Text##content", buffer, sizeof(buffer)))
            {
                if (!isChanged)
                {
                    oldState = textComp;
                    isChanged = true;
                }
                textComp.text = buffer;
            }
            if (ImGui::IsItemActive()) isInputting = true;
        }

        // Font Settings Section
        ImGui::SeparatorText("Font Settings");
        {
            // Font selection dropdown
            if (ImGui::BeginCombo("Font", textComp.fontName.c_str()))
            {
                for (const auto& [fontName, font] : AssetStore::GetAllFonts())
                {
                    bool isSelected = (textComp.fontName == fontName);
                    if (ImGui::Selectable(fontName.c_str(), isSelected))
                    {
                        if (!isChanged)
                        {
                            oldState = textComp;
                            isChanged = true;
                        }
                        textComp.fontName = fontName;
                    }
                    if (isSelected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }

            // Font Size

            int fontSize = textComp.fontSize;
            if (ImGui::InputInt("Font Size", &fontSize))
            {
                if (!isChanged)
                {
                    oldState = textComp;
                    isChanged = true;
                }
                if (fontSize < 0) fontSize = 0;
                textComp.fontSize = fontSize;
            }
            if (ImGui::IsItemActive()) isInputting = true;

            //// Font Size selection
            //ImGui::PushItemWidth(100);
            //int currentSize = static_cast<int>(textComp.fontSize);
            //ImGui::Text("Font Size: %d", currentSize);
            //ImGui::PopItemWidth();

            //// font sizes
            //static const int commonSizes[] = { 8, 12, 16, 24, 32, 48, 64, 72, 96 };
            //ImGui::Text("Sizes:");
            //ImGui::SameLine();
            //float avail = ImGui::GetContentRegionAvail().x;
            //float buttonWidth = 40.0f;
            //float spacing = ImGui::GetStyle().ItemSpacing.x;
            //int buttonsPerRow = static_cast<int>((avail + spacing) / (buttonWidth + spacing));

            //for (int i = 0; i < IM_ARRAYSIZE(commonSizes); i++)
            //{
            //    if (i > 0 && i % buttonsPerRow != 0)
            //        ImGui::SameLine();

            //    char buttonLabel[8];
            //    snprintf(buttonLabel, sizeof(buttonLabel), "%d", commonSizes[i]);

            //    if (ImGui::Button(buttonLabel, ImVec2(buttonWidth, 0)))
            //    {
            //        if (!isChanged)
            //        {
            //            oldState = textComp;
            //            isChanged = true;
            //        }
            //        textComp.fontSize = commonSizes[i];
            //    }
            //    if (ImGui::IsItemHovered())
            //    {
            //        ImGui::SetTooltip("Set font size to %d", commonSizes[i]);
            //    }
            //}
        }

        // Color Section
        ImGui::SeparatorText("Color");
        {
            float Alpha[1] = { textComp.alpha };
            if (ImGui::DragFloat("Alpha", Alpha, 0.05f))
            {
                if (!isChanged)
                {
                    oldState = textComp;
                    isChanged = true;
                }
                textComp.alpha = Alpha[0];
            }

            float colors[3] = { textComp.color.x, textComp.color.y, textComp.color.z };
            if (ImGui::ColorEdit3("Text Color", colors, ImGuiColorEditFlags_Float))
            {
                if (!isChanged)
                {
                    oldState = textComp;
                    isChanged = true;
                }

                textComp.color.x = colors[0];
                textComp.color.y = colors[1];
                textComp.color.z = colors[2];
            }
            if (ImGui::IsItemActive()) isInputting = true;
        }


        // Preview Section
        ImGui::SeparatorText("Preview");
        {
            float previewWidth = ImGui::GetContentRegionAvail().x;
            float previewHeight = 50.0f;
            ImVec2 previewStart = ImGui::GetCursorScreenPos();

            // Draw preview background
            ImGui::GetWindowDrawList()->AddRectFilled(
                previewStart,
                ImVec2(previewStart.x + previewWidth, previewStart.y + previewHeight),
                IM_COL32(50, 50, 50, 255)
            );

            // Center the preview text
            float textWidth = ImGui::CalcTextSize(textComp.text.c_str()).x;
            float textX = previewStart.x + (previewWidth - textWidth) * 0.5f;
            float textY = previewStart.y + (previewHeight - ImGui::GetTextLineHeight()) * 0.5f;

            // Draw the preview text
            ImGui::GetWindowDrawList()->AddText(
                ImVec2(textX, textY),
                ImGui::ColorConvertFloat4ToU32(ImVec4(textComp.color.x, textComp.color.y, textComp.color.z, textComp.alpha)),
                textComp.text.c_str()
            );

            ImGui::Dummy(ImVec2(0, previewHeight));
        }

        // Help Section
        ImGui::SeparatorText("Help");
        {
            ImGui::TextDisabled("(?)");
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                ImGui::TextUnformatted(
                    "Font size is rendered at exact pixel sizes.\n"
                    "Choose from common sizes for best quality.\n"
                    "Custom sizes will be loaded on demand.\n\n"
                    "Render Modes:\n"
                    "- In Game (UI): Fixed on screen, follows editor camera in edit mode\n"
                    "- In World: Moves with both editor and game cameras"
                );
                ImGui::PopTextWrapPos();
                ImGui::EndTooltip();
            }

            // Warning for unavailable sizes
            Font* font = AssetStore::GetFont(textComp.fontName);
            if (font && !font->IsSizeLoaded(textComp.fontSize))
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.0f, 1.0f));
                ImGui::Text("Note: This font size will be loaded when first used.");
                ImGui::PopStyleColor();
            }
        }

        // emit change event
        if (isChanged && !isInputting && !InputSystem::IsMouseButtonDown(InputSystem::MouseButton::Left)) // change detection
        {
            eventBus.EmitEvent<SceneChangeEvent<std::any>>(*selectedEntity, textComp, oldState, ChangeAction::MODIFY_COMPONENT);
            isChanged = false;
        }
    }

    template <>
    void Editor::EditComponent(UIComponent& ui)
    {
        static bool isChanged = false;
        static UIComponent oldState;

        if (!selectedEntity)
        {
            return;
        }

        if (ImGui::CollapsingHeader("UI Component", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // Edit isActive
            bool tempIsActive = ui.isActive;
            if (ImGui::Checkbox("Is Active", &tempIsActive))
            {
                if (!isChanged) { oldState = ui; isChanged = true; }
                ui.isActive = tempIsActive;
            }

            // Edit UI Type
            static const char* uiTypes[] = { "BUTTON", "CHECKBOX", "SLIDER_TRACK", "SLIDER_HANDLE" };
            int currentType = static_cast<int>(ui.type);
            if (ImGui::Combo("UI Type", &currentType, uiTypes, IM_ARRAYSIZE(uiTypes)))
            {
                if (!isChanged) { oldState = ui; isChanged = true; }
                ui.type = static_cast<UIType>(currentType);
            }

            // Enable or Disable Hover Effect
            bool tempHasHover = ui.hasHoverEffect;
            if (ImGui::Checkbox("Has Hover Effect", &tempHasHover))
            {
                if (!isChanged) { oldState = ui; isChanged = true; }
                ui.hasHoverEffect = tempHasHover;
            }

            // OnClickAction editing for BUTTON type
            if (ui.type == UIType::BUTTON)
            {
                if (!isChanged) { oldState = ui; isChanged = true; }

                auto& functionRegistry = FunctionRegistry::GetInstance();
                auto availableFunctions = functionRegistry.GetAllFunctionNames();

                std::vector<const char*> actionList = { "None" };
                for (const auto& funcName : availableFunctions)
                {
                    actionList.push_back(funcName.c_str());
                }

                std::vector<std::string> selectedActions = ui.onClickActions;

                if (ImGui::CollapsingHeader("OnClick Actions", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    for (size_t i = 0; i < selectedActions.size(); i++)
                    {
                        int currentAction = 0;
                        for (size_t j = 0; j < actionList.size(); j++)
                        {
                            if (selectedActions[i] == actionList[j]) currentAction = static_cast<int>(j);
                        }

                        if (ImGui::Combo(("Action " + std::to_string(i)).c_str(), &currentAction, actionList.data(), static_cast<int>(actionList.size())))
                        {
                            selectedActions[i] = actionList[currentAction];
                            isChanged = true;

                        }
                    }

                    if (ImGui::Button("Add Action")) { selectedActions.push_back("None"); isChanged = true; }
                    if (!selectedActions.empty() && ImGui::Button("Remove Action")) { selectedActions.pop_back(); isChanged = true; }

                    if (ui.onClickActions != selectedActions) // Only update if there are changes
                    {
                        ui.onClickActions = selectedActions;
                        isChanged = true;

                        // Notify UISystem to update the button callbacks
                        auto& uiSystem = reg.GetSystem<UISystem>();
                        uiSystem.InitializeButtons();
                    }

                }
            }

            // Handle UI-Specific Properties
            if (ui.type == UIType::CHECKBOX)
            {
                bool tempChecked = ui.checked;
                if (ImGui::Checkbox("Checked", &tempChecked))
                {
                    if (!isChanged) { oldState = ui; isChanged = true; }
                    ui.checked = tempChecked;
                }
            }

            if (ui.type == UIType::SLIDER_HANDLE)
            {
                float tempSliderValue = ui.sliderValue;
                if (ImGui::SliderFloat("Slider Value", &tempSliderValue, 0.0f, 1.0f))
                {
                    if (!isChanged) { oldState = ui; isChanged = true; }
                    ui.sliderValue = tempSliderValue;
                }
            }

            if (ui.type == UIType::SLIDER_TRACK || ui.type == UIType::SLIDER_HANDLE)
            {
                // Link to Another UI Element
                int linkedID = ui.linkedEntityId;
                if (ImGui::InputInt("Linked Entity ID", &linkedID))
                {
                    if (!isChanged) { oldState = ui; isChanged = true; }
                    ui.linkedEntityId = linkedID;
                }
            }

            // Handle UI Spritesheet Editing
            if (ui.hasHoverEffect && ImGui::CollapsingHeader("Spritesheet Settings", ImGuiTreeNodeFlags_DefaultOpen))
            {
                int tempFrameX = ui.frameX;
                int tempFrameY = ui.frameY;
                int tempHoverFrameX = ui.hoverFrameX;
                int tempHoverFrameY = ui.hoverFrameY;
                int tempTotalColumns = ui.totalColumns;
                int tempTotalRows = ui.totalRows;
                float tempSheetWidth = ui.sheetWidth;
                float tempSheetHeight = ui.sheetHeight;

                if (ImGui::InputInt("Frame X", &tempFrameX)) { ui.frameX = tempFrameX; isChanged = true; }
                if (ImGui::InputInt("Frame Y", &tempFrameY)) { ui.frameY = tempFrameY; isChanged = true; }
                if (ImGui::InputInt("Hover Frame X", &tempHoverFrameX)) { ui.hoverFrameX = tempHoverFrameX; isChanged = true; }
                if (ImGui::InputInt("Hover Frame Y", &tempHoverFrameY)) { ui.hoverFrameY = tempHoverFrameY; isChanged = true; }
                if (ImGui::InputInt("Total Columns", &tempTotalColumns)) { ui.totalColumns = tempTotalColumns; isChanged = true; }
                if (ImGui::InputInt("Total Rows", &tempTotalRows)) { ui.totalRows = tempTotalRows; isChanged = true; }
                if (ImGui::InputFloat("Sheet Width", &tempSheetWidth)) { ui.sheetWidth = tempSheetWidth; isChanged = true; }
                if (ImGui::InputFloat("Sheet Height", &tempSheetHeight)) { ui.sheetHeight = tempSheetHeight; isChanged = true; }
            }

            // Emit change event ONLY IF a real change occurred
            if (isChanged && !InputSystem::IsMouseButtonDown(InputSystem::MouseButton::Left) && !ImGui::GetIO().WantTextInput)
            {
                eventBus.EmitEvent<SceneChangeEvent<std::any>>(*selectedEntity, ui, oldState, ChangeAction::MODIFY_COMPONENT);
                isChanged = false;
            }
        }
    }

    template <>
    void Editor::EditComponent(LogicComponent& logic) // todo: show script vars
    {
        // change detection
        static bool isChanged = false;
        static LogicComponent oldState;

        bool isInputting = false;

        //char buffer[256]{};

        //if (ImGui::InputText("Script File", buffer, sizeof(buffer))) logic.scriptName = buffer;

        bool a{ logic.isActive };
        if (ImGui::Checkbox("Active", &a))
        {
            if (!isChanged)
            {
                oldState = logic;
                isChanged = true;
            }
            logic.isActive = a;
        }

        //static ScriptLang scriptLang{ ScriptLang::UNASSIGNED };
        //static MonoClass* scriptClass{ nullptr };
        //static MonoObject* scriptInst{ nullptr };

        //scriptLang = ScriptLang::UNASSIGNED;
        //scriptClass = nullptr;
        //scriptInst = nullptr;

        static MonoClass* inspectorScriptClass{ nullptr };
        inspectorScriptClass = nullptr;

        switch (logic.scriptLang)
        {
            case ScriptLang::UNASSIGNED:
            {
                ImGui::Text("Unassigned");
                break;
            }
            case ScriptLang::CPP:
            {
                ImGui::Text("Language: C++");
                break;
            }
            case ScriptLang::CS:
            {
                ImGui::Text("Language: C#");
                break;
            }
            default:
            {
                ImGui::Text("Unknown / Unsupported");
                break;
            }
        }

        // Script selection //

        ImGui::BeginGroup();
        ImGui::Text("Script:");
        ImGui::SameLine();

        if (ImGui::BeginCombo("##Script", logic.scriptName.c_str()))
        {
            // C++
            for (const auto& [name, script] : AssetStore::GetAllScripts())
            {
                bool isSelected = (logic.scriptName == name);

                if (ImGui::Selectable(("C++ | " + name).c_str(), isSelected))
                {
                    if (!isChanged)
                    {
                        oldState = logic;
                        isChanged = true;
                    }
                    logic.scriptName = name;
                    logic.scriptLang = ScriptLang::CPP;
                    logic.ResetScriptVars(); // reset script variable values when changing script
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                    //logic.scriptLang = ScriptLang::CPP;
                }
            }

            // C#
            for (const auto& script : mono.GetLoadedUserClasses())
            {
                //if (script == nullptr) continue;

                std::string name = mono_class_get_namespace(script);

                //if (name != POPPLIO_PROJECT_NAMESPACE) continue;
                MonoClass* tmp = mono_class_get_parent(script);
                static_cast<void>(tmp);
                //if (strcmp(mono_class_get_name(tmp), POPPLIO_PROJECT_BASECLASS) != 0) continue;

                name += "::";
                name += mono_class_get_name(script);

                bool isSelected = (logic.scriptName == name);

                if (ImGui::Selectable(("C# | " + name).c_str(), isSelected))
                {
                    if (!isChanged)
                    {
                        oldState = logic;
                        isChanged = true;
                    }
                    logic.scriptName = name;
                    logic.scriptLang = ScriptLang::CS;
                    inspectorScriptClass = script;
                    logic.ResetScriptVars(); // reset script variable values when changing script
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                    //logic.scriptLang = ScriptLang::CS;
                    //scriptClass = script;
                }
            }

            ImGui::EndCombo();
        }

        ImGui::EndGroup();

        // C# Script variables //

        // only can see in editing mode for now due to crash // TODO FIX - seems to be due to rename
        if (editorState == State::EDITING && logic.scriptLang == ScriptLang::CS)
        {
            std::pair<std::string, std::string> splitName = mono.SplitName(logic.scriptName);

            // Get the MonoClass for the script
            MonoClass* scriptClass = mono.GetAssClass(splitName.first.c_str(), splitName.second.c_str()); 

            // Iterate over fields in the Mono class
            static void* scriptIter = nullptr;
            static MonoClassField* varField = nullptr;
            static MonoCustomAttrInfo* varAttr = nullptr;
            static MonoObject* inspectorAttr = nullptr;
            static MonoProperty* attrNameProp = nullptr;
            //static uint32_t fieldIndex = 0;
            static MonoObject* scriptObj = nullptr;
            static MonoClass* scriptCurrClass = nullptr;

            scriptIter = nullptr;
            varField = nullptr;
            varAttr = nullptr;
            inspectorAttr = nullptr;
            attrNameProp = nullptr;
            scriptObj = nullptr;
            scriptCurrClass = nullptr;

            if (scriptCurrClass != scriptClass)
            {
                scriptObj = mono.InstLifeClass(splitName.first.c_str(), splitName.second.c_str());
                scriptCurrClass = scriptClass;
            }

            //try
            //{
            //    if (classObj == nullptr || mono_object_get_class(classObj) != monoClass)
            //    {
            //        classObj = mono.InstLifeClass(splitName.first.c_str(), splitName.second.c_str());
            //    }
            //}
            //catch (const std::exception& e)
            //{
            //    (void)e;
            //    try
            //    {
            //        classObj = mono.InstLifeClass(splitName.first.c_str(), splitName.second.c_str());
            //    }
            //    catch (const std::exception& e)
            //    {
            //        std::stringstream ss{ "" };
            //        ss << "Failed to instantiate class: " << splitName.first << "::" << splitName.second;
            //        Logger::Error(ss.str());
            //        return;
            //    }
            //}

            MonoClass* inspectorAttrClass = mono.GetAssClass("Team_Popplio.Attributes", "InspectorAttribute");
            MonoClass* ignoreAttrClass = mono.GetAssClass("Team_Popplio.Attributes", "InspectorIgnoreAttribute");

            MonoClass* headerAttrClass = mono.GetAssClass("Team_Popplio.Attributes", "InspectorHeaderAttribute");
            // ^ TODO implement header attribute
            static_cast<void>(headerAttrClass);

            //fieldIndex = 0;
            try
            {
                static unsigned int varFieldIndex = 1;
                varFieldIndex = 1;
                // Iterate over fields in the Mono class
                while ((varField = mono_class_get_fields(scriptClass, &scriptIter)) != nullptr) // TODO FIX iteration
                {
                    // check for header attr TODO
                    //MonoCustomAttrInfo* headerAttr = mono_custom_attrs_from_class(scriptClass);

                    if (mono.IsFieldStatic(varField)) // skip static
                    {
                        ++varFieldIndex;
                        continue; // skip static fields
                    }

                    // Check for the Inspector attribute
                    varAttr = mono_custom_attrs_from_field(scriptClass, varField);

                    bool isPublic = mono.GetFieldAccess(varField) & 
                        static_cast<uint8_t>(PopplioScriptAPI::MonoAPI::Access::Public);

                    if (isPublic) // public
                    {
                        if (varAttr) 
                        {
                            if (mono_custom_attrs_has_attr(varAttr, ignoreAttrClass)) // has ignore attribute
                            {
                                mono_custom_attrs_free(varAttr);
                                ++varFieldIndex;
                                continue;
                            }

                            if (!mono_custom_attrs_has_attr(varAttr, inspectorAttrClass)) // no inspector attribute
                                varAttr = nullptr;
                        }
                    }
                    else // private
                    {
                        if (!varAttr) // no attributes
                        {
                            ++varFieldIndex;
                            continue;
                        }
                        if (mono_custom_attrs_has_attr(varAttr, ignoreAttrClass)) // has ignore attribute
                        {
                            mono_custom_attrs_free(varAttr);
                            ++varFieldIndex;
                            continue;
                        }
                        if (!mono_custom_attrs_has_attr(varAttr, inspectorAttrClass)) // no inspector attribute
                        {
                            mono_custom_attrs_free(varAttr);
                            ++varFieldIndex;
                            continue;
                        }
                    }

                    if (varAttr != nullptr)
                    {
                        inspectorAttr = mono_custom_attrs_get_attr(varAttr, inspectorAttrClass);
                        attrNameProp = mono.GetProperty(mono_object_get_class(inspectorAttr), "Name");
                    }
                    else
                    {
                        inspectorAttr = nullptr;
                        attrNameProp = nullptr;
                    }

                    static MonoType* fieldType = nullptr;
                    static MonoTypeEnum typeEnum = MONO_TYPE_END;
                    static std::string fieldName = "";
                    static std::string displayName = "";
                    static MonoObject* fieldObj = nullptr;

                    // zero out
                    fieldType = nullptr;
                    typeEnum = MONO_TYPE_END;
                    fieldName = "";
                    displayName = "";
                    fieldObj = nullptr;

                    // set
                    fieldName = mono_field_get_name(varField);
                    fieldType = mono_field_get_type(varField);
                    if (fieldType == nullptr)
                    {
                        Logger::Error("EditComponent.cpp | LogicComponent | C# | Failed to get field type for field: " + fieldName);
                        //ImGui::PopID();
                        ++varFieldIndex;
                        continue;
                    }
                    typeEnum = static_cast<MonoTypeEnum>(mono_type_get_type(fieldType));
                    // NOTE do not use appdomain here, will crash when it reaches MONO_TYPE_GENERICINST vvvvv
                    fieldObj = mono_field_get_value_object(&mono.GetDomain(), varField, scriptObj);
                    if (fieldObj == nullptr)
                    {
                        Logger::Debug("EditComponent.cpp | LogicComponent | C# | Failed to get field object for field: " + fieldName);
                        Logger::Debug("EditComponent.cpp | LogicComponent | C# | Possibly does not exist anymore");
                        //ImGui::PopID();
                        ++varFieldIndex;
                        continue;
                    }

                    ImGui::PushID(static_cast<int>(varFieldIndex++));

                    ImGui::BeginGroup();

                    displayName = ((attrNameProp != nullptr) ?
                        mono.GetProp<std::string>(inspectorAttr, attrNameProp) : mono_field_get_name(varField));
                    ImGui::Text(displayName.c_str());
                    ImGui::SameLine();

                    // display / edit according to type
                    switch (typeEnum)
                    {
                        case MONO_TYPE_BOOLEAN: // bool
                        {
                            bool* val = nullptr;

                            for (auto& [name, value] : logic.bools)
                            {
                                if (name != fieldName) continue;

                                val = &value;
                                break;
                            }
                            if (val == nullptr)
                            {
                                logic.bools.push_back(std::make_pair(fieldName, mono.Get<bool>(scriptObj, varField)));
                                val = &logic.bools.back().second;
                            }

                            bool temp = *val;
                            if (ImGui::Checkbox("##bool", &temp)) //mono.Set(nullptr, field, val);
                            {
                                if (!isChanged)
                                {
                                    oldState = logic;
                                    isChanged = true;
                                }
                                *val = temp;
                            }

                            break;
                        }
                        case MONO_TYPE_I4: // int
                        {
                            int* val = nullptr;
                            for (auto& [name, value] : logic.ints)
                            {
                                if (name != fieldName) continue;
                                val = &value;
                                break;
                            }
                            if (val == nullptr)
                            {
                                logic.ints.push_back(std::make_pair(fieldName, mono.Get<int>(scriptObj, varField)));
                                val = &logic.ints.back().second;
                            }
                            int temp = *val;
                            if (ImGui::InputInt("##int", &temp)) //mono.Set(nullptr, field, val);
                            {
                                if (!isChanged)
                                {
                                    oldState = logic;
                                    isChanged = true;
                                }
                                *val = temp;
                            }
                            if (ImGui::IsItemActive()) isInputting = true;
                            break;
                        }
                        case MONO_TYPE_R4: // float
                        {
                            float* val = nullptr;
                            for (auto& [name, value] : logic.floats)
                            {
                                if (name != fieldName) continue;
                                val = &value;
                                break;
                            }
                            if (val == nullptr)
                            {
                                logic.floats.push_back(std::make_pair(fieldName, mono.Get<float>(scriptObj, varField)));
                                val = &logic.floats.back().second;
                            }
                            float temp = *val;
                            if (ImGui::InputFloat("##float", &temp)) 
                            {
                                if (!isChanged)
                                {
                                    oldState = logic;
                                    isChanged = true;
                                }
                                *val = temp;
                            }
                            if (ImGui::IsItemActive()) isInputting = true;
                            break;
                        }
                        case MONO_TYPE_R8: // double
                        {
                            double* val = nullptr;
                            for (auto& [name, value] : logic.doubles)
                            {
                                if (name != fieldName) continue;
                                val = &value;
                                break;
                            }
                            if (val == nullptr)
                            {
                                logic.doubles.push_back(std::make_pair(fieldName, mono.Get<double>(scriptObj, varField)));
                                val = &logic.doubles.back().second;
                            }
                            double temp = *val;
                            if (ImGui::InputDouble("##double", &temp)) 
                            {
                                if (!isChanged)
                                {
                                    oldState = logic;
                                    isChanged = true;
                                }
                                *val = temp;
                            }
                            if (ImGui::IsItemActive()) isInputting = true;
                            break;
                        }
                        case MONO_TYPE_CHAR: // char
                        {
                            char* val = nullptr;
                            
                            for (auto& [name, value] : logic.chars)
                            {
                                if (name != fieldName) continue;
                                val = &value;
                                break;
                            }
                            if (val == nullptr)
                            {
                                logic.chars.push_back(std::make_pair(fieldName, mono.Get<char>(scriptObj, varField)));
                                val = &logic.chars.back().second;
                            }
                            char c = *val;
                            char temp[2] { c, '\0'};
                            if (ImGui::InputText("##char", temp, IM_ARRAYSIZE(temp))) 
                            {
                                if (!isChanged)
                                {
                                    oldState = logic;
                                    isChanged = true;
                                }
                                *val = *temp;
                            }
                            if (ImGui::IsItemActive()) isInputting = true;

                            break;
                        }
                        case MONO_TYPE_STRING: // string
                        {
                            std::string* val = nullptr;
                            for (auto& [name, value] : logic.strings)
                            {
                                if (name != fieldName) continue;
                                val = &value;
                                break;
                            }
                            if (val == nullptr)
                            {
                                logic.strings.push_back(std::make_pair(fieldName, mono.Get<std::string>(scriptObj, varField)));
                                val = &logic.strings.back().second;
                            }
                            
                            char temp[POPPLIO_MAX_STRING] {""};
                            strcat_s(temp, val->c_str());
                            if (ImGui::InputText("##string", temp, POPPLIO_MAX_STRING))
                            {
                                if (!isChanged)
                                {
                                    oldState = logic;
                                    isChanged = true;
                                }
                                *val = temp;
                            }
                            if (ImGui::IsItemActive()) isInputting = true;

                            break;
                        }
                        case MONO_TYPE_CLASS: // class
                        {
                            std::string csType = mono_type_get_name(fieldType);
                            // Entity
                            if (csType == "Team_Popplio.ECS.Entity")
                            {
                                int* id = nullptr;
                                for (auto& [name, value] : logic.entities)
                                {
                                    if (name != fieldName) continue;
                                    id = &value;
                                    break;
                                }
                                if (id == nullptr)
                                {
                                    int value = mono.Get<int>(mono.Get(scriptObj, varField), 
                                        mono_class_get_field_from_name(
                                            mono.GetAssClass("Team_Popplio.ECS", "Entity"), "_NativeID"));
                                    logic.entities.push_back(std::make_pair(fieldName, value));
                                    id = &logic.entities.back().second;
                                }

                                //char buffer[POPPLIO_MAX_STRING]{ "" };
                                //_itoa_s(*id, buffer, 10);

                                if (!reg.EntityExists(*id) && editorState != State::EDITING) break; // ignore destroyed entities

                                try
                                {
                                    std::string entityName = *id < 0 ? "None" :
                                        std::to_string(*id) + " [" +
                                        reg.GetEntityTag(reg.GetEntityById(*id)).c_str() + "]";
                                    if (ImGui::BeginCombo("##ScriptEntity", entityName.c_str()))
                                    {
                                        for (auto& entity : reg.GetAllEntitiesMutable())
                                        {
                                            entityName = "Entity " + std::to_string(entity.GetId());
                                            if (reg.EntityHasAnyTag(entity))
                                            {
                                                entityName += " [" + reg.GetEntityTag(entity) + "]";
                                            }
                                            bool isSelected = (*id == entity.GetId());
                                            if (ImGui::Selectable(entityName.c_str()))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                *id = entity.GetId();
                                            }
                                            if (isSelected) ImGui::SetItemDefaultFocus();
                                        }
                                        ImGui::EndCombo();
                                    }
                                }
                                catch (const std::exception& e)
                                {
                                    std::stringstream ss { "" };
                                    ss << "C# script variable | Failed to display entity | " << e.what();
                                    Logger::Error(ss.str());
                                }
                            }
                            // prefab
                            else if (csType == "Team_Popplio.ECS.Prefab")
                            {
                                std::string* prefabName = nullptr;
                                for (auto& [name, value] : logic.prefabs)
                                {
                                    if (name != fieldName) continue;
                                    prefabName = &value;
                                    break;
                                }
                                if (prefabName == nullptr)
                                {
                                    std::string value = mono.Get<std::string>(mono.Get(scriptObj, varField),
                                        mono_class_get_field_from_name(
                                            mono.GetAssClass("Team_Popplio.ECS", "Prefab"), "_NameID"));
                                    logic.prefabs.push_back(std::make_pair(fieldName, value));
                                    prefabName = &logic.prefabs.back().second;
                                }

                                if (!prefabManager.PrefabExists(*prefabName) && editorState != State::EDITING) break; // ignore destroyed entities

                                try
                                {
                                    std::string prefabDisplay = *prefabName == "" ? "None" :
                                         "Prefab [" + *prefabName + "]";
                                    if (ImGui::BeginCombo("##ScriptPrefab", prefabDisplay.c_str()))
                                    {
                                        for (auto& prefab : prefabManager.GetPrefabs())
                                        {
                                            prefabDisplay = prefab.first;
                                            prefabDisplay += " [" + prefabManager.GetPrefabFilePath(prefab.first) + "]";

                                            bool isSelected = (*prefabName == prefab.first);
                                            if (ImGui::Selectable(prefabDisplay.c_str()))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                *prefabName = prefab.first;
                                            }
                                            if (isSelected) ImGui::SetItemDefaultFocus();
                                        }
                                        ImGui::EndCombo();
                                    }
                                }
                                catch (const std::exception& e)
                                {
                                    std::stringstream ss{ "" };
                                    ss << "C# script variable | Failed to display prefab | " << e.what();
                                    Logger::Error(ss.str());
                                }
                            }
                            else if (mono.IsLoadedUserClass(csType)) // TODO script
                            {

                            }
                            else
                            {
                                std::stringstream ss{ "" };
                                ss << "Unsupported class type in C# script variable: " << mono_type_get_name(fieldType) 
                                    << " | " << fieldName;
                                Logger::Warning(ss.str());
                            }
                            break;
                        }
                        case MONO_TYPE_ARRAY: // array
                        case MONO_TYPE_SZARRAY: // single-dim array
                        {
                            MonoClass* arrayClass = mono_type_get_class(fieldType);
                            MonoClass* elementClass = mono_class_get_element_class(arrayClass);
                            MonoType* elementType = mono_class_get_type(elementClass);
                            MonoTypeEnum elementTypeEnum = static_cast<MonoTypeEnum>(mono_type_get_type(elementType));

                            // oh god staircase to heaven

                            switch (elementTypeEnum)
                            {
                                case MONO_TYPE_BOOLEAN:
                                {
                                    std::vector<bool>* val = nullptr;
                                    for (auto& [name, value] : logic.boolLists)
                                    {
                                        if (name != fieldName) continue;
                                        val = &value;
                                        break;
                                    }
                                    if (val == nullptr)
                                    {
                                        logic.boolLists.push_back(
                                            std::make_pair(fieldName, PopplioScriptAPI::MonoToArray1D<bool>(
                                                mono.GetArray(mono.Get(scriptObj, varField)), &mono.GetAppDomain())));
                                        val = &logic.boolLists.back().second;
                                    }

                                    std::vector<bool> temp = *val;

                                    if (ImGui::TreeNode("Elements"))
                                    {
                                        if (ImGui::Button("Add Element"))
                                        {
                                            if (!isChanged)
                                            {
                                                oldState = logic;
                                                isChanged = true;
                                            }
                                            temp.push_back(false);
                                            *val = temp;
                                        }

                                        for (std::vector<bool>::const_iterator i{temp.begin()}; i != temp.end(); ++i)
                                        {
                                            bool b = *i;

                                            ImGui::PushID(-static_cast<int>(i - temp.begin()));
                                            if (ImGui::Checkbox("##bool", &b))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp[i - temp.begin()] = b;
                                                *val = temp;
                                            }
                                            ImGui::SameLine();

                                            if (ImGui::Button("Remove"))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp.erase(i);
                                                *val = temp;

                                                ImGui::PopID();
                                                break; // skip the rest to avoid crashing due to iterator invalidation
                                            }

                                            ImGui::PopID();
                                        }
                                        ImGui::TreePop();
                                    }

                                    break;
                                }
                                case MONO_TYPE_I4: // int
                                {
                                    std::vector<int>* val = nullptr;
                                    for (auto& [name, value] : logic.intLists)
                                    {
                                        if (name != fieldName) continue;
                                        val = &value;
                                        break;
                                    }
                                    if (val == nullptr)
                                    {
                                        logic.intLists.push_back(
                                            std::make_pair(fieldName, PopplioScriptAPI::MonoToArray1D<int>(
                                                mono.GetArray(mono.Get(scriptObj, varField)), &mono.GetAppDomain())));
                                        val = &logic.intLists.back().second;
                                    }

                                    std::vector<int> temp = *val;

                                    if (ImGui::TreeNode("Elements"))
                                    {
                                        if (ImGui::Button("Add Element"))
                                        {
                                            if (!isChanged)
                                            {
                                                oldState = logic;
                                                isChanged = true;
                                            }
                                            temp.push_back(0);
                                            *val = temp;
                                        }

                                        for (std::vector<int>::const_iterator i{ temp.begin() }; i != temp.end(); ++i)
                                        {
                                            ImGui::PushID(-static_cast<int>(i - temp.begin()));
                                            int integer = *i;
                                            if (ImGui::InputInt("##int", &integer)) 
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp[i - temp.begin()] = integer;
                                                *val = temp;
                                            }
                                            if (ImGui::IsItemActive()) isInputting = true;
                                            ImGui::SameLine();

                                            if (ImGui::Button("Remove"))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp.erase(i);
                                                *val = temp;

                                                ImGui::PopID();
                                                break; // skip the rest to avoid crashing due to iterator invalidation
                                            }

                                            ImGui::PopID();
                                        }
                                        ImGui::TreePop();
                                    }
                                    break;
                                }
                                case MONO_TYPE_R4: // float
                                {
                                    std::vector<float>* val = nullptr;
                                    for (auto& [name, value] : logic.floatLists)
                                    {
                                        if (name != fieldName) continue;
                                        val = &value;
                                        break;
                                    }
                                    if (val == nullptr)
                                    {
                                        logic.floatLists.push_back(
                                            std::make_pair(fieldName, PopplioScriptAPI::MonoToArray1D<float>(
                                                mono.GetArray(mono.Get(scriptObj, varField)), &mono.GetAppDomain())));
                                        val = &logic.floatLists.back().second;
                                    }

                                    std::vector<float> temp = *val;

                                    if (ImGui::TreeNode("Elements"))
                                    {
                                        if (ImGui::Button("Add Element"))
                                        {
                                            if (!isChanged)
                                            {
                                                oldState = logic;
                                                isChanged = true;
                                            }
                                            temp.push_back(0.f);
                                            *val = temp;
                                        }

                                        for (std::vector<float>::const_iterator i{ temp.begin() }; i != temp.end(); ++i)
                                        {
                                            ImGui::PushID(-static_cast<int>(i - temp.begin()));
                                            float f = *i;
                                            if (ImGui::InputFloat("##float", &f)) 
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp[i - temp.begin()] = f;
                                                *val = temp;
                                            }
                                            if (ImGui::IsItemActive()) isInputting = true;
                                            ImGui::SameLine();

                                            if (ImGui::Button("Remove"))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp.erase(i);
                                                *val = temp;

                                                ImGui::PopID();
                                                break; // skip the rest to avoid crashing due to iterator invalidation
                                            }

                                            ImGui::PopID();
                                        }
                                        ImGui::TreePop();
                                    }
                                    break;
                                }
                                case MONO_TYPE_R8: // double
                                {
                                    std::vector<double>* val = nullptr;
                                    for (auto& [name, value] : logic.doubleLists)
                                    {
                                        if (name != fieldName) continue;
                                        val = &value;
                                        break;
                                    }
                                    if (val == nullptr)
                                    {
                                        logic.doubleLists.push_back(
                                            std::make_pair(fieldName, PopplioScriptAPI::MonoToArray1D<double>(
                                                mono.GetArray(mono.Get(scriptObj, varField)), &mono.GetAppDomain())));
                                        val = &logic.doubleLists.back().second;
                                    }

                                    std::vector<double> temp = *val;

                                    if (ImGui::TreeNode("Elements"))
                                    {
                                        if (ImGui::Button("Add Element"))
                                        {
                                            if (!isChanged)
                                            {
                                                oldState = logic;
                                                isChanged = true;
                                            }
                                            temp.push_back(0.);
                                            *val = temp;
                                        }

                                        for (std::vector<double>::const_iterator i{ temp.begin() }; i != temp.end(); ++i)
                                        {
                                            ImGui::PushID(-static_cast<int>(i - temp.begin()));
                                            double d = *i;
                                            if (ImGui::InputDouble("##double", &d)) 
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp[i - temp.begin()] = d;
                                                *val = temp;
                                            }
                                            if (ImGui::IsItemActive()) isInputting = true;
                                            ImGui::SameLine();

                                            if (ImGui::Button("Remove"))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp.erase(i);
                                                *val = temp;

                                                ImGui::PopID();
                                                break; // skip the rest to avoid crashing due to iterator invalidation
                                            }

                                            ImGui::PopID();
                                        }
                                        ImGui::TreePop();
                                    }
                                    break;
                                }
                                case MONO_TYPE_CHAR: // char
                                {
                                    std::vector<char>* val = nullptr;
                                    for (auto& [name, value] : logic.charLists)
                                    {
                                        if (name != fieldName) continue;
                                        val = &value;
                                        break;
                                    }
                                    if (val == nullptr)
                                    {
                                        logic.charLists.push_back(
                                            std::make_pair(fieldName, PopplioScriptAPI::MonoToArray1D<char>(
                                                mono.GetArray(mono.Get(scriptObj, varField)), &mono.GetAppDomain())));
                                        val = &logic.charLists.back().second;
                                    }

                                    std::vector<char> temp = *val;

                                    if (ImGui::TreeNode("Elements"))
                                    {
                                        if (ImGui::Button("Add Element"))
                                        {
                                            if (!isChanged)
                                            {
                                                oldState = logic;
                                                isChanged = true;
                                            }
                                            temp.push_back(' ');
                                            *val = temp;
                                        }

                                        for (std::vector<char>::const_iterator i{ temp.begin() }; i != temp.end(); ++i)
                                        {
                                            ImGui::PushID(-static_cast<int>(i - temp.begin()));

                                            char c = *i;
                                            char ch[2]{ c, '\0' };
                                            if (ImGui::InputText("##char", ch, IM_ARRAYSIZE(ch)))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp[i - temp.begin()] = *ch;
                                                *val = temp;
                                            }
                                            if (ImGui::IsItemActive()) isInputting = true;
                                            ImGui::SameLine();

                                            if (ImGui::Button("Remove"))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp.erase(i);
                                                *val = temp;

                                                ImGui::PopID();
                                                break; // skip the rest to avoid crashing due to iterator invalidation
                                            }

                                            ImGui::PopID();
                                        }
                                        ImGui::TreePop();
                                    }
                                    break;
                                }
                                case MONO_TYPE_STRING: // string
                                {
                                    std::vector<std::string>* val = nullptr;
                                    for (auto& [name, value] : logic.stringLists)
                                    {
                                        if (name != fieldName) continue;
                                        val = &value;
                                        break;
                                    }
                                    if (val == nullptr)
                                    {
                                        logic.stringLists.push_back(
                                            std::make_pair(fieldName, PopplioScriptAPI::MonoToArray1D<std::string>(
                                                mono.GetArray(mono.Get(scriptObj, varField)), &mono.GetAppDomain())));
                                        val = &logic.stringLists.back().second;
                                    }

                                    std::vector<std::string> temp = *val;

                                    if (ImGui::TreeNode("Elements"))
                                    {
                                        if (ImGui::Button("Add Element"))
                                        {
                                            if (!isChanged)
                                            {
                                                oldState = logic;
                                                isChanged = true;
                                            }
                                            temp.push_back("");
                                            *val = temp;
                                        }

                                        for (std::vector<std::string>::const_iterator i{ temp.begin() }; i != temp.end(); ++i)
                                        {
                                            ImGui::PushID(-static_cast<int>(i - temp.begin()));
                                            char str[POPPLIO_MAX_STRING]{ "" };
                                            strcat_s(str, i->c_str());
                                            if (ImGui::InputText("##string", str, POPPLIO_MAX_STRING))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp[i - temp.begin()] = str;
                                                *val = temp;
                                            }
                                            if (ImGui::IsItemActive()) isInputting = true;
                                            ImGui::SameLine();

                                            if (ImGui::Button("Remove"))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp.erase(i);
                                                *val = temp;

                                                ImGui::PopID();
                                                break; // skip the rest to avoid crashing due to iterator invalidation
                                            }

                                            ImGui::PopID();
                                        }
                                        ImGui::TreePop();
                                    }
                                    break;
                                }
                                case MONO_TYPE_CLASS: // class
                                {
                                    // Entity
                                    if (strcmp(mono_type_get_name(elementType), "Team_Popplio.ECS.Entity") == 0)
                                    {
                                        std::vector<int>* id = nullptr;
                                        for (auto& [name, value] : logic.entityLists)
                                        {
                                            if (name != fieldName) continue;
                                            id = &value;
                                            break;
                                        }
                                        if (id == nullptr)
                                        {
                                            MonoClassField* entityIdField = mono_class_get_field_from_name(
                                                mono.GetAssClass("Team_Popplio.ECS", "Entity"), "_NativeID");

                                            std::vector<MonoObject*> objects = PopplioScriptAPI::MonoToArray1D<MonoObject*>(
                                                mono.GetArray(mono.Get(scriptObj, varField)), &mono.GetAppDomain());

                                            std::vector<int> value{};
                                            for (MonoObject* obj : objects)
                                            {
                                                int val = mono.Get<int>(obj, entityIdField);
                                                value.push_back(val);
                                            }

                                            logic.entityLists.push_back(std::make_pair(fieldName, value));
                                            id = &logic.entityLists.back().second;
                                        }

                                        std::vector<int> temp = *id;

                                        if (ImGui::TreeNode("Elements"))
                                        {
                                            if (ImGui::Button("Add Element"))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp.push_back(-1);
                                                *id = temp;
                                            }

                                            for (std::vector<int>::const_iterator i{ temp.begin() }; i != temp.end(); ++i)
                                            {
                                                if (!reg.EntityExists(*i) && editorState != State::EDITING) continue; // ignore destroyed entities

                                                ImGui::PushID(-static_cast<int>(i - temp.begin()));

                                                try
                                                {
                                                    std::string entityName = *i < 0 ? "None" :
                                                        std::to_string(*i) + " [" +
                                                        reg.GetEntityTag(reg.GetEntityById(*i)).c_str() + "]";

                                                    //if (ImGui::Button("Clear"))
                                                    //{
                                                    //    if (!isChanged)
                                                    //    {
                                                    //        oldState = logic;
                                                    //        isChanged = true;
                                                    //    }
                                                    //    temp[i - temp.begin()] = -1;
                                                    //    *id = temp;
                                                    //}

                                                    if (ImGui::BeginCombo("##ScriptEntity", entityName.c_str()))
                                                    {
                                                        for (auto& entity : reg.GetAllEntitiesMutable())
                                                        {
                                                            entityName = "Entity " + std::to_string(entity.GetId());
                                                            if (reg.EntityHasAnyTag(entity))
                                                            {
                                                                entityName += " [" + reg.GetEntityTag(entity) + "]";
                                                            }
                                                            bool isSelected = (*i == entity.GetId());
                                                            if (ImGui::Selectable(entityName.c_str()))
                                                            {
                                                                if (!isChanged)
                                                                {
                                                                    oldState = logic;
                                                                    isChanged = true;
                                                                }
                                                                temp[i - temp.begin()] = entity.GetId();
                                                                *id = temp;
                                                            }
                                                            if (isSelected) ImGui::SetItemDefaultFocus();
                                                        }
                                                        ImGui::EndCombo();
                                                    }
                                                }
                                                catch (const std::exception& e)
                                                {
                                                    std::stringstream ss{ "" };
                                                    ss << "C# script variable | Failed to display entity | " << e.what();
                                                    Logger::Error(ss.str());
                                                }

                                                ImGui::SameLine();

                                                if (ImGui::Button("Remove"))
                                                {
                                                    if (!isChanged)
                                                    {
                                                        oldState = logic;
                                                        isChanged = true;
                                                    }
                                                    temp.erase(i);
                                                    *id = temp;

                                                    ImGui::PopID();
                                                    break; // skip the rest to avoid crashing due to iterator invalidation
                                                }

                                                ImGui::PopID();
                                            }
                                            ImGui::TreePop();
                                        }
                                    }
                                    else
                                    {
                                        std::stringstream ss{ "" };
                                        ss << "Unsupported class type in C# script variable: " << mono_type_get_name(fieldType)
                                            << " | " << fieldName;
                                        Logger::Warning(ss.str());
                                    }
                                    break;
                                }
                                default:
                                {
                                    std::stringstream ss{ "" };
                                    ss << "Unsupported type in C# script array variable: " << mono_type_get_name(fieldType)
                                        << " | " << fieldName << " | TypeEnum: " << typeEnum;
                                    Logger::Warning(ss.str());
                                    break;
                                }
                            }
                            break;
                        }
                        case MONO_TYPE_GENERICINST: // generic instance (List<T>, etc)
                        {
                            if (true) break; // TODO FIX
                            //MonoClass* arrayClass = mono_type_get_class(fieldType);
                            MonoClass* arrayClass = mono_object_get_class(fieldObj);
                            //MonoClass* elementClass = mono_class_get_element_class(arrayClass);
                            //MonoType* elementType = mono_class_get_type(elementClass);
                            //MonoTypeEnum elementTypeEnum = static_cast<MonoTypeEnum>(mono_type_get_type(elementType));

                            MonoType* arrayType = mono_class_get_type(arrayClass);

                            if (arrayType == nullptr)
                            {
                                std::stringstream ss{ "" };
                                ss << "EditComponent | LogicComponent | C# | MONO_TYPE_GENERICINST | Failed to get array type for "
                                    << mono_type_get_name(fieldType);
                                Logger::Error(ss.str());
                                break;
                            }

                            MonoClassField* genericListItems = nullptr;
                            try
                            {
                                genericListItems = mono_class_get_field_from_name(arrayClass, "_items");
                            }
                            catch (const std::exception& e)
                            {
                                std::stringstream ss{ "" };
                                ss << "EditComponent | LogicComponent | C# | MONO_TYPE_GENERICINST | Failed to get generic list items field for "
                                    << mono_type_get_name(arrayType) << " | " << e.what();
                                Logger::Error(ss.str());
                                break;
                            }

                            MonoArray* genericListItemsArray = nullptr;

                            if (genericListItems == nullptr)
                            {
                                std::stringstream ss{ "" };
                                ss << "EditComponent | LogicComponent | C# | MONO_TYPE_GENERICINST | Failed to get generic list items field for "
                                    << mono_type_get_name(arrayType);
                                Logger::Error(ss.str());
                                break;
                            }

                            mono_field_get_value(mono.Get(scriptObj, varField), genericListItems, &genericListItemsArray);

                            if (genericListItemsArray == nullptr)
                            {
                                std::stringstream ss{ "" };
                                ss << "EditComponent | LogicComponent | C# | MONO_TYPE_GENERICINST | Failed to get generic list items array for "
                                    << mono_type_get_name(arrayType);
                                Logger::Error(ss.str());
                                break;
                            }

                            MonoType* itemType = mono_field_get_type(genericListItems);
                            MonoClass* elementClass = mono_class_get_element_class(mono_type_get_class(itemType));
                            MonoType* elementType = mono_class_get_type(elementClass);
                            MonoTypeEnum elementTypeEnum = static_cast<MonoTypeEnum>(mono_type_get_type(elementType));


                            switch (elementTypeEnum)
                            {
                                case MONO_TYPE_BOOLEAN:
                                {
                                    std::vector<bool>* val = nullptr;
                                    for (auto& [name, value] : logic.boolLists)
                                    {
                                        if (name != fieldName) continue;
                                        val = &value;
                                        break;
                                    }
                                    if (val == nullptr)
                                    {
                                        logic.boolLists.push_back(
                                            std::make_pair(fieldName, PopplioScriptAPI::MonoToArray1D<bool>(
                                                genericListItemsArray, &mono.GetAppDomain())));
                                        val = &logic.boolLists.back().second;
                                    }

                                    std::vector<bool> temp = *val;

                                    if (ImGui::TreeNode("Elements"))
                                    {
                                        if (ImGui::Button("Add Element"))
                                        {
                                            if (!isChanged)
                                            {
                                                oldState = logic;
                                                isChanged = true;
                                            }
                                            temp.push_back(false);
                                            *val = temp;
                                        }

                                        for (std::vector<bool>::const_iterator i{ temp.begin() }; i != temp.end(); ++i)
                                        {
                                            bool b = *i;

                                            ImGui::PushID(-static_cast<int>(i - temp.begin()));
                                            if (ImGui::Checkbox("##bool", &b))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp[i - temp.begin()] = b;
                                                *val = temp;
                                            }
                                            ImGui::SameLine();

                                            if (ImGui::Button("Remove"))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp.erase(i);
                                                *val = temp;

                                                ImGui::PopID();
                                                break; // skip the rest to avoid crashing due to iterator invalidation
                                            }

                                            ImGui::PopID();
                                        }
                                        ImGui::TreePop();
                                    }

                                    break;
                                }
                                case MONO_TYPE_I4: // int
                                {
                                    std::vector<int>* val = nullptr;
                                    for (auto& [name, value] : logic.intLists)
                                    {
                                        if (name != fieldName) continue;
                                        val = &value;
                                        break;
                                    }
                                    if (val == nullptr)
                                    {
                                        logic.intLists.push_back(
                                            std::make_pair(fieldName, PopplioScriptAPI::MonoToArray1D<int>(
                                                genericListItemsArray, &mono.GetAppDomain())));
                                        val = &logic.intLists.back().second;
                                    }

                                    std::vector<int> temp = *val;

                                    if (ImGui::TreeNode("Elements"))
                                    {
                                        if (ImGui::Button("Add Element"))
                                        {
                                            if (!isChanged)
                                            {
                                                oldState = logic;
                                                isChanged = true;
                                            }
                                            temp.push_back(0);
                                            *val = temp;
                                        }

                                        for (std::vector<int>::const_iterator i{ temp.begin() }; i != temp.end(); ++i)
                                        {
                                            ImGui::PushID(-static_cast<int>(i - temp.begin()));
                                            int integer = *i;
                                            if (ImGui::InputInt("##int", &integer))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp[i - temp.begin()] = integer;
                                                *val = temp;
                                            }
                                            if (ImGui::IsItemActive()) isInputting = true;
                                            ImGui::SameLine();

                                            if (ImGui::Button("Remove"))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp.erase(i);
                                                *val = temp;

                                                ImGui::PopID();
                                                break; // skip the rest to avoid crashing due to iterator invalidation
                                            }

                                            ImGui::PopID();
                                        }
                                        ImGui::TreePop();
                                    }
                                    break;
                                }
                                case MONO_TYPE_R4: // float
                                {
                                    std::vector<float>* val = nullptr;
                                    for (auto& [name, value] : logic.floatLists)
                                    {
                                        if (name != fieldName) continue;
                                        val = &value;
                                        break;
                                    }
                                    if (val == nullptr)
                                    {
                                        logic.floatLists.push_back(
                                            std::make_pair(fieldName, PopplioScriptAPI::MonoToArray1D<float>(
                                                genericListItemsArray, &mono.GetAppDomain())));
                                        val = &logic.floatLists.back().second;
                                    }

                                    std::vector<float> temp = *val;

                                    if (ImGui::TreeNode("Elements"))
                                    {
                                        if (ImGui::Button("Add Element"))
                                        {
                                            if (!isChanged)
                                            {
                                                oldState = logic;
                                                isChanged = true;
                                            }
                                            temp.push_back(0.f);
                                            *val = temp;
                                        }

                                        for (std::vector<float>::const_iterator i{ temp.begin() }; i != temp.end(); ++i)
                                        {
                                            ImGui::PushID(-static_cast<int>(i - temp.begin()));
                                            float f = *i;
                                            if (ImGui::InputFloat("##float", &f))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp[i - temp.begin()] = f;
                                                *val = temp;
                                            }
                                            if (ImGui::IsItemActive()) isInputting = true;
                                            ImGui::SameLine();

                                            if (ImGui::Button("Remove"))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp.erase(i);
                                                *val = temp;

                                                ImGui::PopID();
                                                break; // skip the rest to avoid crashing due to iterator invalidation
                                            }

                                            ImGui::PopID();
                                        }
                                        ImGui::TreePop();
                                    }
                                    break;
                                }
                                case MONO_TYPE_R8: // double
                                {
                                    std::vector<double>* val = nullptr;
                                    for (auto& [name, value] : logic.doubleLists)
                                    {
                                        if (name != fieldName) continue;
                                        val = &value;
                                        break;
                                    }
                                    if (val == nullptr)
                                    {
                                        logic.doubleLists.push_back(
                                            std::make_pair(fieldName, PopplioScriptAPI::MonoToArray1D<double>(
                                                genericListItemsArray, &mono.GetAppDomain())));
                                        val = &logic.doubleLists.back().second;
                                    }

                                    std::vector<double> temp = *val;

                                    if (ImGui::TreeNode("Elements"))
                                    {
                                        if (ImGui::Button("Add Element"))
                                        {
                                            if (!isChanged)
                                            {
                                                oldState = logic;
                                                isChanged = true;
                                            }
                                            temp.push_back(0.);
                                            *val = temp;
                                        }

                                        for (std::vector<double>::const_iterator i{ temp.begin() }; i != temp.end(); ++i)
                                        {
                                            ImGui::PushID(-static_cast<int>(i - temp.begin()));
                                            double d = *i;
                                            if (ImGui::InputDouble("##double", &d))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp[i - temp.begin()] = d;
                                                *val = temp;
                                            }
                                            if (ImGui::IsItemActive()) isInputting = true;
                                            ImGui::SameLine();

                                            if (ImGui::Button("Remove"))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp.erase(i);
                                                *val = temp;

                                                ImGui::PopID();
                                                break; // skip the rest to avoid crashing due to iterator invalidation
                                            }

                                            ImGui::PopID();
                                        }
                                        ImGui::TreePop();
                                    }
                                    break;
                                }
                                case MONO_TYPE_CHAR: // char
                                {
                                    std::vector<char>* val = nullptr;
                                    for (auto& [name, value] : logic.charLists)
                                    {
                                        if (name != fieldName) continue;
                                        val = &value;
                                        break;
                                    }
                                    if (val == nullptr)
                                    {
                                        logic.charLists.push_back(
                                            std::make_pair(fieldName, PopplioScriptAPI::MonoToArray1D<char>(
                                                genericListItemsArray, &mono.GetAppDomain())));
                                        val = &logic.charLists.back().second;
                                    }

                                    std::vector<char> temp = *val;

                                    if (ImGui::TreeNode("Elements"))
                                    {
                                        if (ImGui::Button("Add Element"))
                                        {
                                            if (!isChanged)
                                            {
                                                oldState = logic;
                                                isChanged = true;
                                            }
                                            temp.push_back(' ');
                                            *val = temp;
                                        }

                                        for (std::vector<char>::const_iterator i{ temp.begin() }; i != temp.end(); ++i)
                                        {
                                            ImGui::PushID(-static_cast<int>(i - temp.begin()));

                                            char c = *i;
                                            char ch[2]{ c, '\0' };
                                            if (ImGui::InputText("##char", ch, IM_ARRAYSIZE(ch)))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp[i - temp.begin()] = *ch;
                                                *val = temp;
                                            }
                                            if (ImGui::IsItemActive()) isInputting = true;
                                            ImGui::SameLine();

                                            if (ImGui::Button("Remove"))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp.erase(i);
                                                *val = temp;

                                                ImGui::PopID();
                                                break; // skip the rest to avoid crashing due to iterator invalidation
                                            }

                                            ImGui::PopID();
                                        }
                                        ImGui::TreePop();
                                    }
                                    break;
                                }
                                case MONO_TYPE_STRING: // string
                                {
                                    std::vector<std::string>* val = nullptr;
                                    for (auto& [name, value] : logic.stringLists)
                                    {
                                        if (name != fieldName) continue;
                                        val = &value;
                                        break;
                                    }
                                    if (val == nullptr)
                                    {
                                        logic.stringLists.push_back(
                                            std::make_pair(fieldName, PopplioScriptAPI::MonoToArray1D<std::string>(
                                                genericListItemsArray, &mono.GetAppDomain())));
                                        val = &logic.stringLists.back().second;
                                    }

                                    std::vector<std::string> temp = *val;

                                    if (ImGui::TreeNode("Elements"))
                                    {
                                        if (ImGui::Button("Add Element"))
                                        {
                                            if (!isChanged)
                                            {
                                                oldState = logic;
                                                isChanged = true;
                                            }
                                            temp.push_back("");
                                            *val = temp;
                                        }

                                        for (std::vector<std::string>::const_iterator i{ temp.begin() }; i != temp.end(); ++i)
                                        {
                                            ImGui::PushID(-static_cast<int>(i - temp.begin()));
                                            char str[POPPLIO_MAX_STRING]{ "" };
                                            strcat_s(str, i->c_str());
                                            if (ImGui::InputText("##string", str, POPPLIO_MAX_STRING))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp[i - temp.begin()] = str;
                                                *val = temp;
                                            }
                                            if (ImGui::IsItemActive()) isInputting = true;
                                            ImGui::SameLine();

                                            if (ImGui::Button("Remove"))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp.erase(i);
                                                *val = temp;

                                                ImGui::PopID();
                                                break; // skip the rest to avoid crashing due to iterator invalidation
                                            }

                                            ImGui::PopID();
                                        }
                                        ImGui::TreePop();
                                    }
                                    break;
                                }
                                case MONO_TYPE_CLASS: // class
                                {
                                    std::string csType = mono_type_get_name(elementType);
                                    // Entity
                                    if (csType == "Team_Popplio.ECS.Entity")
                                    {
                                        std::vector<int>* id = nullptr;
                                        for (auto& [name, value] : logic.entityLists)
                                        {
                                            if (name != fieldName) continue;
                                            id = &value;
                                            break;
                                        }
                                        if (id == nullptr)
                                        {
                                            MonoClassField* entityIdField = mono_class_get_field_from_name(
                                                mono.GetAssClass("Team_Popplio.ECS", "Entity"), "_NativeID");

                                            std::vector<MonoObject*> objects = PopplioScriptAPI::MonoToArray1D<MonoObject*>(
                                                genericListItemsArray, &mono.GetAppDomain());

                                            std::vector<int> value{};
                                            for (MonoObject* obj : objects)
                                            {
                                                int val = mono.Get<int>(obj, entityIdField);
                                                value.push_back(val);
                                            }

                                            logic.entityLists.push_back(std::make_pair(fieldName, value));
                                            id = &logic.entityLists.back().second;
                                        }

                                        std::vector<int> temp = *id;

                                        if (ImGui::TreeNode("Elements"))
                                        {
                                            if (ImGui::Button("Add Element"))
                                            {
                                                if (!isChanged)
                                                {
                                                    oldState = logic;
                                                    isChanged = true;
                                                }
                                                temp.push_back(-1);
                                                *id = temp;
                                            }

                                            for (std::vector<int>::const_iterator i{ temp.begin() }; i != temp.end(); ++i)
                                            {
                                                if (!reg.EntityExists(*i) && editorState != State::EDITING) continue; // ignore destroyed entities

                                                ImGui::PushID(-static_cast<int>(i - temp.begin()));

                                                try
                                                {
                                                    std::string entityName = *i < 0 ? "None" :
                                                        std::to_string(*i) + " [" +
                                                        reg.GetEntityTag(reg.GetEntityById(*i)).c_str() + "]";

                                                    //if (ImGui::Button("Clear"))
                                                    //{
                                                    //    if (!isChanged)
                                                    //    {
                                                    //        oldState = logic;
                                                    //        isChanged = true;
                                                    //    }
                                                    //    temp[i - temp.begin()] = -1;
                                                    //    *id = temp;
                                                    //}

                                                    if (ImGui::BeginCombo("##ScriptEntity", entityName.c_str()))
                                                    {
                                                        for (auto& entity : reg.GetAllEntitiesMutable())
                                                        {
                                                            entityName = "Entity " + std::to_string(entity.GetId());
                                                            if (reg.EntityHasAnyTag(entity))
                                                            {
                                                                entityName += " [" + reg.GetEntityTag(entity) + "]";
                                                            }
                                                            bool isSelected = (*i == entity.GetId());
                                                            if (ImGui::Selectable(entityName.c_str()))
                                                            {
                                                                if (!isChanged)
                                                                {
                                                                    oldState = logic;
                                                                    isChanged = true;
                                                                }
                                                                temp[i - temp.begin()] = entity.GetId();
                                                                *id = temp;
                                                            }
                                                            if (isSelected) ImGui::SetItemDefaultFocus();
                                                        }
                                                        ImGui::EndCombo();
                                                    }
                                                }
                                                catch (const std::exception& e)
                                                {
                                                    std::stringstream ss{ "" };
                                                    ss << "C# script variable | Failed to display entity | " << e.what();
                                                    Logger::Error(ss.str());
                                                }

                                                ImGui::SameLine();

                                                if (ImGui::Button("Remove"))
                                                {
                                                    if (!isChanged)
                                                    {
                                                        oldState = logic;
                                                        isChanged = true;
                                                    }
                                                    temp.erase(i);
                                                    *id = temp;

                                                    ImGui::PopID();
                                                    break; // skip the rest to avoid crashing due to iterator invalidation
                                                }

                                                ImGui::PopID();
                                            }
                                            ImGui::TreePop();
                                        }
                                    }
                                    // prefab TODO
                                    else if (csType == "Team_Popplio.ECS.Prefab")
                                    {

                                    }
                                    else if (mono.IsLoadedUserClass(csType)) // TODO script
                                    {

                                    }
                                    else
                                    {
                                        std::stringstream ss{ "" };
                                        ss << "Unsupported class type in C# script variable: " << mono_type_get_name(fieldType)
                                            << " | " << fieldName;
                                        Logger::Warning(ss.str());
                                    }
                                    break;
                                }
                                default:
                                {
                                    std::stringstream ss{ "" };
                                    ss << "Unsupported type in C# script array variable: " << mono_type_get_name(fieldType)
                                        << " | " << fieldName << " | TypeEnum: " << typeEnum;
                                    Logger::Warning(ss.str());
                                    break;
                                }
                            }
                            break;
                        }
                        default:
                        {
                            std::stringstream ss{ "" };
                            ss << "Unsupported type in C# script variable: " << mono_type_get_name(fieldType)
                                << " | " << fieldName << " | TypeEnum: " << typeEnum;
                            Logger::Warning(ss.str());
                            break;
                        }
                    }
                    ImGui::EndGroup();

                    ImGui::PopID();

                    mono_custom_attrs_free(varAttr);
                }
            }
            catch (const std::exception& e)
            {
                if (varAttr) mono_custom_attrs_free(varAttr);

                std::stringstream ss{ "" };
                ss << "Error while iterating over fields: " << e.what();
                Logger::Error(ss.str());
            }
        }

        // emit change event
        if (isChanged && !InputSystem::IsMouseButtonDown(InputSystem::MouseButton::Left) && !isInputting) // change detection
        {
            eventBus.EmitEvent<SceneChangeEvent<std::any>>(*selectedEntity, logic, oldState, ChangeAction::MODIFY_COMPONENT);
            isChanged = false;
        }

        //if (logic.scriptLang != scriptLang)
        //{
        //    logic.scriptLang = scriptLang;
        //}

        //if (!isCSharp)
        //{
        //    // emit change event
        //    if (isChanged) // change detection
        //    {
        //        eventBus.EmitEvent<SceneChangeEvent<std::any>>(*selectedEntity, logic, oldState, ChangeAction::MODIFY);
        //        isChanged = false;
        //    }
        //    return;
        //}

        //if (true)
        //{
        //    // emit change event
        //    if (isChanged) // change detection
        //    {
        //        eventBus.EmitEvent<SceneChangeEvent<std::any>>(*selectedEntity, logic, oldState, ChangeAction::MODIFY);
        //        isChanged = false;
        //    }
        //    return; // skip for now
        //}

        //return; // skip for now
    }

    template <>
    void Editor::EditComponent(AnimationComponent& animation)
    {
        // change detection
        static bool isChanged = false;
        static AnimationComponent oldState;

        bool isInputting = false;

        bool a{ animation.isActive };
        if (ImGui::Checkbox("Active", &a))
        {
            if (!isChanged)
            {
                oldState = animation;
                isChanged = true;
            }
            animation.isActive = a;
        }

        // Check if there's a current animation and load its properties into edit fields if not already loaded
        if (!animation.propertiesLoaded && !animation.currentAnimation.empty() && animation.animations.count(animation.currentAnimation) > 0)
        {
            const auto& currentAnim = animation.animations[animation.currentAnimation];
            animation.editStartX = currentAnim.startX;
            animation.editStartY = currentAnim.startY;
            animation.editEndX = currentAnim.endX;
            animation.editEndY = currentAnim.endY;
            animation.editFrameWidth = currentAnim.frameWidth;
            animation.editFrameHeight = currentAnim.frameHeight;
            animation.editAnimSpeed = currentAnim.animationSpeed;
            animation.propertiesLoaded = true;
        }

        float tempF = 0.f;
        int tempI = 0;

        tempF = animation.sheetWidth;
        if (ImGui::InputFloat("Spritesheet Width", &tempF))
        {
            if (!isChanged)
            {
                oldState = animation;
                isChanged = true;
            }
            animation.sheetWidth = tempF;
        }
        if (ImGui::IsItemActive()) isInputting = true;

        tempF = animation.sheetHeight;
        if (ImGui::InputFloat("Spritesheet Height", &tempF))
        {
            if (!isChanged)
            {
                oldState = animation;
                isChanged = true;
            }
            animation.sheetHeight = tempF;
        }
        if (ImGui::IsItemActive()) isInputting = true;

        bool tempB = !animation.playOnce;
        if (ImGui::Checkbox("Loop", &tempB))
        {
            if (!isChanged)
            {
                oldState = animation;
                isChanged = true;
            }
            animation.playOnce = !tempB;
        }

        // Animation selection
        ImGui::BeginGroup();
        ImGui::Text("Animation:");
        ImGui::SameLine();

        static bool isPlaying = false;

        if (ImGui::SmallButton("Play"))
        {
            animation.Play();
            isPlaying = true;
        }

        ImGui::SameLine();

        if (ImGui::SmallButton("Pause"))
        {
            animation.Pause();
            isPlaying = false;
        }

        ImGui::SameLine();

        if (ImGui::SmallButton("Stop"))
        {
            animation.Stop();
            isPlaying = false;
        }

        if (isPlaying)
        {
            // Animation update
            PerformanceViewer::GetInstance()->start("animation");
            reg.GetSystem<AnimationSystem>().Update(static_cast<float>(Engine::timer.GetFixedDeltaTime()));
            PerformanceViewer::GetInstance()->end("animation");
        }

        if (ImGui::BeginCombo("Animation", animation.currentAnimation.c_str()))
        {
            for (const auto& animationPair : animation.animations)
            {
                bool isSelected = (animationPair.first == animation.currentAnimation);

                if (ImGui::Selectable(animationPair.first.c_str(), isSelected))
                {
                    if (!isChanged)
                    {
                        oldState = animation;
                        isChanged = true;
                    }

                    animation.SetAnimation(animationPair.first);

                    if (isSelected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }

                    // Load current animation properties into edit fields
                    const auto& currentAnim = animation.animations[animation.currentAnimation];
                    animation.editStartX = currentAnim.startX;
                    animation.editStartY = currentAnim.startY;
                    animation.editEndX = currentAnim.endX;
                    animation.editEndY = currentAnim.endY;
                    animation.editFrameWidth = currentAnim.frameWidth;
                    animation.editFrameHeight = currentAnim.frameHeight;
                    animation.editAnimSpeed = currentAnim.animationSpeed;
                    animation.propertiesLoaded = true;
                }
            }
            ImGui::EndCombo();
        }

        // Edit current animation properties
        if (!animation.currentAnimation.empty())
        {
            ImGui::SeparatorText("Edit Current Animation");

            int temp = animation.editStartX;
            if (ImGui::InputInt("Edit Start Frame X", &temp))
            {
                if (!isChanged)
                {
                    oldState = animation;
                    isChanged = true;
                }
                animation.editStartX = temp;
            }
            if (ImGui::IsItemActive()) isInputting = true;

            temp = animation.editStartY;
            if (ImGui::InputInt("Edit Start Frame Y", &temp))
            {
                if (!isChanged)
                {
                    oldState = animation;
                    isChanged = true;
                }
                animation.editStartY = temp;
            }
            if (ImGui::IsItemActive()) isInputting = true;

            temp = animation.editEndX;
            if (ImGui::InputInt("Edit End Frame X", &temp))
            {
                if (!isChanged)
                {
                    oldState = animation;
                    isChanged = true;
                }
                animation.editEndX = temp;
            }
            if (ImGui::IsItemActive()) isInputting = true;

            temp = animation.editEndY;
            if (ImGui::InputInt("Edit End Frame Y", &temp))
            {
                if (!isChanged)
                {
                    oldState = animation;
                    isChanged = true;
                }
                animation.editEndY = temp;
            }
            if (ImGui::IsItemActive()) isInputting = true;

            float temp2 = animation.editFrameWidth;
            if (ImGui::InputFloat("Edit Frame Width", &temp2))
            {
                if (!isChanged)
                {
                    oldState = animation;
                    isChanged = true;
                }
                animation.editFrameWidth = temp2;
            }
            if (ImGui::IsItemActive()) isInputting = true;

            temp2 = animation.editFrameHeight;
            if (ImGui::InputFloat("Edit Frame Height", &temp2))
            {
                if (!isChanged)
                {
                    oldState = animation;
                    isChanged = true;
                }
                animation.editFrameHeight = temp2;
            }
            if (ImGui::IsItemActive()) isInputting = true;

            //temp2 = animation.editAnimSpeed;
            //if (ImGui::InputFloat("Edit Speed", &temp2))
            //{
            //    if (!isChanged)
            //    {
            //        oldState = animation;
            //        isChanged = true;
            //    }
            //    animation.editAnimSpeed = temp2;
            //}
            //if (ImGui::IsItemActive()) isInputting = true;

            if (animation.editAnimSpeed == 0.f) temp2 = 0.f;
            else temp2 = 1.f / animation.editAnimSpeed;

            if (ImGui::InputFloat("Edit FPS", &temp2))
            {
                if (!isChanged)
                {
                    oldState = animation;
                    isChanged = true;
                }
                if (temp2 == 0.f) animation.editAnimSpeed = 0.f;
                else animation.editAnimSpeed = 1.f / temp2;
            }
            if (ImGui::IsItemActive()) isInputting = true;

            if (ImGui::Button("Update Animation"))
            {
                if (!isChanged)
                {
                    oldState = animation;
                    isChanged = true;
                }

                // Update the currently selected animation with new values
                auto& currentAnim = animation.animations[animation.currentAnimation];
                currentAnim.startX = animation.editStartX;
                currentAnim.startY = animation.editStartY;
                currentAnim.endX = animation.editEndX;
                currentAnim.endY = animation.editEndY;
                currentAnim.frameWidth = animation.editFrameWidth;
                currentAnim.frameHeight = animation.editFrameHeight;
                currentAnim.animationSpeed = animation.editAnimSpeed;

                // Force the animation system to apply the updated properties
                animation.SetAnimation(animation.currentAnimation);
            }

            ImGui::SameLine();

            // Add delete button for current animation
            if (ImGui::Button("Delete Animation"))
            {
                if (!isChanged)
                {
                    oldState = animation;
                    isChanged = true;
                }

                // Erase the current animation from the animations map
                animation.animations.erase(animation.currentAnimation);

                // Check if there are any other animations left
                if (animation.animations.size() > 0)
                {
                    // Set the new current animation to the first one in the map
                    animation.currentAnimation = animation.animations.begin()->first;
                    const auto& newAnim = animation.animations[animation.currentAnimation];
                    animation.currentFrameX = newAnim.startX;
                    animation.currentFrameY = newAnim.startY;
                    animation.timeAccumulator = 0.0f;
                    animation.isPaused = false;
                }
                else
                {
                    // If there are no more animations, clear the current animation
                    animation.currentAnimation.clear();
                    animation.currentFrameX = 0;
                    animation.currentFrameY = 0;
                    animation.timeAccumulator = 0.0f;
                    animation.isPaused = false;
                    animation.playOnce = false;
                }

                // Reset the properties loaded flag
                animation.propertiesLoaded = false;
            }
        }

        // Add new animation state
        if (ImGui::CollapsingHeader("Add Animation State"))
        {
            char tempS[POPPLIO_MAX_STRING] {};
            if (animation.newAnimName[0] != '\0') strncpy_s(tempS, animation.newAnimName, POPPLIO_MAX_STRING);
            if (ImGui::InputText("Name", tempS, POPPLIO_MAX_STRING))
            {
                if (!isChanged)
                {
                    oldState = animation;
                    isChanged = true;
                }
                strncpy_s(animation.newAnimName, tempS, POPPLIO_MAX_STRING);
            }
            if (ImGui::IsItemActive()) isInputting = true;

            //float tempF = 0.f;
            //int tempI = 0;

            // Show sheet width and height input only if animations are empty
            //if (animation.animations.empty())
            //{
            //    tempF = animation.sheetWidth;
            //    if (ImGui::InputFloat("Spritesheet Width", &tempF))
            //    {
            //        if (!isChanged)
            //        {
            //            oldState = animation;
            //            isChanged = true;
            //        }
            //        animation.sheetWidth = tempF;
            //    }
            //    if (ImGui::IsItemActive()) isInputting = true;

            //    tempF = animation.sheetHeight;
            //    if (ImGui::InputFloat("Spritesheet Height", &tempF))
            //    {
            //        if (!isChanged)
            //        {
            //            oldState = animation;
            //            isChanged = true;
            //        }
            //        animation.sheetHeight = tempF;
            //    }
            //    if (ImGui::IsItemActive()) isInputting = true;
            //}

            tempI = animation.newStartX;
            if (ImGui::InputInt("Start Frame X", &tempI))
            {
                if (!isChanged)
                {
                    oldState = animation;
                    isChanged = true;
                }
                animation.newStartX = tempI;
            }
            if (ImGui::IsItemActive()) isInputting = true;

            tempI = animation.newStartY;
            if (ImGui::InputInt("Start Frame Y", &tempI))
            {
                if (!isChanged)
                {
                    oldState = animation;
                    isChanged = true;
                }
                animation.newStartY = tempI;
            }
            if (ImGui::IsItemActive()) isInputting = true;

            tempI = animation.newEndX;
            if (ImGui::InputInt("End Frame X", &tempI))
            {
                if (!isChanged)
                {
                    oldState = animation;
                    isChanged = true;
                }
                animation.newEndX = tempI;
            }
            if (ImGui::IsItemActive()) isInputting = true;

            tempI = animation.newEndY;
            if (ImGui::InputInt("End Frame Y", &tempI))
            {
                if (!isChanged)
                {
                    oldState = animation;
                    isChanged = true;
                }
                animation.newEndY = tempI;
            }
            if (ImGui::IsItemActive()) isInputting = true;

            tempF = animation.newFrameWidth;
            if (ImGui::InputFloat("Frame Width", &tempF))
            {
                if (!isChanged)
                {
                    oldState = animation;
                    isChanged = true;
                }
                animation.newFrameWidth = tempF;
            }
            if (ImGui::IsItemActive()) isInputting = true;

            tempF = animation.newFrameHeight;
            if (ImGui::InputFloat("Frame Height", &tempF))
            {
                if (!isChanged)
                {
                    oldState = animation;
                    isChanged = true;
                }
                animation.newFrameHeight = tempF;
            }
            if (ImGui::IsItemActive()) isInputting = true;

            if (animation.newAnimSpeed == 0.f) tempF = 0.f;
            else tempF = 1.f / animation.newAnimSpeed;

            if (ImGui::InputFloat("FPS", &tempF))
            {
                if (!isChanged)
                {
                    oldState = animation;
                    isChanged = true;
                }

                if (tempF == 0.f) animation.newAnimSpeed = 0.f;
                else animation.newAnimSpeed = 1.f / tempF;
            }
            if (ImGui::IsItemActive()) isInputting = true;

            if (ImGui::Button("Add Animation"))
            {
                // Only add if name is not empty
                if (strlen(animation.newAnimName) > 0)
                {
                    if (!isChanged)
                    {
                        oldState = animation;
                        isChanged = true;
                    }

                    // If this is the first animation, check sheet dimensions
                    if (animation.animations.empty())
                    {
                        // Check if sheet dimensions are valid, if so initialize the animation with spritesheet dimension
                        if (animation.sheetWidth > 0.0f && animation.sheetHeight > 0.0f)
                        {
                            animation.Initialize(animation.sheetWidth, animation.sheetHeight);
                        }
                        else
                        {
                           
                            ImGui::OpenPopup("Invalid Sheet Dimensions");
                            return;
                        }
                    }

                    animation.AddAnimation(animation.newAnimName, animation.newStartX, animation.newStartY,
                        animation.newEndX, animation.newEndY, animation.newAnimSpeed,
                        animation.newFrameWidth, animation.newFrameHeight);

                    // Automatically set the first animation as current if none selected
                    if (animation.animations.size() == 1)
                    {
                        animation.currentAnimation = animation.newAnimName;

                        // Set this animation as the active one
                        animation.SetAnimation(animation.currentAnimation);

                        // Load properties for display in the editor
                        const auto& firstAnim = animation.animations[animation.currentAnimation];
                        animation.editStartX = firstAnim.startX;
                        animation.editStartY = firstAnim.startY;
                        animation.editEndX = firstAnim.endX;
                        animation.editEndY = firstAnim.endY;
                        animation.editFrameWidth = firstAnim.frameWidth;
                        animation.editFrameHeight = firstAnim.frameHeight;
                        animation.editAnimSpeed = firstAnim.animationSpeed;
                        animation.propertiesLoaded = true;
                    }

                    // Reset fields after adding a new animation
                    memset(animation.newAnimName, 0, sizeof(animation.newAnimName));
                    animation.newStartX = animation.newStartY = animation.newEndX = animation.newEndY = 0;
                    animation.newFrameWidth = animation.newFrameHeight = animation.newAnimSpeed = 1.0f;
                }
            }
        }

        ImGui::EndGroup();

        // emit change event
        if (isChanged && !isInputting) // change detection
        {
            eventBus.EmitEvent<SceneChangeEvent<std::any>>(*selectedEntity, animation, oldState, ChangeAction::MODIFY_COMPONENT);
            isChanged = false;
        }
    }

    template <>
    void Editor::EditComponent(AudioComponent& audio) // todo: show fmod interface
    {
        // change detection
        static bool isChanged = false;
        static AudioComponent oldState;

        //char buffer[256]{};

        //if (ImGui::InputText("Audio File", buffer, sizeof(buffer))) audio.audio = buffer;

        bool active{ audio.isActive };
        if (ImGui::Checkbox("Active", &active))
        {
            if (!isChanged)
            {
                oldState = audio;
                isChanged = true;
            }
            audio.isActive = active;
        }

        // Script selection
        ImGui::BeginGroup();
        ImGui::Text("Audio:");
        ImGui::SameLine();

        if (ImGui::BeginCombo("##Audio", audio.audio.c_str()))
        {
            for (const auto& [name, a] : AssetStore::GetAllAudio())
            {
                bool isSelected = (audio.audio == name);

                if (ImGui::Selectable(name.c_str(), isSelected))
                {
                    if (!isChanged)
                    {
                        oldState = audio;
                        isChanged = true;
                    }
                    audio.audio = name;

                    //audio.Update(reg.GetSystem<AudioSystem>());

                    //if (audio.data != nullptr)
                    //{
                    //    reg.GetSystem<AudioSystem>().RemoveAudioInst(audio.data->GetIndex());
                    //    //audio.data.reset();
                    //}

                    if (reg.GetSystem<AudioSystem>().GetAudioInst(audio.data).GetPlaying())
                    {
                        reg.GetSystem<AudioSystem>().GetAudioInst(audio.data).Stop();
                    }

                    try
                    {
                        audio.data = reg.GetSystem<AudioSystem>().InstAudio(audio.audio);
                        if (audio.data < 0) throw std::runtime_error("Failed to instantiate Audio");
                        Audio& data = reg.GetSystem<AudioSystem>().GetAudioInst(audio.data);
                        data.SetVolume(audio.volume);
                        data.SetMute(audio.mute);
                        data.SetLoop(audio.loop);
                        data.SetBPM(audio.bpm);
                        data.SetTimeSig(audio.timeSig);
                        data.playOnAwake = audio.playOnAwake;
                        data.SetGroup(reg.GetSystem<AudioSystem>().GetCGroup(audio.grp));
                    }
                    catch (const std::exception& e)
                    {
                        std::stringstream ss{ "" };
                        ss << "AudioComponent: " << e.what();
                        Logger::Error(ss.str());
                    }
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        ImGui::EndGroup();

        if (audio.audio == "" || audio.data == -1) return; // -------------------------------------------------------

        AudioSystem& sys = reg.GetSystem<AudioSystem>();
        //Audio& a = sys.GetAudio(audio.audio);

        Audio& a = reg.GetSystem<AudioSystem>().GetAudioInst(audio.data);

        // volume
        float valF = a.GetVolume();
        if (ImGui::SliderFloat("Volume", &valF, 0.f, 1.f))
        {
            if (!isChanged)
            {
                oldState = audio;
                isChanged = true;
            }

            audio.volume = valF;
            //audio.Update(reg.GetSystem<AudioSystem>());

            if (a.GetPlaying())
            {
                sys.SetResult(a.UseChannel()->setVolume(valF));
                sys.ErrorCheck();
            }
            a.SetVolume(valF);
        }

        // mute
        bool valB = a.GetMute();
        if (ImGui::Checkbox("Mute", &valB))
        {
            if (!isChanged)
            {
                oldState = audio;
                isChanged = true;
            }
            audio.mute = valB;
            a.SetMute(valB);
        }

        // loop
        valB = a.GetLoop();
        if (ImGui::Checkbox("Loop", &valB))
        {
            if (!isChanged)
            {
                oldState = audio;
                isChanged = true;
            }
            audio.loop = valB;
            //audio.Update(reg.GetSystem<AudioSystem>());
            a.SetLoop(valB);
        }

        // bpm
        valF = a.GetBPM();
        if (ImGui::InputFloat("BPM", &valF))
        {
            if (!isChanged)
            {
                oldState = audio;
                isChanged = true;
            }
            audio.bpm = valF;
            //audio.Update(reg.GetSystem<AudioSystem>());
            a.SetBPM(valF);
        }

        // timesig
        int temp[2] = { a.GetTimeSig().first, a.GetTimeSig().second };
        if (ImGui::DragInt2("Time Signature [Top/Bot]", temp))
        {
            if (!isChanged)
            {
                oldState = audio;
                isChanged = true;
            }
            audio.timeSig.first = temp[0];
            audio.timeSig.second = temp[1];
            //audio.Update(reg.GetSystem<AudioSystem>());
            a.SetTimeSig({temp[0], temp[1]});
        }

        // playonawake
        valB = a.playOnAwake;
        if (ImGui::Checkbox("Play On Awake", &valB))
        {
            if (!isChanged)
            {
                oldState = audio;
                isChanged = true;
            }
            audio.playOnAwake = valB;
            //audio.Update(reg.GetSystem<AudioSystem>());
            a.playOnAwake = valB;
        }

        // channel group
        ImGui::BeginGroup();
        ImGui::Text("Channel:");
        ImGui::SameLine();

        static std::map<int, std::string> channelName = 
        {
            {0, "Master"},
            {1, "BGM"},
            {2, "SFX"}
        };

        if (ImGui::BeginCombo("##Channel", channelName[audio.grp].c_str()))
        {
            {
                bool isSelected = (audio.grp == 0);

                if (ImGui::Selectable("Master", isSelected))
                {
                    if (!isChanged)
                    {
                        oldState = audio;
                        isChanged = true;
                    }
                    audio.grp = 0;

                    a.SetGroup(reg.GetSystem<AudioSystem>().GetCGroup(0));
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            {
                bool isSelected = (audio.grp == 1);

                if (ImGui::Selectable("BGM", isSelected))
                {
                    if (!isChanged)
                    {
                        oldState = audio;
                        isChanged = true;
                    }
                    audio.grp = 1;

                    a.SetGroup(reg.GetSystem<AudioSystem>().GetCGroup(1));
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            {
                bool isSelected = (audio.grp == 2);

                if (ImGui::Selectable("SFX", isSelected))
                {
                    if (!isChanged)
                    {
                        oldState = audio;
                        isChanged = true;
                    }
                    audio.grp = 2;

                    a.SetGroup(reg.GetSystem<AudioSystem>().GetCGroup(2));
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }

        ImGui::EndGroup();


        // emit change event
        if (isChanged && !InputSystem::IsMouseButtonDown(InputSystem::MouseButton::Left) && !ImGui::GetIO().WantTextInput) // change detection
        {
            //audio.Update(reg.GetSystem<AudioSystem>());
            eventBus.EmitEvent<SceneChangeEvent<std::any>>(*selectedEntity, audio, oldState, ChangeAction::MODIFY_COMPONENT);
            isChanged = false;
        }

        // -------------------------------------------------------

        // Play / Pause / Stop

        if (ImGui::SmallButton("Play"))
        {
            FMOD_RESULT res = a.Play();
            if (res != FMOD_OK) Logger::Error("AudioComponent returned FMOD_RESULT: " + res);
        }

        ImGui::SameLine();

        if (ImGui::SmallButton("Pause") && a.GetPlaying())
        {
            FMOD_RESULT res = a.Pause();
            if (res != FMOD_OK) Logger::Error("AudioComponent returned FMOD_RESULT: " + res);
        }

        ImGui::SameLine();

        if (ImGui::SmallButton("Stop") && (a.GetPlaying() || a.GetPaused()))
        {
            FMOD_RESULT res = a.Stop();
            if (res != FMOD_OK) Logger::Error("AudioComponent returned FMOD_RESULT: " + res);
        }

        if (a.GetPlaying() || a.GetPaused())
        {
            if (a.GetPaused())
            {
                ImGui::SameLine();
                ImGui::Text("Paused");
            }
            else
            {
                ImGui::SameLine();
                ImGui::Text("Playing Audio");
            }

            // Audio update
            PerformanceViewer::GetInstance()->start("audio");
            reg.GetSystem<AudioSystem>().Update();
            PerformanceViewer::GetInstance()->end("audio");
        }

        // Track slider

        //static ImGuiSliderFlags flags = ImGuiSliderFlags_None;

        static const ImGuiSliderFlags sliderFlags = ImGuiSliderFlags_None & ~ImGuiSliderFlags_WrapAround;

        static int currTime{};
        
        if (a.GetCurr() >= a.GetLen() || !a.GetPlaying())
        {
            a.SetCurr(0);
            if (!a.GetLoop()) a.Stop();
        }

        currTime = static_cast<int>(a.GetCurr());

        ImGui::Text("Time: %d m %d s", (currTime / 1000) / 60, (currTime / 1000) % 60);

        if (ImGui::SliderInt("Mins / Secs", &currTime, 0, a.GetLen(), "", sliderFlags))
            a.SetCurr(static_cast<unsigned int>(currTime));

        if (!a.GetPlaying()) return; // -------------------------------------------------------

        //return;

        // Audio Visual

        //ImGui::plot

        static bool useFreePlot = false;

        if (ImGui::Checkbox("Use Free Plot", &useFreePlot)) {}

        //todo: fix
        static float* fft[2]; // todo: this has to be set to new [len]
        static float fftL[POPPLIO_AUDIO_FFT_BIN_COUNT]{};
        static float fftR[POPPLIO_AUDIO_FFT_BIN_COUNT]{};

        fft[0] = fftL;
        fft[1] = fftR;

        if (a.GetSpectrumLog(fft, 1.f)) // mag modifier
        {
            if (useFreePlot)
            {
                if (ImPlot::BeginPlot("Audio"))
                {
                    ImPlot::PlotBars("FFT", fft[0], POPPLIO_AUDIO_FFT_BIN_COUNT);
                    ImPlot::EndPlot();
                }
            }
            else
            {
                ImGui::PlotHistogram("Audio L", fft[0], POPPLIO_AUDIO_FFT_BIN_COUNT);
                ImGui::PlotHistogram("Audio R", fft[1], POPPLIO_AUDIO_FFT_BIN_COUNT);
            }
        }

        if (editorState == State::EDITING) reg.GetSystem<AudioSystem>().Update();

        //if (sys.GetFFT(fft))
        //{
        //    ImGui::PlotHistogram("Audio FFT Spectrum", fft, sys.fftSamplePerBarLog.size());
        //}

        //if (sys.GetFFT(a, fft, len) == FMOD_OK && fft != nullptr)
        //{
        //    ImGui::PlotHistogram("Audio FFT Spectrum", fft[0], len);
        //    Logger::Error("AudioComponent1");
        //} else  Logger::Error("AudioComponent2");
    }

    template <>
    void Editor::EditComponent(PrefabInstanceComponent& prefabInstComp) // TODO change detection + undo redo
    {
        // Display the linked prefab name (read-only)
        ImGui::Text("Linked Prefab: %s", prefabInstComp.prefabName.c_str());

        // Add sync toggle checkbox
        ImGui::Checkbox("Sync with Prefab", &prefabInstComp.syncWithPrefab);

        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::Text("When enabled, this instance will automatically update\nwhen the source prefab changes.");
            ImGui::EndTooltip();
        }

        // Add UI indication of sync status
        ImGui::SameLine();
        if (prefabInstComp.syncWithPrefab)
        {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "\u00B7"); // Green dot
            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("Instance is synced with prefab");
            }
        }
        else
        {
            ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "\u00B7"); // Orange dot
            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("Instance is not synced with prefab");
            }
        }

        // Show a warning if the prefab doesn't exist
        if (!prefabManager.GetPrefab(prefabInstComp.prefabName))
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            ImGui::TextWrapped("Warning: Source prefab '%s' not found!",
                prefabInstComp.prefabName.c_str());
            ImGui::PopStyleColor();
        }
    }

	template <>
    void Editor::EditComponent(ParentComponent& parent)
    {
        static bool isChanged = false;
        static ParentComponent oldState;

        // parent / child
        static bool otherIsChanged = false;
        static Entity eOther = Entity(-1);
        static ParentComponent newStateOther;
        static ParentComponent oldStateOther;

        // Show parent info
        if (parent.parent.GetId() != -1 && reg.EntityExists(parent.parent.GetId()))
        {
            ImGui::Text("Parent: Entity %d", parent.parent.GetId());
            if (ImGui::Button("Clear Parent"))
            {
                if (!isChanged)
                {
                    oldState = parent;
                    isChanged = true;

                    otherIsChanged = true;
                    eOther = parent.parent;
                    oldStateOther = parent.parent.GetComponent<ParentComponent>();
                }
                Entity temp = parent.parent;
                parent.parent.GetComponent<ParentComponent>().RemoveChild(*selectedEntity);
                //parent.parent = Entity(-1);
                newStateOther = temp.GetComponent<ParentComponent>();
            }

            if (ImGui::CollapsingHeader("Local Transform", ImGuiTreeNodeFlags_DefaultOpen))
            {
				// Local Position
				PopplioMath::Vec2f temp = parent.localPosition;
                if (ImGui::DragFloat2("Position", &temp.x))
                {
                    if (!isChanged)
                    {
                        oldState = parent;
						isChanged = true;
                    }
					parent.localPosition = temp;
                }

                // Local Scale
				temp = parent.localScale;
                if (ImGui::DragFloat2("Scale", &temp.x))
                {
                    if (!isChanged)
                    {
                        oldState = parent;
                        isChanged = true;
                    }
					parent.localScale = temp;
                }

				// Local Rotation
				float localRot = static_cast<float>(parent.localRotation);
                if (ImGui::DragFloat("Rotation", &localRot))
                {
                    if (!isChanged)
                    {
						oldState = parent;
						isChanged = true;
                    }
					parent.localRotation = localRot;
                }
            }
        }
        else
        {
            ImGui::Text("No Parent");
        }

        // Inheritance controls
        bool inheritActive = parent.inheritActive;
        if (ImGui::Checkbox("Inherit Active State", &inheritActive))
        {
            if (!isChanged)
            {
                oldState = parent;
                isChanged = true;
            }
            parent.inheritActive = inheritActive;
        }

        bool inheritTransform = parent.inheritTransform;
        if (ImGui::Checkbox("Inherit Transforms", &inheritTransform))
        {
            if (!isChanged)
            {
                oldState = parent;
                isChanged = true;
            }
            parent.inheritTransform = inheritTransform;
        }

        // Show children - with enhanced safety and debugging
        if (!parent.children.empty())
        {
            if (ImGui::TreeNode("Children"))
            {
                // Create a temporary vector to store valid children
                std::vector<Entity> validChildren;
                validChildren.reserve(parent.children.size());
                for (const auto& child : parent.children)
                {
                    if (reg.EntityExists(child.GetId()))
                    {
                        validChildren.push_back(child);
                    }
                    else
                    {
                        Logger::Warning("Found invalid child entity ID: " + std::to_string(child.GetId()));
                    }
                }

                // Display only valid children
                for (size_t i = 0; i < validChildren.size(); ++i)
                {
                    const Entity& child = validChildren[i];

                    // Create a unique string ID for each child
                    std::string childId = "Child_" + std::to_string(child.GetId());
                    ImGui::PushID(childId.c_str());

                    if (ImGui::Button("Select"))
                    {
                        selectedEntity = child;
                        eventBus.EmitEvent<EntitySelectedEvent>(child);
                    }

                    ImGui::SameLine();
                    if (ImGui::Button("Remove"))
                    {
                        if (!isChanged)
                        {
                            oldState = parent;
                            isChanged = true;

                            otherIsChanged = true;
                            eOther = validChildren[i];
                            oldStateOther = child.GetComponent<ParentComponent>();
                        }
                        parent.RemoveChild(child);
                        newStateOther = child.GetComponent<ParentComponent>();
                    }

                    ImGui::SameLine();
                    ImGui::Text("Entity %d", child.GetId());

                    // Only show tag if entity exists and has a tag
                    if (reg.EntityExists(child.GetId()) && reg.EntityHasAnyTag(child))
                    {
                        ImGui::SameLine();
                        ImGui::Text("[%s]", reg.GetEntityTag(child).c_str());
                    }

                    ImGui::PopID();
                }

                ImGui::TreePop();
            }
        }
        else
        {
            ImGui::Text("No Children");
        }

        // Add child button
        if (ImGui::Button("Add Child"))
        {
            ImGui::OpenPopup("Select Child Entity");
        }

        // Child selection popup
        if (ImGui::BeginPopup("Select Child Entity"))
        {
            ImGui::Text("Select an entity to add as child:");
            for (auto& entity : reg.GetAllEntitiesMutable())
            {
                // Skip if:
                // 1. Entity is the same as current entity
                // 2. Entity would create a cycle
                // 3. Entity already has a parent
                if (entity.GetId() != selectedEntity->GetId() &&
                    !WouldCreateCycle(entity, *selectedEntity) &&
                    (!entity.HasComponent<ParentComponent>() ||
                        entity.GetComponent<ParentComponent>().parent.GetId() == -1))
                {
                    std::string entityName = "Entity " + std::to_string(entity.GetId());
                    if (reg.EntityHasAnyTag(entity))
                    {
                        entityName += " [" + reg.GetEntityTag(entity) + "]";
                    }

                    if (ImGui::Selectable(entityName.c_str()))
                    {
                        if (!isChanged)
                        {
                            oldState = parent;
                            isChanged = true;

                            otherIsChanged = true;
                            eOther = entity;
                            if (entity.HasComponent<ParentComponent>()) 
                                oldStateOther = entity.GetComponent<ParentComponent>();
                            else
                                oldStateOther = ParentComponent();
                        }
                        //parent.AddChild(entity);
                        reg.GetSystem<HierarchySystem>().AddChild(*selectedEntity, entity);

                        // Add ParentComponent to child if it doesn't exist
                        //if (!entity.HasComponent<ParentComponent>())
                        //{
                        //    entity.AddComponent<ParentComponent>(*selectedEntity);
                        //}
                        //else
                        //{
                        //    entity.GetComponent<ParentComponent>().parent = *selectedEntity;
                        //}

                        //entity.GetComponent<ParentComponent>().InitLocalTransform(entity.GetComponent<TransformComponent>());

                        entity.GetComponent<ParentComponent>().UpdateLocalTransform(
                            entity.GetComponent<TransformComponent>(), selectedEntity->GetComponent<TransformComponent>());

                        newStateOther = entity.GetComponent<ParentComponent>();

                        // Initialize the transform tracking here
                        //reg.GetSystem<HierarchySystem>().InitializeParentTransform(*selectedEntity);
                    }
                }
            }
            ImGui::EndPopup();
        }

        // Emit change event if needed
        if (isChanged && !InputSystem::IsMouseButtonDown(InputSystem::MouseButton::Left) && !ImGui::GetIO().WantTextInput)
        {
            // parent changes
            SceneChangeEvent<std::any> event{ *selectedEntity, parent, oldState, ChangeAction::MODIFY_COMPONENT };

            // child changes
            if (otherIsChanged)
            {
                event.entities.push_back(eOther);
                event.newState.push_back(newStateOther);
                event.oldState.push_back(oldStateOther);
                event.changes.push_back(ChangeAction::MODIFY_COMPONENT);
            }

            eventBus.EmitEvent<SceneChangeEvent<std::any>>(std::move(event));

            isChanged = false;
            otherIsChanged = false;
        }
    }

    template<>
    void Editor::EditComponent(PersistOnLoadComponent& persist)
    {
        // change detection
        static bool isChanged = false;
        static PersistOnLoadComponent oldState;

        bool p{ persist.persistOnLoad };
        if (ImGui::Checkbox("Persist", &p))
        {
            if (!isChanged)
            {
                oldState = persist;
                isChanged = true;
            }
            persist.persistOnLoad = p;
        }

        if (isChanged) // change detection
        {
            eventBus.EmitEvent<SceneChangeEvent<std::any>>(*selectedEntity, persist, oldState, ChangeAction::MODIFY_COMPONENT);
            isChanged = false;
        }
    }

    template <>
    void Editor::EditComponent(ParticleComponent& particles)
    {
        // change detection
        static bool isChanged = false;
        static ParticleComponent oldState;

        // Active // ====================================================== //

        bool tempB = particles.isActive;
        if (ImGui::Checkbox("Active", &tempB))
        {
            if (!isChanged)
            {
                oldState = particles;
                isChanged = true;
            }
            particles.isActive = tempB;
        }

        std::string displayName = "";

        // Play // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

        // TODO reset emitter and particles value

        static bool isPlaying = false;
        static int cachedEntity = -1;
        static std::unordered_map<size_t, Popplio::Emitter> cachedEmitters{};

        if (cachedEntity != selectedEntity->GetId() && isPlaying)
        {
            if (reg.GetEntityById(cachedEntity).HasComponent<ParticleComponent>())
            {
                for (auto [index, emitter] : cachedEmitters)
                {
                    reg.GetEntityById(cachedEntity).GetComponent<ParticleComponent>().emission[index] = emitter;
                }
            }

            isPlaying = false;
            cachedEmitters.clear();
        }

        if (isPlaying)
        {
            reg.GetSystem<ParticleSystem>().UpdateEnt(*selectedEntity, Engine::timer.GetFixedDeltaTime());
            if (ImGui::Button("Stop"))
            {
                for (auto [index, emitter] : cachedEmitters)
                {
                    reg.GetEntityById(cachedEntity).GetComponent<ParticleComponent>().emission[index] = emitter;
                }

                isPlaying = false;
                cachedEmitters.clear();
            }
        }
        else if (ImGui::Button("Play"))
        {
            cachedEntity = selectedEntity->GetId();

            for (size_t i{}; i < particles.emission.size(); ++i)
            {
                if (particles.emission[i].active) cachedEmitters[i] = particles.emission[i];
            }

            isPlaying = true;
        }

        // Rendering Option // ====================================================== //

        ImGui::BeginGroup();
        ImGui::Text("Rendering:");
        ImGui::SameLine();

        switch (particles.renderOption)
        {
            case 0:
                displayName = "Texture";
                break;
            case 1:
                displayName = "Animation";
                break;
            default:
                displayName = "None";
                break;
        }

        if (ImGui::BeginCombo("##Rendering", displayName.c_str()))
        {
            bool isSelected = particles.renderOption == 0;

            if (ImGui::Selectable("Texture", isSelected))
            {
                if (!isChanged)
                {
                    oldState = particles;
                    isChanged = true;
                }
                particles.renderOption = 0;
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }

            isSelected = particles.renderOption == 1;

            if (ImGui::Selectable("Animation", isSelected))
            {
                if (!isChanged)
                {
                    oldState = particles;
                    isChanged = true;
                }
                particles.renderOption = 1;
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }
        ImGui::EndGroup();

        // usePhysics // ====================================================== //

        tempB = particles.usePhysics;
        if (ImGui::Checkbox("Use Physics", &tempB))
        {
            if (!isChanged)
            {
                oldState = particles;
                isChanged = true;
            }
            particles.usePhysics = tempB;
        }

        // Emitters // ====================================================== //

        ImGui::Text("Emitters");

        if (ImGui::Button("Add Emitter"))
        {
            if (!isChanged)
            {
                oldState = particles;
                isChanged = true;
            }
            reg.GetSystem<ParticleSystem>().NewEmit(particles);
        }

        std::vector<Emitter>& tempEmit = particles.emission;

        if (ImGui::TreeNode("Emitters"))
        {
            for (std::vector<Emitter>::iterator i{ tempEmit.begin() }; i != tempEmit.end(); ++i)
            {
                if (!i->active) continue;

                displayName = "Emitter " + std::to_string(i - tempEmit.begin());

                if (ImGui::TreeNode(displayName.c_str()))
                {
                    ImGui::SameLine();
                    // Active // +++++++++++++++++++++++++++++ //
                    if (ImGui::Button("Remove"))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        reg.GetSystem<ParticleSystem>().KillEmit(particles, static_cast<int>(i - tempEmit.begin()));
                        // do not need to break because not removing from vector (just deactivating)
                    }

                    // Loop // +++++++++++++++++++++++++++++ //
                    tempB = i->loop;
                    if (ImGui::Checkbox("Loop", &tempB))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->loop = tempB;
                    }

                    // elapsedTime // +++++++++++++++++++++++++++++ //
                    float tempF = i->elapsedTime;
                    if (ImGui::InputFloat("Elapsed Time", &tempF))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->elapsedTime = tempF;
                    }

                    // rate // +++++++++++++++++++++++++++++ //
                    tempF = i->rate;
                    if (ImGui::InputFloat("Rate", &tempF))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->rate = tempF;
                    }

                    // tLoop // +++++++++++++++++++++++++++++ //
                    tempF = i->tLoop;
                    if (ImGui::InputFloat("Loop Duration", &tempF))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->tLoop = tempF;
                    }

                    // LifeTime // +++++++++++++++++++++++++++++ //
                    ImGui::Text("Life Time");

                    tempF = i->minLifeTime;
                    ImGui::Text("Min");
                    ImGui::SameLine();
                    if (ImGui::InputFloat("##MinL", &tempF))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->minLifeTime = tempF;
                    }
                    tempF = i->maxLifeTime;
                    ImGui::Text("Max");
                    ImGui::SameLine();
                    if (ImGui::InputFloat("##MaxL", &tempF))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->maxLifeTime = tempF;
                    }

                    // Rotation // +++++++++++++++++++++++++++++ //
                    ImGui::Text("Rotation");
                    tempF = i->minRot;
                    ImGui::Text("Min");
                    ImGui::SameLine();
                    if (ImGui::InputFloat("##MinR", &tempF))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->minRot = tempF;
                    }
                    tempF = i->maxRot;
                    ImGui::Text("Max");
                    ImGui::SameLine();
                    if (ImGui::InputFloat("##MaxR", &tempF))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->maxRot = tempF;
                    }
                    tempF = i->minSpinVel;
                    ImGui::Text("Min Vel");
                    ImGui::SameLine();
                    if (ImGui::InputFloat("##Min Vel R", &tempF))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->minSpinVel = tempF;
                    }
                    tempF = i->maxSpinVel;
                    ImGui::Text("Max Vel");
                    ImGui::SameLine();
                    if (ImGui::InputFloat("##Max Vel R", &tempF))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->maxSpinVel = tempF;
                    }
                    tempF = i->minSpinAccel;
                    ImGui::Text("Min Accel");
                    ImGui::SameLine();
                    if (ImGui::InputFloat("##Min Accel R", &tempF))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->minSpinAccel = tempF;
                    }
                    tempF = i->maxSpinAccel;
                    ImGui::Text("Max Accel");
                    ImGui::SameLine();
                    if (ImGui::InputFloat("##Max Accel R", &tempF))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->maxSpinAccel = tempF;
                    }
                    tempF = i->minSpinDirAccel;
                    ImGui::Text("Min Dir Accel");
                    ImGui::SameLine();
                    if (ImGui::InputFloat("##Min Dir Accel R", &tempF))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->minSpinDirAccel = tempF;
                    }
                    tempF = i->maxSpinDirAccel;
                    ImGui::Text("Max Dir Accel");
                    ImGui::SameLine();
                    if (ImGui::InputFloat("##Max Dir Accel R", &tempF))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->maxSpinDirAccel = tempF;
                    }

                    // Position // +++++++++++++++++++++++++++++ //
                    ImGui::Text("Position");

                    PopplioMath::Vec2f tempV = i->minPos;
                    ImGui::Text("Min");
                    ImGui::SameLine();
                    if (ImGui::DragFloat2("##Min P", &tempV.x))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->minPos = tempV;
                    }
                    tempV = i->maxPos;
                    ImGui::Text("Max");
                    ImGui::SameLine();
                    if (ImGui::DragFloat2("##Max P", &tempV.x))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->maxPos = tempV;
                    }
                    tempV = i->minVel;
                    ImGui::Text("Min Vel");
                    ImGui::SameLine();
                    if (ImGui::DragFloat2("##Min Vel P", &tempV.x))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->minVel = tempV;
                    }
                    tempV = i->maxVel;
                    ImGui::Text("Max Vel");
                    ImGui::SameLine();
                    if (ImGui::DragFloat2("##Max Vel P", &tempV.x))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->maxVel = tempV;
                    }
                    tempV = i->minAccel;
                    ImGui::Text("Min Accel");
                    ImGui::SameLine();
                    if (ImGui::DragFloat2("##Min Accel P", &tempV.x))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->minAccel = tempV;
                    }
                    tempV = i->maxAccel;
                    ImGui::Text("Max Accel");
                    ImGui::SameLine();
                    if (ImGui::DragFloat2("##Max Accel P", &tempV.x))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->maxAccel = tempV;
                    }
                    tempV = i->minDirAccel;
                    ImGui::Text("Min Dir Accel");
                    ImGui::SameLine();
                    if (ImGui::DragFloat2("##Min Dir Accel P", &tempV.x))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->minDirAccel = tempV;
                    }
                    tempV = i->maxDirAccel;
                    ImGui::Text("Max Dir Accel");
                    ImGui::SameLine();
                    if (ImGui::DragFloat2("##Max Dir Accel P", &tempV.x))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->maxDirAccel = tempV;
                    }

                    // Scale // +++++++++++++++++++++++++++++ //
                    ImGui::Text("Scale");

                    tempV = i->minScale;
                    ImGui::Text("Min");
                    ImGui::SameLine();
                    if (ImGui::DragFloat2("##MinS", &tempV.x))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->minScale = tempV;
                    }
                    tempV = i->maxScale;
                    ImGui::Text("Max");
                    ImGui::SameLine();
                    if (ImGui::DragFloat2("##MaxS", &tempV.x))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->maxScale = tempV;
                    }
                    tempV = i->minScaleVel;
                    ImGui::Text("Min Vel");
                    ImGui::SameLine();
                    if (ImGui::DragFloat2("##Min Vel S", &tempV.x))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->minScaleVel = tempV;
                    }
                    tempV = i->maxScaleVel;
                    ImGui::Text("Max Vel");
                    ImGui::SameLine();
                    if (ImGui::DragFloat2("##Max Vel S", &tempV.x))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->maxScaleVel = tempV;
                    }
                    tempV = i->minScaleAccel;
                    ImGui::Text("Min Accel");
                    ImGui::SameLine();
                    if (ImGui::DragFloat2("##Min Accel S", &tempV.x))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->minScaleAccel = tempV;
                    }
                    tempV = i->maxScaleAccel;
                    ImGui::Text("Max Accel");
                    ImGui::SameLine();
                    if (ImGui::DragFloat2("##Max Accel S", &tempV.x))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->maxScaleAccel = tempV;
                    }
                    tempV = i->minScaleDirAccel;
                    ImGui::Text("Min Dir Accel");
                    ImGui::SameLine();
                    if (ImGui::DragFloat2("##Min Dir Accel S", &tempV.x))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->minScaleDirAccel = tempV;
                    }
                    tempV = i->maxScaleDirAccel;
                    ImGui::Text("Max Dir Accel");
                    ImGui::SameLine();
                    if (ImGui::DragFloat2("##Max Dir Accel S", &tempV.x))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->maxScaleDirAccel = tempV;
                    }

                    // Color // +++++++++++++++++++++++++++++ //
                    ImGui::Text("Color");

                    Color tempC = i->minCol;
                    ImGui::Text("Min");
                    ImGui::SameLine();
                    if (ImGui::ColorEdit4("##MinC", &tempC.r))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->minCol = tempC;
                    }
                    tempC = i->maxCol;
                    ImGui::Text("Max");
                    ImGui::SameLine();
                    if (ImGui::ColorEdit4("##MaxC", &tempC.r))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->maxCol = tempC;
                    }
                    tempC = i->minColVel;
                    ImGui::Text("Min Vel");
                    ImGui::SameLine();
                    if (ImGui::ColorEdit4("##Min Vel C", &tempC.r))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->minColVel = tempC;
                    }
                    tempC = i->maxColVel;
                    ImGui::Text("Max Vel");
                    ImGui::SameLine();
                    if (ImGui::ColorEdit4("##Max Vel C", &tempC.r))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->maxColVel = tempC;
                    }
                    tempC = i->minColAccel;
                    ImGui::Text("Min Accel");
                    ImGui::SameLine();
                    if (ImGui::ColorEdit4("##Min Accel C", &tempC.r))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->minColAccel = tempC;
                    }
                    tempC = i->maxColAccel;
                    ImGui::Text("Max Accel");
                    ImGui::SameLine();
                    if (ImGui::ColorEdit4("##Max Accel C", &tempC.r))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->maxColAccel = tempC;
                    }
                    tempC = i->minColDirAccel;
                    ImGui::Text("Min Dir Accel");
                    ImGui::SameLine();
                    if (ImGui::ColorEdit4("##Min Dir Accel C", &tempC.r))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->minColDirAccel = tempC;
                    }
                    tempC = i->maxColDirAccel;
                    ImGui::Text("Max Dir Accel");
                    ImGui::SameLine();
                    if (ImGui::ColorEdit4("##Max Dir Accel C", &tempC.r))
                    {
                        if (!isChanged)
                        {
                            oldState = particles;
                            isChanged = true;
                        }
                        i->maxColDirAccel = tempC;
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }

        // change detection // ====================================================== //

        if (isChanged && !InputSystem::IsMouseButtonDown(InputSystem::MouseButton::Left) && !ImGui::GetIO().WantTextInput) 
        {
            eventBus.EmitEvent<SceneChangeEvent<std::any>>(*selectedEntity, particles, oldState, ChangeAction::MODIFY_COMPONENT);
            isChanged = false;
        }
    }
}