/******************************************************************************/
/*!
\file   SceneManagement.cpp
\author Team Popplio
\author Bryan Ang Wei Ze
\co-author Shawn Ng Jun Heng
\contribution Bryan - 50% | Shawn - 50%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/05
\brief
	Renders the editor's top menu bar, contains various settings and tools.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>
#include "Editor.h"
#include "../Utilities/FileDialog.h"
#include "EntitySelectionEvents.h"

#include "../Graphic/RenderSystem.h"

namespace Popplio
{
	void Editor::RenderMainMenuBar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			ImGui::Text(serializer.GetConfiguration().engineVer.c_str());

			ImGui::SameLine();
			ImGui::Text("|");
			ImGui::SameLine();

            ImGui::Text(GetCurrentSceneName().c_str());

			ImGui::SameLine();
			ImGui::Text("|");
			ImGui::SameLine();

			if (hasUnsavedChanges)
			{
				ImGui::Text("*");
				ImGui::SameLine();
				ImGui::Text("|");
				ImGui::SameLine();
			}

			if (serializer.GetConfiguration().safeMode)
			{
				ImGui::Text("Safe Mode");

				ImGui::SameLine();
				ImGui::Text("|");
				ImGui::SameLine();
			}

			if (serializer.GetConfiguration().verbose)
			{
				ImGui::Text("Verbose");

				ImGui::SameLine();
				ImGui::Text("|");
				ImGui::SameLine();
			}

			if (debugMode)
			{
                ImGui::Text("Debug Mode");
				ImGui::SameLine();
				ImGui::Text("|");
				ImGui::SameLine();
			}

			RenderSceneMenu();

			ImGui::SameLine();
			ImGui::Text("|");
			ImGui::SameLine();

            RenderUndoRedoMenu();

			ImGui::SameLine();
			ImGui::Text("|");
			ImGui::SameLine();

			RenderWindowToggleMenu();

			ImGui::SameLine();
			ImGui::Text("|");
			ImGui::SameLine();

			RenderReloadMonoMenu();

			ImGui::SameLine();
			ImGui::Text("|");
			ImGui::SameLine();

			RenderDebugMenu();

			ImGui::EndMainMenuBar();
		}
	}

	void Editor::RenderReloadMonoMenu()
	{
		if (ImGui::MenuItem("Reload Mono"))
		{
			mono.ReloadAssembly();
		}
	}

	void Editor::RenderDebugMenu()
	{
		if (ImGui::MenuItem("Toggle Debug"))
		{
			debugMode = !debugMode;
			auto& render = reg.GetSystem<RenderSystem>();
			// toggle debug drawing
			render.ToggleDebugDrawing();
		}
	}
}