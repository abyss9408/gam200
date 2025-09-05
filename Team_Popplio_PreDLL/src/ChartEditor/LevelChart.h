/******************************************************************************/
/*!
\file           LevelChart.h
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

#pragma once

constexpr auto TOPNUMBER = 0;
constexpr auto BOTTOMNUMBER = 1;

namespace Popplio
{
	struct Note
	{
		enum NOTETYPE
		{
			BEAT = 0,
			HOLD,
			DIR_UP,
			DIR_DOWN
		};

		int id;

		// not being used in final, purely for calculation now
		float barstart;

		// which bar?
		int bar;

		// which beat in the bar? (0 to timeSig[0])
		float beat;

		//// which subbeat in the beat? (0 to timeSig[1])
		//int subbeat;

		// [Hold note only!!] how long this note is?
		float beatLength;

		int lane;			// which lane this is
		NOTETYPE noteType;	// type of note this is
		bool rainbow;		// this note has rainbow?
		int spriteID;		// spriteID



		// default constructor
		Note() :
			id(0),
			barstart(0),
			bar(0),
			beat(0),
			beatLength(0),
			lane(0),
			noteType(NOTETYPE::BEAT),
			rainbow(false),
			spriteID(0)
		{
		}

		// overloaded constructor
		Note(int id, float barstart, int bar, float beat, float beatLength, 
			int lane, NOTETYPE notetype, bool rainbow, int sprite) :
			id(id),
			barstart(barstart),
			bar(bar),
			beat(beat),
			beatLength(beatLength),
			lane(lane),
			noteType(notetype),
			rainbow(rainbow),
			spriteID(0)
		{
            (void)sprite;
		}
	};

	struct LevelChart
	{
		std::string chartName;
		// duration of the song
		double songDuration;
		double bpm;
		int timeSig[2];
		std::string audioFilePath;
		std::string chartSavePath;
		//int timeSig[2];

		// list of lanes, with a list of notes in that lane
		std::vector<Note> notesList;

		// no default constructor
		LevelChart() = delete;

		LevelChart(std::string chartName, double songDuration, double bpm, int timeSigTop, int timeSigBot, std::string audioFilePath, std::string chartSavePath);
		~LevelChart();

		static void AddBeatNote(LevelChart*& chart, float barstart, int bar, float beat, float beatLength, int lane, bool rainbow, int spriteID);

		static void AddDirectionalNote(LevelChart*& chart, Note::NOTETYPE type, float barstart, int bar, float beat, float beatLength, int lane, bool rainbow, int spriteID);

		static void AddHoldNote(LevelChart*& chart, float barstart, int bar, float beat, float beatLength, int lane, bool rainbow, int spriteID);
	};

}
