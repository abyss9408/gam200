/******************************************************************************/
/*!
\file           ChartEditorSerialization.cpp
\author         Team Popplio
\author         Val Tay Yun Ying
\co-author
\contribution   Val - 100%
\par            Course : CSD2401 / CSD2400
\par            Section : A
\date           2025/3/3
\brief
	This is the header file for the serializer of the engine

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include "pch.h"
#include "ChartEditorSerialization.h"
#include "Windows.h"
namespace Popplio
{
	std::string ChartEditorSerialization::_fileExtension{ ".grookey" };

	bool ChartEditorSerialization::NewChart(LevelChart*& outLevel, std::string chartName, double songDuration, double bpm, int timeSigTop, int timeSigBot, std::string const& audioFilePath, std::string const& saveFilePath)
	{
		std::stringstream ss;
		ss << "Generating chart: " << chartName + _fileExtension << " at " << saveFilePath;
		Logger::Info(ss.str());
		outLevel = new LevelChart(chartName, songDuration, bpm, timeSigTop, timeSigBot, audioFilePath, saveFilePath);

		// save the document to this location
		std::string fileName = chartName + _fileExtension;
		Logger::Debug(saveFilePath + fileName);

		// open a new json document
		rapidjson::Document document;
		document.SetObject();
		auto& allocator = document.GetAllocator();

		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

		//rapidjson::Value levelVal{ rapidjson::kObjectType };
		//std::string stringRefName{ "chart" };

		// name of chart
		rapidjson::Value name{ rapidjson::kStringType };
		name.SetString(chartName.c_str(), allocator);
		document.AddMember("name", name, allocator);

		// duration of the song
		rapidjson::Value duration{ rapidjson::kObjectType };
		duration.SetDouble(songDuration);
		document.AddMember("duration", duration, allocator);

		// bpm of the song
		rapidjson::Value beatspm{ rapidjson::kObjectType };
		beatspm.SetDouble(bpm);
		document.AddMember("bpm", beatspm, allocator);

		// time sig of the song
		rapidjson::Value timeSigT{ rapidjson::kObjectType };
		timeSigT.SetInt(timeSigTop);
		document.AddMember("time sig top", timeSigT, allocator);

		rapidjson::Value timeSigB{ rapidjson::kObjectType };
		timeSigB.SetInt(timeSigBot);
		document.AddMember("time sig bot", timeSigB, allocator);

		// nfilepath of the audio
		rapidjson::Value audiopath{ rapidjson::kStringType };
		audiopath.SetString(audioFilePath.c_str(), allocator);
		document.AddMember("audio file path", audiopath, allocator);

		// chart file's save location
		rapidjson::Value savefile{ rapidjson::kStringType };
		savefile.SetString((saveFilePath + fileName).c_str(), allocator);
		document.AddMember("save file path", savefile, allocator);

		rapidjson::Value notes{ rapidjson::kArrayType };
		notes.SetArray();
		document.AddMember("notes", notes, allocator);

		//// adding the chart into the document
		//document.AddMember(rapidjson::StringRef(stringRefName.c_str()), levelVal, allocator);

		document.Accept(writer);


		std::ofstream outFile(saveFilePath + fileName);
		if (!outFile)
		{
			Logger::Error("Failed to open file for writing: " + fileName);
			return false;
		}

		outFile << buffer.GetString();
		outFile.close();

		return true;
	}

	bool ChartEditorSerialization::LoadChart(LevelChart*& outLevel, std::filesystem::path const& filePath)
	{

		// if the filepath given is not a .grookey file
		if (filePath.extension().string() != _fileExtension)
		{
			Logger::Warning("Given path is not a valid chart file! \nGiven file: " + filePath.string());
			return false;
		}

		//open file 
		std::ifstream inFile(filePath.string());
		if (!inFile)
		{
			Logger::Error("Failed to open file for reading: " + filePath.string());
			return false;
		}

		rapidjson::IStreamWrapper isw(inFile);
		rapidjson::Document chartDoc;
		chartDoc.ParseStream(isw);

		if (chartDoc.HasParseError())
		{
			Logger::Error("Failed to parse prefab JSON file: " + filePath.string(),
				LogDestination::WINDOWS_CONSOLE);

			inFile.close(); // added to fix error that cant save / load after
			return false;
		}

		outLevel->chartName = chartDoc["name"].GetString();
		outLevel->songDuration = chartDoc["duration"].GetDouble();
		outLevel->bpm = chartDoc["bpm"].GetDouble();
		outLevel->timeSig[0] = chartDoc["time sig top"].GetInt();
		outLevel->timeSig[1] = chartDoc["time sig bot"].GetInt();
		outLevel->audioFilePath = chartDoc["audio file path"].GetString();
		outLevel->chartSavePath = chartDoc["save file path"].GetString();

		auto notesArray = chartDoc["notes"].GetArray();
		std::vector<Note> noteVector;
		for (const auto& note : notesArray)
		{
			Note newNote = Note();
			newNote.id = note["id"].GetInt();
			//newNote.barstart = note["barstart"].GetInt();
			newNote.bar = note["bar"].GetInt();
			newNote.beat = note["beat"].GetFloat();
			newNote.beatLength = note["beatlength"].GetFloat();
			newNote.lane = note["lane"].GetInt();
			newNote.noteType = (Note::NOTETYPE)note["notetype"].GetInt();
			newNote.rainbow = note["rainbow"].GetBool();
			newNote.spriteID = note["spriteid"].GetInt();

			noteVector.push_back(newNote);
		}

		outLevel->notesList = noteVector;


		return true;
	}

	void SortNotes(std::vector<Note>& notesList)
	{
		auto compareNotesLambda = [](const Note& a, const Note& b) {
			if (a.bar != b.bar) {
				return a.bar < b.bar;
			}
			return a.beat < b.beat;
			};

		std::sort(notesList.begin(), notesList.end(), compareNotesLambda);
	}

	bool ChartEditorSerialization::SaveChart(LevelChart* const& level)
	{
		//sort the notes in the level according to note position & beat first
		//std::vector<Note> sortedNotes{ level->notesList };
		//sortedNotes.reserve(level->notesList.size());

		SortNotes(level->notesList);


		std::stringstream ss;
		ss << "Generating chart: " << level->chartName + _fileExtension << " at " << level->chartSavePath;
		Logger::Info(ss.str());

		// save the document to this location
		std::string fileName = level->chartName + _fileExtension;
		Logger::Debug(level->chartSavePath + fileName);

		// open a new json document
		rapidjson::Document document;
		document.SetObject();
		auto& allocator = document.GetAllocator();


		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

		//rapidjson::Value levelVal{ rapidjson::kObjectType };
		//std::string stringRefName{ "chart" };

		// name of chart
		rapidjson::Value name{ rapidjson::kStringType };
		name.SetString(level->chartName.c_str(), allocator);
		document.AddMember("name", name, allocator);

		// duration of the song
		rapidjson::Value duration{ rapidjson::kObjectType };
		duration.SetDouble(level->songDuration);
		document.AddMember("duration", duration, allocator);

		// bpm of the song
		rapidjson::Value bpm{ rapidjson::kObjectType };
		bpm.SetDouble(level->bpm);
		document.AddMember("bpm", bpm, allocator);

		// time sig of the song
		rapidjson::Value timeSigTop{ rapidjson::kObjectType };
		timeSigTop.SetInt(level->timeSig[0]);
		document.AddMember("time sig top", timeSigTop, allocator);

		rapidjson::Value timeSigBot{ rapidjson::kObjectType };
		timeSigBot.SetInt(level->timeSig[1]);
		document.AddMember("time sig bot", timeSigBot, allocator);

		// nfilepath of the audio
		rapidjson::Value audiopath{ rapidjson::kStringType };
		audiopath.SetString(level->audioFilePath.c_str(), allocator);
		document.AddMember("audio file path", audiopath, allocator);

		// chart file's save location
		rapidjson::Value savefile{ rapidjson::kStringType };
		savefile.SetString(level->chartSavePath.c_str(), allocator);
		document.AddMember("save file path", savefile, allocator);

		rapidjson::Value notes{ rapidjson::kArrayType };
		notes.SetArray();

		// lambda for saving notes
		auto addNoteMember = [&allocator, &notes, &level, &document](int index)
			{
				rapidjson::Value note{ rapidjson::kObjectType };

				//1 
				rapidjson::Value id{ rapidjson::kNumberType };
				id.SetInt(index);
				note.AddMember("id", id, allocator);

				////2
				//rapidjson::Value barstart{ rapidjson::kNumberType };
				//barstart.SetInt(level->notesList[index].barstart);
				//note.AddMember("barstart", barstart, allocator);

				//3
				rapidjson::Value bar{ rapidjson::kNumberType };
				bar.SetInt(level->notesList[index].bar);
				note.AddMember("bar", bar, allocator);

				//4
				rapidjson::Value beat{ rapidjson::kNumberType };
				beat.SetFloat(level->notesList[index].beat);
				note.AddMember("beat", beat, allocator);

				////5
				//rapidjson::Value beatFractionTop{ rapidjson::kNumberType };
				//beatFractionTop.SetInt(level->notesList[index].beatFractionTop);
				//note.AddMember("beatfractiontop", beatFractionTop, allocator);

				////6
				//rapidjson::Value beatFractionBot{ rapidjson::kNumberType };
				//beatFractionBot.SetInt(level->notesList[index].beatFractionBot);
				//note.AddMember("beatfractionbot", beatFractionBot, allocator);

				//7
				rapidjson::Value beatLength{ rapidjson::kNumberType };
				beatLength.SetFloat(level->notesList[index].beatLength);
				note.AddMember("beatlength", beatLength, allocator);

				////8
				//rapidjson::Value beatLengthFractionTop{ rapidjson::kNumberType };
				//beatLengthFractionTop.SetInt(level->notesList[index].beatLengthFractionTop);
				//note.AddMember("beatlengthfractiontop", beatLengthFractionTop, allocator);

				////9
				//rapidjson::Value beatLengthFractionBot{ rapidjson::kNumberType };
				//beatLengthFractionBot.SetInt(level->notesList[index].beatLengthFractionBot);
				//note.AddMember("beatlengthfractionbot", beatLengthFractionBot, allocator);

				//10
				rapidjson::Value lane{ rapidjson::kNumberType };
				lane.SetInt(level->notesList[index].lane);
				note.AddMember("lane", lane, allocator);

				//11
				rapidjson::Value noteType{ rapidjson::kNumberType };
				noteType.SetInt(level->notesList[index].noteType);
				note.AddMember("notetype", noteType, allocator);

				//12
				rapidjson::Value rainbow{ rapidjson::kObjectType };
				rainbow.SetBool(level->notesList[index].rainbow);
				note.AddMember("rainbow", rainbow, allocator);

				//13
				rapidjson::Value spriteID{ rapidjson::kNumberType };
				spriteID.SetInt(level->notesList[index].spriteID);
				note.AddMember("spriteid", spriteID, allocator);

				notes.PushBack(note, allocator);
			};

		for (size_t i = 0; i < level->notesList.size(); ++i)
		{
			addNoteMember(static_cast<int>(i));
		}

		document.AddMember("notes", notes, allocator);

		document.Accept(writer);

		std::cout << level->chartSavePath + fileName << std::endl;
		std::ofstream outFile(level->chartSavePath + fileName);
		if (!outFile)
		{
			Logger::Error("Failed to open file for writing: " + fileName);
			return false;
		}

		outFile << buffer.GetString();
		outFile.close();

		return true;
	}

}