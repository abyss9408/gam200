/******************************************************************************/
/*!
\file   AssetBatchLoader.h
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/04/06
\brief
This file defines the AssetBatchLoader class, which is responsible for loading
assets in batches within the game engine. It manages the scanning, queuing, and
loading of various asset types such as audio, textures, shaders, fonts, scripts,
and scenes. The class provides progress updates to prevent blocking the main
thread during loading, allowing the loading screen to update and render between
batches.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include <filesystem>
#include <functional>
#include <queue>
#include <vector>
#include <string>
#include <unordered_set>
#include <memory>

#include "AssetLoader.h"
#include "../Audio/AudioSystem.h"
#include "../Script/LogicSystem.h"

namespace Popplio {

    namespace fs = std::filesystem;

    /**
     * @struct AssetLoadTask
     * @brief Represents a single asset loading task
     */
    struct AssetLoadTask {
        enum class Type {
            Audio,
            Texture,
            Shader,
            Font,
            Script,
            Scene
        };

        Type type;
        fs::path filepath;
        std::string assetName;

        // Additional parameters for specific asset types
        int channelGroup = 0;
        float bpm = 0.0f;
        std::pair<int, int> timeSig = { 4, 4 };
        std::list<unsigned int> fontSizes;

        AssetLoadTask(Type type, const fs::path& path);
    };

    /**
     * @class AssetBatchLoader
     * @brief Manages asset loading in batches with progress feedback
     */
    class AssetBatchLoader {
    public:
        using ProgressCallback = std::function<void(float)>;

        /**
         * @brief Constructor
         * @param audioSystem Reference to the audio system
         * @param logicSystem Reference to the logic system
         * @param batchSize Number of assets to load per batch
         */
        AssetBatchLoader(AudioSystem& audioSystem, LogicSystem& logicSystem, size_t batchSize = 20);

        /**
         * @brief Scan for assets recursively and prepare them for loading
         * @param directory Directory to scan for assets
         */
        void ScanAssets(const fs::path& directory = fs::current_path());

        /**
         * @brief Begin loading assets in batches
         * @param callback Progress callback function
         * @return True if loading has started, false if already loading
         */
        bool StartLoading(ProgressCallback callback);

        /**
         * @brief Process a single batch of assets
         * @return True if there are more assets to load, false if loading is complete
         */
        bool ProcessBatch();

        /**
         * @brief Check if asset loading is in progress
         * @return True if loading, false otherwise
         */
        bool IsLoading() const;

        /**
         * @brief Get the current loading progress
         * @return Progress as a value between 0.0 and 1.0
         */
        float GetProgress() const;

    private:
        AudioSystem& audioSystem;
        LogicSystem& logicSystem;
        size_t batchSize;
        bool isLoading;
        size_t totalAssets;
        size_t loadedAssets;
        ProgressCallback progressCallback;
        std::queue<AssetLoadTask> loadQueue;

        /**
         * @brief Recursively list all files in a directory
         * @param directory Directory to scan
         * @return Pair of vectors containing asset files and meta files
         */
        std::pair<std::vector<fs::path>, std::unordered_set<fs::path>> ListFilesRecursively(const fs::path& directory);

        /**
         * @brief Queue a single asset for loading
         * @param filepath Path to the asset file
         */
        void QueueAssetLoad(const fs::path& filepath);

        /**
         * @brief Load a single asset based on its task parameters
         * @param task Asset loading task
         * @return True if loaded successfully, false otherwise
         */
        bool LoadAsset(const AssetLoadTask& task);
    };

} // namespace Popplio