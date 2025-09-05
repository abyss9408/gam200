/******************************************************************************/
/*!
\file			Serialization.cpp
\author         Team Popplio
\author         Bryan Ang Wei Ze
\co-author      Hafawati Binte Mohd Ali | Ng Jun Heng Shawn | Shannon
\contribution   Bryan - 65%, Hafawati - 20%, Shawn - 10%, Shannon - 5%
\par            Course : CSD2401 / CSD2400
\par            Section : A
\date           2025/03/13
\brief
	This is the source file for the serializer of the engine

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>

#include "Serialization.h"

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

#include "../PrefabManagement/PrefabSyncSystem.h"
#include "../Hierarchy/HierarchySystem.h"
#include "../Engine/Engine.h"
#include "../Serialization/ChartEditorSerialization.h"
#include "../PerformanceViewer/PerformanceViewer.h"

#include "../Cloning/CloneSystem.h"

#include <filesystem>

namespace Popplio
{
	Serializer::Serializer(Registry& reg, LayerManager& layerMgr, PrefabManager& prefabMgr, EventBus& eventBus)
		: registry(reg), configuration(), layerManager(layerMgr), prefabManager(prefabMgr), eventBus(eventBus)
	{

	}

	bool Serializer::LoadSceneFromFile(const std::string& filename, CameraManager& cameraMgr, const bool& clearPersistent)
	{
		PerformanceViewer::GetInstance()->start("Serializer");
		std::filesystem::current_path(std::filesystem::path(Popplio::Engine::programPath));

		Logger::Info("Loading scene data from file: " + filename);

		std::ifstream ifs(filename);
		if (!ifs)
		{
			Logger::Error("Error opening file: " + filename + " for reading");
			std::filesystem::current_path(std::filesystem::path(Popplio::Engine::programPath));
			return false;
		}

		//registry.GetSystem<LogicSystem>().monoAPI.UnloadAssembly();

		std::set<Popplio::Entity> persistent{};

		// Clear existing scene before loading new one
		persistent = ClearCurrentScene(clearPersistent);

		rapidjson::IStreamWrapper isw(ifs);
		document.ParseStream(isw);

		if (document.HasParseError())
		{
			Logger::Error("JSON parse error: " + std::to_string(document.GetParseError()));
			std::filesystem::current_path(std::filesystem::path(Popplio::Engine::programPath));
			return false;
		}

		// Persistent doesnt work currently

		registry.ClearFreeIds();
		//if (clearPersistent) registry.ResetEntityComponentSignatures();
		registry.ResetEntityComponentSignatures();

		//     std::vector<Popplio::Signature> signatures = 
				 //registry.StashPersistentEntities(persistent); // Stash persistent entities before deserializing scene entities

			 //else
			 //{
			 //	int entIndex = document["entities"].GetArray().Size();

			 //	for (Entity e : persistent)
			 //	{
			 //		CloneEntityEvent cloneEvent(e.GetId(), entIndex++);
			 //		registry.GetSystem<CloneSystem>().CloneEntity(cloneEvent);
			 //		e.Kill(); // kill queue
			 //	}

			 //	registry.Update();
			 //}

			 ////registry.KillAllEntities();
			 ////registry.ResetEntityComponentSignatures();
			 //registry.ClearFreeIds();

		registry.Update();

		// reset mono if we are loading a new scene at runtime
		if (!clearPersistent)
		{
			registry.GetSystem<LogicSystem>().monoAPI.CallMethod(
				registry.GetSystem<LogicSystem>().monoAPI.GetAssClass(
					"Team_Popplio.ECS", "Entity"), "ResetEntities");
			registry.GetSystem<LogicSystem>().monoAPI.CallMethod(
				registry.GetSystem<LogicSystem>().monoAPI.GetAssClass(
					"Team_Popplio", "PopplioMonoScript"), "ResetScripts");

			//registry.GetSystem<LogicSystem>().monoAPI.ClearAllGC();
		}

		DeserializeLayers();
		DeserializeEntities();
		DeserializeCameraSettings(cameraMgr);

		//registry.RestorePersistentEntities(signatures); // Restore persistent entities after deserializing scene

		// Emit event when scene loading is complete
		eventBus.EmitEvent<SceneLoadedEvent>(filename);

		ifs.close();

		//registry.GetSystem<LogicSystem>().monoAPI.StartMono();

		Logger::Info("Successfully loaded level data from: " + filename);
		std::filesystem::current_path(std::filesystem::path(Popplio::Engine::programPath));
		PerformanceViewer::GetInstance()->end("Serializer");

		Logger::Info("Scene Load | Time taken: " +
			std::to_string(PerformanceViewer::GetInstance()->GetSystemTime("Serializer").count() / 1000000.0) +
			" seconds");

		return true;
	}

	bool Serializer::LoadConfiguration(const std::string& filename)
	{
		std::ifstream ifs(filename);
		if (!ifs)
		{
			Logger::Error("Error opening file: " + filename + " for reading");
			return false;
		}

		rapidjson::IStreamWrapper isw(ifs);
		document.ParseStream(isw);

		if (document.HasParseError())
		{
			Logger::Error("JSON parse error: " + std::to_string(document.GetParseError()));
			return false;
		}

		if (!document.IsObject())
		{
			Logger::Error("Invalid JSON structure");
			return false;
		}

        if (document.HasMember("engine version") && document["engine version"].IsString())
        {
            configuration.engineVer = document["engine version"].GetString();
        }

        if (document.HasMember("app version") && document["app version"].IsString())
        {
            configuration.appVer = document["app version"].GetString();
        }

		if (document.HasMember("window width") && document["window width"].IsInt())
		{
			configuration.width = document["window width"].GetInt();
		}

		if (document.HasMember("window height") && document["window height"].IsInt())
		{
			configuration.height = document["window height"].GetInt();
		}

		if (document.HasMember("v-sync") && document["v-sync"].IsBool())
		{
			configuration.vSync = document["v-sync"].GetBool();
		}

		if (document.HasMember("safe mode") && document["safe mode"].IsBool())
		{
			configuration.safeMode = document["safe mode"].GetBool();
		}

		if (document.HasMember("last scene") && document["last scene"].IsString())
		{
			configuration.lastScene = document["last scene"].GetString();
		}

		if (document.HasMember("start scene") && document["start scene"].IsString())
		{
			configuration.startScene = document["start scene"].GetString();
		}

        if (document.HasMember("verbose") && document["verbose"].IsBool())
        {
            configuration.verbose = document["verbose"].GetBool();
        }

		std::ostringstream config;
		config << "Loaded configuration ~ "
            << "Engine Version: " << configuration.engineVer
            << ", App Version: " << configuration.appVer
			<< ", Window: " << configuration.width << "x" << configuration.height
			<< ", V-Sync: " << (configuration.vSync ? "On" : "Off")
			<< ", Safe Mode: " << (configuration.safeMode ? "On" : "Off")
			<< ", Last Scene: " << (configuration.lastScene)
			<< ", Start Scene: " << (configuration.startScene)
			<< ", Verbose: " << (configuration.verbose ? "On" : "Off");
		Logger::Info(config.str());

		ifs.close(); // added to fix error that cant save / load after

		return true;
	}

#pragma region REWORKING


	////TODO: this function should not be used anymore, this should be handled by the AssetLoader class
	//bool Serializer::LoadAsset(const std::string& filename, AudioSystem& audioSystem, LogicSystem& logicSystem)
	//{
	//	std::ifstream ifs(filename);
	//	if (!ifs)
	//	{
	//		Logger::Error("Error opening file: " + filename + " for reading");
	//		return false;
	//	}

	//	rapidjson::IStreamWrapper isw(ifs);
	//	document.ParseStream(isw);

	//	if (document.HasParseError())
	//	{
	//		Logger::Error("JSON parse error: " + std::to_string(document.GetParseError()));
	//		ifs.close(); // added to fix error that cant save / load after
	//		return false;
	//	}

	//	if (!document.IsObject())
	//	{
	//		Logger::Error("Invalid JSON structure");
	//		ifs.close(); // added to fix error that cant save / load after
	//		return false;
	//	}
	//	// Load different asset types
	//	//if (document.HasMember("Texture") && document["Texture"].IsArray()) {
	//	//	if (!LoadTextures(document["Texture"])) return false;
	//	//}

	//	//if (document.HasMember("Shader") && document["Shader"].IsArray()) {
	//	//	if (!LoadShaders(document["Shader"])) return false;
	//	//}

	//	/*if (document.HasMember("Font") && document["Font"].IsArray()) {
	//		if (!LoadFonts(document["Font"])) return false;
	//	}*/

	//	if (document.HasMember("Audio") && document["Audio"].IsArray()) {
	//		if (!LoadAudio(document["Audio"], audioSystem)) return false;
	//	}

		//if (document.HasMember("Logic") && document["Logic"].IsArray()) {
		//	if (!LoadCPPScripts(document["Logic"], logicSystem)) return false;
		//}

		//if (document.HasMember("Prefab") && document["Prefab"].IsArray()) {
		//	if (!LoadPrefabs(document["Prefab"])) return false;
		//}

	//	ifs.close(); // added to fix error that cant save / load after

	//	return true;
	//}

	//bool Serializer::LoadTextures(const rapidjson::Value& texturesArray) {
	//	for (const auto& texturePath : texturesArray.GetArray()) {
	//		if (!texturePath.IsString()) continue;

	//		std::string path = texturePath.GetString();
	//		std::string assetName = GenerateAssetName(path);

	//		try {
	//			AssetStore::StoreTextureAsset(
	//				assetName,
	//				Texture(path).GetTextureID()
	//			);
	//			Logger::Info("Loaded texture: " + path);
	//		}
	//		catch (const std::exception&) {
	//			Logger::Error("Failed to load texture: " + path);
	//			return false;
	//		}
	//	}
	//	return true;
	//}

	//bool Serializer::LoadShaders(const rapidjson::Value& shadersArray) {
	//	for (const auto& shaderPath : shadersArray.GetArray()) {
	//		if (!shaderPath.IsString()) continue;

	//		std::string path = shaderPath.GetString();
	//		std::string assetName = GenerateAssetName(path);

	//		try {
	//			AssetStore::StoreShaderAsset(
	//				assetName,
	//				std::make_unique<Shader>(path)
	//			);
	//			Logger::Info("Loaded shader: " + path);
	//		}
	//		catch (const std::exception&) {
	//			Logger::Error("Failed to load shader: " + path);
	//			return false;
	//		}
	//	}
	//	return true;
	//}

	//bool Serializer::LoadFonts(const rapidjson::Value& fontsArray) {
	//	for (const auto& fontPath : fontsArray.GetArray()) {
	//		if (!fontPath.IsObject()) continue;

	//		// Retrieve the font path
	//		if (!fontPath.HasMember("path") || !fontPath["path"].IsString()) {
	//			Logger::Error("Font resource missing 'path' or 'path' is not a string.");
	//			continue;
	//		}
	//		std::string path = fontPath["path"].GetString();


	//}


	//bool Serializer::LoadAudio(const rapidjson::Value& audioArray, AudioSystem& audioSystem)
	//{
	//	for (const auto& audio : audioArray.GetArray()) {
	//		if (!audio.IsObject()) continue;

	//		std::string path = audio["path"].GetString();
	//		std::string key = audio["key"].GetString();
	//		int channelGroup = audio["channelGroup"].GetInt();

	//		try {
	//			audioSystem.SetAudio(path.c_str(), key, audioSystem.GetCGroup(channelGroup), beatsPM, timeSignature);
	//			Logger::Info("Loaded audio: " + path);
	//		}
	//		catch (const std::exception&) {
	//			Logger::Error("Failed to load audio: " + path);
	//			return false;
	//		}
	//	}
	//	return true;
	////}

	bool Serializer::LoadPrefabs(const rapidjson::Value& prefabsArray)
	{
		for (const auto& prefabPath : prefabsArray.GetArray())
		{
			if (!prefabPath.IsString()) continue;

			std::string path = prefabPath.GetString();
			auto loadedPrefab = LoadPrefabFromFile(path);

			if (loadedPrefab)
			{
				// Register with absolute file path
				prefabManager.RegisterPrefab(
					loadedPrefab->GetName(),
					std::make_unique<Prefab>(*loadedPrefab),
					std::filesystem::absolute(std::filesystem::path(path)).string()
				);
				Logger::Info("Prefab loaded successfully from: " + path, LogDestination::IMGUI_CONSOLE);
			}
			else
			{
				Logger::Error("Failed to load prefab from: " + path, LogDestination::IMGUI_CONSOLE);
				return false;
			}
		}

		return true;
	}

	//bool Serializer::LoadCPPScripts(const rapidjson::Value& logicArray, LogicSystem& logicSystem) // C++ scripts
	//{
	//	for (const auto& script : logicArray.GetArray()) {
	//		if (!script.IsObject()) continue;

	//		std::string scriptClass = script["class"].GetString();

	//		try
	//		{
	//			logicSystem.LoadByName(scriptClass);
	//			Logger::Info("Loaded script: " + scriptClass);
	//		}
	//		catch (const std::exception&) {
	//			Logger::Error("Failed to load script: " + scriptClass);
	//			return false;
	//		}
	//	}
	//	return true;
	//}
#pragma endregion

	std::string Serializer::GenerateAssetName(const std::string& filepath) {
		// Extract filename without extension
		size_t lastSlash = filepath.find_last_of("/\\");
		size_t lastDot = filepath.find_last_of(".");

		std::string filename = filepath.substr(
			lastSlash + 1,
			lastDot - lastSlash - 1
		);
		return filename;
	}

	bool Serializer::SerializeMetaFile(const std::filesystem::path& filename)
	{
		std::string fileExtension = filename.filename().string().substr(filename.filename().string().find('.'));
		std::string fileNoExtension = filename.string().substr(0, filename.string().find('.'));
		std::string metaFileName = filename.string().substr(0, filename.string().find('.')) + AssetLoader::_metaExtension;
		bool isSerializable = false;


		rapidjson::Document document;
		document.SetObject();
		auto& allocator = document.GetAllocator();

		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

		rapidjson::Value metaVal{ rapidjson::kObjectType };
		std::string stringRefName{};
		if (fileExtension == ".wav")
		{
			stringRefName = "Audio";
			rapidjson::Value metaMetaVal{ rapidjson::kNumberType };
			if (filename.filename().string().find("sfx") != std::string::npos)
			{
				metaMetaVal.SetInt(2);
				metaVal.AddMember(rapidjson::StringRef("channelGroup"), metaMetaVal, allocator);
			}
			else
			{
				metaMetaVal.SetInt(1);
				metaVal.AddMember(rapidjson::StringRef("channelGroup"), metaMetaVal, allocator);
				metaMetaVal.SetFloat(75.0);
				metaVal.AddMember(rapidjson::StringRef("beatsPM"), metaMetaVal, allocator);

				rapidjson::Value arrayVal{ rapidjson::kArrayType };
				arrayVal.SetArray();
				arrayVal.PushBack(4, allocator);
				arrayVal.PushBack(4, allocator);
				metaVal.AddMember(rapidjson::StringRef("timeSignature"), arrayVal, allocator);
			}
			isSerializable = true;
		}
		else if (fileExtension == ".ttf")
		{
			stringRefName = "Font";
			rapidjson::Value metaMetaVal{ rapidjson::kArrayType };
			metaMetaVal.SetArray();
			//[8, 12, 16, 24, 32, 48, 72, 96]
			metaMetaVal.PushBack(8, allocator);
			metaMetaVal.PushBack(12, allocator);
			metaMetaVal.PushBack(16, allocator);
			metaMetaVal.PushBack(24, allocator);
			metaMetaVal.PushBack(32, allocator);
			metaMetaVal.PushBack(48, allocator);
			metaMetaVal.PushBack(72, allocator);
			metaMetaVal.PushBack(96, allocator);
			metaVal.AddMember(rapidjson::StringRef("sizes"), metaMetaVal, allocator);
			isSerializable = true;

		}
		else if (fileExtension == ".png")
		{
			stringRefName = "Image";
			isSerializable = true;
		}
		else if (fileExtension == ".vert" || fileExtension == ".frag")
		{
			stringRefName = "Shader";
			//check if either one of the shaders dont exist
			if (!std::filesystem::exists(std::filesystem::path(fileNoExtension + ".frag")) || !std::filesystem::exists(std::filesystem::path(fileNoExtension + ".vert")))
			{
				Logger::Error("Shader file missing one of the pair! File we have: " + filename.string());
				return false;
			}
			isSerializable = true;

		}
		else if (fileExtension == ".cpp" || fileExtension == ".h")
		{
			stringRefName = "Script";

			rapidjson::Value metametaVal{ rapidjson::kStringType };

			std::ifstream ifs;
			ifs.open(filename);
			if (!ifs.is_open())
			{
				ifs.close();
				Logger::Error("Unable to read script: " + filename.string());
			}

			std::string extractedClass{};
			while (getline(ifs, extractedClass))
			{
				if (extractedClass.find("class") != std::string::npos)
				{
					break;
				}
			}
			ifs.close();

			// want the word after "class"
			std::stringstream ss{ extractedClass };
			std::list<std::string> classless;
			std::string token;
			while (ss >> token)
			{
				classless.push_back(token);
			}

			std::string className;
			for (std::list<std::string>::iterator it = classless.begin(); it != classless.end(); ++it)
			{
				if (*it != "class")continue;

				//next one is the 
				className = *(++it);
			}
			className = "BassNKick::" + className;
			metametaVal.SetString(className.c_str(), allocator);
			metaVal.AddMember(rapidjson::StringRef("class"), metametaVal, allocator);


			isSerializable = true;
		}
		else if (fileExtension == ".scene")
		{
			stringRefName = "Scene";
			isSerializable = true;
		}

		if (isSerializable)
		{
			std::ofstream outFile(metaFileName);
			if (!outFile)
			{
				Logger::Error("Failed to open file for writing: " + metaFileName);
				return false;
			}
			document.AddMember(rapidjson::StringRef(stringRefName.c_str()), metaVal, allocator);

			document.Accept(writer);
			outFile << buffer.GetString();
			outFile.close();
		}

		return true;
	}

	bool Serializer::DeserializeMetaFile(const std::filesystem::path& filename)
	{
		if (!AssetLoader::IsAsset(filename.filename().string()))
			return true;

		// Use replace_extension to construct the meta file path
		std::filesystem::path metaFileName = filename;
		metaFileName.replace_extension(AssetLoader::_metaExtension);

		// Open file using RAII
		std::ifstream inFile(metaFileName);
		if (!inFile) {
			Logger::Error("Failed to open file for reading: " + metaFileName.string());
			return false;
		}

		// Parse JSON
		rapidjson::Document metaDoc;
		{
			rapidjson::IStreamWrapper isw(inFile);
			metaDoc.ParseStream(isw);
		}

		if (metaDoc.HasParseError()) {
			Logger::Error("Failed to parse prefab JSON file: " + metaFileName.string(), LogDestination::WINDOWS_CONSOLE);
			return false;
		}

		// Determine file extension
		std::string fileExtension = filename.extension().string();

		try {
			// Use structured bindings to avoid repeated lookups
			if (fileExtension == ".wav") {
				if (!metaDoc.HasMember("Audio")) {
					Logger::Error("Invalid audio meta file: " + metaFileName.string());
					return false;
				}

				const auto& audioMeta = metaDoc["Audio"];
				int channelGroup = audioMeta.HasMember("channelGroup") ? audioMeta["channelGroup"].GetInt() : 0;
				float beatsPM = audioMeta.HasMember("beatsPM") ? audioMeta["beatsPM"].GetFloat() : 0.0f;

				std::pair<int, int> timeSignature = { 4, 4 }; // Default
				if (audioMeta.HasMember("timeSignature") && audioMeta["timeSignature"].IsArray()) {
					const auto& tsArray = audioMeta["timeSignature"].GetArray();
					timeSignature = { tsArray[0].GetInt(), tsArray[1].GetInt() };
				}

				return AssetLoader::LoadAudio(
					filename.string().c_str(),
					Serializer::GenerateAssetName(filename.filename().string()),
					channelGroup, beatsPM, timeSignature
				);
			}

			if (fileExtension == ".ttf") {
				if (!metaDoc.HasMember("Font") || !metaDoc["Font"].HasMember("sizes") || !metaDoc["Font"]["sizes"].IsArray()) {
					Logger::Error("Invalid font meta file: " + metaFileName.string());
					return false;
				}

				std::list<unsigned int> fontSizes;
				for (const auto& size : metaDoc["Font"]["sizes"].GetArray()) {
					fontSizes.push_back(size.GetUint());
				}

				return AssetLoader::LoadFont(filename.string(), fontSizes);
			}

			if (fileExtension == ".png") {
				if (!metaDoc.HasMember("Image")) {
					Logger::Error("Invalid image meta file: " + metaFileName.string());
					return false;
				}
				return AssetLoader::LoadTexture(filename.string());
			}

			if (fileExtension == ".vert" || fileExtension == ".frag") {
				if (!metaDoc.HasMember("Shader")) {
					Logger::Error("Invalid shader meta file: " + metaFileName.string());
					return false;
				}
				return AssetLoader::LoadShader(filename.string());
			}

			if (fileExtension == ".cpp" || fileExtension == ".h") {
				if (!metaDoc.HasMember("Script") || !metaDoc["Script"].HasMember("class")) {
					Logger::Error("Invalid script meta file: " + metaFileName.string());
					return false;
				}

				return AssetLoader::LoadCPPScripts(metaDoc["Script"]["class"].GetString());
			}

			if (fileExtension == ".scene") {
				if (!metaDoc.HasMember("Scene")) {
					Logger::Error("Invalid scene meta file: " + metaFileName.string());
					return false;
				}
				return AssetLoader::LoadScene(filename.string());
			}

			Logger::Warning("Unsupported asset type: " + fileExtension);
			return false;
		}
		catch (const std::exception& e) {
			Logger::Error("Exception while processing meta file: " + metaFileName.string() + " - " + e.what());
			return false;
		}
	}

	std::unique_ptr<Prefab> Serializer::LoadPrefabFromFile(const std::string& filename)
	{
		// Store full path for child prefab resolution
		std::unordered_set<std::string> loadedPrefabs;
		return LoadPrefabFromFileInternal(filename, loadedPrefabs);
	}

	std::unique_ptr<Prefab> Serializer::LoadPrefabFromFileInternal(const std::string& filename,
		std::unordered_set<std::string>& loadedPrefabs)
	{
		// Prevent circular dependencies by checking absolute paths
		std::filesystem::path absolutePath = std::filesystem::absolute(filename);
		if (loadedPrefabs.find(absolutePath.string()) != loadedPrefabs.end()) {
			Logger::Error("Circular prefab dependency detected: " + filename);
			return nullptr;
		}
		loadedPrefabs.insert(absolutePath.string());

		std::ifstream ifs(filename);
		if (!ifs) {
			Logger::Error("Failed to open prefab file: " + filename);
			return nullptr;
		}

		rapidjson::IStreamWrapper isw(ifs);
		rapidjson::Document prefabDoc;
		prefabDoc.ParseStream(isw);
		ifs.close();

		if (prefabDoc.HasParseError() || !prefabDoc.HasMember("prefab")) {
			Logger::Error("Invalid prefab file: " + filename);
			return nullptr;
		}

		const auto& prefabData = prefabDoc["prefab"];
		if (!prefabData.HasMember("name") || !prefabData.HasMember("components")) {
			Logger::Error("Invalid prefab structure in: " + filename);
			return nullptr;
		}

		// Create base prefab
		std::string prefabName = prefabData["name"].GetString();
		auto prefab = std::make_unique<Prefab>(prefabName);

		// Deserialize components
		DeserializePrefabComponents(*prefab, prefabData["components"]);

		// Handle child prefabs if present
		if (prefabData.HasMember("childPrefabs") && prefabData["childPrefabs"].IsArray()) {
			std::filesystem::path parentDir = std::filesystem::path(filename).parent_path();

			for (const auto& childRef : prefabData["childPrefabs"].GetArray()) {
				if (!childRef.HasMember("path")) {
					Logger::Warning("Child prefab missing path in: " + filename);
					continue;
				}

				// Construct absolute path for child prefab
				std::filesystem::path childPath = parentDir / childRef["path"].GetString();

				// Try to get existing prefab first
				std::string childPrefabName = std::filesystem::path(childRef["path"].GetString()).stem().string();
				auto existingPrefab = prefabManager.GetPrefab(childPrefabName);

				if (!existingPrefab) {
					// Load child prefab if not already loaded
					auto childPrefab = LoadPrefabFromFileInternal(childPath.string(), loadedPrefabs);
					if (!childPrefab) {
						Logger::Error("Failed to load child prefab: " + childPath.string());
						continue;
					}

					// Register child prefab
					prefabManager.RegisterPrefab(
						childPrefab->GetName(),
						std::make_unique<Prefab>(*childPrefab),
						childPath.string()
					);
				}

				// Extract transform data with defaults
				PopplioMath::Vec2f position{ 0, 0 }, scale{ 1, 1 };
				double rotation = 0;

				if (childRef.HasMember("position")) {
					position.x = childRef["position"]["x"].GetFloat();
					position.y = childRef["position"]["y"].GetFloat();
				}
				if (childRef.HasMember("scale")) {
					scale.x = childRef["scale"]["x"].GetFloat();
					scale.y = childRef["scale"]["y"].GetFloat();
				}
				if (childRef.HasMember("rotation")) {
					rotation = childRef["rotation"].GetDouble();
				}

				// Add child reference to parent using relative path
				prefab->AddChildPrefab(PrefabRef(
					std::filesystem::path(childRef["path"].GetString()).string(),
					position,
					scale,
					rotation
				));
			}
		}

		return prefab;
	}

	void Serializer::DeserializePrefabComponents(Prefab& prefab, const rapidjson::Value& components)
	{
		DeserializeComponent<ActiveComponent>(prefab, components["active"]);
		DeserializeComponent<TransformComponent>(prefab, components["transform"]);

		if (components.HasMember("render"))
			DeserializeComponent<RenderComponent>(prefab, components["render"]);
		if (components.HasMember("rigidbody"))
			DeserializeComponent<RigidBodyComponent>(prefab, components["rigidbody"]);
		if (components.HasMember("boxcollider"))
			DeserializeComponent<BoxColliderComponent>(prefab, components["boxcollider"]);
		if (components.HasMember("text"))
			DeserializeComponent<TextComponent>(prefab, components["text"]);
		if (components.HasMember("animation"))
			DeserializeComponent<AnimationComponent>(prefab, components["animation"]);
		if (components.HasMember("logic"))
			DeserializeComponent<LogicComponent>(prefab, components["logic"]);
		if (components.HasMember("audio"))
			DeserializeComponent<AudioComponent>(prefab, components["audio"]);
		if (components.HasMember("parent"))
			DeserializeComponent<ParentComponent>(prefab, components["parent"]);
		if (components.HasMember("persist"))
			DeserializeComponent<PersistOnLoadComponent>(prefab, components["persist"]);
        if (components.HasMember("ui"))
            DeserializeComponent<UIComponent>(prefab, components["ui"]);
        if (components.HasMember("particles"))
            DeserializeComponent<ParticleComponent>(prefab, components["particles"]);
	}

	void Serializer::DeserializeEntities()
	{
		if (!document.IsObject() || !document.HasMember("entities") || !document["entities"].IsArray())
		{
			Logger::Error("Invalid JSON structure : expected 'entities' array");
			return;
		}

		Logger::Info("Starting entity deserialization");

		// First pass: Deserialize all entities
		const auto& entities = document["entities"];
		for (const auto& entityVal : entities.GetArray())
		{
			DeserializeEntity(entityVal);
		}

		// Update local transforms for entities with parents
		for (const auto obj : registry.GetAllEntities())
		{
			if (!obj.HasComponent<ParentComponent>()) continue;
			if (!obj.GetComponent<ParentComponent>().parent.Exists()) continue;

			obj.GetComponent<ParentComponent>().SetLocalToWorld(obj.GetComponent<TransformComponent>());
		}

		// Third pass: Set up child relationships if they exist
		if (document.HasMember("child_relationships") && document["child_relationships"].IsArray())
		{
			const auto& relationships = document["child_relationships"].GetArray();
			for (const auto& rel : relationships)
			{
				if (rel.HasMember("parentId") && rel.HasMember("childrenIds"))
				{
					int parentId = rel["parentId"].GetInt();
					if (registry.EntityExists(parentId))
					{
						Entity parent = registry.GetEntityById(parentId);
						if (parent.HasComponent<ParentComponent>())
						{
							auto& parentComp = parent.GetComponent<ParentComponent>();

							const auto& childrenIds = rel["childrenIds"].GetArray();
							for (const auto& childId : childrenIds)
							{
								if (childId.IsInt() && registry.EntityExists(childId.GetInt()))
								{
									Entity child = registry.GetEntityById(childId.GetInt());

									parentComp.AddChild(child, parent);

									// Set up child's parent component if needed
									if (!child.HasComponent<ParentComponent>())
									{
										child.AddComponent<ParentComponent>(parent);
									}
									else
									{
										child.GetComponent<ParentComponent>().parent = parent;
									}

									child.GetComponent<ParentComponent>().UpdateLocalTransform(
										child.GetComponent<TransformComponent>(), parent.GetComponent<TransformComponent>());
								}
							}

							// Initialize the transform tracking for this parent
							registry.GetSystem<HierarchySystem>().InitializeParentTransform(parent);
						}
					}
				}
			}
		}

		Logger::Info("Entity deserialization completed");
	}

	void Serializer::DeserializeEntity(const rapidjson::Value& entityVal)
	{
		if (!entityVal.IsObject())
		{
			Logger::Error("Invalid entity structure: expected object");
			return;
		}

		Entity entity = Entity(-1);

		if (entityVal.HasMember("id"))
		{
			entity = registry.CreateEntity(-1, entityVal["id"].GetInt());
		}
		else
		{
			Logger::Error("Entity missing 'id' field");
			return;
			//entity = registry.CreateEntity();
		}

		if (entity.GetId() < 0) return;

		if (entityVal.HasMember("tag"))
		{
			DeserializeTag(entity, entityVal["tag"]);
		}

		if (entityVal.HasMember("group"))
		{
			DeserializeGroup(entity, entityVal["group"]);
		}

		if (entityVal.HasMember("layer"))
		{
			layerManager.AddEntityToLayer(entity, entityVal["layer"].GetString());
		}

		if (entityVal.HasMember("active"))
		{
			DeserializeComponent<ActiveComponent>(entity, entityVal["active"]);
		}

		if (entityVal.HasMember("transform"))
		{
			DeserializeComponent<TransformComponent>(entity, entityVal["transform"]);
		}

		if (entityVal.HasMember("rigidbody"))
		{
			DeserializeComponent<RigidBodyComponent>(entity, entityVal["rigidbody"]);
		}

		if (entityVal.HasMember("boxcollider"))
		{
			DeserializeComponent<BoxColliderComponent>(entity, entityVal["boxcollider"]);
		}

		if (entityVal.HasMember("render"))
		{
			DeserializeComponent<RenderComponent>(entity, entityVal["render"]);
		}

		if (entityVal.HasMember("text"))
		{
			DeserializeComponent<TextComponent>(entity, entityVal["text"]);
		}

		if (entityVal.HasMember("animation"))
		{
			DeserializeComponent<AnimationComponent>(entity, entityVal["animation"]);
		}

		if (entityVal.HasMember("ui"))
		{
			DeserializeComponent<UIComponent>(entity, entityVal["ui"]);
		}


		if (entityVal.HasMember("logic"))
		{
			DeserializeComponent<LogicComponent>(entity, entityVal["logic"]);
		}

		if (entityVal.HasMember("audio"))
		{
			DeserializeComponent<AudioComponent>(entity, entityVal["audio"]);
		}

		if (entityVal.HasMember("prefab_instance"))
		{
			Logger::Info("- This entity is a prefab instance");
			DeserializeComponent<PrefabInstanceComponent>(entity, entityVal["prefab_instance"]);
		}

		if (entityVal.HasMember("parent"))
		{
			DeserializeComponent<ParentComponent>(entity, entityVal["parent"]);
		}

        if (entityVal.HasMember("persist"))
        {
            DeserializeComponent<PersistOnLoadComponent>(entity, entityVal["persist"]);
        }

        if (entityVal.HasMember("particles"))
        {
            DeserializeComponent<ParticleComponent>(entity, entityVal["particles"]);
        }

		Logger::Debug("Deserialized Entity: " + std::to_string(entity.GetId()));
	}

	std::unique_ptr<Prefab> Serializer::DeserializePrefab(const rapidjson::Value& prefabValue)
	{
		if (!prefabValue.HasMember("name") || !prefabValue.HasMember("components"))
		{
			Logger::Error("Invalid prefab format: missing required fields");
			return nullptr;
		}

		std::string prefabName = prefabValue["name"].GetString();
		auto prefab = std::make_unique<Prefab>(prefabName);
		const auto& components = prefabValue["components"];

		Logger::Info("Deserializing prefab components:");

		// Use the generic template for each component type

		if (components.HasMember("active"))
		{
			Logger::Info("- Found active component");
			DeserializeComponent<ActiveComponent>(*prefab, components["active"]);
		}

		if (components.HasMember("transform"))
		{
			Logger::Info("- Found transform component");
			DeserializeComponent<TransformComponent>(*prefab, components["transform"]);
		}

		if (components.HasMember("rigidbody"))
		{
			Logger::Info("- Found rigidbody component");
			DeserializeComponent<RigidBodyComponent>(*prefab, components["rigidbody"]);
		}

		if (components.HasMember("boxcollider"))
		{
			Logger::Info("- Found boxcollider component");
			DeserializeComponent<BoxColliderComponent>(*prefab, components["boxcollider"]);
		}

		if (components.HasMember("render"))
		{
			Logger::Info("- Found render component");
			DeserializeComponent<RenderComponent>(*prefab, components["render"]);
		}

		if (components.HasMember("text"))
		{
			Logger::Info("- Found text component");
			DeserializeComponent<TextComponent>(*prefab, components["text"]);
		}

		if (components.HasMember("logic"))
		{
			Logger::Info("- Found logic component");
			DeserializeComponent<LogicComponent>(*prefab, components["logic"]);
		}

		if (components.HasMember("audio"))
		{
			Logger::Info("- Found audio component");
			DeserializeComponent<AudioComponent>(*prefab, components["audio"]);
		}

		if (components.HasMember("animation"))
		{
			Logger::Info("- Found animation component");
			DeserializeComponent<AnimationComponent>(*prefab, components["animation"]);
		}

		if (components.HasMember("parent"))
		{
			Logger::Info("- Found parent component");
			DeserializeComponent<ParentComponent>(*prefab, components["parent"]);
		}

		if (components.HasMember("persist"))
		{
			Logger::Info("- Found persist component");
			DeserializeComponent<PersistOnLoadComponent>(*prefab, components["persist"]);
		}

        if (components.HasMember("ui"))
        {
            Logger::Info("- Found ui component");
            DeserializeComponent<UIComponent>(*prefab, components["ui"]);
        }

        if (components.HasMember("particles"))
        {
            Logger::Info("- Found particles component");
            DeserializeComponent<ParticleComponent>(*prefab, components["particles"]);
        }

		if (prefabValue.HasMember("childPrefabs") && prefabValue["childPrefabs"].IsArray())
		{
			Logger::Info("- Found child prefabs");
			const auto& childPrefabs = prefabValue["childPrefabs"].GetArray();

			for (const auto& childRef : childPrefabs)
			{
				if (!childRef.IsObject() || !childRef.HasMember("path"))
				{
					Logger::Warning("Invalid child prefab reference structure");
					continue;
				}

				std::string path = childRef["path"].GetString();
				PopplioMath::Vec2f position = { 0, 0 };
				PopplioMath::Vec2f scale = { 1, 1 };
				double rotation = 0;

				if (childRef.HasMember("position"))
				{
					position.x = childRef["position"]["x"].GetFloat();
					position.y = childRef["position"]["y"].GetFloat();
				}

				if (childRef.HasMember("scale"))
				{
					scale.x = childRef["scale"]["x"].GetFloat();
					scale.y = childRef["scale"]["y"].GetFloat();
				}

				if (childRef.HasMember("rotation"))
				{
					rotation = childRef["rotation"].GetDouble();
				}

				prefab->AddChildPrefab(PrefabRef(path, position, scale, rotation));
				Logger::Info("  - Added child prefab reference: " + path);
			}
		}

		return prefab;
	}

	void Serializer::DeserializeTag(Entity entity, const rapidjson::Value& tagVal)
	{
		if (!tagVal.IsString())
		{
			Logger::Error("Error: Entity " + std::to_string(entity.GetId()) +
				" has invalid tag: expected string", LogDestination::WINDOWS_CONSOLE);
			return;
		}

		entity.Tag(tagVal.GetString());
	}

	void Serializer::DeserializeGroup(Entity entity, const rapidjson::Value& groupVal)
	{
		if (!groupVal.IsString())
		{
			Logger::Error("Error: Entity " + std::to_string(entity.GetId()) +
				" has invalid group: expected string", LogDestination::WINDOWS_CONSOLE);
			return;
		}

		entity.Group(groupVal.GetString());
	}

	bool Serializer::SaveSceneToFile(const std::string& filename, CameraManager& cameraMgr)
	{
		std::filesystem::current_path(std::filesystem::path(Popplio::Engine::programPath));

		// Clear the existing document and set it as an object
		document.SetObject();

		SerializeEntities();
		SerializeLayers();
		SerializeCameraSettings(cameraMgr);

		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		document.Accept(writer);

		std::ofstream outFile(filename);
		if (!outFile)
		{
			Logger::Error("Failed to open file for writing: " + filename);
			std::filesystem::current_path(std::filesystem::path(Popplio::Engine::programPath));
			return false;
		}

		outFile << buffer.GetString();
		outFile.close();

		Logger::Info("Saved level data to file: " + filename);
		std::filesystem::current_path(std::filesystem::path(Popplio::Engine::programPath));
		return true;
	}

	bool Serializer::SavePrefabToFile(const std::string& filename, const Prefab& prefab)
	{
		document.SetObject();
		auto& allocator = document.GetAllocator();

		rapidjson::Value prefabValue(rapidjson::kObjectType);
		SerializePreFab(prefab, prefabValue, allocator, document);

		//document.AddMember("prefab", prefabValue, allocator);

		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		document.Accept(writer);

		std::ofstream outFile(filename);
		if (!outFile)
		{
			Logger::Error("Failed to open file for writing: " + filename);
			return false;
		}

		outFile << buffer.GetString();
		outFile.close();
		Logger::Info("Saved prefab data to file: " + filename);
		return true;
	}

	void Serializer::SerializeEntities()
	{
		rapidjson::Value entities(rapidjson::kArrayType);
		auto& allocator = document.GetAllocator();

		// Store child relationships separately
		rapidjson::Value childRelationships(rapidjson::kArrayType);

		for (const auto& entity : registry.GetAllEntities())
		{
			rapidjson::Value entityValue(rapidjson::kObjectType);
			SerializeEntity(entity, entityValue, allocator);
			entities.PushBack(entityValue, allocator);

			if (entity.HasComponent<ParentComponent>())
			{
				const auto& parentComp = entity.GetComponent<ParentComponent>();
				if (!parentComp.children.empty())
				{
					rapidjson::Value relationship(rapidjson::kObjectType);
					relationship.AddMember("parentId", entity.GetId(), allocator);

					rapidjson::Value children(rapidjson::kArrayType);
					for (const auto& child : parentComp.children)
					{
						children.PushBack(child.GetId(), allocator);
					}
					relationship.AddMember("childrenIds", children, allocator);
					childRelationships.PushBack(relationship, allocator);
				}
			}
		}
		document.AddMember("entities", entities, allocator);
		document.AddMember("child_relationships", childRelationships, allocator);
	}

	void Serializer::SerializeEntity(Entity entity, rapidjson::Value& entityValue, rapidjson::Document::AllocatorType& allocator)
	{
		entityValue.AddMember("id", entity.GetId(), allocator);

		if (registry.EntityHasAnyTag(entity))
		{
			std::string tag = registry.GetEntityTag(entity);
			rapidjson::Value tagValue;
			tagValue.SetString(tag.c_str(), allocator);
			entityValue.AddMember("tag", tagValue, allocator);
		}

		if (registry.EntityBelongsToAnyGroup(entity))
		{
			std::string group = registry.GetEntityGroup(entity);
			rapidjson::Value groupValue;
			groupValue.SetString(group.c_str(), allocator);
			entityValue.AddMember("group", groupValue, allocator);
		}

		// Add Layer information
		std::string layerName = layerManager.GetEntityLayer(entity);
		if (!layerName.empty())
		{
			rapidjson::Value layerValue;
			layerValue.SetString(layerName.c_str(), allocator);
			entityValue.AddMember("layer", layerValue, allocator);
		}

		SerializeComponent<ActiveComponent>(entity, entityValue, allocator);
		SerializeComponent<TransformComponent>(entity, entityValue, allocator);
		SerializeComponent<RigidBodyComponent>(entity, entityValue, allocator);
		SerializeComponent<BoxColliderComponent>(entity, entityValue, allocator);
		SerializeComponent<RenderComponent>(entity, entityValue, allocator);
		SerializeComponent<TextComponent>(entity, entityValue, allocator);
		SerializeComponent<AnimationComponent>(entity, entityValue, allocator);
		SerializeComponent<AudioComponent>(entity, entityValue, allocator);
		SerializeComponent<LogicComponent>(entity, entityValue, allocator);
		SerializeComponent<PrefabInstanceComponent>(entity, entityValue, allocator);
		SerializeComponent<ParentComponent>(entity, entityValue, allocator);
		SerializeComponent<PersistOnLoadComponent>(entity, entityValue, allocator);
		SerializeComponent<UIComponent>(entity, entityValue, allocator);
        SerializeComponent<ParticleComponent>(entity, entityValue, allocator);
	}

	void Serializer::SerializePreFab(const Prefab& prefab, rapidjson::Value& prefabValue, 
		rapidjson::Document::AllocatorType& allocator, rapidjson::Document& doc)
	{
        static_cast<void>(doc);

		// Serialize prefab name
		rapidjson::Value nameValue;
		nameValue.SetString(prefab.GetName().c_str(), allocator);
		prefabValue.AddMember("name", nameValue, allocator);
		Logger::Info("Added prefab name: " + prefab.GetName());

		// Create components value
		rapidjson::Value components(rapidjson::kObjectType);

		// Use the generic template for each component type
		SerializeComponent<ActiveComponent>(prefab, components, allocator);
		SerializeComponent<TransformComponent>(prefab, components, allocator);
		SerializeComponent<RigidBodyComponent>(prefab, components, allocator);
		SerializeComponent<BoxColliderComponent>(prefab, components, allocator);
		SerializeComponent<RenderComponent>(prefab, components, allocator);
		SerializeComponent<TextComponent>(prefab, components, allocator);
		SerializeComponent<AudioComponent>(prefab, components, allocator);
		SerializeComponent<LogicComponent>(prefab, components, allocator);
		SerializeComponent<AnimationComponent>(prefab, components, allocator);
		SerializeComponent<ParentComponent>(prefab, components, allocator);
        SerializeComponent<PersistOnLoadComponent>(prefab, components, allocator);
        SerializeComponent<UIComponent>(prefab, components, allocator);
        SerializeComponent<ParticleComponent>(prefab, components, allocator);

		prefabValue.AddMember("components", components, allocator);

		// Serialize child prefabs
		if (!prefab.GetChildPrefabs().empty())
		{
			rapidjson::Value childPrefs(rapidjson::kArrayType);

			for (const auto& childRef : prefab.GetChildPrefabs())
			{
				rapidjson::Value childPrefab(rapidjson::kObjectType);

				// Serialize prefab path
				rapidjson::Value path;
				path.SetString(childRef.prefabPath.c_str(), allocator);
				childPrefab.AddMember("path", path, allocator);

				// Serialize transform data
				rapidjson::Value position(rapidjson::kObjectType);
				position.AddMember("x", childRef.localPosition.x, allocator);
				position.AddMember("y", childRef.localPosition.y, allocator);
				childPrefab.AddMember("position", position, allocator);

				rapidjson::Value scale(rapidjson::kObjectType);
				scale.AddMember("x", childRef.localScale.x, allocator);
				scale.AddMember("y", childRef.localScale.y, allocator);
				childPrefab.AddMember("scale", scale, allocator);

				childPrefab.AddMember("rotation", childRef.localRotation, allocator);

				childPrefs.PushBack(childPrefab, allocator);
			}

			prefabValue.AddMember("childPrefabs", childPrefs, allocator);
			Logger::Info("Added child prefab references");
		}

		//std::string prefabName = ;
		document.AddMember("prefab", prefabValue, allocator);
	}

	void Serializer::SerializeLayers()
	{
		auto& allocator = document.GetAllocator();

		// Create layers array
		rapidjson::Value layersArray(rapidjson::kArrayType);

		// Get sorted layers
		auto sortedLayers = layerManager.GetSortedLayers();

		for (const auto& layer : sortedLayers)
		{
			rapidjson::Value layerObj(rapidjson::kObjectType);

			// Serialize layer properties
			rapidjson::Value nameValue;
			nameValue.SetString(layer.name.c_str(), allocator);
			layerObj.AddMember("name", nameValue, allocator);
			layerObj.AddMember("order", layer.order, allocator);
			layerObj.AddMember("active", layer.active, allocator);

			// Get entities in this layer
			rapidjson::Value entitiesArray(rapidjson::kArrayType);
			auto layerEntities = layerManager.GetEntitiesInLayer(layer.name);

			for (const auto& entity : layerEntities)
			{
				entitiesArray.PushBack(entity.GetId(), allocator);
			}

			layerObj.AddMember("entities", entitiesArray, allocator);
			layersArray.PushBack(layerObj, allocator);
		}

		// Add Layers array to document
		document.AddMember("layers", layersArray, allocator);

		// Serialize layer order interactions
		rapidjson::Value interactionsObj(rapidjson::kObjectType);
		const auto& interactions = layerManager.GetLayerInteractions();

		for (const auto& [layer1, layerInteractions] : interactions)
		{
			rapidjson::Value layer1Interactions(rapidjson::kObjectType);

			for (const auto& [layer2, interacts] : layerInteractions)
			{
				layer1Interactions.AddMember(
					rapidjson::Value(layer2.c_str(), allocator).Move(),
					interacts,
					allocator
				);
			}

			interactionsObj.AddMember(
				rapidjson::Value(layer1.c_str(), allocator).Move(),
				layer1Interactions,
				allocator
			);
		}

		document.AddMember("layer_interactions", interactionsObj, allocator);
	}

	void Serializer::DeserializeLayers()
	{
		if (!document.HasMember("layers"))
		{
			Logger::Warning("No layer data found in file");
			return;
		}

		const auto& layersArray = document["layers"];
		if (!layersArray.IsArray())
		{
			Logger::Error("Invalid layer data format");
			return;
		}

		// Clear existing layers
		layerManager.Clear();

		// First pass: Create all Layers
		for (const auto& layerObj : layersArray.GetArray())
		{
			if (!layerObj.HasMember("name") || !layerObj.HasMember("active"))
			{
				Logger::Warning("Skipping invalid layer entry");
				continue;
			}

			std::string name = layerObj["name"].GetString();
			bool active = layerObj["active"].GetBool();

			layerManager.AddLayer(name, active);
		}

		// Second pass: Set orders and restore entity assignments
		for (const auto& layerObj : layersArray.GetArray())
		{
			std::string name = layerObj["name"].GetString();

			// Set order
			if (layerObj.HasMember("order"))
			{
				int order = layerObj["order"].GetInt();
				layerManager.SetLayerOrder(name, order);
			}

			// Restore entity assignments
			if (layerObj.HasMember("entities") && layerObj["entities"].IsArray())
			{
				const auto& entitiesArray = layerObj["entities"].GetArray();
				for (const auto& entityId : entitiesArray)
				{
					if (!entityId.IsInt())
					{
						continue;
					}

					if (registry.EntityExists(entityId.GetInt()))
					{
						Entity entity = registry.GetEntityById(entityId.GetInt());
						layerManager.AddEntityToLayer(entity, name);
					}
				}
			}
		}

		// Deserialize layer interactions
		if (document.HasMember("layer_interactions"))
		{
			const auto& interactionsObj = document["layer_interactions"];
			if (interactionsObj.IsObject())
			{
				// Create temporary map to store all interactions
				std::unordered_map<std::string, std::unordered_map<std::string, bool>> interactions;

				// Parse the interactions
				for (auto it1 = interactionsObj.MemberBegin(); it1 != interactionsObj.MemberEnd(); ++it1)
				{
					const std::string layer1 = it1->name.GetString();
					const auto& layer1Interactions = it1->value;

					if (layer1Interactions.IsObject())
					{
						for (auto it2 = layer1Interactions.MemberBegin();
							it2 != layer1Interactions.MemberEnd(); ++it2)
						{
							const std::string layer2 = it2->name.GetString();
							bool interacts = it2->value.GetBool();

							layerManager.SetLayerInteraction(layer1, layer2, interacts);
						}
					}
				}
			}
		}

		Logger::Info("Successfully deserialized layers and interactions");
	}

	// doesnt clear children currently
	std::set<Popplio::Entity>& Serializer::ClearEntitiesIfNotPersistent(std::set<Popplio::Entity>& persistentEntities, Entity entity)
	{
		//if (std::find(persistentLayers.begin(), persistentLayers.end(), layerManager.GetEntityLayer(entity)) 
		//	== persistentLayers.end())
//                   persistentLayers.push_back(layerManager.GetEntityLayer(entity));

		persistentEntities.insert(entity);

		//if (entity.HasComponent<ParentComponent>())
		//{
		//	for (auto& child : entity.GetComponent<ParentComponent>().children)
		//	{
		//		persistentEntities = ClearEntitiesIfNotPersistent(persistentEntities, child); // recursion to add children
		//	}
		//}

		return persistentEntities;
	}

	std::set<Popplio::Entity> Serializer::ClearCurrentScene(bool const& clearPersistent)
	{
		Logger::Info("Clearing current scene...");

		// Clear all layers first
		//layerManager.Clear();

		std::set<Popplio::Entity> persistentEntities{};

		// Get all entities and remove them
		auto entities = registry.GetAllEntities();
		for (auto entity : entities)
		{
			if (!clearPersistent)
			{
				if (entity.HasComponent<PersistOnLoadComponent>())
				{
					if (entity.GetComponent<PersistOnLoadComponent>().persistOnLoad)
					{
						persistentEntities = ClearEntitiesIfNotPersistent(persistentEntities, entity);
					}
					else registry.KillEntity(entity);
				}
				else registry.KillEntity(entity);
			}
			else registry.KillEntity(entity);
		}

		//for (auto entity : persistentEntities)
		//{
		//    registry.KillEntity(entity);
		//}

		//std::vector<Popplio::Signature> signatures{};

  //      for (auto entity : registry.GetAllEntities())
  //      {
  //          signatures.push_back(registry.entityComponentSignatures[entity.GetId()]);
  //      }

		layerManager.Clear(persistentEntities); // clear layers

		// Process any pending entity deletions
		registry.Update();

		registry.ClearFreeIds();

		//if (clearPersistent) registry.ResetNumEntities();
		//else registry.UpdateNumEntities();

		Logger::Info("Scene cleared successfully");

		return persistentEntities;
	}

	bool Serializer::SaveEntityAsPrefab(const Entity& entity, const std::string& prefabName, const std::string& filePath,
		bool isChild, std::unordered_map<int, std::string>* savedPrefabs)
	{
        static_cast<void>(isChild);

		if (prefabName.empty()) {
			Logger::Error("Cannot save prefab with empty name", LogDestination::IMGUI_CONSOLE);
			return false;
		}

		// Create map for tracking saved prefabs if this is the root call
		std::unordered_map<int, std::string> localSavedPrefabs;
		if (!savedPrefabs) {
			savedPrefabs = &localSavedPrefabs;
		}

		// Check if this entity was already saved as a prefab
		if (savedPrefabs->find(entity.GetId()) != savedPrefabs->end()) {
			return true;
		}

		// Create a new document for each prefab
		rapidjson::Document prefabDoc;
		prefabDoc.SetObject();
		auto& allocator = prefabDoc.GetAllocator();

		rapidjson::Value prefabValue(rapidjson::kObjectType);

		// Basic prefab properties
		rapidjson::Value nameValue;
		nameValue.SetString(prefabName.c_str(), allocator);
		prefabValue.AddMember("name", nameValue, allocator);

		// Serialize components
		rapidjson::Value components(rapidjson::kObjectType);
		SerializeEntity(entity, components, allocator);
		prefabValue.AddMember("components", components, allocator);

		// Handle child prefabs
		if (entity.HasComponent<ParentComponent>() && !entity.GetComponent<ParentComponent>().children.empty()) {
			rapidjson::Value childPrefabs(rapidjson::kArrayType);
			std::filesystem::path parentPath = std::filesystem::path(filePath).parent_path();

			for (auto& child : entity.GetComponent<ParentComponent>().children) {
				std::string childPrefabName = prefabName + "_" + std::to_string(child.GetId());
				std::string childFileName = childPrefabName + ".prefab";
				std::string childPath = (parentPath / childFileName).string();

				// Save child to its own file
				if (SaveEntityAsPrefab(child, childPrefabName, childPath, true, savedPrefabs)) {
					rapidjson::Value childRef(rapidjson::kObjectType);

					// Add child reference
					rapidjson::Value pathValue;
					pathValue.SetString(childFileName.c_str(), allocator);
					childRef.AddMember("path", pathValue, allocator);

					// Add transform data
					auto& childTransform = child.GetComponent<TransformComponent>();
					rapidjson::Value position(rapidjson::kObjectType);
					position.AddMember("x", childTransform.position.x, allocator);
					position.AddMember("y", childTransform.position.y, allocator);
					childRef.AddMember("position", position, allocator);

					rapidjson::Value scale(rapidjson::kObjectType);
					scale.AddMember("x", childTransform.scale.x, allocator);
					scale.AddMember("y", childTransform.scale.y, allocator);
					childRef.AddMember("scale", scale, allocator);

					childRef.AddMember("rotation", childTransform.rotation, allocator);

					childPrefabs.PushBack(childRef, allocator);
				}
			}
			prefabValue.AddMember("childPrefabs", childPrefabs, allocator);
		}

		prefabDoc.AddMember("prefab", prefabValue, allocator);

		// Save to file
		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		prefabDoc.Accept(writer);

		std::ofstream outFile(filePath);
		if (!outFile) {
			Logger::Error("Failed to open file for writing: " + filePath);
			return false;
		}

		outFile << buffer.GetString();
		outFile.close();

		// Record that this prefab has been saved
		(*savedPrefabs)[entity.GetId()] = std::filesystem::path(filePath).filename().string();

		//prefabManager.RegisterPrefab(prefabName, CreatePrefabFromEntity(entity), filePath);
		Logger::Info("Successfully saved prefab: " + prefabName);
		return true;
	}

	std::unique_ptr<Prefab> Serializer::CreatePrefabFromEntity(Entity const& entity)
	{
		auto prefab = std::make_unique<Prefab>(registry.GetEntityTag(entity));
		CopyEntityToPrefab(entity, *prefab);
		return prefab;
	}

	void Serializer::CopyEntityToPrefab(Entity const& entity, Prefab& prefab)
	{
		// Copy transform component
		if (entity.HasComponent<TransformComponent>())
		{
			prefab.AddComponent<TransformComponent>(entity.GetComponent<TransformComponent>());
		}

		// Copy render component
		if (entity.HasComponent<RenderComponent>())
		{
			prefab.AddComponent<RenderComponent>(entity.GetComponent<RenderComponent>());
		}

		// Copy box collider component
		if (entity.HasComponent<BoxColliderComponent>())
		{
			prefab.AddComponent<BoxColliderComponent>(entity.GetComponent<BoxColliderComponent>());
		}

		// Copy rigid body component
		if (entity.HasComponent<RigidBodyComponent>())
		{
			prefab.AddComponent<RigidBodyComponent>(entity.GetComponent<RigidBodyComponent>());
		}

		// Copy text component
		if (entity.HasComponent<TextComponent>())
		{
			prefab.AddComponent<TextComponent>(entity.GetComponent<TextComponent>());
		}

		// Copy animation component
		if (entity.HasComponent<AnimationComponent>())
		{
			prefab.AddComponent<AnimationComponent>(entity.GetComponent<AnimationComponent>());
		}

		// Copy audio component
		if (entity.HasComponent<AudioComponent>())
		{
			prefab.AddComponent<AudioComponent>(entity.GetComponent<AudioComponent>());
		}

		// Copy script component
		if (entity.HasComponent<LogicComponent>())
		{
			prefab.AddComponent<LogicComponent>(entity.GetComponent<LogicComponent>());
		}

		// Copy active component
		if (entity.HasComponent<ActiveComponent>())
		{
			prefab.AddComponent<ActiveComponent>(entity.GetComponent<ActiveComponent>());
		}

		// Copy parent component
		if (entity.HasComponent<ParentComponent>())
		{
			prefab.AddComponent<ParentComponent>(entity.GetComponent<ParentComponent>());
		}

		// Copy persist on load component
		if (entity.HasComponent<PersistOnLoadComponent>())
		{
			prefab.AddComponent<PersistOnLoadComponent>(entity.GetComponent<PersistOnLoadComponent>());
		}

        // Copy UI component
        if (entity.HasComponent<UIComponent>())
        {
            prefab.AddComponent<UIComponent>(entity.GetComponent<UIComponent>());
        }

        // Copy Particle component
        if (entity.HasComponent<ParticleComponent>())
        {
            prefab.AddComponent<ParticleComponent>(entity.GetComponent<ParticleComponent>());
        }
	}

	bool Serializer::HasCircularReference(Entity const& entity, std::set<int>& visitedIds)
	{
		if (visitedIds.find(entity.GetId()) != visitedIds.end())
		{
			return true;
		}

		visitedIds.insert(entity.GetId());

		if (entity.HasComponent<ParentComponent>())
		{
			auto& parentComp = entity.GetComponent<ParentComponent>();
			for (auto& child : parentComp.children)
			{
				if (HasCircularReference(child, visitedIds))
				{
					return true;
				}
			}
		}

		visitedIds.erase(entity.GetId());
		return false;
	}

	void Serializer::SerializeCameraSettings(CameraManager& cameraManager)
	{
		auto& allocator = document.GetAllocator();

		// Create camera settings object
		rapidjson::Value cameraSettings(rapidjson::kObjectType);

		// Get camera reference
		const auto& camera = cameraManager.GetGameCamera();

		// Serialize position
		rapidjson::Value position(rapidjson::kObjectType);
		position.AddMember("x", camera.GetPosition().x, allocator);
		position.AddMember("y", camera.GetPosition().y, allocator);
		cameraSettings.AddMember("position", position, allocator);

		// Serialize zoom
		cameraSettings.AddMember("zoom", camera.GetZoom(), allocator);

		// Add to main document
		document.AddMember("camera_settings", cameraSettings, allocator);
	}

	void Serializer::DeserializeCameraSettings(CameraManager& cameraManager)
	{
		if (!document.HasMember("camera_settings"))
		{
			Logger::Info("No camera settings found in scene file");
			return;
		}

		const auto& cameraSettings = document["camera_settings"];

		// Get camera reference
		auto& camera = cameraManager.GetGameCamera();

		// Deserialize position
		if (cameraSettings.HasMember("position"))
		{
			const auto& position = cameraSettings["position"];
			PopplioMath::Vec2f pos(
				position["x"].GetFloat(),
				position["y"].GetFloat()
			);
			camera.SetPosition(pos);
		}

		// Deserialize zoom
		if (cameraSettings.HasMember("zoom"))
		{
			camera.SetZoom(cameraSettings["zoom"].GetFloat());
		}
	}
}