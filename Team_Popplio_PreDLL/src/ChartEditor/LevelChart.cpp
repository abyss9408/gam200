/******************************************************************************/
/*!
\file           LevelChart.cpp
\author         Team Popplio
\author         Val Tay Yun Ying
\co-author
\contribution   Val - 100%
\par            Course : CSD2401 / CSD2400
\par            Section : A
\date           2025/1/14
\brief
	This is the header file for the serializer of the engine

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include "pch.h"
#include "LevelChart.h"

namespace Popplio
{
	LevelChart::LevelChart(std::string chartName, double songDuration, double bpm, int timeSigTop, int timeSigBot, std::string audioFilePath, std::string chartSavePath) :
		chartName(chartName),
		songDuration(songDuration),
		bpm(bpm),
		audioFilePath(audioFilePath),
		chartSavePath(chartSavePath)
	{
		timeSig[TOPNUMBER] = timeSigTop;
		timeSig[BOTTOMNUMBER] = timeSigBot;
	}

	LevelChart::~LevelChart()
	{
	}

	void LevelChart::AddBeatNote(LevelChart*& chart, float barstart, int bar, float beat, float beatLength, int lane, bool rainbow, int spriteID)
	{
		chart->notesList.push_back(Note(static_cast<int>(chart->notesList.size()), barstart, bar, beat, beatLength, lane, Note::NOTETYPE::BEAT, rainbow, spriteID));
	}

	void LevelChart::AddDirectionalNote(LevelChart*& chart, Note::NOTETYPE type, float barstart, int bar, float beat, float beatLength, int lane, bool rainbow, int spriteID)
	{
		chart->notesList.push_back(Note(static_cast<int>(chart->notesList.size()), barstart, bar, beat, beatLength, lane, type, rainbow, spriteID));
	}

	void LevelChart::AddHoldNote(LevelChart*& chart, float barstart, int bar, float beat, float beatLength, int lane, bool rainbow, int spriteID)
	{
		chart->notesList.push_back(Note(static_cast<int>(chart->notesList.size()), barstart, bar, beat, beatLength, lane, Note::NOTETYPE::HOLD, rainbow, spriteID));
	}
}
