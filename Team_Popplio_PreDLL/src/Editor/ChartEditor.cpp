/******************************************************************************/
/*!
\file   ChartEditor.cpp
\author Team Popplio
\author Pearl Goh
\co-author Val
\contribution Pearl - 95% | Val - 5%
\par    Course : CSD2401
\par    Section : A
\date   2025/03/14 04:06:00 PM (Last Edit)
\brief	This source file implements a chart editor that allows
		creation and editing of beatmaps. Features include:
		- Audio file loading and playback controls
		- BPM and time signature adjustment
		- Timeline visualization with measure markers
		- Note placement on multiple lanes
		- Support for both regular and hold notes
		- Save/Load functionality for .grookey chart files


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
#include "../Serialization/ChartEditorSerialization.h"

namespace Popplio
{
	static LevelChart* currentChart = nullptr;
	static bool isElementDisabled = false;
	static bool isPlacingHold = false;
	static bool isRainbowNote = false;
	static bool isPlacingDirectional = false;
	static bool isDown = false;

	void Editor::RenderChartEditor()
	{
		ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

		if (ImGui::Begin(ICON_FA_MUSIC " Chart Editor", nullptr,
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
		{
			// Calculate available content height
			float availableHeight = ImGui::GetContentRegionAvail().y;
			float controlBarHeight = 80.0f;  // Height for controls
			float timelineHeight = 100.0f;

			// Remaining space for lanes
			float lanesHeight = availableHeight - controlBarHeight - timelineHeight -
				ImGui::GetStyle().ItemSpacing.y * 2;

			// Control Bar
			ImGui::BeginChild("ControlBar", ImVec2(0, controlBarHeight), true,
				ImGuiWindowFlags_NoScrollbar);
			{
				// First row - Add audio file selector
				RenderAudioFileSelector();
				ImGui::Separator();

				// First row of buttons
				if (ImGui::Button(ICON_FA_FILE " New Chart", ImVec2(120, 30)))
				{
					OpenNewChartDialog();
				}

				ImGui::SameLine();
				if (ImGui::Button(" Save Chart", ImVec2(120, 30)))
				{
					SaveChartMenu();
				}

				ImGui::SameLine();
				if (ImGui::Button(ICON_FA_FOLDER_OPEN " Load Chart", ImVec2(120, 30)))
				{
					OpenLoadChartDialog();

				}

				// Second row - BPM and Time Signature controls
				ImGui::SameLine();
				float spacing = 20.0f;
				ImGui::Dummy(ImVec2(spacing, 0));
				ImGui::SameLine();

				// BPM Control with large number box
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted("BPM");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(60);
				bool bpmChanged = ImGui::InputDouble("##BPM", &bpm, 0, 0, "%.1f");
				if (bpmChanged)
				{
					bpm = (std::max)(1.0, (std::min)(300.0, bpm));
				}

				ImGui::SameLine();
				ImGui::Dummy(ImVec2(spacing, 0));
				ImGui::SameLine();

				// Time Signature with two separate input boxes
				ImGui::TextUnformatted("Time Signature");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(35);

				// Numerator input
				if (ImGui::InputInt("##TimeSigNum", &numerator, 0, 0))
				{
					numerator = std::clamp(numerator, 1, 32);
				}

				ImGui::SameLine(0, 5);
				ImGui::SetNextItemWidth(35);

				// Denominator input
				if (ImGui::InputInt("##TimeSigDen", &denominator, 0, 0))
				{
					denominator = std::clamp(denominator, 1, 32);
				}
			}
			ImGui::EndChild();

			if (!isAudioLoaded)
			{
				ImGui::BeginDisabled();
				isElementDisabled = true;
			}

			// Update playhead position for both timeline and lanes
			if (isChartPlaying && m_audioComponent.data >= 0)
			{
				AudioSystem& audioSys = reg.GetSystem<AudioSystem>();

				// Make sure the instance still exists
				if (audioSys.GetAudioInst(m_audioComponent.data).GetID() != selectedAudioFile)
				{
					// Audio instance has been removed or changed, recreate it
					m_audioComponent.data = -1;
					m_audioComponent.Update(audioSys);

					if (m_audioComponent.data >= 0)
					{
						Audio& audioData = audioSys.GetAudioInst(m_audioComponent.data);
						audioData.Play();
					}
					else
					{
						isChartPlaying = false;
					}
				}

				// Continue with existing code...
				if (m_audioComponent.data >= 0)
				{
					Audio& audioData = audioSys.GetAudioInst(m_audioComponent.data);
					double currentTimeSeconds = audioData.GetCurr() / 1000.0; // Convert ms to seconds
					double beatsPerSecond = bpm / 60.0;
					m_playheadPosition = currentTimeSeconds * beatsPerSecond;

					// Stop at the end
					if (m_playheadPosition >= totalMeasures)
					{
						m_playheadPosition = 0.0;
						isChartPlaying = false;
					}

					m_lastFrameTime = ImGui::GetTime();
				}
			}

			// controls

			ImGui::BeginChild("Note control", ImVec2(0, lanesHeight / 3.0f), true,
				ImGuiWindowFlags_NoScrollbar);

			ImGui::Checkbox("Hold Shift and drag mouse for HOLD notes!", &isPlacingHold);
			ImGui::Checkbox("Hold Ctrl for RAINBOW notes!", &isRainbowNote);
			ImGui::Checkbox("Hold Up/Down arrows for DIRECTIONAL notes!", &isPlacingDirectional);
			ImGui::SameLine();
			if (isPlacingDirectional)
				ImGui::Text(isDown ? "v" : "^");
			ImGui::SameLine();

			ImGui::Spacing();
			ImGui::EndChild();

			// Second row: Playback controls (only visible if audio is loaded)
			if (isAudioLoaded)
			{

				//ImGui::SameLine();
				//ImGui::Dummy(ImVec2(20.0f, 0)); // Spacing
				//ImGui::SameLine();

				// Play/Pause button
				if (ImGui::Button(isChartPlaying ? ICON_FA_PAUSE : ICON_FA_PLAY, ImVec2(40, 30)))
				{
					if (isAudioLoaded)
					{
						ToggleAudioPlayback();
					}
				}

				ImGui::SameLine();

				// Stop Button
				if (ImGui::Button(ICON_FA_STOP, ImVec2(40, 30)))
				{
					StopAudio();
				}
			}

			// Timeline window with horizontal scrolling
			static double timeline_x_min = 0;
			static double timeline_x_max = 16;

			ImGui::BeginChild("Timeline", ImVec2(0, timelineHeight), true);
			if (ImPlot::BeginPlot("##Timeline",
				ImVec2(-1, timelineHeight - ImGui::GetStyle().WindowPadding.y * 2),
				ImPlotFlags_NoTitle | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect))
			{
				// Configure the timeline plot with scrolling
				ImPlot::SetupAxisLimits(ImAxis_X1, timeline_x_min, timeline_x_max, ImGuiCond_Once);
				ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1, ImGuiCond_Always);

				ImPlot::SetupAxis(ImAxis_X1, "Timeline");
				ImPlot::SetupAxis(ImAxis_Y1, nullptr,
					ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoTickMarks);

				const int interval = 60;
				const int numTicks = (1800 / interval) + 1;
				static std::vector<double> customTicks(numTicks);
				static std::vector<std::string> labelStrings(numTicks);
				static std::vector<const char*> customLabels(numTicks);

				// Generate ticks and labels
				for (int i = 0; i < numTicks; i++) {
					customTicks[i] = i * interval;
					labelStrings[i] = std::to_string(static_cast<int>(customTicks[i]));
					customLabels[i] = labelStrings[i].c_str();
				}

				// Set custom ticks
				ImPlot::SetupAxisTicks(ImAxis_X1, customTicks.data(), numTicks, customLabels.data());

				// Draw beat lines (subdivisions within measures)
				if (numerator > 1)
				{
					std::vector<double> beatX, beatY;
					for (int measure = 0; measure < totalMeasures; ++measure)
					{
						for (int beat = 1; beat < numerator; ++beat)
						{
							double beatPos = measure + (double)beat / numerator;
							beatX.push_back(beatPos);
							beatY.push_back(0.5);
						}
					}
					ImPlot::SetNextLineStyle(ImVec4(0.5f, 0.5f, 0.5f, 0.5f), 1.0f);
					ImPlot::PlotLine("##beats", beatX.data(), beatY.data(), static_cast<int>(beatX.size()));
				}

				// Draw playhead
				double playheadX[2] = { m_playheadPosition, m_playheadPosition };
				double playheadY[2] = { 0.0, 1.0 };
				ImPlot::SetNextLineStyle(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), 2.0f);
				ImPlot::PlotLine("##playhead", playheadX, playheadY, 2);

				if (editorState != State::PLAYING && ImPlot::IsPlotHovered() && ImGui::IsMouseDown(0))
				{
					ImPlotPoint mouse = ImPlot::GetPlotMousePos();
					m_playheadPosition = std::clamp(mouse.x, 0.0, 129.0);
				}

				ImPlot::EndPlot();

			}
			ImGui::EndChild();

			// Lanes window
			ImGui::BeginChild("Lanes", ImVec2(0, lanesHeight), true,
				ImGuiWindowFlags_NoScrollbar);

			bool isHoldKeyPressed = ImGui::GetIO().KeyShift;
			bool isRainbowKeyPressed = ImGui::GetIO().KeyCtrl;

			if (isPlacingDirectional == ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_UpArrow)))
			{
				//isPlacingDirectional = true;
				isDown = false;
			}
			else if (isPlacingDirectional == ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_DownArrow)))
			{
				//isPlacingDirectional = true;
				isDown = true;
			}

			if (isRainbowKeyPressed) isRainbowNote = true;
			else isRainbowNote = false;

			ImPlotFlags flags = ImPlotFlags_NoTitle | ImPlotFlags_NoMenus;
			if (isPlacingHold) flags |= ImPlotFlags_NoInputs;


			if (ImPlot::BeginPlot("##Lanes", ImVec2(-1, -1), flags))
			{
				// Configure the lanes plot
				ImPlot::SetupAxisLimits(ImAxis_X1, 0, 16, ImGuiCond_Once);
				ImPlot::SetupAxisLimits(ImAxis_Y1, -0.2, 2.2, ImGuiCond_Always);
				ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_NoTickLabels);
				ImPlot::SetupAxis(ImAxis_Y1, "Lanes");


				// Draw bar lines
				for (int bar = 0; bar <= totalMeasures; ++bar)
				{
					double barX[2] = { static_cast<double>(bar), static_cast<double>(bar) };
					double barY[2] = { -0.2, 2.2 };

					// Make bar lines more prominent every 4 bars
					if (bar % 4 == 0)
					{
						ImPlot::SetNextLineStyle(ImVec4(0.0f, 0.0f, 1.0f, 0.8f), 2.0f); // Blue bars (prominent)
					}
					else
					{
						ImPlot::SetNextLineStyle(ImVec4(0.0f, 0.0f, 1.0f, 0.5f), 1.0f); // Blue bars (thin)
					}
					ImPlot::PlotLine(("##barline" + std::to_string(bar)).c_str(), barX, barY, 2);
				}

				// Draw beat lines within each bar if time signature numerator > 1
				if (numerator > 1)
				{
					for (int bar = 0; bar < totalMeasures; ++bar)
					{
						for (int beat = 1; beat < numerator; ++beat)
						{
							double beatPos = bar + (static_cast<double>(beat) / numerator);
							double beatX[2] = { beatPos, beatPos };
							double beatY[2] = { -0.2, 2.2 };
							ImPlot::SetNextLineStyle(ImVec4(0.3f, 0.3f, 0.3f, 0.3f), 1.0f);
							ImPlot::PlotLine(("##beatline" + std::to_string(bar) + "_" + std::to_string(beat)).c_str(),
								beatX, beatY, 2);
						}
					}
				}

				// Draw lane backgrounds
				for (int lane = 0; lane < 3; ++lane)
				{
					double laneY[] = { static_cast<double>(lane), static_cast<double>(lane) };
					double laneX[] = { 0, static_cast<double>(totalMeasures) };
					ImPlot::SetNextLineStyle(ImVec4(1.0f, 0.0f, 0.0f, 0.8f), 2.0f); // Red lanes
					ImPlot::PlotLine(("##lane" + std::to_string(lane)).c_str(), laneX, laneY, 2);
				}

				// Handle note placement
				static double holdStartX = 0.0;
				static int holdStartLane = -1;

				if (ImPlot::IsPlotHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					ImPlotPoint mousePos = ImPlot::GetPlotMousePos();

					// Round to nearest lane
					int lane = static_cast<int>(std::round(mousePos.y));

					if (lane >= 0 && lane < 3)
					{
						// Round to nearest subdivision based on time signature
						double subdivision = 1.0 / numerator;
						double roundedX = std::round(mousePos.x / subdivision) * subdivision;

						if (isHoldKeyPressed)
						{
							// Start placing a hold note
							holdStartX = roundedX;
							holdStartLane = lane;
							isPlacingHold = true;
						}
						else if (isPlacingDirectional)
						{
							// i switched the directions here -shawn

							// if the lane isnt bottom & im putting a UP note
							// OR
							// if the lane isnt top & im putting a DOWN note
							if ((lane != 0 && !isDown) || (lane != 2 && isDown))
							{
								// Place directional note
								Note newNote;
								newNote.barstart = static_cast<float>(roundedX);
								newNote.lane = lane;
								newNote.noteType = isDown ? Note::NOTETYPE::DIR_DOWN : Note::NOTETYPE::DIR_UP;
								newNote.rainbow = isRainbowNote;
								notes.push_back(newNote);
								hasUnsavedChanges_chart = true;
							}
						}
						else
						{
							// Place regular note
							Note newNote;
							newNote.barstart = static_cast<float>(roundedX);
							newNote.lane = lane;
							newNote.noteType = Note::NOTETYPE::BEAT;
							newNote.rainbow = isRainbowNote;
							notes.push_back(newNote);
							hasUnsavedChanges_chart = true;
						}
					}
				}

				// Handle hold note placement completion
				if (isPlacingHold && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
				{
					ImPlotPoint mousePos = ImPlot::GetPlotMousePos();
					double subdivision = 1.0 / numerator;
					double roundedX = std::round(mousePos.x / subdivision) * subdivision;

					if (roundedX > holdStartX && holdStartX >= 0)
					{
						Note newNote;
						newNote.barstart = static_cast<float>(holdStartX);
						newNote.lane = holdStartLane;
						newNote.noteType = Note::NOTETYPE::HOLD;
						newNote.beatLength = static_cast<float>(roundedX - holdStartX);
						newNote.rainbow = isRainbowNote;
						notes.push_back(newNote);
						hasUnsavedChanges_chart = true;
					}

					isPlacingHold = false;
					holdStartLane = -1;
				}

				// Draw notes
				for (size_t i = 0; i < notes.size(); i++)
				{
					Note& note = notes[i];
					double noteX[] = { static_cast<double>(note.barstart) };
					double noteY[] = { static_cast<double>(note.lane) };

					std::string noteId = "##note" + std::to_string(i);

					// Draw hold note line and fill
					if (note.noteType == Note::NOTETYPE::HOLD && note.beatLength > 0.0)
					{
						// Draw start note
						ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 6);
						ImPlot::PlotScatter(noteId.c_str(), noteX, noteY, 1);

						// Draw end note
						double endX[] = { static_cast<double>(note.barstart + note.beatLength) };
						double endY[] = { static_cast<double>(note.lane) };
						ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 6);
						ImPlot::PlotScatter((noteId + "_end").c_str(), endX, endY, 1);

						// Draw the connecting line
						double lineX[] = { static_cast<double>(note.barstart), static_cast<double>(note.barstart + note.beatLength) };
						double lineY[] = { static_cast<double>(note.lane), static_cast<double>(note.lane) };
						ImPlot::SetNextLineStyle(ImVec4(1.0f, 0.8f, 0.0f, 0.8f), 4.0f);
						ImPlot::PlotLine(("##holdline" + noteId).c_str(), lineX, lineY, 2);
					}

					// Draw the note point
					ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 6);
					ImPlot::PlotScatter(noteId.c_str(), noteX, noteY, 1);

					// Get screen coordinates for note position
					ImPlotPoint plotPos = ImPlotPoint(note.barstart, note.lane);
					ImVec2 screenPos = ImPlot::PlotToPixels(plotPos);

					// Create a larger click area
					float clickRadius = 10.0f;
					ImVec2 mousePos = ImGui::GetMousePos();
					float dx = mousePos.x - screenPos.x;
					float dy = mousePos.y - screenPos.y;
					float distSquared = dx * dx + dy * dy;

					// Check if mouse is over note
					bool isHovered = distSquared <= clickRadius * clickRadius && ImPlot::IsPlotHovered();

					if (isHovered)
					{
						// Visual feedback
						ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 8, ImVec4(1, 1, 0, 1));
						ImPlot::PlotScatter(("Right click to delete" + noteId).c_str(), noteX, noteY, 1);
						ImPlot::PlotScatter(("Rainbow: " + std::to_string(notes[i].rainbow)).c_str(), noteX, noteY, 1);

						// Handle right-click deletion
						if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
						{
							notes.erase(notes.begin() + i);
							hasUnsavedChanges_chart = true;
							break;
						}
					}
				}

				// Draw playhead
				double playheadX[2] = { m_playheadPosition, m_playheadPosition };
				double playheadY[2] = { -0.5, 2.5 };
				ImPlot::SetNextLineStyle(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), 2.0f);
				ImPlot::PlotLine("##laneplayhead", playheadX, playheadY, 2);

				ImPlot::EndPlot();
			}
			ImGui::EndChild();

			if (!isAudioLoaded)
			{
				ImGui::EndDisabled();
				isElementDisabled = false;
			}
		}
		ImGui::End();
	}

	//OPEN NEW CHART
	void Editor::OpenNewChartDialog()
	{
		// Allow selection of both .wav and .grookey files
		std::string path = FileDialog::OpenFile("Audio Files (*.wav)\0*.wav\0Chart Files (*.grookey)\0*.grookey\0", "");
		if (path.empty()) return;

		std::string extension = std::filesystem::path(path).extension().string();

		if (extension == ".wav")
		{
			// Create new chart from audio file
			LoadAudioFile(path);
			if (isAudioLoaded)
			{
				CreateNewChart();
			}
		}
		else if (extension == ".grookey")
		{
			LoadGrookeyChart(path);
		}
	}

	void Editor::CreateNewChart()
	{
		if (!isAudioLoaded) return;

		// Create new chart object
		std::string chartName = "New Chart";
		float audioLengthInSeconds = 0.0f;

		// Get audio length from audio system
		AudioSystem& audioSys = reg.GetSystem<AudioSystem>();
		int tempAudioInstance = audioSys.InstAudio(selectedAudioFile);
		if (tempAudioInstance >= 0)
		{
			Audio& audioData = audioSys.GetAudioInst(tempAudioInstance);
			audioLengthInSeconds = audioData.GetLen() / 1000.0f;
			audioSys.RemoveAudioInst(tempAudioInstance);
		}

		// Generate new chart
		std::string defaultSavePath = "Charts/"; // Default save directory
		currentChart = new LevelChart(
			chartName,
			audioLengthInSeconds,
			bpm,
			numerator,
			denominator,
			selectedAudioFile,
			defaultSavePath
		);

		// Reset editor state
		notes.clear();
		currentChartPath.clear();
		hasUnsavedChanges_chart = false;
		m_playheadPosition = 0.0;
		isChartPlaying = false;

		Logger::Info("New chart created");
	}

	//SAVE CHART
	void Editor::SaveChartMenu()
	{
		if (currentChartPath.empty())
		{
			// No current path - open "Save As" dialog
			std::string path = FileDialog::SaveFile("Chart Files\0*.grookey\0", "grookey");
			if (!path.empty())
			{
				SaveChart(path);
			}
		}
		else
		{
			// Has existing path - save to current path
			SaveChart(currentChartPath);
		}
	}


	void Editor::SaveChart(const std::string& path)
	{
		if (!currentChart || !isAudioLoaded)
		{
			Logger::Warning("Cannot save chart: No chart or audio loaded");
			return;
		}

		// Update chart data before saving
		currentChart->chartSavePath = std::filesystem::path(path).parent_path().string() + "/";
		currentChart->chartName = std::filesystem::path(path).stem().string();
		currentChart->audioFilePath = selectedAudioFile;
		currentChart->bpm = bpm;

		// Clear existing notes and update with current editor notes
		currentChart->notesList.clear();
		for (size_t i = 0; i < notes.size(); ++i)
		{
			int startBar = static_cast<int>(std::floor(notes[i].barstart / numerator));

			float startBeatNum = static_cast<float>(notes[i].barstart - (numerator * startBar));
			if (notes[i].noteType == Note::NOTETYPE::HOLD)
			{

				currentChart->AddHoldNote(
					currentChart,
					static_cast<float>(startBar),
					static_cast<int>(startBar),
					startBeatNum,
					//totalBeats,
					notes[i].beatLength,
					notes[i].lane,
					notes[i].rainbow,
					0
				);
			}
			else if (notes[i].noteType == Note::NOTETYPE::BEAT)
			{

				currentChart->AddBeatNote(
					currentChart,				//chart
					0,
					startBar,
					startBeatNum,
					notes[i].beatLength,
					notes[i].lane,
					notes[i].rainbow,
					notes[i].spriteID
				);
			}
			else
			{
                // i switched it here -shawn
				// if bottom lane has a UP dir note
				if (notes[i].noteType == Note::NOTETYPE::DIR_UP && notes[i].lane == 0)
				{
					continue;
				}
				// if top lane has a DOWN dir note
				else if (notes[i].noteType == Note::NOTETYPE::DIR_DOWN && notes[i].lane == 2)
				{
					continue;
				}

				currentChart->AddDirectionalNote(
					currentChart,				//chart
					notes[i].noteType,
					0,
					startBar,
					startBeatNum,
					notes[i].beatLength,
					notes[i].lane,
					notes[i].rainbow,
					notes[i].spriteID
				);
			}
		}

		// Save using serialization
		if (ChartEditorSerialization::SaveChart(currentChart))
		{
			currentChartPath = path;
			hasUnsavedChanges_chart = false;
			Logger::Info("Chart saved successfully: " + path);
		}
		else
		{
			Logger::Error("Failed to save chart: " + path);
		}
	}

	//LOAD CHART
	void Editor::OpenLoadChartDialog()
	{
		// Allow selection of both .wav and .grookey files
		std::string path = FileDialog::OpenFile("Audio Files (*.wav)\0*.wav\0Chart Files (*.grookey)\0*.grookey\0", "");
		if (path.empty()) return;

		std::string extension = std::filesystem::path(path).extension().string();

		if (extension == ".wav")
		{
			// Create new chart from audio file
			LoadAudioFile(path);
			if (isAudioLoaded)
			{
				CreateNewChart();
			}
		}
		else if (extension == ".grookey")
		{
			// Load existing chart
			LoadGrookeyChart(path);
		}
	}

	void Editor::LoadGrookeyChart(const std::string& path)
	{
		// Clear existing chart data
		notes.clear();

		// Delete existing chart if any
		if (currentChart)
		{
			delete currentChart;
		}

		// Create a temporary chart first
		currentChart = new LevelChart("temp", 0.0, 0.0, 0, 0, "", "");

		// Load chart using serialization
		if (ChartEditorSerialization::LoadChart(currentChart, path))
		{
			numerator = currentChart->timeSig[TOPNUMBER];
			denominator = currentChart->timeSig[BOTTOMNUMBER];
			bpm = currentChart->bpm;

			// Convert chart notes to editor format
			for (const auto& note : currentChart->notesList)
			{
				Note editorNote;
				// Convert bar and beat to barstart position
				editorNote.barstart = (note.bar * numerator) + note.beat;
				editorNote.lane = note.lane;
				editorNote.noteType = note.noteType;
				editorNote.rainbow = note.rainbow;
				if (note.noteType == Note::NOTETYPE::HOLD)
				{
					editorNote.beatLength = note.beatLength;
					editorNote.noteType = Note::NOTETYPE::HOLD;
				}


				notes.push_back(editorNote);
			}

			// Load associated audio file
			LoadAudioFile(currentChart->audioFilePath);

			currentChartPath = path;
			hasUnsavedChanges_chart = false;
			Logger::Info("Chart loaded successfully: " + path);
		}
		else
		{
			Logger::Error("Failed to load chart: " + path);
			// Clean up on failure
			delete currentChart;
			currentChart = nullptr;
		}
	}

	//AUDIO STUFF
	void Editor::RenderAudioFileSelector()
	{
		ImGui::Text("Audio File: %s", selectedAudioFile.empty() ? "None" : selectedAudioFile.c_str());
		ImGui::SameLine();

		if (ImGui::Button(ICON_FA_FOLDER_OPEN " Browse"))
		{
			std::string filePath = FileDialog::OpenFile("Audio Files\0*.wav\0", "wav");
			if (!filePath.empty())
			{
				LoadAudioFile(filePath);
			}
		}

		
		if (!isAudioLoaded)
		{
			// Show warning inline with other elements
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(20.0f, 0)); // Spacing
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f),
				"Please select an audio file to enable chart editing");
		}
	}

	void Editor::LoadAudioFile(const std::string& path)
	{
		if (path.empty()) return;

		try
		{
			// Clean up existing audio instance if any
			if (m_audioComponent.data >= 0)
			{
				AudioSystem& audioSys = reg.GetSystem<AudioSystem>();
				audioSys.RemoveAudioInst(m_audioComponent.data);
				m_audioComponent.data = -1;
			}

			// Convert file path to asset name format
			selectedAudioFile = Serializer::GenerateAssetName(path);

			// Configure the audio component
			m_audioComponent.isActive = true;
			m_audioComponent.audio = selectedAudioFile;
			m_audioComponent.volume = 1.0f;
			m_audioComponent.mute = false;
			m_audioComponent.loop = false;
			m_audioComponent.grp = 1; // BGM group

			// Initialize the audio component
			AudioSystem& audioSys = reg.GetSystem<AudioSystem>();
			m_audioComponent.Update(audioSys);

			if (m_audioComponent.data >= 0) // Successfully created audio instance
			{
				// Get a reference to the audio instance
				Audio& audioData = audioSys.GetAudioInst(m_audioComponent.data);

				// Calculate total measures based on audio length
				float audioLengthInSeconds = audioData.GetLen() / 1000.0f; // Convert ms to seconds
				totalMeasures = audioLengthInSeconds > 0 ?
					static_cast<int>(std::ceil(audioLengthInSeconds * (bpm / 60.0) * numerator)) : 128;

				isAudioLoaded = true;

				// Reset playback state
				m_playheadPosition = 0.0;
				m_lastFrameTime = ImGui::GetTime();
				isChartPlaying = false;

				Logger::Info("Loaded audio file: " + selectedAudioFile);
			}
			else
			{
				Logger::Error("Failed to create audio instance for: " + selectedAudioFile);
				isAudioLoaded = false;
				selectedAudioFile.clear();
			}
		}
		catch (const std::exception& e)
		{
			Logger::Error("Failed to load audio file: " + std::string(e.what()));
			isAudioLoaded = false;
			selectedAudioFile.clear();
		}
	}

	void Editor::ToggleAudioPlayback()
	{
		if (m_audioComponent.data < 0) return;

		AudioSystem& audioSys = reg.GetSystem<AudioSystem>();
		Audio& audioData = audioSys.GetAudioInst(m_audioComponent.data);

		isChartPlaying = !isChartPlaying;
		if (isChartPlaying)
		{
			// Start or resume playback
			m_lastFrameTime = ImGui::GetTime();

			// Convert playhead position to time in seconds
			double beatsPerSecond = bpm / 60.0;
			double timeInSeconds = m_playheadPosition / beatsPerSecond;

			// Convert seconds to milliseconds for the audio API
			int timeInMs = static_cast<int>(timeInSeconds * 1000.0);

			// Set the audio position to match the playhead
			audioData.SetCurr(timeInMs);

			// Ensure the audio is unpaused and playing
			audioData.Play();
		}
		else
		{
			// Pause playback
			audioData.Pause();
		}
	}

	void Editor::StopAudio()
	{
		isChartPlaying = false;
		m_playheadPosition = 0.0;

		if (m_audioComponent.data < 0) return;

		AudioSystem& audioSys = reg.GetSystem<AudioSystem>();
		Audio& audioData = audioSys.GetAudioInst(m_audioComponent.data);

		// Only stop this specific audio instance instead of all audio
		audioData.Stop();
		// Reset playhead to beginning
		audioData.SetCurr(0);
	}
}