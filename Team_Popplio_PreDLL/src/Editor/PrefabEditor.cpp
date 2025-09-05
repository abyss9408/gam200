/******************************************************************************/
/*!
\file   PrefabEditor.cpp
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/06
\brief
    This file defines several methods within the Editor class in the Popplio namespace. 
    These methods are responsible for managing and rendering the prefab editor, 
    which includes creating, loading, saving, and editing prefabs using the ImGui library.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include "Editor.h"
#include <../Utilities/FileDialog.h>

namespace Popplio
{
    void Editor::RegisterCurrentPrefab()
    {
        if (!currentPrefab)
        {
            return;
        }

        try
        {
            // Store the prefab with its file path if it exists
            prefabManager.RegisterPrefab(
                currentPrefab->get()->GetName(),
                std::make_unique<Prefab>(*currentPrefab->get()),
                currentPrefabPath  // This will be empty for new prefabs until saved
            );

            Logger::Debug("Registered prefab: " + currentPrefab->get()->GetName(),
                LogDestination::IMGUI_CONSOLE);
        }
        catch (const std::exception& e)
        {
            Logger::Error("Failed to register prefab: " + std::string(e.what()),
                LogDestination::IMGUI_CONSOLE);
        }
    }

    void Editor::RenderPrefabEditor()
    {
        ImGui::Begin(ICON_FA_CIRCLE_INFO " Prefab Editor", nullptr, ImGuiWindowFlags_MenuBar);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Prefab"))
                {
                    showPrefabNameDialog = true;
                }
                if (ImGui::MenuItem("Load Prefab"))
                {
                    // Use FileDialog to open prefab file
					std::string path = FileDialog::OpenFile("Prefab Files\0*.prefab\0", "prefab");
                    if (!path.empty())
                    {
                        LoadPrefabFromFile(path);
                    }
                }
                if (currentPrefab && ImGui::MenuItem("Save Prefab"))
                {
                    if (currentPrefabPath.empty())
                    {
                        // If no path exists, show save dialog
                        std::string path = FileDialog::SaveFile("Prefab Files\0*.prefab\0", "prefab");
                        if (!path.empty())
                        {
                            currentPrefabPath = path;
                            SaveCurrentPrefab();
                        }
                    }
                    else
                    {
                        SaveCurrentPrefab();
                    }
                }
                if (currentPrefab && ImGui::MenuItem("Save Prefab As..."))
                {
                    std::string path = FileDialog::SaveFile("Prefab Files\0*.prefab\0", "prefab");
                    if (!path.empty())
                    {
                        currentPrefabPath = path;
                        SaveCurrentPrefab();
                    }
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        if (showPrefabNameDialog)
        {
            ImGui::OpenPopup("Create New Prefab");
        }

        // Center the popup
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("Create New Prefab", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            static char nameBuffer[256] = "";
            ImGui::Text("Enter prefab name:");
            ImGui::InputText("##prefabname", nameBuffer, sizeof(nameBuffer));

            if (ImGui::Button("Create", ImVec2(120, 0)))
            {
                CreateNamedPrefab(nameBuffer);
                memset(nameBuffer, 0, sizeof(nameBuffer));
                showPrefabNameDialog = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0)))
            {
                memset(nameBuffer, 0, sizeof(nameBuffer));
                showPrefabNameDialog = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        if (!currentPrefab)
        {
            ImGui::Text("No prefab currently open");
            ImGui::End();
            return;
        }

        // Show current name as read-only
        ImGui::Text("Prefab Name: %s", currentPrefab->get()->GetName().c_str());

        // Component Management Section
        if (ImGui::CollapsingHeader("Components", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::Button("Add Component"))
            {
                ImGui::OpenPopup("Add Component");
            }

            AddComponentPopup(*currentPrefab->get());

            // Render existing components
            RenderComponentEditor<ActiveComponent>(*currentPrefab->get());
            RenderComponentEditor<TransformComponent>(*currentPrefab->get());
            RenderComponentEditor<RenderComponent>(*currentPrefab->get());
            RenderComponentEditor<RigidBodyComponent>(*currentPrefab->get());
            RenderComponentEditor<BoxColliderComponent>(*currentPrefab->get());
            RenderComponentEditor<TextComponent>(*currentPrefab->get());
            RenderComponentEditor<LogicComponent>(*currentPrefab->get());
            RenderComponentEditor<AudioComponent>(*currentPrefab->get());
            RenderComponentEditor<AnimationComponent>(*currentPrefab->get()); 
            RenderComponentEditor<ParentComponent>(*currentPrefab->get());
            RenderComponentEditor<PersistOnLoadComponent>(*currentPrefab->get());
            RenderComponentEditor<UIComponent>(*currentPrefab->get());
            RenderComponentEditor<ParticleComponent>(*currentPrefab->get());
        }

        // Prefab Statistics
        if (ImGui::CollapsingHeader("Statistics"))
        {
            // Count components
            int componentCount = 0;
            componentCount += currentPrefab->get()->HasComponent<ActiveComponent>();
            componentCount += currentPrefab->get()->HasComponent<TransformComponent>();
            componentCount += currentPrefab->get()->HasComponent<RenderComponent>();
            componentCount += currentPrefab->get()->HasComponent<RigidBodyComponent>();
            componentCount += currentPrefab->get()->HasComponent<BoxColliderComponent>();
            componentCount += currentPrefab->get()->HasComponent<TextComponent>();
            componentCount += currentPrefab->get()->HasComponent<LogicComponent>();
            componentCount += currentPrefab->get()->HasComponent<AudioComponent>();
            componentCount += currentPrefab->get()->HasComponent<AnimationComponent>();
            componentCount += currentPrefab->get()->HasComponent<ParentComponent>();
            componentCount += currentPrefab->get()->HasComponent<PersistOnLoadComponent>();
            componentCount += currentPrefab->get()->HasComponent<UIComponent>();
            componentCount += currentPrefab->get()->HasComponent<ParticleComponent>();

            ImGui::Text("Total Components: %d", componentCount);
            ImGui::Text("File Path: %s",
                currentPrefabPath.empty() ? "Not saved" : currentPrefabPath.c_str());
        }

        // Status bar at the bottom
        ImGui::Separator();
        if (currentPrefab->get()->IsValid())
        {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Status: Valid");
        }
        else
        {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Status: Invalid");
        }

        ImGui::End();
    }

    void Editor::CreateNamedPrefab(const std::string& name)
    {
        if (name.empty()) {
            Logger::Warning("Cannot create prefab with empty name", LogDestination::IMGUI_CONSOLE);
            return;
        }

        // Check if a prefab with this name already exists
        if (prefabManager.GetPrefab(name)) {
            Logger::Warning("A prefab with name '" + name + "' already exists", LogDestination::IMGUI_CONSOLE);
            return;
        }

        currentPrefab = std::make_unique<Prefab>(name);
        strncpy_s(prefabNameBuffer, sizeof(prefabNameBuffer), name.c_str(), _TRUNCATE);
        currentPrefabPath.clear();

        Logger::Info("Created new prefab: " + name, LogDestination::IMGUI_CONSOLE);
    }

    bool Editor::SaveCurrentPrefab()
    {
        if (!currentPrefab)
        {
            return false;
        }

        try
        {
            if (serializer.SavePrefabToFile(currentPrefabPath, *currentPrefab->get()))
            {
                // Re-register the prefab with its new file path
                prefabManager.RegisterPrefab(
                    currentPrefab->get()->GetName(),
                    std::make_unique<Prefab>(*currentPrefab->get()),
                    currentPrefabPath
                );
                Logger::Info("Prefab saved successfully to: " + currentPrefabPath,
                    LogDestination::IMGUI_CONSOLE);
                return true;
            }
            else
            {
                Logger::Error("Failed to save prefab to: " + currentPrefabPath,
                    LogDestination::IMGUI_CONSOLE);
                return false;
            }
        }
        catch (const std::exception& e)
        {
            Logger::Error("Error saving prefab: " + std::string(e.what()),
                LogDestination::IMGUI_CONSOLE);
            return false;
        }
    }

    void Editor::LoadPrefabFromFile(const std::string& path)
    {
        try
        {
            auto prefab = serializer.LoadPrefabFromFile(path);
            if (prefab)
            {
                currentPrefab = std::move(prefab);
                currentPrefabPath = path;
                strncpy_s(prefabNameBuffer, sizeof(prefabNameBuffer),
                    currentPrefab->get()->GetName().c_str(), _TRUNCATE);

                // Register with file path
                prefabManager.RegisterPrefab(
                    currentPrefab->get()->GetName(),
                    std::make_unique<Prefab>(*currentPrefab->get()),
                    path
                );
                Logger::Info("Prefab loaded successfully from: " + path, LogDestination::IMGUI_CONSOLE);
            }
            else
            {
                Logger::Error("Failed to load prefab from: " + path, LogDestination::IMGUI_CONSOLE);
            }
        }
        catch (const std::exception& e)
        {
            Logger::Error("Error loading prefab: " + std::string(e.what()), LogDestination::IMGUI_CONSOLE);
        }
    }

    void Editor::AddComponentPopup(Prefab& prefab)
    {
        if (ImGui::BeginPopup("Add Component"))
        {
            AddComponentOption<ActiveComponent>(prefab, "Active");
            AddComponentOption<TransformComponent>(prefab, "Transform");
            AddComponentOption<RenderComponent>(prefab, "Render");
            AddComponentOption<RigidBodyComponent>(prefab, "Rigid Body");
            AddComponentOption<BoxColliderComponent>(prefab, "Box Collider");
            AddComponentOption<TextComponent>(prefab, "Text");
            AddComponentOption<LogicComponent>(prefab, "Script");
            AddComponentOption<AnimationComponent>(prefab, "Animation");
            AddComponentOption<AudioComponent>(prefab, "Audio");
            AddComponentOption<ParentComponent>(prefab, "Parent/Child");
            AddComponentOption<PersistOnLoadComponent>(prefab, "Persist On Load");
            AddComponentOption<UIComponent>(prefab, "UI");
            AddComponentOption<ParticleComponent>(prefab, "Particles");
            ImGui::EndPopup();
        }
    }

    bool Editor::IsValidPrefabName(const std::string& name)
    {
        if (name.empty())
            return false;

        // Check if name starts with a letter or underscore
        if (!std::isalpha(name[0]) && name[0] != '_')
            return false;

        // Check for invalid characters
        static const std::string invalidChars = "\\/:*?\"<>|";
        if (name.find_first_of(invalidChars) != std::string::npos)
            return false;

        // Check if name contains only alphanumeric characters and underscores
        return std::all_of(name.begin(), name.end(), [](char c) {
            return std::isalnum(c) || c == '_';
        });
    }

    void Editor::RenderPrefabList()
    {
        ImGui::Begin(ICON_FA_BARS " Prefab List");

        // First, collect all child prefab names
        std::unordered_set<std::string> childPrefabNames;
        for (const auto& [prefabName, prefab] : prefabManager.GetPrefabs())
        {
            for (const auto& childRef : prefab->GetChildPrefabs())
            {
                std::string childName = std::filesystem::path(childRef.prefabPath).stem().string();
                childPrefabNames.insert(childName);
            }
        }


        // Helper function to render a single prefab and its children
        std::function<void(const std::string&, const Prefab&, int)> renderPrefabNode =
            [&](const std::string& prefabName, const Prefab& prefab, int depth)
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
            const auto& childPrefabs = prefab.GetChildPrefabs();

            if (childPrefabs.empty())
            {
                flags |= ImGuiTreeNodeFlags_Leaf;
            }

            if (selectedPrefabName == prefabName)
            {
                flags |= ImGuiTreeNodeFlags_Selected;
            }

            // Indent based on depth
            if (depth > 0)
            {
                ImGui::Indent(depth * 20.0f);
            }

            // Add prefab icon and name
            std::string label = ICON_FA_CUBE " " + prefabName;
            bool nodeOpen = ImGui::TreeNodeEx(prefabName.c_str(), flags, "%s", label.c_str());

            // Handle selection
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            {
                selectedPrefabName = prefabName;
                OnPrefabSelected(prefabName);
            }

            // Context menu for right-click operations
            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem("Edit"))
                {
                    auto prefabOpt = prefabManager.GetPrefab(prefabName);
                    if (prefabOpt) {
                        currentPrefab = std::make_unique<Prefab>(prefabOpt->get());
                        strncpy_s(prefabNameBuffer, sizeof(prefabNameBuffer),
                            currentPrefab->get()->GetName().c_str(), _TRUNCATE);
                    }
                }

                if (ImGui::MenuItem("Instantiate"))
                {
                    InstantiatePrefab();
                }

                if (ImGui::MenuItem("Delete"))
                {
                    showDeleteConfirmation = true;
                }

                ImGui::EndPopup();
            }

            // Preview
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("Preview:");

                // Show transform info for child prefabs
                if (!childPrefabs.empty())
                {
                    ImGui::Text("Child Prefabs:");
                    for (const auto& childRef : childPrefabs)
                    {
                        ImGui::BulletText("%s", std::filesystem::path(childRef.prefabPath).stem().string().c_str());
                        ImGui::Indent();
                        ImGui::Text("Position: (%.2f, %.2f)", childRef.localPosition.x, childRef.localPosition.y);
                        ImGui::Text("Scale: (%.2f, %.2f)", childRef.localScale.x, childRef.localScale.y);
                        ImGui::Text("Rotation: %.2f", childRef.localRotation);
                        ImGui::Unindent();
                    }
                }

                ImGui::EndTooltip();
            }

            if (nodeOpen)
            {
                // Render child prefabs
                for (const auto& childRef : childPrefabs)
                {
                    std::string childPath = childRef.prefabPath;
                    std::string childName = std::filesystem::path(childPath).stem().string();

                    auto childPrefabOpt = prefabManager.GetPrefab(childName);
                    if (childPrefabOpt)
                    {
                        renderPrefabNode(childName, childPrefabOpt->get(), depth + 1);
                    }
                }
                ImGui::TreePop();
            }

            // Unindent if we indented
            if (depth > 0)
            {
                ImGui::Unindent(depth * 20.0f);
            }
        };

        // Render all root-level prefabs
        for (const auto& [prefabName, prefab] : prefabManager.GetPrefabs())
        {
            // Skip if this prefab is a child of another prefab
            if (childPrefabNames.find(prefabName) == childPrefabNames.end())
            {
                renderPrefabNode(prefabName, *prefab, 0);
            }
        }

        // Handle delete confirmation popup
        if (showDeleteConfirmation)
        {
            ImGui::OpenPopup("Delete Prefab?");
            showDeleteConfirmation = false;
        }
        DeleteSelectedPrefab();
		
        ImGui::End();
    }

    void Editor::RenderPrefabListMenu()
    {
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Edit"))
            {
                auto prefabOpt = prefabManager.GetPrefab(selectedPrefabName);
                if (prefabOpt)
                {
                    currentPrefab = std::make_unique<Prefab>(prefabOpt->get());
                    strncpy_s(prefabNameBuffer, sizeof(prefabNameBuffer),
                        currentPrefab->get()->GetName().c_str(), _TRUNCATE);
                }
            }

            if (ImGui::MenuItem("Instantiate"))
            {
                InstantiatePrefab();
            }

            if (ImGui::MenuItem("Delete"))
            {
                showDeleteConfirmation = true;
            }

            ImGui::EndPopup();
        }

        // Handle delete confirmation popup
        if (showDeleteConfirmation)
        {
            ImGui::OpenPopup("Delete Prefab?");
            showDeleteConfirmation = false; // Reset flag
        }

        // Render delete confirmation popup
        DeleteSelectedPrefab();
    }

    void Editor::OnPrefabSelected(const std::string& prefabName)
    {
        selectedPrefabName = prefabName;
        auto prefabOpt = prefabManager.GetPrefab(prefabName);
        if (prefabOpt)
        {
            // Load the prefab for editing
			currentPrefabPath = prefabManager.GetPrefabFilePath(prefabName);
            currentPrefab = std::make_unique<Prefab>(prefabOpt->get());
            strncpy_s(prefabNameBuffer, sizeof(prefabNameBuffer),
                currentPrefab->get()->GetName().c_str(), _TRUNCATE);
        }
        Logger::Info("Selected prefab: " + prefabName, LogDestination::IMGUI_CONSOLE);
    }

    void Editor::DeleteSelectedPrefab()
    {
        // Center the confirmation popup
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("Delete Prefab?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Are you sure you want to delete '%s' and its file?", selectedPrefabName.c_str());
            ImGui::Text("This operation cannot be undone.");
            ImGui::Separator();

            if (ImGui::Button("Yes", ImVec2(120, 0)))
            {
                if (prefabManager.DeletePrefab(selectedPrefabName))
                {
                    // If the deleted prefab was being edited, clear the editor
                    if (currentPrefab && currentPrefab->get()->GetName() == selectedPrefabName)
                    {
                        currentPrefab.reset();
                        memset(prefabNameBuffer, 0, sizeof(prefabNameBuffer));
                    }
                    selectedPrefabName.clear();
                }
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();
            if (ImGui::Button("No", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void Editor::InstantiatePrefab()
    {
        if (selectedPrefabName.empty())
        {
            Logger::Warning("No prefab selected for instantiation", LogDestination::IMGUI_CONSOLE);
            return;
        }

        auto entity = prefabManager.InstantiatePrefab(reg, selectedPrefabName);
        if (entity)
        {
            Logger::Info("Instantiated prefab: " + selectedPrefabName, LogDestination::IMGUI_CONSOLE);
        }
    }
}