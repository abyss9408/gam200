/******************************************************************************/
/*!
\file   SavingEntityPrefab.cpp
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/06
\brief
    This source file defines the methods for saving entity prefabs within the Popplio namespace.
    It includes functionality for rendering the prefab UI, showing save dialogs, and previewing
    the components and children of an entity.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include "Editor.h"
#include "../Utilities/FileDialog.h"

namespace Popplio
{
	void Editor::RenderPrefabUI(Entity& entity)
	{
		if (ImGui::CollapsingHeader("Prefab", ImGuiTreeNodeFlags_DefaultOpen))
		{
			bool hasChildren = entity.HasComponent<ParentComponent>() &&
				!entity.GetComponent<ParentComponent>().children.empty();

			// Show current prefab status
			if (entity.HasComponent<PrefabInstanceComponent>())
			{
				auto& prefabComp = entity.GetComponent<PrefabInstanceComponent>();
				ImGui::Text("Instance of: %s", prefabComp.prefabName.c_str());
				ImGui::SameLine();

				if (prefabComp.syncWithPrefab)
				{
					ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), ICON_FA_LINK);
					if (ImGui::IsItemHovered())
					{
						ImGui::SetTooltip("Synced with prefab");
					}
				}
				else
				{
					ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), ICON_FA_LINK_SLASH);
					if (ImGui::IsItemHovered())
					{
						ImGui::SetTooltip("Not synced with prefab");
					}
				}
			}
			else
			{
				ImGui::Text("Not a prefab instance");
			}

			// Save as prefab button
			static bool showPopup = false;  // Track popup state
			if (hasChildren)
			{
				if (ImGui::Button("Save As Nested Prefab"))
				{
					showPopup = true;  // Set flag to show popup
				}
			}
			else
			{
				if (ImGui::Button("Save As Prefab"))
				{
					showPopup = true;  // Set flag to show popup
				}
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::TextUnformatted(hasChildren ? "Save this entity and its children as a nested prefab" : "Save this entity as a prefab");
				if (hasChildren)
				{
					ImGui::Text("Number of children: %d",
						entity.GetComponent<ParentComponent>().children.size());
				}
				ImGui::EndTooltip();
			}

			if (showPopup)
			{
				ShowSaveAsPrefabDialog(entity, &showPopup);
			}

			ImGui::SameLine();
			if (ImGui::Button("Unpack Prefab") && entity.HasComponent<PrefabInstanceComponent>())
			{
				entity.RemoveComponent<PrefabInstanceComponent>();
				Logger::Info("Unpacked prefab instance");
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Remove prefab link and make this an independent entity");
			}

			// Preview section
			if (ImGui::TreeNode("Preview"))
			{
				RenderPrefabPreview(entity);
				ImGui::TreePop();
			}
		}
	}

	void Editor::RenderPrefabPreview(Entity const& entity)
	{
		// Show preview of what will be saved
		ImGui::BeginChild("PrefabPreview", ImVec2(0, 200), true);

		// Components section
		if (ImGui::TreeNode("Components"))
		{
			if (entity.HasComponent<ActiveComponent>())
				ImGui::Text(ICON_FA_EYE " Active");
			if (entity.HasComponent<TransformComponent>())
				ImGui::Text(ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT " Transform");
			if (entity.HasComponent<RenderComponent>())
				ImGui::Text(ICON_FA_IMAGE " Render");
			if (entity.HasComponent<BoxColliderComponent>())
				ImGui::Text(ICON_FA_SQUARE " Box Collider");
			if (entity.HasComponent<RigidBodyComponent>())
				ImGui::Text(ICON_FA_CIRCLE_NOTCH " Rigid Body");
			if (entity.HasComponent<TextComponent>())
				ImGui::Text(ICON_FA_FONT " Text");
			if (entity.HasComponent<AudioComponent>())
				ImGui::Text(ICON_FA_VOLUME_HIGH " Audio");
			if (entity.HasComponent<LogicComponent>())
				ImGui::Text(ICON_FA_CODE " Script");
			if (entity.HasComponent<AnimationComponent>())
				ImGui::Text("Animation");

			ImGui::TreePop();
		}

		// Children section (if any)
		if (entity.HasComponent<ParentComponent>())
		{
			auto& parentComp = entity.GetComponent<ParentComponent>();
			if (!parentComp.children.empty() && ImGui::TreeNode("Children"))
			{
				for (auto& child : parentComp.children)
				{
					ImGui::PushID(child.GetId());
					bool hasGrandchildren = child.HasComponent<ParentComponent>() &&
						!child.GetComponent<ParentComponent>().children.empty();

					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
					if (!hasGrandchildren) flags |= ImGuiTreeNodeFlags_NoTreePushOnOpen;

					bool nodeOpen = ImGui::TreeNodeEx("Entity", flags);
					ImGui::SameLine();

					if (reg.EntityHasAnyTag(child))
					{
						ImGui::Text("%s [%s]", ICON_FA_CUBE, reg.GetEntityTag(child).c_str());
					}
					else
					{
						ImGui::Text("%s #%d", ICON_FA_CUBE, child.GetId());
					}

					if (hasGrandchildren && nodeOpen)
					{
						RenderPrefabPreview(child);
						ImGui::TreePop();
					}
					ImGui::PopID();
				}
				ImGui::TreePop();
			}
		}
		ImGui::EndChild();
	}

	void Editor::ShowSaveAsPrefabDialog(Entity& entity, bool* popupShown)
	{
		static char nameBuffer[256] = "";
		static bool dialogOpen = false;
		static bool* showPopup = nullptr;  // Pointer to the showPopup flag

		if (!dialogOpen)
		{
			// If entity has a tag, use it as default name
			if (reg.EntityHasAnyTag(entity))
			{
				strncpy_s(nameBuffer, reg.GetEntityTag(entity).c_str(), sizeof(nameBuffer));
			}
			dialogOpen = true;

			// Get pointer to the showPopup flag in the parent scope
			showPopup = popupShown;
		}

		// Center the popup
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		// Open the popup with a fixed flag
		if (ImGui::BeginPopupModal("Save As Prefab", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Enter prefab name(not file name):");
			ImGui::InputText("##prefabname", nameBuffer, sizeof(nameBuffer));

			// Show what will be included
			bool hasChildren = entity.HasComponent<ParentComponent>() &&
				!entity.GetComponent<ParentComponent>().children.empty();

			ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f),
				hasChildren ? "Will save entire hierarchy (%d children)" : "Will save single entity",
				hasChildren ? entity.GetComponent<ParentComponent>().children.size() : 0);

			ImGui::Separator();

			if (ImGui::Button("Save", ImVec2(120, 0)))
			{
				std::string prefabName = nameBuffer;
				if (!prefabName.empty())
				{
					std::string path = FileDialog::SaveFile("Prefab Files\0*.prefab\0", "prefab");
					if (!path.empty())
					{
						if (serializer.SaveEntityAsPrefab(entity, prefabName, path))
						{
							Logger::Info("Successfully saved prefab: " + prefabName);
						}
					}
				}
				dialogOpen = false;
				if (showPopup) *popupShown = false;  // Reset the parent's showPopup flag
				memset(nameBuffer, 0, sizeof(nameBuffer));  // Clear the buffer
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				dialogOpen = false;
				if (showPopup) *popupShown = false;  // Reset the parent's showPopup flag
				memset(nameBuffer, 0, sizeof(nameBuffer));  // Clear the buffer
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
		else
		{
			// If the popup isn't visible, open it
			ImGui::OpenPopup("Save As Prefab");
		}
	}
}