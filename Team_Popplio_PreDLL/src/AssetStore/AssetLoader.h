/******************************************************************************/
/*!
\file   AssetLoader.h
\author Team Popplio
\author Val Tay Yun Ying
\co-author
\contribution Val - 100%
\par    Course : CSD2401
\par    Section : A
\date   2024/11/30
\brief
	This source file declares methods that can be called by any file for the loading of Assets into the AssetStore

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "../Serialization/Serialization.h"
#include "../Script/LogicSystem.h"
#include "../ChartEditor/LevelChart.h"
namespace Popplio
{
	class AssetLoader
	{
	public:
		AssetLoader() = delete;
		AssetLoader(const AssetLoader&) = delete;
		AssetLoader(AssetLoader&&) = delete;
		AssetLoader& operator=(const AssetLoader&) = delete;
		AssetLoader& operator=(AssetLoader&&) = delete;
		~AssetLoader() = delete;

		// extension for meta files
		static const std::string& _metaExtension;

		/**
		 * \brief Function to assign audio system and logic system for loader
		 */
		static void Setup(AudioSystem& audioRef, LogicSystem& logicRef);

		/**
		 * \brief Function to recheck if everything in assetstore matches the file directory
		 */
		static void ReloadAssets();

		/**
		 * \brief Function to regenerate every meta file back to it's default state
		 * USE ONLY IF THERE IS NO OTHER CHOICE!!!!
		 * WARNING!!! ALL YOUR META SETTINGS WILL BE GONE IF YOU CALL THIS!!!!
		 */
		static void TotalWipe();

		/**
		 * \brief Checker for if a file is a recognized asset
		 * \return Returns true if the file extension is a recognized asset
		 */
		static bool IsAsset(const std::string& filename);

		/**
		 * \brief Loads Audio Asset
		 * \return returns true if the asset can be loaded into memory
		 */
		static bool LoadAudio(const char* filepath, const std::string& key, int channelGroup, float bpm = 0.f, std::pair<int, int> timeSig = std::pair<int, int>{});
		/**
		 * \brief Loads Texture Asset
		 * \return returns true if the asset can be loaded into memory
		 */
		static bool LoadTexture(const std::string& filepath);
		/**
		 * \brief Loads Shader Asset
		 * \return returns true if the asset can be loaded into memory
		 */
		static bool LoadShader(const std::string& filepath);
		/**
		 * \brief Loads Font Asset
		 * \return returns true if the asset can be loaded into memory
		 */
		static bool LoadFont(const std::string& filepath, std::list<unsigned int> fontsize);
		/**
		 * \brief Loads Script Asset (ONLY C++ SCRIPTS)
		 * C# scripts will be loaded elsewhere
		 * \return returns true if the asset can be loaded into memory
		 */
		static bool LoadCPPScripts(const std::string& scriptClass);
		/**
		 * \brief Loads Scene Asset
		 * \return returns true if the asset can be loaded into memory
		 */
		static bool LoadScene(const std::string& filepath);

	private:
		// reference to audio system
		static AudioSystem* audioSystemRef;
		// reference to logic system
		static LogicSystem* logicSystemRef;
	};

}
