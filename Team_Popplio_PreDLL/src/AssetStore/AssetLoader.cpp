/******************************************************************************/
/*!
\file   AssetLoader.cpp
\author Team Popplio
\author Val Tay Yun Ying
\co-author
\contribution Val - 100%
\par    Course : CSD2401
\par    Section : A
\date   2024/11/30
\brief
	This source file defines methods for the loading of assets into AssetStore.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>

#include "AssetStore.h"
#include "AssetLoader.h"
#include "../Serialization/ChartEditorSerialization.h"
#include "../PerformanceViewer/PerformanceViewer.h"

#include <filesystem>
#include <algorithm>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

namespace Popplio {
	namespace fs = std::filesystem;

	AudioSystem* AssetLoader::audioSystemRef = nullptr;
	LogicSystem* AssetLoader::logicSystemRef = nullptr;
	const std::string& AssetLoader::_metaExtension{ ".cyndaquil" };

	std::pair<std::vector<fs::path>, std::unordered_set<fs::path>> ListFilesRecursively(const fs::path& directory)
	{
		std::vector<fs::path> assetFiles;
		std::unordered_set<fs::path> metaFiles; // Use set for O(1) lookup

		// Estimate size for preallocation (not exact, but helps reduce reallocation)
		size_t fileCount = std::distance(fs::recursive_directory_iterator(directory), {});
		assetFiles.reserve(fileCount / 2); // Rough guess: half might be asset files

		for (const auto& entry : fs::recursive_directory_iterator(directory)) {
			if (entry.is_regular_file()) {
				const fs::path& filePath = entry.path();

				if (filePath.extension() == AssetLoader::_metaExtension) {
					metaFiles.insert(filePath);
				}
				else if (AssetLoader::IsAsset(filePath.filename().string())) {
					assetFiles.push_back(filePath);
				}
			}
		}

		return { assetFiles, metaFiles };
	}

	void AssetLoader::ReloadAssets()
	{
		auto [assetFiles, metaFiles] = ListFilesRecursively(fs::current_path());

		PerformanceViewer::GetInstance()->StartRealTime("GetMetaFiles");

		for (const fs::path& file : assetFiles) {
			fs::path metaPath = file;
			metaPath.replace_extension(AssetLoader::_metaExtension);

			// Faster lookup using unordered_set instead of fs::exists()
			if (metaFiles.find(metaPath) == metaFiles.end()) {
				Serializer::SerializeMetaFile(file);
			}

			if (!Serializer::DeserializeMetaFile(file)) {
				Logger::Warning("Unable to deserialize file: " + metaPath.string());
			}
		}

		PerformanceViewer::GetInstance()->EndRealTime("GetMetaFiles");
	}

	void AssetLoader::TotalWipe()
	{
		// find every file thats != folder
		auto [assetFiles, metaFiles] = ListFilesRecursively(fs::current_path());

		for (fs::path const& meta : metaFiles)
		{
			if (!fs::remove(meta))
				Logger::Warning("Unable to delete meta file: " + meta.filename().string());
		}

		//check for each assetfile if theres a meta equiv
		for (fs::path const& file : assetFiles)
		{
			std::string metaPath = file.filename().string();
			metaPath = metaPath.substr(0, metaPath.find('.')) + _metaExtension;

			if (!fs::exists(metaPath))
			{
				Serializer::SerializeMetaFile(file);
			}

			if (!Serializer::DeserializeMetaFile(file))
			{
				Logger::Warning("Unable to deserialize file: " + metaPath);
			}

		}
	}

	bool AssetLoader::IsAsset(const std::string& filename)
	{
		std::string extension = filename.substr(filename.find("."));
		if (extension == ".wav" || extension == ".png" || extension == ".vert" ||
			extension == ".frag" || extension == ".ttf" || extension == ".cpp" ||
			extension == ".h" || extension == ".scene")
		{
			if (filename == "fa-brands-400.ttf" || filename == "fa-regular-400.ttf" || filename == "fa-solid-900.ttf")
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			return false;
		}
	}


	void AssetLoader::Setup(AudioSystem& audioRef, LogicSystem& logicRef)
	{
		audioSystemRef = &audioRef;
		logicSystemRef = &logicRef;
	}

	bool AssetLoader::LoadAudio(const char* filepath, const std::string& key, int channelGroup, float bpm, std::pair<int, int> timeSig)
	{
		//find the meta file with the same name
			// if dont have, generate one
			// otherwise deserialize the meta file and load the audio

		// meta file name
		try {
			audioSystemRef->SetAudio(filepath, key, audioSystemRef->GetCGroup(channelGroup), bpm, timeSig);
			return true;
		}
		catch (const std::exception&)
		{
			Logger::Error("Failed to load audio: " + std::string(filepath));
		}
		return false;
	}

	bool AssetLoader::LoadTexture(const std::string& filepath)
	{
		try {
			AssetStore::StoreTextureAsset(
				Serializer::GenerateAssetName(filepath),
				Texture(filepath).GetTextureID()
			);
		}
		catch (const std::exception&) {
			Logger::Error("Failed to load texture: " + filepath);
			return false;
		}
		return true;
	}

	bool AssetLoader::LoadShader(const std::string& filepath)
	{
		try {
			if (AssetStore::GetAllShaders().find(Serializer::GenerateAssetName(filepath)) != AssetStore::GetAllShaders().end()) return true;

			// remove extension from filepath
			std::string pathNoExtension = filepath.substr(0, filepath.find("."));
			AssetStore::StoreShaderAsset(
				Serializer::GenerateAssetName(filepath),
				std::make_unique<Shader>(pathNoExtension)
			);
		}
		catch (const std::exception&) {
			Logger::Error("Failed to load shader: " + filepath);
			return false;
		}

		return true;
	}

	bool AssetLoader::LoadFont(const std::string& filepath, std::list<unsigned int> fontsize)
	{
		// Generate an asset name based on the font path
		std::string assetName = Serializer::GenerateAssetName(filepath);

		// Create font instance with default size
		auto font = std::make_unique<Font>(filepath);

		// Load additional sizes
		for (const auto& size : fontsize) {
			font->LoadSize(size);
		}

		// Store the font
		try {
			AssetStore::StoreFontAsset(assetName, std::move(font));
		}
		catch (const std::exception&) {
			Logger::Error("Failed to load font: " + filepath);
			return false;
		}
		return true;
	}

	bool AssetLoader::LoadCPPScripts(const std::string& scriptClass)
	{
		try
		{
			logicSystemRef->LoadByName(scriptClass);
		}
		catch (const std::exception&)
		{
			Logger::Error("Failed to load script: " + scriptClass);
		}
		return true;
	}

	bool AssetLoader::LoadScene(const std::string& filepath)
	{
		// Generate an asset name based on the scene path
		std::string assetName = Serializer::GenerateAssetName(filepath);
		// Store the scene
		try {
			AssetStore::StoreSceneAsset(assetName, filepath);
		}
		catch (const std::exception&) {
			Logger::Error("Failed to load scene: " + filepath);
			return false;
		}
		return true;
	}
}