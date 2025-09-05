/******************************************************************************/
/*!
\file   AssetBatchLoader.cpp
\author Bryan Ang Wei Ze
\contribution Bryan - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/04/06
\brief
This file contains the implementation of the AssetBatchLoader class, which is
responsible for loading assets in batches within the game engine. It manages
the scanning, queuing, and loading of various asset types such as audio, textures,
shaders, fonts, scripts, and scenes.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>
#include "AssetBatchLoader.h"
#include "../Logging/Logger.h"

namespace Popplio {

    AssetLoadTask::AssetLoadTask(Type type, const fs::path& path)
        : type(type), filepath(path), assetName(Serializer::GenerateAssetName(path.filename().string())) {
    }

    AssetBatchLoader::AssetBatchLoader(AudioSystem& audioSystem, LogicSystem& logicSystem, size_t batchSize)
        : audioSystem(audioSystem), logicSystem(logicSystem), batchSize(batchSize), isLoading(false), totalAssets(0), loadedAssets(0) {
    }

    void AssetBatchLoader::ScanAssets(const fs::path& directory) {
        Logger::Info("Scanning assets in: " + directory.string());

        std::vector<fs::path> assetFiles;
        std::unordered_set<fs::path> metaFiles;

        assetFiles.reserve(500);
        metaFiles.reserve(500);

        // Get all asset files and meta files
        std::tie(assetFiles, metaFiles) = ListFilesRecursively(directory);

        // Clear the queue and reset counters
        while (!loadQueue.empty()) loadQueue.pop();
        totalAssets = 0;
        loadedAssets = 0;

        // Create tasks for each asset
        for (const fs::path& file : assetFiles) {
            fs::path metaPath = file;
            metaPath.replace_extension(AssetLoader::_metaExtension);

            // Create meta file if it doesn't exist
            if (metaFiles.find(metaPath) == metaFiles.end()) {
                Serializer::SerializeMetaFile(file);
            }

            // Create and queue the load task
            QueueAssetLoad(file);
        }

        totalAssets = loadQueue.size();
        Logger::Info("Found " + std::to_string(totalAssets) + " assets to load");
    }

    bool AssetBatchLoader::StartLoading(ProgressCallback callback) {
        if (isLoading) return false;

        this->progressCallback = callback;
        isLoading = true;
        loadedAssets = 0;

        // Report initial progress
        if (progressCallback) {
            progressCallback(GetProgress());
        }

        return true;
    }

    bool AssetBatchLoader::ProcessBatch() {
        if (!isLoading || loadQueue.empty()) {
            if (isLoading) {
                isLoading = false;
                Logger::Info("Asset loading complete");

                // Report final progress
                if (progressCallback) {
                    progressCallback(1.0f);
                }
            }
            return false;
        }

        size_t currentBatchSize = std::min(batchSize, loadQueue.size());
        Logger::Info("Processing batch of " + std::to_string(currentBatchSize) + " assets");

        for (size_t i = 0; i < currentBatchSize; ++i) {
            if (loadQueue.empty()) break;

            AssetLoadTask task = loadQueue.front();
            loadQueue.pop();

            // Load the asset
            LoadAsset(task);

            // Update progress
            loadedAssets++;
        }

        // Report progress
        if (progressCallback) {
            progressCallback(GetProgress());
        }

        return !loadQueue.empty();
    }

    bool AssetBatchLoader::IsLoading() const {
        return isLoading;
    }

    float AssetBatchLoader::GetProgress() const {
        if (totalAssets == 0) return 1.0f;
        return static_cast<float>(loadedAssets) / static_cast<float>(totalAssets);
    }

    std::pair<std::vector<fs::path>, std::unordered_set<fs::path>> AssetBatchLoader::ListFilesRecursively(const fs::path& directory) {
        std::vector<fs::path> assetFiles;
        std::unordered_set<fs::path> metaFiles;

        // Estimate size for preallocation
        size_t fileCount = std::distance(fs::recursive_directory_iterator(directory), {});
        assetFiles.reserve(fileCount / 2);

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

    void AssetBatchLoader::QueueAssetLoad(const fs::path& filepath) {
        std::string extension = filepath.extension().string();

        if (extension == ".wav") {
            auto task = AssetLoadTask(AssetLoadTask::Type::Audio, filepath);
            // Audio parameters will be set later from meta file
            loadQueue.push(task);
        }
        else if (extension == ".png") {
            loadQueue.push(AssetLoadTask(AssetLoadTask::Type::Texture, filepath));
        }
        else if (extension == ".vert" || extension == ".frag") {
            // Only queue shader once per pair (using .vert as the trigger)
            if (extension == ".vert") {
                loadQueue.push(AssetLoadTask(AssetLoadTask::Type::Shader, filepath));
            }
        }
        else if (extension == ".ttf") {
            auto task = AssetLoadTask(AssetLoadTask::Type::Font, filepath);
            // Font sizes will be set later from meta file
            loadQueue.push(task);
        }
        else if (extension == ".cpp" || extension == ".h") {
            // Only queue scripts once (using .cpp as the trigger)
            if (extension == ".cpp") {
                loadQueue.push(AssetLoadTask(AssetLoadTask::Type::Script, filepath));
            }
        }
        else if (extension == ".scene") {
            loadQueue.push(AssetLoadTask(AssetLoadTask::Type::Scene, filepath));
        }
    }

    bool AssetBatchLoader::LoadAsset(const AssetLoadTask& task) {
        try {
            // Load meta data first
            fs::path metaPath = task.filepath;
            metaPath.replace_extension(AssetLoader::_metaExtension);

            switch (task.type) {
            case AssetLoadTask::Type::Audio:
                return AssetLoader::LoadAudio(
                    task.filepath.string().c_str(),
                    task.assetName,
                    task.channelGroup,
                    task.bpm,
                    task.timeSig);

            case AssetLoadTask::Type::Texture:
                return AssetLoader::LoadTexture(task.filepath.string());

            case AssetLoadTask::Type::Shader:
                return AssetLoader::LoadShader(task.filepath.string());

            case AssetLoadTask::Type::Font:
                return AssetLoader::LoadFont(task.filepath.string(), task.fontSizes);

            case AssetLoadTask::Type::Script:
                return AssetLoader::LoadCPPScripts(task.assetName);

            case AssetLoadTask::Type::Scene:
                return AssetLoader::LoadScene(task.filepath.string());

            default:
                Logger::Warning("Unknown asset type: " + task.filepath.string());
                return false;
            }
        }
        catch (const std::exception& e) {
            Logger::Error("Failed to load asset: " + task.filepath.string() + " - " + e.what());
            return false;
        }
    }

} // namespace Popplio