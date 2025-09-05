/******************************************************************************/
/*!
\file   AssetStore.h
\author Team Popplio
\author Val Tay Yun Ying
\co-author
\contribution Val - 100%
\par    Course : CSD2401
\par    Section : A
\date   2024/10/03
\brief
	This source file declares methods for the AssetStore. Functions related to the SAVING of assets is in this static class.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#ifndef ASSET_STORE_HEADER
#define ASSET_STORE_HEADER

#include <memory>
#include <iostream>
#include <string>
#include <typeindex>
#include <vector>
#include <unordered_map>
//#include <any>

#include <fmod.hpp>
#include "../Graphic/Mesh/Mesh.h"
#include "../Graphic/Shader/Shader.h"
#include "../Graphic/Texture/Texture.h"
#include "../Graphic/Font/Font.h"
#include "../Graphic/GraphicComponent.h"
#include "../Script/LogicScript.h"
#include "../Script/LogicData.h"

namespace Popplio
{
	extern const char* assetTypeList[];

	class AssetStore
	{
	private:
		AssetStore(const AssetStore&) = delete;
		AssetStore& operator=(const AssetStore&) = delete;

		static std::unordered_map<std::string, FMOD::Sound*> loadedAudioList;
		static std::unordered_map<std::string, std::unique_ptr<Shader>> shaderList;
		static std::unordered_map<std::string, unsigned int> textureList;
		static std::unordered_map<std::string, std::unique_ptr<Font>> fontList;
		static std::unordered_map<MeshType, Mesh> meshList;
		static std::unordered_map<std::string, std::shared_ptr<ILogicScript>> loadedScriptList;
		static std::unordered_map<std::string, std::shared_ptr<ILogicData>> loadedDataList; // deprecated

		static std::unordered_map<std::string, std::filesystem::path> sceneList;

		static std::unordered_map<std::string, std::filesystem::path> chartList;
		static std::unordered_map<std::string, std::pair<int, int>> textureSizeList; // Stores texture sizes

	public:

		// Storing Asset (Setters)
		static std::pair<int, int> GetTextureSize(const std::string& assetId); // New function

		/**
		 * \brief Stores Audio Asset
		 * \param assetID The ID to save the asset to
		 * \param sound The Sound asset to be saved into the assetstore
		 */
		static void StoreAudioAsset(const std::string& assetId, FMOD::Sound* sound);
		/**
		 * \brief Stores Shader Asset
		 * \param assetID The ID to save the asset to
		 * \param shader The shader asset to be saved into the assetstore
		 */
		static void StoreShaderAsset(const std::string& assetId, std::unique_ptr<Shader> shader);
		/**
		 * \brief Stores Texture Asset
		 * \param assetID The ID to save the asset to
		 * \param texture The texture asset to be saved into the assetstore
		 */
		static void StoreTextureAsset(const std::string& assetId, unsigned int texture);
		/**
		 * \brief Stores Mesh Asset
		 * \param assetID The ID to save the asset to
		 * \param mesh The mesh to be saved into the assetstore
		 */
		static void StoreMeshAsset(MeshType type, const Mesh& mesh);
		/**
		 * \brief Stores Font Asset
		 * \param assetID The ID to save the asset to
		 * \param font The Font asset to be saved into the assetstore
		 */
		static void StoreFontAsset(const std::string& assetId, std::unique_ptr<Font> font);
		/**
		 * \brief Stores Script Asset
		 * \param assetID The ID to save the asset to
		 * \param scp The Script asset to be saved into the assetstore
		 */
		static void StoreScriptAsset(const std::string& assetId, std::shared_ptr<ILogicScript> scp);
		/**
		 * \brief Stores Data Asset
		 * \param assetID The ID to save the asset to
		 * \param data The ILogicData asset to be saved into the assetstore
		 */
		static void StoreDataAsset(const std::string& assetId, std::shared_ptr<ILogicData> data);
		/**
		 * \brief Stores Scene Asset
		 * \param assetID The ID to save the asset to
		 * \param path The path to the scene asset to be saved into the assetstore
		 */
		static void StoreSceneAsset(const std::string& assetId, const std::filesystem::path& path);

		static void StoreChartAsset(const std::string& assetId, const std::filesystem::path& path);

		// Getters

		/**
		 * \brief Getter for Audio Asset
		 * \param assetID The ID key of the asset
		 * \return The Sound asset
		 */
		static FMOD::Sound* GetAudio(const std::string& assetId);
		/**
		 * \brief Getter for Shader Asset
		 * \param assetID The ID key of the asset
		 * \return The Shader asset
		 */
		static Shader* GetShader(const std::string& assetId);
		/**
		 * \brief Getter for Texture Asset
		 * \param assetID The ID key of the asset
		 * \return The Texture asset
		 */
		static unsigned int GetTexture(const std::string& assetId);
		/**
		 * \brief Getter for Mesh
		 * \param type The MeshType of the mesh
		 * \return The Mesh
		 */
		static Mesh& GetMesh(MeshType type);
		/**
		 * \brief Getter for Font Asset
		 * \param assetID The ID key of the asset
		 * \return The Font asset
		 */
		static Font* GetFont(const std::string& assetId);
		/**
		 * \brief Getter for Script Asset
		 * \param assetID The ID key of the asset
		 * \return The Script asset
		 */
		static std::shared_ptr<ILogicScript> GetScript(const std::string& assetId);
		/**
		 * \brief Getter for Data Asset
		 * \param assetID The ID key of the asset
		 * \return The ILogicData asset
		 */
		static std::shared_ptr<ILogicData> GetData(const std::string& assetId);
		/**
		 * \brief Getter for Scene Asset
		 * \param assetID The ID key of the asset
		 * \return The Scene asset
		 */
		static std::filesystem::path GetScene(const std::string& assetId);
		static std::string GetSceneId(std::string const& path);

		static std::filesystem::path GetChart(const std::string& assetId);

		/**
		 * \brief Getter for unordered_map of Shaders
		 * \return The Unordered map of Shader
		 */
		static const std::unordered_map<std::string, std::unique_ptr<Shader>>& GetAllShaders();
		/**
		 * \brief Getter for unordered_map of Textures
		 * \return The Unordered map of Textures
		 */
		static const std::unordered_map<std::string, unsigned int>& GetAllTextures();
		/**
		 * \brief Getter for unordered_map of Fonts
		 * \return The Unordered map of Fonts
		 */
		static const std::unordered_map<std::string, std::unique_ptr<Font>>& GetAllFonts();
		/**
		 * \brief Getter for unordered_map of Audio
		 * \return The Unordered map of Audio
		 */
		static const std::unordered_map<std::string, FMOD::Sound*>& GetAllAudio();
		/**
		 * \brief Getter for unordered_map of Scripts
		 * \return The Unordered map of Scripts
		 */
		static const std::unordered_map<std::string, std::shared_ptr<ILogicScript>>& GetAllScripts();
		/**
		 * \brief Getter for unordered_map of ILogicData
		 * \return The Unordered map of ILogicData
		 */
		static const std::unordered_map<std::string, std::shared_ptr<ILogicData>>& GetAllData();
		/**
		 * \brief Getter for unordered_map of Scenes
		 * \return The Unordered map of Scenes
		 */
		static const std::unordered_map<std::string, std::filesystem::path>& GetAllScenes();

		static const std::unordered_map<std::string, std::filesystem::path>& GetAllCharts();

		//returns size of array of keys via parameter size
		/**
		 * \brief Getter for vector of keys of Shader
		 * \return The vector of keys of Shader
		 */
		static const std::vector<std::string> GetShaderKeys();

		/**
		 * \brief Getter for vector of keys of Texture
		 * \return The vector of keys of Texture
		 */
		static const std::vector<std::string> GetTextureKeys();

		/**
		 * \brief Getter for vector of keys of Font
		 * \return The vector of keys of Font
		 */
		static const std::vector<std::string> GetFontKeys();

		/**
		 * \brief Getter for vector of keys of Audio
		 * \return The vector of keys of Audio
		 */
		static const std::vector<std::string> GetAudioKeys();

		/**
		 * \brief Getter for vector of keys of Script
		 * \return The vector of keys of Script
		 */
		static const std::vector<std::string> GetScriptKeys();

		/**
		 * \brief Getter for vector of keys of ILogicData
		 * \return The vector of keys of ILogicData
		 */
		static const std::vector<std::string> GetDataKeys();

		/**
		 * \brief Getter for vector of keys of Scene
		 * \return The vector of keys of Scene
		 */
		static const std::vector<std::string> GetSceneKeys();

		static const std::vector<std::string> GetChartKeys();

		/**
		 * \brief static function to unload all assets from memory
		 */
		static void Clear();

	};
}
#endif // !