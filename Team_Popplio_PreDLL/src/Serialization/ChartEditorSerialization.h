/******************************************************************************/
/*!
\file           ChartEditorSerialization.h
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

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/istreamwrapper.h>

#include "../ChartEditor/LevelChart.h"

namespace Popplio
{
	class ChartEditorSerialization
	{
	public:
		static std::string _fileExtension;

		// New chart (Serialize new .Grookey file)
		static bool NewChart(LevelChart*& outLevel, std::string chartName, double songDuration, double bpm, int timeSigTop, int timeSigBot, std::string const& audioFilePath, std::string const& saveFilePath);

		// Load chart (Deserialize)
		static bool LoadChart(LevelChart*& outLevel, std::filesystem::path const& filePath);

		// Save chart (Serialize)
		static bool SaveChart(LevelChart* const& level);

	};
}
