/******************************************************************************/
/*!
\file   LayerEditor.cpp
\author Team Popplio
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/02/06
\brief
	This soruce file defines methods for handling the layer management within the 
	Editor class in the Popplio namespace. The methods allow for adding, listing, 
	and managing layers using the ImGui library.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include "Editor.h"

namespace Popplio
{
	void Editor::RenderLayerWindow()
	{
		ImGui::Begin(ICON_FA_LAYER_GROUP " Layers");

		// Add Layer button and popup (keeping your existing code)
		if (ImGui::Button("Add Layer"))
		{
			ImGui::OpenPopup("Add Layer");
		}

		RenderAddLayerPopup();
		ImGui::Separator();

		// Add tab bar for different views
		if (ImGui::BeginTabBar("LayerViewsTabBar"))
		{
			if (ImGui::BeginTabItem("List View"))
			{
				RenderLayerList();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Matrix View"))
			{
				RenderLayerMatrix();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::End();
	}

	void Editor::RenderAddLayerPopup()
	{
		static char layerName[256] = "";
		static bool isActive = true;

		if (ImGui::BeginPopup("Add Layer"))
		{
			ImGui::Text("New Layer");
			ImGui::Separator();

			ImGui::InputText("Name", layerName, sizeof(layerName));
			ImGui::Checkbox("Active", &isActive);

			if (ImGui::Button("Create"))
			{
				if (strlen(layerName) > 0)
				{
					layerManager.AddLayer(layerName, isActive);
					memset(layerName, 0, sizeof(layerName));
					isActive = true;
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				memset(layerName, 0, sizeof(layerName));
				isActive = true;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void Editor::RenderLayerList()
	{
		auto sortedLayers = layerManager.GetSortedLayers();

		// Column headers
		ImGui::Columns(4, "LayerColumns", true);
		ImGui::Text("Name");
		ImGui::NextColumn();
		ImGui::Text("Order");
		ImGui::NextColumn();
		ImGui::Text("Active");
		ImGui::NextColumn();
		ImGui::Text("Actions");
		ImGui::NextColumn();
		ImGui::Separator();

		for (const auto& layer : sortedLayers)
		{
			// Layer name
			ImGui::Text("%s", layer.name.c_str());
			if (ImGui::IsItemClicked())
			{
				selectedLayer = layer.name;
			}
			ImGui::NextColumn();

			// Order
			int order = layer.order;
			if (ImGui::InputInt(("##order" + layer.name).c_str(), &order))
			{
				if (order >= 0)
				{
					layerManager.SetLayerOrder(layer.name, order);
				}
			}
			ImGui::NextColumn();

			// Active toggle
			bool active = layer.active;
			if (ImGui::Checkbox(("##active" + layer.name).c_str(), &active))
			{
				layerManager.SetLayerActive(layer.name, active);
			}
			ImGui::NextColumn();

			// Actions
			if (ImGui::Button(("Delete##" + layer.name).c_str()))
			{
				ImGui::OpenPopup(("Delete Layer##" + layer.name).c_str());
			}

			// Delete confirmation popup
			if (ImGui::BeginPopupModal(("Delete Layer##" + layer.name).c_str()))
			{
				ImGui::Text("Are you sure you want to delete layer '%s'?", layer.name.c_str());
				ImGui::Text("This will remove all entity assignments to this layer.");

				if (ImGui::Button("Yes"))
				{
					layerManager.RemoveLayer(layer.name);
					if (selectedLayer == layer.name)
					{
						selectedLayer.clear();
					}
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("No"))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			ImGui::NextColumn();
		}
		ImGui::Columns(1);
	}

	void Editor::RenderEntityLayerControls()
	{
		// Only show if an entity is selected
		if (!selectedEntity.has_value())
		{
			return;
		}

		ImGui::Begin("Layer Assignment");

		Entity entity = selectedEntity.value();
		std::string currentLayer = layerManager.GetEntityLayer(entity);

		// Show selected entity ID
		ImGui::Text("Selected Entity: %d", entity.GetId());

		// Show current layer assignment
		ImGui::Text("Current Layer: %s", currentLayer.empty() ? "Name" : currentLayer.c_str());
		
		// Layer selection combo
		if (ImGui::BeginCombo("Assign to Layer", currentLayer.c_str()))
		{
			// Add "None" option to remove from layer
			if (ImGui::Selectable("None", currentLayer.empty()))
			{
				layerManager.RemoveEntityFromCurrentLayer(entity);
			}

			// List all layers
			for (const auto& layer : layerManager.GetSortedLayers())
			{
				bool isSelected = (currentLayer == layer.name);
				if (ImGui::Selectable(layer.name.c_str(), isSelected))
				{
					layerManager.AddEntityToLayer(entity, layer.name);
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		ImGui::End();
	}

	void Editor::RenderLayerMatrix()
	{
		auto sortedLayers = layerManager.GetSortedLayers();
		if (sortedLayers.empty())
		{
			ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "No layers available.");
			return;
		}

		// Constants for layout
		const float cellSize = 24.0f;
		const float headerSize = 150.0f;
		const float headerHeight = 24.0f;
		ImGuiTableFlags flags = ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInnerV;

		// Create header table for layer names along the top
		if (ImGui::BeginTable("LayerMatrixHeader", static_cast<int>(sortedLayers.size() + 1), flags))
		{
			// Setup columns
			ImGui::TableSetupColumn("##Corner", ImGuiTableColumnFlags_WidthFixed, headerSize);
			for (const auto& layer : sortedLayers)
			{
				ImGui::TableSetupColumn(layer.name.c_str(), ImGuiTableColumnFlags_WidthFixed, cellSize);
			}

			// Draw header row
			ImGui::TableNextRow(ImGuiTableRowFlags_Headers, headerHeight);
			ImGui::TableSetColumnIndex(0);
			ImGui::Text(""); // Empty corner cell

			// Draw column headers
			for (size_t i = 0; i < sortedLayers.size(); ++i)
			{
				ImGui::TableSetColumnIndex(static_cast<int>(i + 1));
				// Draw rotated layer name or number
				char buf[32];
				snprintf(buf, sizeof(buf), "%zu", i);
				ImGui::Text("%s", buf);
			}
			ImGui::EndTable();
		}

		// Create main matrix table
		if (ImGui::BeginTable("LayerMatrix", static_cast<int>(sortedLayers.size() + 1), flags))
		{
			// Setup columns (same as header)
			ImGui::TableSetupColumn("##Labels", ImGuiTableColumnFlags_WidthFixed, headerSize);
			for (const auto& layer : sortedLayers)
			{
				ImGui::TableSetupColumn(layer.name.c_str(), ImGuiTableColumnFlags_WidthFixed, cellSize);
			}

			// Draw matrix rows
			for (int i = static_cast<int>(sortedLayers.size()) - 1; i >= 0; --i)
			{
				ImGui::TableNextRow(ImGuiTableRowFlags_None, cellSize);

				// Draw row header (layer name)
				ImGui::TableSetColumnIndex(0);
				bool isActive = sortedLayers[i].active;
				if (ImGui::Checkbox(("##" + sortedLayers[i].name).c_str(), &isActive))
				{
					layerManager.SetLayerActive(sortedLayers[i].name, isActive);
				}
				ImGui::SameLine();
				ImGui::Text("%s", sortedLayers[i].name.c_str());

				// Draw interaction cells
				for (size_t j = 0; j < sortedLayers.size(); ++j)
				{
					ImGui::TableSetColumnIndex(static_cast<int>(j + 1));

					// Only draw cells in the lower triangle (where j <= i)
					if (j > i)
					{
						// Empty cell in lower triangle
						ImGui::Dummy(ImVec2(cellSize - 4, cellSize - 4));
						continue;
					}

					// Create unique ID for the cell
					std::string cellId = "##cell_" + std::to_string(i) + "_" + std::to_string(j);

					// Get interaction state
					bool shouldInteract = layerManager.ShouldLayersInteract(
						sortedLayers[i].name,
						sortedLayers[j].name
					);

					// Set cell colors
					ImVec4 cellColor;
					if (i == j)
					{
						// Same layer - gray
						cellColor = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
					}
					else
					{
						// Different layers - green for interaction, red for no interaction
						cellColor = shouldInteract ?
							ImVec4(0.2f, 0.6f, 0.2f, 1.0f) :  // Green
							ImVec4(0.6f, 0.2f, 0.2f, 1.0f);   // Red
					}

					// Draw the cell
					ImGui::PushStyleColor(ImGuiCol_Button, cellColor);
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
						ImVec4(cellColor.x + 0.1f, cellColor.y + 0.1f, cellColor.z + 0.1f, 1.0f));

					if (ImGui::Button(cellId.c_str(), ImVec2(cellSize - 4, cellSize - 4)))
					{
						if (i != j) // Don't toggle interaction with self
						{
							layerManager.SetLayerInteraction(
								sortedLayers[i].name,
								sortedLayers[j].name,
								!shouldInteract
							);
						}
					}

					// Tooltip
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Text("%s %s %s",
							sortedLayers[i].name.c_str(),
							shouldInteract ? "collides with" : "does not collide with",
							sortedLayers[j].name.c_str());
						ImGui::EndTooltip();
					}

					ImGui::PopStyleColor(2);
				}
			}
			ImGui::EndTable();
		}

		// Legend
		ImGui::Separator();
		ImGui::TextDisabled("(?) Layer Matrix Help");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(
				"Layer Matrix Controls:\n"
				"- Green cells: Layers will collide/interact\n"
				"- Red cells: Layers will not collide/interact\n"
				"- Gray cells: Same layer (cannot be modified)\n"
				"- Click cells to toggle interaction\n"
				"- Checkboxes control layer visibility\n"
				"- Numbers show layer rendering order\n"
				"- Only upper triangle shown since interactions are symmetric"
			);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}
}