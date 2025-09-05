/******************************************************************************/
/*!
\file AssetStore.cpp
\author Val Tay Yun Ying				 (tay.v) (100%)
\co-author:
\date 03-10-2024


All content � 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/
#include <pch.h>
#include "AssetStore.h"

//#include "../Utilities/Any.h"

namespace Popplio
{
	std::unordered_map<std::string, FMOD::Sound*> AssetStore::loadedAudioList;
	std::unordered_map<std::string, std::unique_ptr<Shader>> AssetStore::shaderList;
	std::unordered_map<std::string, unsigned int> AssetStore::textureList;
	std::unordered_map<std::string, std::unique_ptr<Font>> AssetStore::fontList;
	std::unordered_map<MeshType, Mesh> AssetStore::meshList;
	std::unordered_map<std::string, std::shared_ptr<ILogicScript>> AssetStore::loadedScriptList;
	std::unordered_map<std::string, std::shared_ptr<ILogicData>> AssetStore::loadedDataList;

	std::unordered_map<std::string, std::filesystem::path> AssetStore::sceneList;
	std::unordered_map<std::string, std::pair<int, int>> AssetStore::textureSizeList;

	void AssetStore::StoreAudioAsset(const std::string& assetId, FMOD::Sound* sound)
	{
		loadedAudioList[assetId] = sound;
	}

	void AssetStore::StoreShaderAsset(const std::string& assetId, std::unique_ptr<Shader> shader)
	{
		shaderList.emplace(assetId, std::move(shader));
	}

	void AssetStore::StoreTextureAsset(const std::string& assetId, unsigned int texture)
	{
		int width = 0, height = 0;

		// Bind the texture and query its dimensions
		glBindTexture(GL_TEXTURE_2D, texture);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

		// Store texture ID
		textureList.emplace(assetId, std::move(texture));

		// Store texture size in a separate map
		textureSizeList[assetId] = { width, height };
	}

	void AssetStore::StoreMeshAsset(MeshType type, const Mesh& mesh)
	{
		meshList.emplace(type, mesh);
	}

	void AssetStore::StoreFontAsset(const std::string& assetId, std::unique_ptr<Font> font)
	{
		fontList.emplace(assetId, std::move(font));
	}

	void AssetStore::StoreScriptAsset(const std::string& assetId, std::shared_ptr<ILogicScript> scp)
	{
		loadedScriptList.emplace(assetId, scp);
	}

	void AssetStore::StoreDataAsset(const std::string& assetId, std::shared_ptr<ILogicData> data)
	{
		loadedDataList.emplace(assetId, data);
	}

	void AssetStore::StoreSceneAsset(const std::string& assetId, const std::filesystem::path& path)
	{
		sceneList.emplace(assetId, path);
	}

	//void AssetStore::StoreChartAsset(const std::string& assetId, const std::filesystem::path& path)
	//{
	//	chartList.emplace(assetId, path);
	//}
	std::pair<int, int> AssetStore::GetTextureSize(const std::string& assetId)
	{
		auto it = textureSizeList.find(assetId);
		if (it != textureSizeList.end())
		{
			return it->second;
		}
		return { 0, 0 };
	}


	Shader* AssetStore::GetShader(const std::string& assetId)
	{
		auto it = shaderList.find(assetId);
		if (it != shaderList.end())
		{
			return it->second.get();
		}
		Logger::Warning("Shader asset not found: " + assetId);
		return nullptr;
	}

	unsigned int AssetStore::GetTexture(const std::string& assetId)
	{
		auto it = textureList.find(assetId);
		if (it != textureList.end())
		{
			return it->second;
		}
		Logger::Warning("Texture asset not found: " + assetId);
		return 0;
	}

	Font* AssetStore::GetFont(const std::string& assetId)
	{
		auto it = fontList.find(assetId);
		if (it != fontList.end())
		{
			return it->second.get();
		}
		Logger::Warning("Font asset not found: " + assetId);
		return nullptr;
	}

	Mesh& AssetStore::GetMesh(MeshType type)
	{
		auto it = meshList.find(type);
		if (it != meshList.end())
		{
			return it->second;
		}
		Logger::Warning("Mesh asset not found: " + type);
		static Mesh defaultMesh; // Return a static default mesh to avoid undefined behavior
		return defaultMesh;
	}

	FMOD::Sound* AssetStore::GetAudio(const std::string& assetId)
	{
		auto it = loadedAudioList.find(assetId);
		if (it != loadedAudioList.end())
		{
			return it->second;
		}
		Logger::Warning("Audio asset not found: " + assetId);
		return nullptr;
	}

	std::shared_ptr<ILogicScript> AssetStore::GetScript(const std::string& assetId)
	{
		auto it = loadedScriptList.find(assetId);
		if (it != loadedScriptList.end())
		{
			return it->second;
		}
		Logger::Warning("Script asset not found: " + assetId);
		return nullptr;
	}

	std::shared_ptr<ILogicData> AssetStore::GetData(const std::string& assetId)
	{
		auto it = loadedDataList.find(assetId);
		if (it != loadedDataList.end())
		{
			return it->second;
		}
		Logger::Warning("Data asset not found: " + assetId);
		return nullptr;
	}

	std::filesystem::path AssetStore::GetScene(const std::string& assetId)
	{
		auto it = sceneList.find(assetId);
		if (it != sceneList.end())
		{
			return it->second;
		}
		Logger::Warning("Scene asset not found: " + assetId);
		return std::filesystem::path("");
	}

	std::string AssetStore::GetSceneId(std::string const& path)
	{
		for (auto const& [id, scenePath] : sceneList)
		{
			if (scenePath == path)
			{
				return id;
			}
		}

		for (auto const& [id, scenePath] : sceneList)
		{
			if (scenePath == std::filesystem::absolute(path))
			{
				return id;
			}
		}

		Logger::Warning("Scene not found: " + path);
		return "";
	}

	//std::filesystem::path AssetStore::GetChart(const std::string& assetId)
	//{
	//	auto it = sceneList.find(assetId);
	//	if (it != sceneList.end())
	//	{
	//		return it->second;
	//	}
	//	Logger::Warning("Chart asset not found: " + assetId);
	//	return std::filesystem::path("");
	//}

	void AssetStore::Clear()
	{
		shaderList.clear();
		textureList.clear();
		fontList.clear();
		meshList.clear();

		loadedScriptList.clear();
		loadedDataList.clear();

		// For FMOD sounds, we need to release them properly
		for (std::pair<const std::string, FMOD::Sound*> pair : loadedAudioList)
		{
			pair.second->release();
		}

		loadedAudioList.clear();
	}

	const std::unordered_map<std::string, std::unique_ptr<Shader>>& AssetStore::GetAllShaders()
	{
		return shaderList;
	}

	const std::unordered_map<std::string, unsigned int>& AssetStore::GetAllTextures()
	{
		return textureList;
	}

	const std::unordered_map<std::string, std::unique_ptr<Font>>& AssetStore::GetAllFonts()
	{
		return fontList;
	}

	const std::unordered_map<std::string, FMOD::Sound*>& AssetStore::GetAllAudio()
	{
		return loadedAudioList;
	}

	const std::unordered_map<std::string, std::shared_ptr<ILogicScript>>& AssetStore::GetAllScripts()
	{
		return loadedScriptList;
	}

	const std::unordered_map<std::string, std::shared_ptr<ILogicData>>& AssetStore::GetAllData()
	{
		return loadedDataList;
	}

	const std::unordered_map<std::string, std::filesystem::path>& AssetStore::GetAllScenes()
	{
		return sceneList;
	}

	//const std::unordered_map<std::string, std::filesystem::path>& AssetStore::GetAllCharts()
	//{
	//	return chartList;
	//}

	const std::vector<std::string> AssetStore::GetShaderKeys()
	{
		std::vector<std::string> returning;
		auto it = shaderList.begin();
		for (; it != shaderList.end(); ++it)
		{
			if (it->first != "")
				returning.emplace_back(it->first);
		}
		return returning;
	}
	const std::vector<std::string> AssetStore::GetTextureKeys()
	{
		std::vector<std::string> returning;
		auto it = textureList.begin();
		for (; it != textureList.end(); ++it)
		{
			if (it->first != "")
				returning.emplace_back(it->first);
		}
		return returning;
	}
	const std::vector<std::string> AssetStore::GetFontKeys()
	{
		std::vector<std::string> returning;
		auto it = fontList.begin();
		for (; it != fontList.end(); ++it)
		{
			if (it->first != "")
				returning.emplace_back(it->first);
		}
		return returning;
	}
	const std::vector<std::string> AssetStore::GetAudioKeys()
	{
		std::vector<std::string> returning;
		auto it = loadedAudioList.begin();
		for (; it != loadedAudioList.end(); ++it)
		{
			if (it->first != "")
				returning.emplace_back(it->first);
		}
		return returning;
	}
	const std::vector<std::string> AssetStore::GetScriptKeys()
	{
		std::vector<std::string> returning;
		auto it = loadedScriptList.begin();
		for (; it != loadedScriptList.end(); ++it)
		{
			if (it->first != "")
				returning.emplace_back(it->first);
		}
		return returning;
	}
	const std::vector<std::string> AssetStore::GetDataKeys()
	{
		std::vector<std::string> returning;
		auto it = loadedDataList.begin();
		for (; it != loadedDataList.end(); ++it)
		{
			if (it->first != "")
				returning.emplace_back(it->first);
		}
		return returning;
	}
	const std::vector<std::string> AssetStore::GetSceneKeys()
	{
		std::vector<std::string> returning;
		auto it = sceneList.begin();
		for (; it != sceneList.end(); ++it)
		{
			if (it->first != "")
				returning.emplace_back(it->first);
		}
		return returning;
	}
	//const std::vector<std::string> AssetStore::GetChartKeys()
	//{
	//	std::vector<std::string> returning;
	//	auto it = chartList.begin();
	//	for (; it != chartList.end(); ++it)
	//	{
	//		if (it->first != "")
	//			returning.emplace_back(it->first);
	//	}
	//	return returning;
	//}
}