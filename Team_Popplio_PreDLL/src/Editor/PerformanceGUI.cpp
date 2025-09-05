/******************************************************************************/
/*!
\file		PerformanceGUI.cpp
\author		Team Popplio
\author		Val Tay Yun Ying (100%)
\co-author
\par		Course : CSD2400 / CSD2401
\par		Section : A
\date		2024/11/5
\brief
		This source file defines the PerformanceViewer class

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>
#include "Editor.h"
#include "../PerformanceViewer/PerformanceViewer.h"

namespace Popplio
{
	static PerformanceViewer* pvInstance = PerformanceViewer::GetInstance();

	void Editor::RenderPerformanceGUI()
	{
		static bool showGraph = false;
		ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
		static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

		if (ImGui::Begin(ICON_FA_CHART_SIMPLE " Performance Viewer"))

			ImGui::Checkbox("Show in Graph format", &showGraph);
		{
			if (showGraph)
			{
				if (ImPlot::BeginPlot("Performance Graph"))
				{
					//ImPlot::PlotBars("Engine Time",PerformanceViewer::GetInstance()->GetSystemTimeQueue(PerformanceViewer::_MainEngine))
					ImPlot::EndPlot();
				}
				//	// draw graph of the overall Engine time first

				//	//------------------------probably need to improve this-------------------------------------------------

				//	// make a copy of the queue
				//	std::queue<int> timeQueue{ pvInstance->GetSystemTimeQueue("engine") };
				//	size_t originalSize = timeQueue.size();
				//	float* engineTimes = new float[originalSize];

				//	for (size_t i = 0; i < originalSize; ++i)
				//	{
				//		engineTimes[i] = (float)timeQueue.front();
				//		timeQueue.pop();
				//	}
				//	//------------------------probably need to improve this-------------------------------------------------
				//	//ImGui::plot
				//	ImGui::PlotHistogram("Engine Performance", engineTimes, pvInstance->GetSystemTimeQueue("engine").size());

				//	// delete the temp new array
				//	delete[] engineTimes;

				//	// draw graph of individual systems here

			}
			else
			{
				if (ImGui::BeginTable("Performance Table", 3, flags))
				{
					ImGui::TableSetupColumn("Name");
					ImGui::TableSetupColumn("Time (Microseconds | us)");
					ImGui::TableSetupColumn("Time %");
					ImGui::TableHeadersRow();

					std::vector<std::string> sysNameVec{};
					for (auto& it : pvInstance->GetAllSystemTimeMap())
					{
						sysNameVec.push_back(it.first);
					}

					//make Engine the first ROW
					ImGui::TableNextRow();
					for (int col = 0; col < 3; ++col)
					{
						ImGui::TableSetColumnIndex(col);
						std::string buf{};
						switch (col)
						{
						case 0:
							//sprintf_s(buf, "Hi %s", (std::string)(sysNameVec[row]));
							buf = PerformanceViewer::_MainEngine;
							break;
						case 1:
							//sprintf_s(buf, "Hello %ld", pvInstance->GetSystemTime(sysNameVec[row]).count());
							buf = std::to_string(pvInstance->GetSystemTime(PerformanceViewer::_MainEngine).count()) + " us";
							break;
						case 2:
							//sprintf_s(buf, "%d", pvInstance->GetSystemTimePercentage(sysNameVec[row]));
							buf = std::to_string(pvInstance->GetSystemTimePercentage(PerformanceViewer::_MainEngine)) + "%";
							break;
						}
						ImGui::TextUnformatted(buf.c_str());
					}


					for (int row = 0; row < pvInstance->GetNumSystemsRegistered(); ++row)
					{
						// skip the engine category
						if (sysNameVec[row] == PerformanceViewer::_MainEngine) continue;

						ImGui::TableNextRow();
						for (int col = 0; col < 3; ++col)
						{
							ImGui::TableSetColumnIndex(col);
							std::string buf{};
							switch (col)
							{
							case 0:
								//sprintf_s(buf, "Hi %s", (std::string)(sysNameVec[row]));
								buf = sysNameVec[row];
								break;
							case 1:
								//sprintf_s(buf, "Hello %ld", pvInstance->GetSystemTime(sysNameVec[row]).count());
								buf = std::to_string(pvInstance->GetSystemTime(sysNameVec[row]).count()) + " us";
								break;
							case 2:
								//sprintf_s(buf, "%d", pvInstance->GetSystemTimePercentage(sysNameVec[row]));
								buf = std::to_string(pvInstance->GetSystemTimePercentage(sysNameVec[row])) + "%";
								break;
							}
							ImGui::TextUnformatted(buf.c_str());
						}
					}
					ImGui::EndTable();
				}
			}
		}
		ImGui::End();
	}
}
