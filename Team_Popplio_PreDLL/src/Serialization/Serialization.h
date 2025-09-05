/******************************************************************************/
/*!
\file           Serialization.h
\author         Team Popplio
\author         Bryan Ang Wei Ze
\co-author      Hafawati Binte Mohd Ali | Ng Jun Heng Shawn | Shannon | Val
\contribution   Bryan - 60%, Hafawati - 20%, Shawn - 10%, Shannon - 5%, Val - 5%
\par            Course : CSD2401 / CSD2400
\par            Section : A
\date           2025/03/13 10:01:45 AM (Last Edit)
\brief
	This is the header file for the serializer of the engine

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include <rapidjson/document.h>

#include <algorithm>
#include <typeinfo>
#include <filesystem>
#include <unordered_set>

//#include "../Engine/Engine.h"

#include "../Transformation/TransformComponent.h"
#include "../Physics/RigidBodyComponent.h"
#include "../Graphic/GraphicComponent.h"
#include "../Collision/BoxColliderComponent.h"
#include "../Collision/CircleColliderComponent.h"
#include "../Animation/AnimationComponent.h"
#include "../UI/UIComponent.h"
#include "../ECS/ECS.h"
#include "../PrefabManagement/Prefab.h"
#include "../AssetStore/AssetStore.h"
#include "../Audio/AudioSystem.h"
#include "../Script/LogicSystem.h"
#include "../Utilities/Components.h"
#include "../Layering/LayerManager.h"
#include "../AssetStore/AssetLoader.h"
#include "../PrefabManagement/PrefabManager.h"
#include "../Graphic/Color.h"

#include "../Utilities/QueueSystem.h"

#include "SerializeTypes.h"

namespace Popplio
{
	/**
	 * \struct Config
	 * \brief Represents the configuration settings for the serializer.
	 */
	struct Config
	{
		std::string engineVer; /**< The version of the engine. */
		std::string appVer; /**< The version of the application. */

		// program settings
		int width; /**< The width of the configuration. */
		int height; /**< The height of the configuration. */
		bool vSync; /**< The vSync flag of the configuration. */

		// editor
		bool safeMode; /**< The safe mode flag (editor) of the configuration. */
		std::string lastScene; /**< The last scene opened of the configuration. */
		bool verbose; /**< The verbose flag of the configuration. */

		// build settings
		std::string startScene; /**< The start scene of the configuration. */
	};

	/**
	 * \class Serializer
	 * \brief Responsible for serializing and deserializing entities.
	 */
	class Serializer
	{
	public:
		/**
		 * \brief Constructs a Serializer object.
		 * \param registry The registry to access entities.
		 */
		Serializer(Registry& reg, LayerManager& layerMgr, PrefabManager& prefabMgr, EventBus& eventBus);

		/**
		 * \brief Loads entities from a file.
		 * \param filename The name of the file to load from.
		 * \param clearPersistent Flag to clear persistent entities.
		 * \return True if the loading is successful, false otherwise.
		 */
		bool LoadSceneFromFile(const std::string& filename, CameraManager& cameraMgr, const bool& clearPersistent = true);

		// TODO save config

		/**
		 * \brief Loads the configuration from a file.
		 * \param filename The name of the file to load the configuration from.
		 * \return True if the loading is successful, false otherwise.
		 */
		bool LoadConfiguration(const std::string& filename);

		//**
		// * \brief Load assets from a JSON file.
		// *
		// * This function reads a JSON file and loads various types of assets, such as textures,
		// * shaders, fonts, and audio. It checks the JSON structure and delegates the loading
		// * of different asset types to specific functions.
		// *
		// * \param filename The path to the JSON file containing asset information.
		// * \param audioSystem Reference to the audio system for loading audio assets.
		// * \return True if all assets are loaded successfully, otherwise false.
		// */
		//bool LoadAsset(const std::string& filename, AudioSystem& audioSystem, LogicSystem& logicSystem);

		/* void DeserializeTexture(const rapidjson::Value& entityValue);
		 void DeserializeShader(const rapidjson::Value& entityValue);
		 void DeserializeFont(const rapidjson::Value& entityValue);

		 void DeserializeFileNames(const std::string& filename);*/

		 /**
		  * \brief Deserializes the entities.
		  */
		void DeserializeEntities();

		/**
		 * \brief Saves the current scene to a file.
		 * \param filename The name of the file to save the scene to.
		 * \return True if the saving is successful, false otherwise.
		 */
		bool SaveSceneToFile(const std::string& filename, CameraManager& cameraMgr);

		/**
		 * \brief Saves a prefab to a file.
		 * \param filename The name of the file to save the prefab to.
		 * \param prefab The prefab to save.
		 * \return True if the saving is successful, false otherwise.
		 */
		bool SavePrefabToFile(const std::string& filename, const Prefab& prefab);

		/**
		 * \brief Loads a prefab from a file.
		 * \param filename The name of the file to load the prefab from.
		 * \return A unique pointer to the loaded prefab, or nullptr if the loading fails.
		 */
		std::unique_ptr<Prefab> LoadPrefabFromFile(const std::string& filename);

		std::unique_ptr<Prefab> LoadPrefabFromFileInternal(const std::string& filename,
			std::unordered_set<std::string>& loadedPrefabs);

		/**
		 * \brief Serializes the entities in the current scene.
		 */
		void SerializeEntities();

		/**
		 * \brief Serializes the layers in the layer manager.
		 * \param layerManager The layer manager containing the layers to serialize.
		 */
		void SerializeLayers();

		/**
		 * \brief Deserializes the layers into the layer manager.
		 * \param layerManager The layer manager to deserialize the layers into.
		 */
		void DeserializeLayers();

		/**
		 * \brief Clears the current scene by removing all entities.
		 * \param clearPersistent Flag to clear persistent entities.
		 * \return persistent entities that were not cleared.
		 */
		std::set<Popplio::Entity> ClearCurrentScene(bool const& clearPersistent);

		/**
		 * \brief Gets the configuration.
		 * \return The configuration.
		 */
		Config GetConfiguration() const { return configuration; }

		/** * \brief Generate an asset name from a file path.
		 *
		 * This function extracts the filename without its extension from a given file path.
		 * It identifies the position of the last slash ("/" or "\\") and the last dot ("."),
		 * then extracts the substring between them to form the asset name.
		 *
		 * \param filepath The full path to the file.
		 * \return The generated asset name without the file extension.
		 */
		static std::string GenerateAssetName(const std::string& filepath);

		/**
		 * \brief Serializes a meta file for assets
		 * \param filename The path to the asset
		 * \return Returns true if meta file was generated from the asset, returns false otherwise
		 */
		static bool SerializeMetaFile(const std::filesystem::path& filename);

		/**
		 * \brief Deserializes a meta file for assets
		 * \param filename The path to the META file
		 * \return Returns true if asset was successfully loaded into the engine
		 */
		static bool DeserializeMetaFile(const std::filesystem::path& filename);

		bool SaveEntityAsPrefab(const Entity& entity, const std::string& prefabName, const std::string& filePath,
			bool isChild = false, std::unordered_map<int, std::string>* savedPrefabs = nullptr);

		std::unique_ptr<Prefab> CreatePrefabFromEntity(const Entity& entity);

		void DeserializePrefabComponents(Prefab& prefab, const rapidjson::Value& components);
	private:

		///**
		// * \brief Load texture assets from a JSON array.
		// *
		// * This function iterates over a JSON array of texture paths, generates asset names,
		// * and stores the texture assets in the AssetStore.
		// *
		// * \param texturesArray The JSON array containing texture file paths.
		// * \return True if all textures are loaded successfully, otherwise false.
		// */
		//bool LoadTextures(const rapidjson::Value& texturesArray);
		///**
		// * \brief Load shader assets from a JSON array.
		// *
		// * This function iterates over a JSON array of shader paths, generates asset names,
		// * and stores the shader assets in the AssetStore.
		// *
		// * \param shadersArray The JSON array containing shader file paths.
		// * \return True if all shaders are loaded successfully, otherwise false.
		// */
		//bool LoadShaders(const rapidjson::Value& shadersArray);

		///**
		// * \brief Load font assets from a JSON array.
		// *
		// * This function iterates over a JSON array of font paths, generates asset names,
		// * and stores the font assets in the AssetStore.
		// *
		// * \param fontsArray The JSON array containing font file paths.
		// * \return True if all fonts are loaded successfully, otherwise false.
		// */
		//bool LoadFonts(const rapidjson::Value& fontsArray);
		///**
		// * \brief Load audio assets from a JSON array.
		// *
		// * This function iterates over a JSON array of audio paths and metadata, generates asset names,
		// * and stores the audio assets in the AudioSystem.
		// *
		// * \param audioArray The JSON array containing audio file paths and metadata.
		// * \param audioSystem Reference to the audio system for loading audio assets.
		// * \return True if all audio assets are loaded successfully, otherwise false.
		// */
		//bool LoadAudio(const rapidjson::Value& audioArray, AudioSystem& audioSystem);
		///**
		// * \brief Load script assets from a JSON array.
		// *
		// * This function iterates over a JSON array of script paths and metadata, generates asset names,
		// * and stores the script assets in the LogicSystem.
		// *
		// * \param logicArray The JSON array containing script file paths and metadata.
		// * \param logicSystem Reference to the logic system for loading script assets.
		// * \return True if all script assets are loaded successfully, otherwise false.
		// */
		//bool LoadCPPScripts(const rapidjson::Value& logicArray, LogicSystem& logicSystem);

		bool LoadPrefabs(const rapidjson::Value& prefabsArray);

		/**
		 * \brief Serializes an entity.
		 * \param entity The entity to serialize.
		 * \param entityValue The JSON value to store the serialized entity.
		 * \param allocator The allocator for JSON document.
		 */
		void SerializeEntity(Entity entity, rapidjson::Value& entityValue, rapidjson::Document::AllocatorType& allocator);

		/**
		 * \brief Deserializes an entity.
		 * \param entityVal The JSON value representing the entity.
		 */
		void DeserializeEntity(const rapidjson::Value& entityVal);


		/**
		 * \brief Deserializes the Tag of an entity.
		 * \param entity The entity to deserialize the Tag for.
		 * \param tagVal The JSON value representing the Tag.
		 */
		void DeserializeTag(Entity entity, const rapidjson::Value& tagVal);

		/**
		 * \brief Deserializes the Group of an entity.
		 * \param entity The entity to deserialize the Group for.
		 * \param groupVal The JSON value representing the Group.
		 */
		void DeserializeGroup(Entity entity, const rapidjson::Value& groupVal);

		/**
		 * \brief Serializes a prefab.
		 * \param prefab The prefab to serialize.
		 * \param prefabValue The JSON value to store the serialized prefab.
		 * \param allocator The allocator for JSON document.
		 * \param doc The document object.
		 */
		void SerializePreFab(const Prefab& prefab, rapidjson::Value& prefabValue,
			rapidjson::Document::AllocatorType& allocator, rapidjson::Document& doc);

		/**
		 * \brief Deserializes a prefab.
		 * \param prefabValue The JSON value representing the prefab.
		 * \return A unique pointer to the deserialized prefab, or nullptr if the deserialization fails.
		 */
		std::unique_ptr<Prefab> DeserializePrefab(const rapidjson::Value& prefabValue);

		/**
		 * \brief Serializes a component of an object.
		 * \tparam T The type of the component.
		 * \tparam ObjectType The type of the object.
		 * \param object The object containing the component.
		 * \param objectValue The JSON value to store the serialized component.
		 * \param allocator The allocator for JSON document.
		 */
		template <typename T, typename ObjectType>
		void SerializeComponent(ObjectType& object, rapidjson::Value& objectValue, rapidjson::Document::AllocatorType& allocator);

		/**
		 * \brief Deserializes a component of an object.
		 * \tparam T The type of the component.
		 * \tparam ObjectType The type of the object.
		 * \param object The object to deserialize the component for.
		 * \param componentVal The JSON value representing the component.
		 */
		template <typename T, typename ObjectType>
		void DeserializeComponent(ObjectType& object, const rapidjson::Value& componentVal);

		std::set<Popplio::Entity>& ClearEntitiesIfNotPersistent(std::set<Popplio::Entity>& persistentEntities, Entity e);

		// Helper methods for saving entities as prefabs
		bool HasCircularReference(Entity const& entity, std::set<int>& visitedIds);
		void CopyEntityToPrefab(Entity const& entity, Prefab& prefab);

		void SerializeCameraSettings(CameraManager& cameraManager);
		void DeserializeCameraSettings(CameraManager& cameraManager);

		Registry& registry; /**< The registry to access entities. */
		rapidjson::Document document; /**< The JSON document. */
		Config configuration; /**< The configuration settings. */
		PrefabManager& prefabManager; /**< The prefab manager. */
		LayerManager& layerManager; /**< The layer manager. */
		EventBus& eventBus; /**< The event bus. */
	};

	template <typename T, typename ObjectType>
	void Serializer::SerializeComponent(ObjectType& object, rapidjson::Value& objectValue, rapidjson::Document::AllocatorType& allocator)
	{
		// Early return if object doesn't have the component
		if (!object.HasComponent<T>())
		{
			return;
		}

		// Add additional components here

		// Get component reference (valid for both Entity and Prefab)
		const auto& component = object.GetComponent<T>();
		rapidjson::Value componentValue(rapidjson::kObjectType);

		if constexpr (std::is_same_v<T, ActiveComponent>)
		{
			rapidjson::Value boolVal;
			boolVal.SetBool(component.isActive);
			componentValue.AddMember("isActive", boolVal, allocator);

			objectValue.AddMember("active", componentValue, allocator);

			return;
		}
		else if constexpr (std::is_same_v<T, TransformComponent>)
		{
			rapidjson::Value position(rapidjson::kObjectType);
			position.AddMember("x", component.position.x, allocator);
			position.AddMember("y", component.position.y, allocator);
			componentValue.AddMember("position", position, allocator);

			rapidjson::Value scaleValue(rapidjson::kObjectType);
			scaleValue.AddMember("x", component.scale.x, allocator);
			scaleValue.AddMember("y", component.scale.y, allocator);
			componentValue.AddMember("scale", scaleValue, allocator);

			componentValue.AddMember("rotation", component.rotation, allocator);

			objectValue.AddMember("transform", componentValue, allocator);

			return;
		}
		else if constexpr (std::is_same_v<T, RigidBodyComponent>)
		{
			rapidjson::Value boolVal;
			boolVal.SetBool(component.isActive);
			componentValue.AddMember("isActive", boolVal, allocator);

			rapidjson::Value velocityValue(rapidjson::kObjectType);
			velocityValue.AddMember("x", component.velocity.x, allocator);
			velocityValue.AddMember("y", component.velocity.y, allocator);
			componentValue.AddMember("velocity", velocityValue, allocator);

			//serialize acceleration
			rapidjson::Value accelerationValue(rapidjson::kObjectType);
			accelerationValue.AddMember("x", component.acceleration.x, allocator);
			accelerationValue.AddMember("y", component.acceleration.y, allocator);
			componentValue.AddMember("acceleration", accelerationValue, allocator);

			//serialize force
			rapidjson::Value forceValue(rapidjson::kObjectType);
			forceValue.AddMember("x", component.force.x, allocator);
			forceValue.AddMember("y", component.force.y, allocator);
			componentValue.AddMember("force", forceValue, allocator);

			//serialize scalar properties
			componentValue.AddMember("mass", component.mass, allocator);
			componentValue.AddMember("useGravity", component.useGravity, allocator);
			componentValue.AddMember("gravityScale", component.gravityScale, allocator);
			componentValue.AddMember("dragCoefficient", component.dragCoefficient, allocator);

			objectValue.AddMember("rigidbody", componentValue, allocator);

			return;
		}
		else if constexpr (std::is_same_v<T, RenderComponent>)
		{
			rapidjson::Value boolVal;
			boolVal.SetBool(component.isActive);
			componentValue.AddMember("isActive", boolVal, allocator);

			componentValue.AddMember("mesh", static_cast<int>(component.meshType), allocator);

			rapidjson::Value shaderNameValue;
			shaderNameValue.SetString(component.shaderName.c_str(), static_cast<rapidjson::SizeType>(component.shaderName.length()), allocator);
			componentValue.AddMember("shader_name", shaderNameValue, allocator);

			rapidjson::Value textureNameValue;
			textureNameValue.SetString(component.textureName.c_str(), static_cast<rapidjson::SizeType>(component.textureName.length()), allocator);
			componentValue.AddMember("texture_name", textureNameValue, allocator);

			componentValue.AddMember("use_instancing", component.useInstancing, allocator);

			rapidjson::Value colorValue(rapidjson::kObjectType);
			colorValue.AddMember("r", component.color.r, allocator);
			colorValue.AddMember("g", component.color.g, allocator);
			colorValue.AddMember("b", component.color.b, allocator);
			componentValue.AddMember("color", colorValue, allocator);

			componentValue.AddMember("alpha", component.alpha, allocator);
			componentValue.AddMember("texture_width", component.textureWidth, allocator);
			componentValue.AddMember("texture_height", component.textureHeight, allocator);

			// Add 9-Slice Scaling Properties
			componentValue.AddMember("enable_nine_slice", component.enableNineSlice, allocator);

			rapidjson::Value borderValues(rapidjson::kObjectType);
			borderValues.AddMember("left", component.borderLeft, allocator);
			borderValues.AddMember("right", component.borderRight, allocator);
			borderValues.AddMember("top", component.borderTop, allocator);
			borderValues.AddMember("bottom", component.borderBottom, allocator);
			componentValue.AddMember("nine_slice_borders", borderValues, allocator);

			objectValue.AddMember("render", componentValue, allocator);

			return;
		}

		else if constexpr (std::is_same_v<T, BoxColliderComponent>)
		{
			rapidjson::Value boolVal;
			boolVal.SetBool(component.isActive);
			componentValue.AddMember("isActive", boolVal, allocator);

			rapidjson::Value sizeValue(rapidjson::kObjectType);
			sizeValue.AddMember("x", component.size.x, allocator);
			sizeValue.AddMember("y", component.size.y, allocator);
			componentValue.AddMember("size", sizeValue, allocator);

			rapidjson::Value offsetValue(rapidjson::kObjectType);
			offsetValue.AddMember("x", component.offset.x, allocator);
			offsetValue.AddMember("y", component.offset.y, allocator);
			componentValue.AddMember("offset", offsetValue, allocator);

			componentValue.AddMember("isTrigger", component.isTrigger, allocator);
			componentValue.AddMember("isEnabled", component.isEnabled, allocator);

			objectValue.AddMember("boxcollider", componentValue, allocator);

			return;
		}
		else if constexpr (std::is_same_v<T, CircleColliderComponent>)
		{
			rapidjson::Value boolVal;
			boolVal.SetBool(component.isActive);
			componentValue.AddMember("isActive", boolVal, allocator);

			componentValue.AddMember("radius", component.radius, allocator);

			rapidjson::Value offsetValue(rapidjson::kObjectType);
			offsetValue.AddMember("x", component.offset.x, allocator);
			offsetValue.AddMember("y", component.offset.y, allocator);
			componentValue.AddMember("offset", offsetValue, allocator);

			componentValue.AddMember("isTrigger", component.isTrigger, allocator);
			componentValue.AddMember("isEnabled", component.isEnabled, allocator);

			objectValue.AddMember("circleCollider", componentValue, allocator);

			return;
		}
		else if constexpr (std::is_same_v<T, TextComponent>)
		{
			rapidjson::Value boolVal;
			boolVal.SetBool(component.isActive);
			componentValue.AddMember("isActive", boolVal, allocator);

			rapidjson::Value textValue;
			textValue.SetString(component.text.c_str(), allocator);
			componentValue.AddMember("text", textValue, allocator);

			rapidjson::Value fontNameValue;
			fontNameValue.SetString(component.fontName.c_str(), allocator);
			componentValue.AddMember("font_name", fontNameValue, allocator);

			rapidjson::Value sizeValue;
			sizeValue.SetInt(static_cast<int>(component.fontSize));
			componentValue.AddMember("size", sizeValue, allocator);

			rapidjson::Value colorValue(rapidjson::kObjectType);
			colorValue.AddMember("r", component.color.x, allocator);
			colorValue.AddMember("g", component.color.y, allocator);
			colorValue.AddMember("b", component.color.z, allocator);
			componentValue.AddMember("color", colorValue, allocator);

			componentValue.AddMember("alpha", component.alpha, allocator);

			objectValue.AddMember("text", componentValue, allocator);

			return;
		}
		else if constexpr (std::is_same_v<T, AnimationComponent>)
		{
			rapidjson::Value boolVal;
			boolVal.SetBool(component.isActive);
			componentValue.AddMember("isActive", boolVal, allocator);

			rapidjson::Value spriteSheetSize(rapidjson::kObjectType);
			spriteSheetSize.AddMember("width", component.sheetWidth, allocator);
			spriteSheetSize.AddMember("height", component.sheetHeight, allocator);
			componentValue.AddMember("sprite_sheet_size", spriteSheetSize, allocator);

			rapidjson::Value playOnce;
			playOnce.SetBool(component.playOnce);
			componentValue.AddMember("playOnce", playOnce, allocator);

			// Create JSON object for animation states
			rapidjson::Value animationState(rapidjson::kObjectType);
			for (const auto& [name, anim] : component.animations) {
				rapidjson::Value animValue(rapidjson::kObjectType);

				rapidjson::Value startFrame(rapidjson::kObjectType);
				startFrame.AddMember("x", anim.startX, allocator);
				startFrame.AddMember("y", anim.startY, allocator);
				animValue.AddMember("start_frame", startFrame, allocator);

				rapidjson::Value endFrame(rapidjson::kObjectType);
				endFrame.AddMember("x", anim.endX, allocator);
				endFrame.AddMember("y", anim.endY, allocator);
				animValue.AddMember("end_frame", endFrame, allocator);

				rapidjson::Value frameSize(rapidjson::kObjectType);
				frameSize.AddMember("width", anim.frameWidth, allocator);
				frameSize.AddMember("height", anim.frameHeight, allocator);
				animValue.AddMember("frame_size", frameSize, allocator);

				animValue.AddMember("animation_speed", anim.animationSpeed, allocator);

				rapidjson::Value animName;
				animName.SetString(name.c_str(), static_cast<rapidjson::SizeType>(name.length()), allocator);
				animationState.AddMember(animName, animValue, allocator);
			}
			componentValue.AddMember("animation_state", animationState, allocator);
			objectValue.AddMember("animation", componentValue, allocator);

			return;
		}
		else if constexpr (std::is_same_v<T, UIComponent>)
		{
			rapidjson::Value boolVal;
			boolVal.SetBool(component.isActive);
			componentValue.AddMember("isActive", boolVal, allocator);

			// Serialize UI type as a string
			rapidjson::Value uiTypeValue;
			std::string uiTypeStr;

			switch (component.type) {
			case UIType::BUTTON: uiTypeStr = "BUTTON"; break;
			case UIType::CHECKBOX: uiTypeStr = "CHECKBOX"; break;
			case UIType::SLIDER_TRACK: uiTypeStr = "SLIDER_TRACK"; break;
			case UIType::SLIDER_HANDLE: uiTypeStr = "SLIDER_HANDLE"; break;
			}

			uiTypeValue.SetString(uiTypeStr.c_str(), allocator);
			componentValue.AddMember("uiType", uiTypeValue, allocator);

			// Serialize additional properties based on type
			if (component.type == UIType::CHECKBOX) {
				componentValue.AddMember("checked", component.checked, allocator);
			}

			if (component.type == UIType::SLIDER_TRACK) {
				componentValue.AddMember("sliderValue", component.sliderValue, allocator);
			}

			if (component.type == UIType::SLIDER_HANDLE || component.type == UIType::SLIDER_TRACK) {
				componentValue.AddMember("linkedEntityId", component.linkedEntityId, allocator);
			}

			// Serialize multiple OnClick actions as an array
			if ((component.type == UIType::BUTTON || component.type == UIType::CHECKBOX) && !component.onClickActions.empty())
			{
				rapidjson::Value actionsArray(rapidjson::kArrayType);
				for (const std::string& action : component.onClickActions)
				{
					rapidjson::Value actionValue;
					actionValue.SetString(action.c_str(), allocator);
					actionsArray.PushBack(actionValue, allocator);
				}
				componentValue.AddMember("onClickActions", actionsArray, allocator);
			}

			// Serialize hover effect
			componentValue.AddMember("hasHoverEffect", component.hasHoverEffect, allocator);

			if (component.hasHoverEffect)
			{
				// Serialize spritesheet data (only if they are set)
				componentValue.AddMember("frameX", component.frameX, allocator);
				componentValue.AddMember("frameY", component.frameY, allocator);
				componentValue.AddMember("hoverFrameX", component.hoverFrameX, allocator);
				componentValue.AddMember("hoverFrameY", component.hoverFrameY, allocator);
				componentValue.AddMember("totalColumns", component.totalColumns, allocator);
				componentValue.AddMember("totalRows", component.totalRows, allocator);
				componentValue.AddMember("sheetWidth", component.sheetWidth, allocator);
				componentValue.AddMember("sheetHeight", component.sheetHeight, allocator);
			}

			objectValue.AddMember("ui", componentValue, allocator);
			return;
		}
		else if constexpr (std::is_same_v<T, LogicComponent>)
		{
			component.Serialize(objectValue, componentValue, allocator, registry.GetSystem<LogicSystem>());

			//rapidjson::Value boolVal;
			//boolVal.SetBool(component.isActive);
			//componentValue.AddMember("isActive", boolVal, allocator);

			//rapidjson::Value name;
			//name.SetString(component.scriptName.c_str(), static_cast<rapidjson::SizeType>(component.scriptName.length()), allocator);
			//componentValue.AddMember("name", name, allocator);

			//rapidjson::Value lang;
			//lang.SetInt(static_cast<int>(component.scriptLang));
			//componentValue.AddMember("scriptLang", lang, allocator);

			// variables //

			// variables //

			//AssetStore::GetScript();

			//for (std::pair<const std::string, std::pair<std::type_info&, void*>> v : component.vars) // serialize variables
			//{
			//    val.Clear();
			//    val.Set<v.second.first>(v.second.second, allocator);
			//    componentValue.AddMember(v.first, val, allocator);
			//}

			//objectValue.AddMember("logic", componentValue, allocator);

			return;
		}
		else if constexpr (std::is_same_v<T, AudioComponent>)
		{
			rapidjson::Value boolVal;
			boolVal.SetBool(component.isActive);
			componentValue.AddMember("isActive", boolVal, allocator);

			rapidjson::Value val;
			val.SetString(component.audio.c_str(), static_cast<rapidjson::SizeType>(component.audio.length()), allocator);
			componentValue.AddMember("name", val, allocator);

			rapidjson::Value vol;
			vol.SetFloat(component.volume);
			componentValue.AddMember("volume", vol, allocator);

			rapidjson::Value mute;
			mute.SetBool(component.mute);
			componentValue.AddMember("mute", mute, allocator);

			rapidjson::Value loop;
			loop.SetBool(component.loop);
			componentValue.AddMember("loop", loop, allocator);

			rapidjson::Value bpm;
			bpm.SetFloat(component.bpm);
			componentValue.AddMember("bpm", bpm, allocator);

			SerializeObjType("timeSig", componentValue, allocator, "top",
				component.timeSig.first, "bot", component.timeSig.second);
			//rapidjson::Value timeSig(rapidjson::kObjectType);
			//timeSig.AddMember("top", component.timeSig.first, allocator);
			//timeSig.AddMember("bot", component.timeSig.second, allocator);
			//componentValue.AddMember("timeSig", timeSig, allocator);

			rapidjson::Value playOnAwake;
			playOnAwake.SetBool(component.playOnAwake);
			componentValue.AddMember("playOnAwake", playOnAwake, allocator);

			rapidjson::Value grp;
			grp.SetInt(component.grp);
			componentValue.AddMember("grp", grp, allocator);

			objectValue.AddMember("audio", componentValue, allocator);

			return;
		}
		else if constexpr (std::is_same_v<T, PrefabInstanceComponent>)
		{
			rapidjson::Value prefabName;
			prefabName.SetString(component.prefabName.c_str(), static_cast<rapidjson::SizeType>(component.prefabName.length()), allocator);
			componentValue.AddMember("prefab_name", prefabName, allocator);

			rapidjson::Value syncPrefab;
			syncPrefab.SetBool(component.syncWithPrefab);
			componentValue.AddMember("sync_prefab", syncPrefab, allocator);

			objectValue.AddMember("prefab_instance", componentValue, allocator);

			return;
		}
		else if constexpr (std::is_same_v<T, ParentComponent>)
		{
			rapidjson::Value boolVal;
			boolVal.SetBool(component.inheritActive);
			componentValue.AddMember("inheritActive", boolVal, allocator);

			boolVal.SetBool(component.inheritTransform);
			componentValue.AddMember("inheritTransform", boolVal, allocator);

			// Serialize parent entity ID
			componentValue.AddMember("parentId", component.parent.GetId(), allocator);

			objectValue.AddMember("parent", componentValue, allocator);

			return;
		}
		else if constexpr (std::is_same_v<T, PersistOnLoadComponent>)
		{
			component.Serialize(objectValue, componentValue, allocator);

			return;
		}
		else if constexpr (std::is_same_v<T, ParticleComponent>)
		{
			rapidjson::Value boolVal;
			boolVal.SetBool(component.isActive);
			componentValue.AddMember("isActive", boolVal, allocator);

			rapidjson::Value intVal;
			intVal.SetInt(component.renderOption);
			componentValue.AddMember("renderOption", intVal, allocator);

			boolVal.SetBool(component.usePhysics);
			componentValue.AddMember("usePhysics", boolVal, allocator);

			intVal.SetInt(static_cast<int>(component.countEmission));
			componentValue.AddMember("countEmission", intVal, allocator);

			rapidjson::Value floatVal;

			// Emitters
			rapidjson::Value emitters(rapidjson::kObjectType);
			for (size_t i{}; i < component.emission.size(); ++i)
			{
				if (!component.emission[i].active) continue;

				rapidjson::Value emitter(rapidjson::kObjectType);

				boolVal.SetBool(component.emission[i].loop);
				emitter.AddMember("loop", boolVal, allocator);

				floatVal.SetFloat(component.emission[i].elapsedTime);
				emitter.AddMember("elapsedTime", floatVal, allocator);

				floatVal.SetFloat(component.emission[i].rate);
				emitter.AddMember("rate", floatVal, allocator);

				floatVal.SetFloat(component.emission[i].tLoop);
				emitter.AddMember("tLoop", floatVal, allocator);

				floatVal.SetFloat(component.emission[i].minLifeTime);
				emitter.AddMember("minLifeTime", floatVal, allocator);

				floatVal.SetFloat(component.emission[i].maxLifeTime);
				emitter.AddMember("maxLifeTime", floatVal, allocator);

				floatVal.SetFloat(component.emission[i].minRot);
				emitter.AddMember("minRot", floatVal, allocator);

				floatVal.SetFloat(component.emission[i].maxRot);
				emitter.AddMember("maxRot", floatVal, allocator);

				floatVal.SetFloat(component.emission[i].minSpinVel);
				emitter.AddMember("minSpinVel", floatVal, allocator);

				floatVal.SetFloat(component.emission[i].maxSpinVel);
				emitter.AddMember("maxSpinVel", floatVal, allocator);

				floatVal.SetFloat(component.emission[i].minSpinAccel);
				emitter.AddMember("minSpinAccel", floatVal, allocator);

				floatVal.SetFloat(component.emission[i].maxSpinAccel);
				emitter.AddMember("maxSpinAccel", floatVal, allocator);

                floatVal.SetFloat(component.emission[i].minSpinDirAccel);
                emitter.AddMember("minSpinDirAccel", floatVal, allocator);

                floatVal.SetFloat(component.emission[i].maxSpinDirAccel);
                emitter.AddMember("maxSpinDirAccel", floatVal, allocator);

				rapidjson::Value minPos(rapidjson::kObjectType);
				minPos.AddMember("x", component.emission[i].minPos.x, allocator);
				minPos.AddMember("y", component.emission[i].minPos.y, allocator);
				emitter.AddMember("minPos", minPos, allocator);

				rapidjson::Value maxPos(rapidjson::kObjectType);
				maxPos.AddMember("x", component.emission[i].maxPos.x, allocator);
				maxPos.AddMember("y", component.emission[i].maxPos.y, allocator);
				emitter.AddMember("maxPos", maxPos, allocator);

				rapidjson::Value minScale(rapidjson::kObjectType);
				minScale.AddMember("x", component.emission[i].minScale.x, allocator);
				minScale.AddMember("y", component.emission[i].minScale.y, allocator);
				emitter.AddMember("minScale", minScale, allocator);

				rapidjson::Value maxScale(rapidjson::kObjectType);
				maxScale.AddMember("x", component.emission[i].maxScale.x, allocator);
				maxScale.AddMember("y", component.emission[i].maxScale.y, allocator);
				emitter.AddMember("maxScale", maxScale, allocator);

				rapidjson::Value minVel(rapidjson::kObjectType);
				minVel.AddMember("x", component.emission[i].minVel.x, allocator);
				minVel.AddMember("y", component.emission[i].minVel.y, allocator);
				emitter.AddMember("minVel", minVel, allocator);

				rapidjson::Value maxVel(rapidjson::kObjectType);
				maxVel.AddMember("x", component.emission[i].maxVel.x, allocator);
				maxVel.AddMember("y", component.emission[i].maxVel.y, allocator);
				emitter.AddMember("maxVel", maxVel, allocator);

				rapidjson::Value minScaleVel(rapidjson::kObjectType);
				minScaleVel.AddMember("x", component.emission[i].minScaleVel.x, allocator);
				minScaleVel.AddMember("y", component.emission[i].minScaleVel.y, allocator);
				emitter.AddMember("minScaleVel", minScaleVel, allocator);

				rapidjson::Value maxScaleVel(rapidjson::kObjectType);
				maxScaleVel.AddMember("x", component.emission[i].maxScaleVel.x, allocator);
				maxScaleVel.AddMember("y", component.emission[i].maxScaleVel.y, allocator);
				emitter.AddMember("maxScaleVel", maxScaleVel, allocator);

				rapidjson::Value minAccel(rapidjson::kObjectType);
				minAccel.AddMember("x", component.emission[i].minAccel.x, allocator);
				minAccel.AddMember("y", component.emission[i].minAccel.y, allocator);
				emitter.AddMember("minAccel", minAccel, allocator);

				rapidjson::Value maxAccel(rapidjson::kObjectType);
				maxAccel.AddMember("x", component.emission[i].maxAccel.x, allocator);
				maxAccel.AddMember("y", component.emission[i].maxAccel.y, allocator);
				emitter.AddMember("maxAccel", maxAccel, allocator);

				rapidjson::Value minScaleAccel(rapidjson::kObjectType);
				minScaleAccel.AddMember("x", component.emission[i].minScaleAccel.x, allocator);
				minScaleAccel.AddMember("y", component.emission[i].minScaleAccel.y, allocator);
				emitter.AddMember("minScaleAccel", minScaleAccel, allocator);

				rapidjson::Value maxScaleAccel(rapidjson::kObjectType);
				maxScaleAccel.AddMember("x", component.emission[i].maxScaleAccel.x, allocator);
				maxScaleAccel.AddMember("y", component.emission[i].maxScaleAccel.y, allocator);
				emitter.AddMember("maxScaleAccel", maxScaleAccel, allocator);

				rapidjson::Value minDirAccel(rapidjson::kObjectType);
				minDirAccel.AddMember("x", component.emission[i].minDirAccel.x, allocator);
				minDirAccel.AddMember("y", component.emission[i].minDirAccel.y, allocator);
				emitter.AddMember("minDirAccel", minDirAccel, allocator);

                rapidjson::Value maxDirAccel(rapidjson::kObjectType);
                maxDirAccel.AddMember("x", component.emission[i].maxDirAccel.x, allocator);
                maxDirAccel.AddMember("y", component.emission[i].maxDirAccel.y, allocator);
                emitter.AddMember("maxDirAccel", maxDirAccel, allocator);

                rapidjson::Value minScaleDirAccel(rapidjson::kObjectType);
                minScaleDirAccel.AddMember("x", component.emission[i].minScaleDirAccel.x, allocator);
                minScaleDirAccel.AddMember("y", component.emission[i].minScaleDirAccel.y, allocator);
                emitter.AddMember("minScaleDirAccel", minScaleDirAccel, allocator);

                rapidjson::Value maxScaleDirAccel(rapidjson::kObjectType);
                maxScaleDirAccel.AddMember("x", component.emission[i].maxScaleDirAccel.x, allocator);
                maxScaleDirAccel.AddMember("y", component.emission[i].maxScaleDirAccel.y, allocator);
                emitter.AddMember("maxScaleDirAccel", maxScaleDirAccel, allocator);

				rapidjson::Value minCol(rapidjson::kObjectType);
				minCol.AddMember("r", component.emission[i].minCol.r, allocator);
				minCol.AddMember("g", component.emission[i].minCol.g, allocator);
				minCol.AddMember("b", component.emission[i].minCol.b, allocator);
				minCol.AddMember("a", component.emission[i].minCol.a, allocator);
				emitter.AddMember("minCol", minCol, allocator);

				rapidjson::Value maxCol(rapidjson::kObjectType);
				maxCol.AddMember("r", component.emission[i].maxCol.r, allocator);
				maxCol.AddMember("g", component.emission[i].maxCol.g, allocator);
				maxCol.AddMember("b", component.emission[i].maxCol.b, allocator);
				maxCol.AddMember("a", component.emission[i].maxCol.a, allocator);
				emitter.AddMember("maxCol", maxCol, allocator);

				rapidjson::Value minColVel(rapidjson::kObjectType);
				minColVel.AddMember("r", component.emission[i].minColVel.r, allocator);
				minColVel.AddMember("g", component.emission[i].minColVel.g, allocator);
				minColVel.AddMember("b", component.emission[i].minColVel.b, allocator);
				minColVel.AddMember("a", component.emission[i].minColVel.a, allocator);
				emitter.AddMember("minColVel", minColVel, allocator);

				rapidjson::Value maxColVel(rapidjson::kObjectType);
				maxColVel.AddMember("r", component.emission[i].maxColVel.r, allocator);
				maxColVel.AddMember("g", component.emission[i].maxColVel.g, allocator);
				maxColVel.AddMember("b", component.emission[i].maxColVel.b, allocator);
				maxColVel.AddMember("a", component.emission[i].maxColVel.a, allocator);
				emitter.AddMember("maxColVel", maxColVel, allocator);

				rapidjson::Value minColAccel(rapidjson::kObjectType);
				minColAccel.AddMember("r", component.emission[i].minColAccel.r, allocator);
				minColAccel.AddMember("g", component.emission[i].minColAccel.g, allocator);
				minColAccel.AddMember("b", component.emission[i].minColAccel.b, allocator);
				minColAccel.AddMember("a", component.emission[i].minColAccel.a, allocator);
				emitter.AddMember("minColAccel", minColAccel, allocator);

				rapidjson::Value maxColAccel(rapidjson::kObjectType);
				maxColAccel.AddMember("r", component.emission[i].maxColAccel.r, allocator);
				maxColAccel.AddMember("g", component.emission[i].maxColAccel.g, allocator);
				maxColAccel.AddMember("b", component.emission[i].maxColAccel.b, allocator);
				maxColAccel.AddMember("a", component.emission[i].maxColAccel.a, allocator);
				emitter.AddMember("maxColAccel", maxColAccel, allocator);

				rapidjson::Value minColDirAccel(rapidjson::kObjectType);
                minColDirAccel.AddMember("r", component.emission[i].minColDirAccel.r, allocator);
                minColDirAccel.AddMember("g", component.emission[i].minColDirAccel.g, allocator);
                minColDirAccel.AddMember("b", component.emission[i].minColDirAccel.b, allocator);
                minColDirAccel.AddMember("a", component.emission[i].minColDirAccel.a, allocator);
                emitter.AddMember("minColDirAccel", minColDirAccel, allocator);

                rapidjson::Value maxColDirAccel(rapidjson::kObjectType);
                maxColDirAccel.AddMember("r", component.emission[i].maxColDirAccel.r, allocator);
                maxColDirAccel.AddMember("g", component.emission[i].maxColDirAccel.g, allocator);
                maxColDirAccel.AddMember("b", component.emission[i].maxColDirAccel.b, allocator);
                maxColDirAccel.AddMember("a", component.emission[i].maxColDirAccel.a, allocator);
                emitter.AddMember("maxColDirAccel", maxColDirAccel, allocator);

				rapidjson::Value id;
				id.SetString(std::to_string(i).c_str(), static_cast<rapidjson::SizeType>(std::to_string(i).length()), allocator);
				emitters.AddMember(id, emitter, allocator);
			}
			componentValue.AddMember("emitters", emitters, allocator);

			objectValue.AddMember("particles", componentValue, allocator);

			return;
		}

		else Logger::Error("Unsupported component type for serialization");
	}

	template <typename T, typename ObjectType>
	void Serializer::DeserializeComponent(ObjectType& object, const rapidjson::Value& componentVal)
	{
		// add components here+
		if constexpr (std::is_same_v<T, ActiveComponent>)
		{
			bool isActive;

			if (GetConfiguration().safeMode)
			{
				if (!componentVal.IsObject())
				{
					Logger::Error("Invalid active component structure");
					return;
				}

				if (!componentVal.HasMember("isActive"))
				{
					isActive = true;
					Logger::Warning("Invalid structure, using defaults");
				}
				else isActive = componentVal["isActive"].GetBool();
			}
			else
			{
				isActive = componentVal["isActive"].GetBool();
			}

			if constexpr (std::is_same_v<ObjectType, Entity>)
			{
				// Update existing active component instead of adding new one
				auto& active = object.GetComponent<ActiveComponent>();
				active.isActive = isActive;
			}
			else if constexpr (std::is_same_v<ObjectType, Prefab>)
			{
				object.AddComponent<ActiveComponent>(isActive);
			}

			return;
		}

		else if constexpr (std::is_same_v<T, TransformComponent>)
		{
			PopplioMath::Vec2f position, scale;
			double rot;

			if (GetConfiguration().safeMode)
			{
				if (!componentVal.IsObject())
				{
					Logger::Error("Invalid transform component structure");
					return;
				}

				if (!componentVal.HasMember("position"))
				{
					position = { 0.0f, 0.0f };
					Logger::Warning("Invalid structure, using defaults");
				}
				else position = { componentVal["position"]["x"].GetFloat(),
					componentVal["position"]["y"].GetFloat() };

				if (!componentVal.HasMember("scale"))
				{
					scale = { 1.0f, 1.0f };
					Logger::Warning("Invalid structure, using defaults");
				}
				else scale = { componentVal["scale"]["x"].GetFloat(),
					componentVal["scale"]["y"].GetFloat() };

				if (!componentVal.HasMember("rotation"))
				{
					rot = 0.0;
					Logger::Warning("Invalid structure, using defaults");
				}
				else rot = componentVal["rotation"].GetDouble();
			}
			else
			{
				position = { componentVal["position"]["x"].GetFloat(),
					componentVal["position"]["y"].GetFloat() };
				scale = { componentVal["scale"]["x"].GetFloat(),
					componentVal["scale"]["y"].GetFloat() };
				rot = componentVal["rotation"].GetDouble();
			}

			if constexpr (std::is_same_v<ObjectType, Entity>)
			{
				// Update existing transform component instead of adding new one
				auto& transform = object.GetComponent<TransformComponent>();
				transform.position = PopplioMath::Vec2f(position.x, position.y);
				transform.scale = PopplioMath::Vec2f(scale.x, scale.y);
				transform.rotation = rot;
			}
			else if constexpr (std::is_same_v<ObjectType, Prefab>)
			{
				object.AddComponent<TransformComponent>(position.x, position.y, scale.x, scale.y, rot);
			}

			return;
		}

		else if constexpr (std::is_same_v<T, RigidBodyComponent>)
		{
			bool isActive, useGravity;

			float mass, gravityScale, vel_x, vel_y, dragCoefficient;

			if (GetConfiguration().safeMode)
			{
				if (!componentVal.IsObject())
				{
					Logger::Error("Invalid rigidbody component structure");
					return;
				}

				if (!componentVal.HasMember("isActive"))
				{
					isActive = true;
					Logger::Warning("Invalid structure, using defaults");
				}
				else isActive = componentVal["isActive"].GetBool();

				if (!componentVal.HasMember("velocity"))
				{
					vel_x = 0.0f;
					vel_y = 0.0f;
					Logger::Warning("Invalid structure, using defaults");
				}
				else
				{
					vel_x = componentVal["velocity"]["x"].GetFloat();
					vel_y = componentVal["velocity"]["y"].GetFloat();
				}

				if (!componentVal.HasMember("acceleration"))
				{
					// ???
					Logger::Warning("Invalid structure, using defaults");
				}
				if (!componentVal.HasMember("force"))
				{
					// ???
					Logger::Warning("Invalid structure, using defaults");
				}
				if (!componentVal.HasMember("mass"))
				{
					mass = 1.0f;
					Logger::Warning("Invalid structure, using defaults");
				}
				else mass = componentVal["mass"].GetFloat();

				if (!componentVal.HasMember("useGravity"))
				{
					useGravity = false;
					Logger::Warning("Invalid structure, using defaults");
				}
				else useGravity = componentVal["useGravity"].GetBool();

				if (!componentVal.HasMember("gravityScale"))
				{
					gravityScale = 1.0f;
					Logger::Warning("Invalid structure, using defaults");
				}
				else gravityScale = componentVal["gravityScale"].GetFloat();

				if (!componentVal.HasMember("dragCoefficient"))
				{
					dragCoefficient = 0.0f;
					Logger::Warning("Invalid structure, using defaults");
				}
				else dragCoefficient = componentVal["dragCoefficient"].GetFloat();
			}
			else
			{
				//read all the values from JSON
				isActive = componentVal["isActive"].GetBool();

				mass = componentVal["mass"].GetFloat();
				useGravity = componentVal["useGravity"].GetBool();
				gravityScale = componentVal["gravityScale"].GetFloat();
				vel_x = componentVal["velocity"]["x"].GetFloat();
				vel_y = componentVal["velocity"]["y"].GetFloat();
				dragCoefficient = componentVal["dragCoefficient"].GetFloat();
			}

			object.AddComponent<RigidBodyComponent>(
				mass,
				useGravity,
				gravityScale,
				vel_x,
				vel_y,
				dragCoefficient
			);

			object.GetComponent<RigidBodyComponent>().isActive = isActive;
		}

		else if constexpr (std::is_same_v<T, RenderComponent>)
		{
			bool isActive, useInstancing;

			MeshType mesh;
			std::string shaderName, textureName;
			Color color;
			float alpha;
			int textureWidth, textureHeight; // Initialize width and height

			bool enableNineSlice;
			float borderLeft, borderRight, borderTop, borderBottom;

			if (GetConfiguration().safeMode)
			{
				if (!componentVal.IsObject())
				{
					Logger::Error("Invalid render component structure");
					return;
				}

				if (!componentVal.HasMember("isActive"))
				{
					isActive = true;
					Logger::Warning("Invalid structure, using defaults");
				}
				else isActive = componentVal["isActive"].GetBool();

				if (!componentVal.HasMember("mesh"))
				{
					mesh = MeshType::Quad;
					Logger::Warning("Invalid structure, using defaults");
				}
				else mesh = static_cast<MeshType>(componentVal["mesh"].GetInt());

				if (!componentVal.HasMember("shader_name"))
				{
					shaderName = "";
					Logger::Warning("Invalid structure, using defaults");
				}
				else shaderName = componentVal["shader_name"].GetString();

				if (!componentVal.HasMember("texture_name"))
				{
					textureName = "";
					Logger::Warning("Invalid structure, using defaults");
				}
				else textureName = componentVal["texture_name"].GetString();

				if (!componentVal.HasMember("use_instancing"))
				{
					useInstancing = false;
					Logger::Warning("Invalid structure, using defaults");
				}
				else useInstancing = componentVal["use_instancing"].GetBool();

				if (!componentVal.HasMember("color"))
				{
					color = { 1.0f, 1.0f, 1.0f };
					Logger::Warning("Invalid structure, using defaults");
				}
				else
				{
					color = { componentVal["color"]["r"].GetFloat(),
						componentVal["color"]["g"].GetFloat(),
						componentVal["color"]["b"].GetFloat() };
				}

				if (!componentVal.HasMember("alpha"))
				{
					alpha = 1.0f;
					Logger::Warning("Invalid alpha structure, using defaults");
				}
				else alpha = componentVal["alpha"].GetFloat();

				//  Load 9-Slice Scaling Properties Safely
				if (!componentVal.HasMember("enable_nine_slice"))
				{
					enableNineSlice = false;
					Logger::Warning("Invalid structure, using defaults");
				}
				else enableNineSlice = componentVal["enable_nine_slice"].GetBool();

				if (!componentVal.HasMember("nine_slice_borders"))
				{
					borderLeft = 0.0f;
					borderRight = 0.0f;
					borderTop = 0.0f;
					borderBottom = 0.0f;
					Logger::Warning("Invalid nine slice border structure, using defaults");
				}
				else
				{
					const auto& borderVals = componentVal["nine_slice_borders"];
					if (borderVals.HasMember("left")) borderLeft = borderVals["left"].GetFloat();
					if (borderVals.HasMember("right")) borderRight = borderVals["right"].GetFloat();
					if (borderVals.HasMember("top")) borderTop = borderVals["top"].GetFloat();
					if (borderVals.HasMember("bottom")) borderBottom = borderVals["bottom"].GetFloat();
				}
			}
			else
			{
				isActive = componentVal["isActive"].GetBool();

				mesh = static_cast<MeshType>(componentVal["mesh"].GetInt());
				shaderName = componentVal["shader_name"].GetString();
				textureName = componentVal["texture_name"].GetString();
				useInstancing = componentVal["use_instancing"].GetBool();
				color = { componentVal["color"]["r"].GetFloat(),
					componentVal["color"]["g"].GetFloat(), componentVal["color"]["b"].GetFloat() };
				alpha = componentVal["alpha"].GetFloat();

				// Load 9-Slice Scaling Properties in Unsafe Mode
				enableNineSlice = componentVal["enable_nine_slice"].GetBool();
				const auto& borderVals = componentVal["nine_slice_borders"];
				borderLeft = borderVals["left"].GetFloat();
				borderRight = borderVals["right"].GetFloat();
				borderTop = borderVals["top"].GetFloat();
				borderBottom = borderVals["bottom"].GetFloat();
			}

			// Retrieve texture dimensions if a texture is assigned
			if (!textureName.empty())
			{
				auto [width, height] = Popplio::AssetStore::GetTextureSize(textureName);
				textureWidth = width;
				textureHeight = height;
			}

			object.AddComponent<RenderComponent>(mesh, shaderName, textureName, useInstancing, color.ToGLMVec3(), alpha, textureWidth, textureHeight
				, enableNineSlice, borderLeft, borderRight, borderTop, borderBottom);
			auto& renderComp = object.GetComponent<RenderComponent>();
			renderComp.isActive = isActive;
		}

		else if constexpr (std::is_same_v<T, BoxColliderComponent>)
		{
			bool isActive, isTrigger, isEnabled;
			PopplioMath::Vec2f size, offset;

			if (GetConfiguration().safeMode)
			{
				if (!componentVal.IsObject())
				{
					Logger::Error("Invalid box collider component structure");
					return;
				}

				if (!componentVal.HasMember("isActive"))
				{
					isActive = true;
					Logger::Warning("Invalid structure, using defaults");
				}
				else isActive = componentVal["isActive"].GetBool();

				if (!componentVal.HasMember("size"))
				{
					size = { 1.0f, 1.0f };
					Logger::Warning("Invalid structure, using defaults");
				}
				else size = { componentVal["size"]["x"].GetFloat(),
					componentVal["size"]["y"].GetFloat() };

				if (!componentVal.HasMember("offset"))
				{
					offset = { 0.0f, 0.0f };
					Logger::Warning("Invalid structure, using defaults");
				}
				else offset = { componentVal["offset"]["x"].GetFloat(),
					componentVal["offset"]["y"].GetFloat() };

				if (!componentVal.HasMember("isTrigger"))
				{
					isTrigger = false;
					Logger::Warning("Invalid structure, using defaults");
				}
				else isTrigger = componentVal["isTrigger"].GetBool();

				if (!componentVal.HasMember("isEnabled"))
				{
					isEnabled = true;
					Logger::Warning("Invalid structure, using defaults");
				}
				else isEnabled = componentVal["isEnabled"].GetBool();
			}
			else
			{
				isActive = componentVal["isActive"].GetBool();
				size = { componentVal["size"]["x"].GetFloat(),
					componentVal["size"]["y"].GetFloat() };
				offset = { componentVal["offset"]["x"].GetFloat(),
					componentVal["offset"]["y"].GetFloat() };
				isTrigger = componentVal["isTrigger"].GetBool();
				isEnabled = componentVal["isEnabled"].GetBool();
			}

			object.AddComponent<BoxColliderComponent>(size.x, size.y, offset.x, offset.y, isTrigger, isEnabled);
			object.GetComponent<BoxColliderComponent>().isActive = isActive;

			return;

			//entity.AddComponent<BoxColliderComponent>(size.X(), size.Y(), offset.X(), offset.Y());
		}

		else if constexpr (std::is_same_v<T, CircleColliderComponent>)
		{
			const bool isActive, isTrigger, isEnabled;
			float radius;
			PopplioMath::Vec2f offset;

			if (GetConfiguration().safeMode)
			{
				if (!componentVal.IsObject())
				{
					Logger::Error("Invalid circle collider component structure");
					return;
				}

				if (!componentVal.HasMember("isActive"))
				{
					isActive = true;
					Logger::Warning("Invalid structure, using defaults");
				}
				else isActive = componentVal["isActive"].GetBool();

				if (!componentVal.HasMember("radius"))
				{
					radius = 1.0f;
					Logger::Warning("Invalid structure, using defaults");
				}
				else radius = componentVal["radius"].GetFloat();

				if (!componentVal.HasMember("offset"))
				{
					offset = { 0.0f, 0.0f };
					Logger::Warning("Invalid structure, using defaults");
				}
				else offset = { componentVal["offset"]["x"].GetFloat(),
					componentVal["offset"]["y"].GetFloat() };

				if (!componentVal.HasMember("isTrigger"))
				{
					isTrigger = false;
					Logger::Warning("Invalid structure, using defaults");
				}
				else isTrigger = componentVal["isTrigger"].GetBool();

				if (!componentVal.HasMember("isEnabled"))
				{
					isEnabled = true;
					Logger::Warning("Invalid structure, using defaults");
				}
				else isEnabled = componentVal["isEnabled"].GetBool();
			}
			else
			{
				isActive = componentVal["isActive"].GetBool();
				radius = componentVal["radius"].GetFloat();
				offset = { componentVal["offset"]["x"].GetFloat(),
					componentVal["offset"]["y"].GetFloat() };
				isTrigger = componentVal["isTrigger"].GetBool();
				isEnabled = componentVal["isEnabled"].GetBool();
			}

			object.AddComponent<CircleColliderComponent>(radius, offset.x, offset.y, isTrigger, isEnabled);
			object.GetComponent<CircleColliderComponent>().isActive = isActive;

			return;
		}

		else if constexpr (std::is_same_v<T, TextComponent>)
		{
			bool isActive;
			std::string text, fontName;
			float alpha, r, g, b;
			unsigned int size;

			if (GetConfiguration().safeMode)
			{
				if (!componentVal.IsObject())
				{
					Logger::Error("Invalid text component structure");
					return;
				}

				if (!componentVal.HasMember("isActive"))
				{
					isActive = true;
					Logger::Warning("Invalid structure, using defaults");
				}
				else isActive = componentVal["isActive"].GetBool();

				if (!componentVal.HasMember("text"))
				{
					text = "";
					Logger::Warning("Invalid structure, using defaults");
				}
				else text = componentVal["text"].GetString();

				if (!componentVal.HasMember("font_name"))
				{
					fontName = "";
					Logger::Warning("Invalid structure, using defaults");
				}
				else fontName = componentVal["font_name"].GetString();

				if (!componentVal.HasMember("size"))
				{
					size = 32;
					Logger::Warning("Invalid structure, using defaults");
				}
				else size = static_cast<unsigned int>(componentVal["size"].GetInt());

				if (!componentVal.HasMember("color"))
				{
					r = 1.0f;
					g = 1.0f;
					b = 1.0f;
					Logger::Warning("Invalid structure, using defaults");
				}
				else
				{
					r = componentVal["color"]["r"].GetFloat();
					g = componentVal["color"]["g"].GetFloat();
					b = componentVal["color"]["b"].GetFloat();
				}

				if (!componentVal.HasMember("alpha"))
				{
					alpha = 1.0f;
					Logger::Warning("Invalid structure, using defaults");
				}
				else alpha = componentVal["alpha"].GetFloat();

			}
			else
			{
				isActive = componentVal["isActive"].GetBool();
				text = componentVal["text"].GetString();
				fontName = componentVal["font_name"].GetString();
				r = componentVal["color"]["r"].GetFloat();
				g = componentVal["color"]["g"].GetFloat();
				b = componentVal["color"]["b"].GetFloat();
				alpha = componentVal["alpha"].GetFloat();
				size = static_cast<unsigned int>(componentVal["size"].GetInt());
			}

			object.AddComponent<TextComponent>(text, fontName, size, alpha);
			object.GetComponent<TextComponent>().isActive = isActive;
			//object.GetComponent<TextComponent>().fontSize = size;

			object.GetComponent<TextComponent>().color.x = r;
			object.GetComponent<TextComponent>().color.y = g;
			object.GetComponent<TextComponent>().color.z = b;

			return;
		}

		else if constexpr (std::is_same_v<T, AnimationComponent>)
		{
			bool isActive, playOnce;
			float sheetWidth, sheetHeight;

			if (GetConfiguration().safeMode)
			{
				if (!componentVal.IsObject())
				{
					Logger::Error("Invalid animation component structure");
					return;
				}

				if (!componentVal.HasMember("isActive"))
				{
					isActive = true;
					Logger::Warning("Invalid structure, using defaults");
				}
				else isActive = componentVal["isActive"].GetBool();

				if (!componentVal.HasMember("sprite_sheet_size"))
				{
					sheetWidth = 1.0f;
					sheetHeight = 1.0f;
					Logger::Warning("Invalid structure, using defaults");
				}
				else
				{
					sheetWidth = componentVal["sprite_sheet_size"]["width"].GetFloat();
					sheetHeight = componentVal["sprite_sheet_size"]["height"].GetFloat();
				}

				if (!componentVal.HasMember("playOnce"))
				{
					playOnce = true;
					Logger::Warning("Invalid structure, using defaults");
				}
				else playOnce = componentVal["playOnce"].GetBool();
			}
			else
			{
				isActive = componentVal["isActive"].GetBool();

				// Get basic animation properties
				sheetWidth = componentVal["sprite_sheet_size"]["width"].GetFloat();
				sheetHeight = componentVal["sprite_sheet_size"]["height"].GetFloat();

				playOnce = componentVal["playOnce"].GetBool();
			}

			// Add component first
			object.AddComponent<AnimationComponent>();
			object.GetComponent<AnimationComponent>().isActive = isActive;
			object.GetComponent<AnimationComponent>().playOnce = playOnce;

			// Get reference to component and initialize it
			auto& animComp = object.GetComponent<AnimationComponent>();
			animComp.Initialize(sheetWidth, sheetHeight);

			// check
			if (GetConfiguration().safeMode)
			{
				if (!componentVal.HasMember("animation_state"))
				{
					Logger::Warning("Invalid structure, using defaults"); // no animations
					return;
				}
			}

			// Parse all animations
			const auto& animations = componentVal["animation_state"].GetObject();
			for (auto it = animations.MemberBegin(); it != animations.MemberEnd(); ++it) {
				const std::string& animName = it->name.GetString();
				const auto& animData = it->value;

				if (!animData.HasMember("start_frame") || !animData.HasMember("end_frame") ||
					!animData.HasMember("animation_speed") || !animData.HasMember("frame_size"))
				{
					Logger::Warning("Animation '" + animName + "' has invalid structure");
					continue;
				}

				// Start and End frame
				int startX = animData["start_frame"]["x"].GetInt();
				int startY = animData["start_frame"]["y"].GetInt();
				int endX = animData["end_frame"]["x"].GetInt();
				int endY = animData["end_frame"]["y"].GetInt();

				//Frame size
				float frameWidth = animData["frame_size"]["width"].GetFloat();
				float frameHeight = animData["frame_size"]["height"].GetFloat();

				float speed = animData["animation_speed"].GetFloat();

				animComp.AddAnimation(animName, startX, startY, endX, endY, speed, frameWidth, frameHeight);
			}

			return;
		}

		else if constexpr (std::is_same_v<T, UIComponent>)
		{
			bool isActive;
			UIType type = UIType::BUTTON;
			bool checked;
			float sliderValue;
			int linkedEntityId;
			std::vector<std::string> onClickActions;

			// Spritesheet properties
			int frameX, frameY;
			int hoverFrameX, hoverFrameY;

			int totalColumns;
			int totalRows;

			float sheetWidth;
			float sheetHeight;

			bool hasHoverEffect;

			if (GetConfiguration().safeMode)
			{
				if (!componentVal.IsObject())
				{
					Logger::Error("Invalid UI component structure");
					return;
				}

				if (!componentVal.HasMember("isActive"))
				{
					isActive = true;
					Logger::Warning("Invalid structure, using defaults");
				}
				else isActive = componentVal["isActive"].GetBool();

				if (componentVal.HasMember("uiType") && componentVal["uiType"].IsString()) {
					std::string uiTypeStr = componentVal["uiType"].GetString();
					if (uiTypeStr == "BUTTON") type = UIType::BUTTON;
					else if (uiTypeStr == "CHECKBOX") type = UIType::CHECKBOX;
					else if (uiTypeStr == "SLIDER_TRACK") type = UIType::SLIDER_TRACK;
					else if (uiTypeStr == "SLIDER_HANDLE") type = UIType::SLIDER_HANDLE;
					else Logger::Warning("Unknown UI type: " + uiTypeStr);
				}
				else
				{
					type = UIType::BUTTON;
					Logger::Warning("Invalid UItype structure, using defaults");
				}

				//if (type == UIType::CHECKBOX && !componentVal.HasMember("checked"))
				//{
				//	checked = false;
				//	Logger::Warning("Invalid checked structure, using defaults");
				//}
				//else if (type != UIType::CHECKBOX)
				//else checked = componentVal["checked"].GetBool();

				if (type == UIType::CHECKBOX)
				{
					if (!componentVal.HasMember("checked"))
					{
						Logger::Warning("Invalid checked structure, using defaults");
						checked = false;
					}
					else
						checked = componentVal["checked"].GetBool();
				}
				else
				{
					checked = false;
				}

				if (type == UIType::SLIDER_TRACK)
				{
					if (!componentVal.HasMember("sliderValue"))
					{
						sliderValue = 0.5f;
						Logger::Warning("Invalid sliderValue structure, using defaults");
					}
					else sliderValue = componentVal["sliderValue"].GetFloat();
				}

				if (type == UIType::SLIDER_HANDLE || type == UIType::SLIDER_TRACK)
				{
					if (!componentVal.HasMember("linkedEntityId"))
					{
						linkedEntityId = -1;
						Logger::Warning("Invalid linkedEntityId structure, using defaults");
					}
					else linkedEntityId = componentVal["linkedEntityId"].GetInt();
				}

				// Deserialize OnClick actions
				if (type == UIType::BUTTON || type == UIType::CHECKBOX)
				{
					if (componentVal.HasMember("onClickActions") && componentVal["onClickActions"].IsArray())
					{
						for (const auto& actionValue : componentVal["onClickActions"].GetArray())
						{
							if (actionValue.IsString())
							{
								onClickActions.push_back(actionValue.GetString());
							}
						}
					}

					if (onClickActions.empty())
					{
						onClickActions.push_back("None");
						Logger::Warning("[UIComponent] No OnClickActions found. Defaulting to 'None'.");
					}
				}

				// Deserialize hover effect
				if (!componentVal.HasMember("hasHoverEffect")) {
					hasHoverEffect = false;
					Logger::Warning("Invalid hasHoverEffect structure, using defaults");
				}
				else hasHoverEffect = componentVal["hasHoverEffect"].GetBool();

				if (hasHoverEffect)
				{
					// Deserialize spritesheet properties if they exist
					if (!componentVal.HasMember("frameX"))
					{
						frameX = 0;
						Logger::Warning("Invalid frameX structure, using defaults");
					}
					else frameX = componentVal["frameX"].GetInt();

					if (!componentVal.HasMember("frameY"))
					{
						frameY = 0;
						Logger::Warning("Invalid frameY structure, using defaults");
					}
					else frameY = componentVal["frameY"].GetInt();

					if (!componentVal.HasMember("hoverFrameX"))
					{
						hoverFrameX = 0;
						Logger::Warning("Invalid hoverFrameX structure, using defaults");
					}
					else hoverFrameX = componentVal["hoverFrameX"].GetInt();

					if (!componentVal.HasMember("hoverFrameY"))
					{
						hoverFrameY = 0;
						Logger::Warning("Invalid hoverFrameY structure, using defaults");
					}
					else hoverFrameY = componentVal["hoverFrameY"].GetInt();

					if (!componentVal.HasMember("totalColumns"))
					{
						totalColumns = 1;
						Logger::Warning("Invalid totalColumns structure, using defaults");
					}
					else totalColumns = componentVal["totalColumns"].GetInt();

					if (!componentVal.HasMember("totalRows"))
					{
						totalRows = 1;
						Logger::Warning("Invalid totalRows structure, using defaults");
					}
					else totalRows = componentVal["totalRows"].GetInt();

					if (!componentVal.HasMember("sheetWidth"))
					{
						sheetWidth = 0.0f;
						Logger::Warning("Invalid sheetWidth structure, using defaults");
					}
					else sheetWidth = componentVal["sheetWidth"].GetFloat();

					if (!componentVal.HasMember("sheetHeight"))
					{
						sheetHeight = 0.0f;
						Logger::Warning("Invalid sheetHeight structure, using defaults");
					}
					else sheetHeight = componentVal["sheetHeight"].GetFloat();
				}
			}
			else // Non-Safe Mode
			{
				isActive = componentVal["isActive"].GetBool();
				std::string uiTypeStr = componentVal["uiType"].GetString();

				if (uiTypeStr == "BUTTON") type = UIType::BUTTON;
				else if (uiTypeStr == "CHECKBOX") type = UIType::CHECKBOX;
				else if (uiTypeStr == "SLIDER_TRACK") type = UIType::SLIDER_TRACK;
				else if (uiTypeStr == "SLIDER_HANDLE") type = UIType::SLIDER_HANDLE;

				if (type == UIType::CHECKBOX) {
					checked = componentVal["checked"].GetBool();
				}

				if (type == UIType::SLIDER_HANDLE) {
					sliderValue = componentVal["sliderValue"].GetFloat();
				}

				if (type == UIType::SLIDER_TRACK || type == UIType::SLIDER_HANDLE) {
					linkedEntityId = componentVal["linkedEntityId"].GetInt();
				}

				if ((type == UIType::BUTTON || type == UIType::CHECKBOX) &&
					componentVal.HasMember("onClickActions") && componentVal["onClickActions"].IsArray())
				{
					for (const auto& actionValue : componentVal["onClickActions"].GetArray())
					{
						onClickActions.push_back(actionValue.GetString());
					}
				}

				// Deserialize hover effect (without checks)
				hasHoverEffect = componentVal["hasHoverEffect"].GetBool();

				if (hasHoverEffect)
				{
					frameX = componentVal["frameX"].GetInt();
					frameY = componentVal["frameY"].GetInt();
					hoverFrameX = componentVal["hoverFrameX"].GetInt();
					hoverFrameY = componentVal["hoverFrameY"].GetInt();
					totalColumns = componentVal["totalColumns"].GetInt();
					totalRows = componentVal["totalRows"].GetInt();
					sheetWidth = componentVal["sheetWidth"].GetFloat();
					sheetHeight = componentVal["sheetHeight"].GetFloat();
				}
			}

			// Assign the values to UIComponent
			object.AddComponent<UIComponent>(
				type, isActive, checked, sliderValue, linkedEntityId,
				hasHoverEffect, frameX, frameY, hoverFrameX, hoverFrameY,
				totalColumns, totalRows, sheetWidth, sheetHeight, onClickActions);

			//auto& ui = object.GetComponent<UIComponent>();
			//ui.isActive = isActive;

			//ui.checked = checked;
			//ui.sliderValue = sliderValue;
			//ui.linkedEntityId = linkedEntityId;
			//ui.onClickActions = onClickActions;
			//ui.hasHoverEffect = hasHoverEffect;

			// Assign optional spritesheet data
			/*if (frameX) ui.frameX = frameX;
			if (frameY) ui.frameY = frameY;
			if (hoverFrameX) ui.hoverFrameX = hoverFrameX;
			if (hoverFrameY) ui.hoverFrameY = hoverFrameY;
			if (totalColumns) ui.totalColumns = totalColumns;
			if (totalRows) ui.totalRows = totalRows;
			if (sheetWidth) ui.sheetWidth = sheetWidth;
			if (sheetHeight) ui.sheetHeight = sheetHeight;*/

			return;
		}


		else if constexpr (std::is_same_v<T, LogicComponent>)
		{
			LogicComponent::Deserialize(componentVal, object, GetConfiguration().safeMode);

			//bool isActive;
			//std::string s;
			//ScriptLang lang;

			//if (GetConfiguration().safeMode)
			//{
		   //             if (!componentVal.IsObject())
		   //             {
		   //                 Logger::Error("Invalid logic component structure");
		   //                 return;
		   //             }

		   //             if (!componentVal.HasMember("isActive"))
		   //             {
		   //                 isActive = true;
		   //                 Logger::Warning("Invalid structure, using defaults");
		   //             }
		   //             else isActive = componentVal["isActive"].GetBool();

		   //             if (!componentVal.HasMember("name"))
		   //             {
		   //                 s = "";
		   //                 Logger::Warning("Invalid structure, using defaults");
		   //             }
		   //             else s = componentVal["name"].GetString();

			//	if (!componentVal.HasMember("scriptLang"))
			//	{
			//		lang = ScriptLang::UNASSIGNED;
			//		Logger::Warning("Invalid structure, using defaults");
			//	}
			//	else lang = static_cast<ScriptLang>(componentVal["scriptLang"].GetInt());
			//}
			//else
			//{
			//	isActive = componentVal["isActive"].GetBool();
			//	s = componentVal["name"].GetString();
			//	lang = static_cast<ScriptLang>(componentVal["scriptLang"].GetInt());
			//}

			////std::map<std::string, void*> vars{};

			////for (auto it = componentVal.MemberBegin(); it != componentVal.MemberEnd(); ++it) // deserialize variables
			////{
			////    if (it->name.GetString() == "name") continue;
			////    
			////    vars.emplace(static_cast<std::string>(it->name), it->value);
			////}

			////object.AddComponent<LogicComponent>(s, vars);

			//// TODO serialize script vars

			//object.AddComponent<LogicComponent>(s);
   //         object.GetComponent<LogicComponent>().isActive = isActive;
   //         object.GetComponent<LogicComponent>().scriptLang = lang;

			return;
		}

		else if constexpr (std::is_same_v<T, AudioComponent>)
		{
			bool isActive, loop, playOnAwake, mute;
			std::string s;
			float vol, bpm;
			std::pair<int, int> timeSig;
			int grp;

			if (GetConfiguration().safeMode)
			{
				if (!componentVal.IsObject())
				{
					Logger::Error("Invalid audio component structure");
					return;
				}

				if (!componentVal.HasMember("isActive"))
				{
					isActive = true;
					Logger::Warning("Invalid structure, using defaults");
				}
				else isActive = componentVal["isActive"].GetBool();

				if (!componentVal.HasMember("name"))
				{
					s = "";
					Logger::Warning("Invalid structure, using defaults");
				}
				else s = componentVal["name"].GetString();

				if (!componentVal.HasMember("volume"))
				{
					vol = 1.0f;
					Logger::Warning("Invalid structure, using defaults");
				}
				else vol = componentVal["volume"].GetFloat();

				if (!componentVal.HasMember("mute"))
				{
					mute = false;
					Logger::Warning("Invalid structure, using defaults");
				}
				else mute = componentVal["mute"].GetBool();

				if (!componentVal.HasMember("loop"))
				{
					loop = false;
					Logger::Warning("Invalid structure, using defaults");
				}
				else loop = componentVal["loop"].GetBool();

				if (!componentVal.HasMember("bpm"))
				{
					bpm = 120.0f;
					Logger::Warning("Invalid structure, using defaults");
				}
				else bpm = componentVal["bpm"].GetFloat();

				if (!componentVal.HasMember("timeSig"))
				{
					timeSig = { 4, 4 };
					Logger::Warning("Invalid structure, using defaults");
				}
				else timeSig = { componentVal["timeSig"]["top"].GetInt(),
					componentVal["timeSig"]["bot"].GetInt() };

				if (!componentVal.HasMember("playOnAwake"))
				{
					playOnAwake = false;
					Logger::Warning("Invalid structure, using defaults");
				}
				else playOnAwake = componentVal["playOnAwake"].GetBool();

				if (!componentVal.HasMember("grp"))
				{
					grp = 0;
					Logger::Warning("Invalid structure, using defaults");
				}
				else grp = componentVal["grp"].GetInt();
			}
			else
			{
				s = componentVal["name"].GetString();

				isActive = componentVal["isActive"].GetBool();

				vol = componentVal["volume"].GetFloat();
				mute = componentVal["mute"].GetBool();
				loop = componentVal["loop"].GetBool();
				bpm = componentVal["bpm"].GetFloat();
				timeSig = { componentVal["timeSig"]["top"].GetInt(),
					componentVal["timeSig"]["bot"].GetInt() };
				playOnAwake = componentVal["playOnAwake"].GetBool();
				grp = componentVal["grp"].GetInt();
			}

			object.AddComponent<AudioComponent>(s);
			object.GetComponent<AudioComponent>().isActive = isActive;

			object.GetComponent<AudioComponent>().volume = vol;
			object.GetComponent<AudioComponent>().mute = mute;
			object.GetComponent<AudioComponent>().loop = loop;
			object.GetComponent<AudioComponent>().bpm = bpm;
			object.GetComponent<AudioComponent>().timeSig = timeSig;
			object.GetComponent<AudioComponent>().playOnAwake = playOnAwake;
			object.GetComponent<AudioComponent>().grp = grp;

			if (s == "")
			{
				Logger::Info("Deserialize: AudioComponent - Empty string ID");
				object.GetComponent<AudioComponent>().data = -1;
				return;
			}

			object.GetComponent<AudioComponent>().Update(registry.GetSystem<AudioSystem>());

			//int index = registry.GetSystem<AudioSystem>().InstAudio(s, grp, bpm, timeSig);

			//object.GetComponent<AudioComponent>().data =
			//	&registry.GetSystem<AudioSystem>().GetAudioInst(index);

			return;
		}

		else if constexpr (std::is_same_v<T, PrefabInstanceComponent>)
		{
			std::string prefabName;
			bool syncWithPrefab;

			if (GetConfiguration().safeMode)
			{
				if (!componentVal.IsObject())
				{
					Logger::Error("Invalid layer component structure");
					return;
				}

				if (!componentVal.HasMember("prefab_name"))
				{
					prefabName = "";
					Logger::Warning("Invalid structure, using defaults");
				}
				else prefabName = componentVal["prefab_name"].GetString();

				if (!componentVal.HasMember("sync_prefab"))
				{
					syncWithPrefab = false;
					Logger::Warning("Invalid structure, using defaults");
				}
				else syncWithPrefab = componentVal["sync_prefab"].GetBool();
			}
			else
			{
				prefabName = componentVal["prefab_name"].GetString();
				syncWithPrefab = componentVal["sync_prefab"].GetBool();
			}
			object.AddComponent<PrefabInstanceComponent>(prefabName, syncWithPrefab);
			return;
		}

		else if constexpr (std::is_same_v<T, ParentComponent>)
		{
			bool inheritActive, inheritTransform;
			int parentId;

			if (GetConfiguration().safeMode)
			{
				if (!componentVal.IsObject())
				{
					Logger::Error("Invalid parent component structure");
					return;
				}

				if (!componentVal.HasMember("inheritActive"))
				{
					inheritActive = true;
					Logger::Warning("Invalid structure, using defaults");
				}
				else inheritActive = componentVal["inheritActive"].GetBool();

				if (!componentVal.HasMember("inheritTransform"))
				{
					inheritTransform = true;
					Logger::Warning("Invalid structure, using defaults");
				}
				else inheritTransform = componentVal["inheritTransform"].GetBool();

				if (!componentVal.HasMember("parentId"))
				{
					parentId = -1;
					Logger::Warning("Invalid structure, using defaults");
				}
				else parentId = componentVal["parentId"].GetInt();
			}
			else
			{
				inheritActive = componentVal["inheritActive"].GetBool();
				inheritTransform = componentVal["inheritTransform"].GetBool();
				parentId = componentVal["parentId"].GetInt();
			}

			// Create parent entity reference
			Entity parent(parentId);
			if (parentId != -1)
			{
				parent.registry = &registry;  // Set the registry reference
			}

			// Add component with just parent reference and flags
			object.AddComponent<ParentComponent>(parent, inheritActive, inheritTransform);

			//auto update = [](Entity obj, Entity par)
			//{
			//	obj.GetComponent<ParentComponent>().UpdateLocalTransform(
			//		obj.GetComponent<TransformComponent>(), par.GetComponent<TransformComponent>());
			//};

   //         // Update local transform
   //         registry.GetSystem<QueueSystem>().Add(std::bind(update, object, parent));

			return;
		}

		else if constexpr (std::is_same_v<T, PersistOnLoadComponent>)
		{
			PersistOnLoadComponent::Deserialize(componentVal, object, GetConfiguration().safeMode);
			return;
		}

		else if constexpr (std::is_same_v<T, ParticleComponent>)
		{
			if (GetConfiguration().safeMode && !componentVal.IsObject())
			{
				Logger::Error("Invalid parent component structure");
				return;
			}

			object.AddComponent<ParticleComponent>();

			auto& component = object.GetComponent<ParticleComponent>();

			if (GetConfiguration().safeMode)
			{
				if (!componentVal.HasMember("isActive"))
				{
					component.isActive = true;
					Logger::Warning("Invalid structure, using defaults");
				}
				else component.isActive = componentVal["isActive"].GetBool();

				if (!componentVal.HasMember("renderOption"))
				{
					component.renderOption = 0;
					Logger::Warning("Invalid structure, using defaults");
				}
				else component.renderOption = componentVal["renderOption"].GetInt();

				if (!componentVal.HasMember("usePhysics"))
				{
					component.usePhysics = false;
					Logger::Warning("Invalid structure, using defaults");
				}
				else component.usePhysics = componentVal["usePhysics"].GetBool();

				if (!componentVal.HasMember("countEmission"))
				{
					component.countEmission = 0;
					Logger::Warning("Invalid structure, using defaults");
				}
				else component.countEmission = componentVal["countEmission"].GetInt();

				if (!componentVal.HasMember("emitters"))
				{
					Logger::Warning("Invalid structure, using defaults");
					return;
				}

				for (int i{}; i < component.emission.size(); ++i)
				{
					if (!componentVal["emitters"].HasMember(std::to_string(i).c_str())) continue;

					component.emission[i].active = true;

					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("loop"))
					{
						component.emission[i].loop = false;
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].loop = componentVal["emitters"][std::to_string(i).c_str()]["loop"].GetBool();
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("elapsedTime"))
					{
						component.emission[i].elapsedTime = 0.0f;
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].elapsedTime = componentVal["emitters"][std::to_string(i).c_str()]["elapsedTime"].GetFloat();
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("rate"))
					{
						component.emission[i].rate = 0.0f;
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].rate = componentVal["emitters"][std::to_string(i).c_str()]["rate"].GetFloat();
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("tLoop"))
					{
						component.emission[i].tLoop = 0.0f;
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].tLoop = componentVal["emitters"][std::to_string(i).c_str()]["tLoop"].GetFloat();
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("minLifeTime"))
					{
						component.emission[i].minLifeTime = 0.0f;
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].minLifeTime = componentVal["emitters"][std::to_string(i).c_str()]["minLifeTime"].GetFloat();
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("maxLifeTime"))
					{
						component.emission[i].maxLifeTime = 0.0f;
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].maxLifeTime = componentVal["emitters"][std::to_string(i).c_str()]["maxLifeTime"].GetFloat();
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("minRot"))
					{
						component.emission[i].minRot = 0.0f;
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].minRot = componentVal["emitters"][std::to_string(i).c_str()]["minRot"].GetFloat();
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("maxRot"))
					{
						component.emission[i].maxRot = 0.0f;
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].maxRot = componentVal["emitters"][std::to_string(i).c_str()]["maxRot"].GetFloat();
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("minSpinVel"))
					{
						component.emission[i].minSpinVel = 0.0f;
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].minSpinVel = componentVal["emitters"][std::to_string(i).c_str()]["minSpinVel"].GetFloat();
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("maxSpinVel"))
					{
						component.emission[i].maxSpinVel = 0.0f;
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].maxSpinVel = componentVal["emitters"][std::to_string(i).c_str()]["maxSpinVel"].GetFloat();
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("minSpinAccel"))
					{
						component.emission[i].minSpinAccel = 0.0f;
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].minSpinAccel = componentVal["emitters"][std::to_string(i).c_str()]["minSpinAccel"].GetFloat();
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("maxSpinAccel"))
					{
						component.emission[i].maxSpinAccel = 0.0f;
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].maxSpinAccel = componentVal["emitters"][std::to_string(i).c_str()]["maxSpinAccel"].GetFloat();

					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("minSpinDirAccel"))
					{
						component.emission[i].minSpinDirAccel = 0.0f;
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].minSpinDirAccel = componentVal["emitters"][std::to_string(i).c_str()]["minSpinDirAccel"].GetFloat();

                    if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("maxSpinDirAccel"))
                    {
                        component.emission[i].maxSpinDirAccel = 0.0f;
                        Logger::Warning("Invalid structure, using defaults");
                    }
                    else component.emission[i].maxSpinDirAccel = componentVal["emitters"][std::to_string(i).c_str()]["maxSpinDirAccel"].GetFloat();

					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("minPos") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["minPos"].HasMember("x") || !componentVal["emitters"][std::to_string(i).c_str()]["minPos"].HasMember("y"))
					{
						component.emission[i].minPos = { 0.0f, 0.0f };
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].minPos = { componentVal["emitters"][std::to_string(i).c_str()]["minPos"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minPos"]["y"].GetFloat() };
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("maxPos") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["maxPos"].HasMember("x") || !componentVal["emitters"][std::to_string(i).c_str()]["maxPos"].HasMember("y"))
					{
						component.emission[i].maxPos = { 0.0f, 0.0f };
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].maxPos = { componentVal["emitters"][std::to_string(i).c_str()]["maxPos"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxPos"]["y"].GetFloat() };
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("minScale") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["minScale"].HasMember("x") || !componentVal["emitters"][std::to_string(i).c_str()]["minScale"].HasMember("y"))
					{
						component.emission[i].minScale = { 0.0f, 0.0f };
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].minScale = { componentVal["emitters"][std::to_string(i).c_str()]["minScale"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minScale"]["y"].GetFloat() };
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("maxScale") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["maxScale"].HasMember("x") || !componentVal["emitters"][std::to_string(i).c_str()]["maxScale"].HasMember("y"))
					{
						component.emission[i].maxScale = { 0.0f, 0.0f };
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].maxScale = { componentVal["emitters"][std::to_string(i).c_str()]["maxScale"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxScale"]["y"].GetFloat() };
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("minVel") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["minVel"].HasMember("x") || !componentVal["emitters"][std::to_string(i).c_str()]["minVel"].HasMember("y"))
					{
						component.emission[i].minVel = { 0.0f, 0.0f };
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].minVel = { componentVal["emitters"][std::to_string(i).c_str()]["minVel"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minVel"]["y"].GetFloat() };
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("maxVel") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["maxVel"].HasMember("x") || !componentVal["emitters"][std::to_string(i).c_str()]["maxVel"].HasMember("y"))
					{
						component.emission[i].maxVel = { 0.0f, 0.0f };
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].maxVel = { componentVal["emitters"][std::to_string(i).c_str()]["maxVel"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxVel"]["y"].GetFloat() };
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("minScaleVel") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["minScaleVel"].HasMember("x") || !componentVal["emitters"][std::to_string(i).c_str()]["minScaleVel"].HasMember("y"))
					{
						component.emission[i].minScaleVel = { 0.0f, 0.0f };
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].minScaleVel = { componentVal["emitters"][std::to_string(i).c_str()]["minScaleVel"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minScaleVel"]["y"].GetFloat() };
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("maxScaleVel") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["maxScaleVel"].HasMember("x") || !componentVal["emitters"][std::to_string(i).c_str()]["maxScaleVel"].HasMember("y"))
					{
						component.emission[i].maxScaleVel = { 0.0f, 0.0f };
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].maxScaleVel = { componentVal["emitters"][std::to_string(i).c_str()]["maxScaleVel"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxScaleVel"]["y"].GetFloat() };
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("minAccel") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["minAccel"].HasMember("x") || !componentVal["emitters"][std::to_string(i).c_str()]["minAccel"].HasMember("y"))
					{
						component.emission[i].minAccel = { 0.0f, 0.0f };
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].minAccel = { componentVal["emitters"][std::to_string(i).c_str()]["minAccel"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minAccel"]["y"].GetFloat() };
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("maxAccel") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["maxAccel"].HasMember("x") || !componentVal["emitters"][std::to_string(i).c_str()]["maxAccel"].HasMember("y"))
					{
						component.emission[i].maxAccel = { 0.0f, 0.0f };
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].maxAccel = { componentVal["emitters"][std::to_string(i).c_str()]["maxAccel"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxAccel"]["y"].GetFloat() };
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("minScaleAccel") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["minScaleAccel"].HasMember("x") || !componentVal["emitters"][std::to_string(i).c_str()]["minScaleAccel"].HasMember("y"))
					{
						component.emission[i].minScaleAccel = { 0.0f, 0.0f };
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].minScaleAccel = { componentVal["emitters"][std::to_string(i).c_str()]["minScaleAccel"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minScaleAccel"]["y"].GetFloat() };
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("maxScaleAccel") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["maxScaleAccel"].HasMember("x") || !componentVal["emitters"][std::to_string(i).c_str()]["maxScaleAccel"].HasMember("y"))
					{
						component.emission[i].maxScaleAccel = { 0.0f, 0.0f };
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].maxScaleAccel = { componentVal["emitters"][std::to_string(i).c_str()]["maxScaleAccel"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxScaleAccel"]["y"].GetFloat() };

					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("minDirAccel") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["minDirAccel"].HasMember("x") || !componentVal["emitters"][std::to_string(i).c_str()]["minDirAccel"].HasMember("y"))
					{
						component.emission[i].minDirAccel = { 0.0f, 0.0f };
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].minDirAccel = { componentVal["emitters"][std::to_string(i).c_str()]["minDirAccel"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minDirAccel"]["y"].GetFloat() };
                    if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("maxDirAccel") ||
                        !componentVal["emitters"][std::to_string(i).c_str()]["maxDirAccel"].HasMember("x") || !componentVal["emitters"][std::to_string(i).c_str()]["maxDirAccel"].HasMember("y"))
                    {
                        component.emission[i].maxDirAccel = { 0.0f, 0.0f };
                        Logger::Warning("Invalid structure, using defaults");
                    }
                    else component.emission[i].maxDirAccel = { componentVal["emitters"][std::to_string(i).c_str()]["maxDirAccel"]["x"].GetFloat(),
                        componentVal["emitters"][std::to_string(i).c_str()]["maxDirAccel"]["y"].GetFloat() };

                    if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("minScaleDirAccel") ||
                        !componentVal["emitters"][std::to_string(i).c_str()]["minScaleDirAccel"].HasMember("x") || !componentVal["emitters"][std::to_string(i).c_str()]["minScaleDirAccel"].HasMember("y"))
                    {
                        component.emission[i].minScaleDirAccel = { 0.0f, 0.0f };
                        Logger::Warning("Invalid structure, using defaults");
                    }
                    else component.emission[i].minScaleDirAccel = { componentVal["emitters"][std::to_string(i).c_str()]["minScaleDirAccel"]["x"].GetFloat(),
                        componentVal["emitters"][std::to_string(i).c_str()]["minScaleDirAccel"]["y"].GetFloat() };
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("maxScaleDirAccel") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["maxScaleDirAccel"].HasMember("x") || !componentVal["emitters"][std::to_string(i).c_str()]["maxScaleDirAccel"].HasMember("y"))
					{
                        component.emission[i].maxScaleDirAccel = { 0.0f, 0.0f };
                        Logger::Warning("Invalid structure, using defaults");
                    }
                    else component.emission[i].maxScaleDirAccel = { componentVal["emitters"][std::to_string(i).c_str()]["maxScaleDirAccel"]["x"].GetFloat(),
                        componentVal["emitters"][std::to_string(i).c_str()]["maxScaleDirAccel"]["y"].GetFloat() };

					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("minCol") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["minCol"].HasMember("r") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["minCol"].HasMember("g") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["minCol"].HasMember("b") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["minCol"].HasMember("a"))
					{
						component.emission[i].minCol = { 0.0f, 0.0f, 0.0f, 0.0f };
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].minCol = {
						componentVal["emitters"][std::to_string(i).c_str()]["minCol"]["r"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minCol"]["g"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minCol"]["b"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minCol"]["a"].GetFloat() };
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("maxCol") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["maxCol"].HasMember("r") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["maxCol"].HasMember("g") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["maxCol"].HasMember("b") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["maxCol"].HasMember("a"))
					{
						component.emission[i].maxCol = { 0.0f, 0.0f, 0.0f, 0.0f };
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].maxCol = {
						componentVal["emitters"][std::to_string(i).c_str()]["maxCol"]["r"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxCol"]["g"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxCol"]["b"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxCol"]["a"].GetFloat() };
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("minColVel") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["minColVel"].HasMember("r") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["minColVel"].HasMember("g") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["minColVel"].HasMember("b") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["minColVel"].HasMember("a"))
					{
						component.emission[i].minColVel = { 0.0f, 0.0f, 0.0f, 0.0f };
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].minColVel = {
						componentVal["emitters"][std::to_string(i).c_str()]["minColVel"]["r"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minColVel"]["g"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minColVel"]["b"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minColVel"]["a"].GetFloat() };
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("maxColVel") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["maxColVel"].HasMember("r") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["maxColVel"].HasMember("g") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["maxColVel"].HasMember("b") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["maxColVel"].HasMember("a"))
					{
						component.emission[i].maxColVel = { 0.0f, 0.0f, 0.0f, 0.0f };
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].maxColVel = {
						componentVal["emitters"][std::to_string(i).c_str()]["maxColVel"]["r"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxColVel"]["g"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxColVel"]["b"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxColVel"]["a"].GetFloat() };
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("minColAccel") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["minColAccel"].HasMember("r") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["minColAccel"].HasMember("g") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["minColAccel"].HasMember("b") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["minColAccel"].HasMember("a"))
					{
						component.emission[i].minColAccel = { 0.0f, 0.0f, 0.0f, 0.0f };
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].minColAccel = {
						componentVal["emitters"][std::to_string(i).c_str()]["minColAccel"]["r"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minColAccel"]["g"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minColAccel"]["b"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minColAccel"]["a"].GetFloat() };
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("maxColAccel") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["maxColAccel"].HasMember("r") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["maxColAccel"].HasMember("g") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["maxColAccel"].HasMember("b") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["maxColAccel"].HasMember("a"))
					{
						component.emission[i].maxColAccel = { 0.0f, 0.0f, 0.0f, 0.0f };
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].maxColAccel = {
						componentVal["emitters"][std::to_string(i).c_str()]["maxColAccel"]["r"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxColAccel"]["g"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxColAccel"]["b"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxColAccel"]["a"].GetFloat() };
					if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("minColDirAccel") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["minColDirAccel"].HasMember("r") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["minColDirAccel"].HasMember("g") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["minColDirAccel"].HasMember("b") ||
						!componentVal["emitters"][std::to_string(i).c_str()]["minColDirAccel"].HasMember("a"))
					{
						component.emission[i].minColDirAccel = { 0.0f, 0.0f, 0.0f, 0.0f };
						Logger::Warning("Invalid structure, using defaults");
					}
					else component.emission[i].minColDirAccel = {
						componentVal["emitters"][std::to_string(i).c_str()]["minColDirAccel"]["r"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minColDirAccel"]["g"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minColDirAccel"]["b"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minColDirAccel"]["a"].GetFloat() };
                    if (!componentVal["emitters"][std::to_string(i).c_str()].HasMember("maxColDirAccel") ||
                        !componentVal["emitters"][std::to_string(i).c_str()]["maxColDirAccel"].HasMember("r") ||
                        !componentVal["emitters"][std::to_string(i).c_str()]["maxColDirAccel"].HasMember("g") ||
                        !componentVal["emitters"][std::to_string(i).c_str()]["maxColDirAccel"].HasMember("b") ||
                        !componentVal["emitters"][std::to_string(i).c_str()]["maxColDirAccel"].HasMember("a"))
                    {
                        component.emission[i].maxColDirAccel = { 0.0f, 0.0f, 0.0f, 0.0f };
                        Logger::Warning("Invalid structure, using defaults");
                    }
                    else component.emission[i].maxColDirAccel = {
                        componentVal["emitters"][std::to_string(i).c_str()]["maxColDirAccel"]["r"].GetFloat(),
                        componentVal["emitters"][std::to_string(i).c_str()]["maxColDirAccel"]["g"].GetFloat(),
                        componentVal["emitters"][std::to_string(i).c_str()]["maxColDirAccel"]["b"].GetFloat(),
                        componentVal["emitters"][std::to_string(i).c_str()]["maxColDirAccel"]["a"].GetFloat() };
				}
			}
			else
			{
				component.isActive = componentVal["isActive"].GetBool();
				component.renderOption = componentVal["renderOption"].GetInt();
				component.usePhysics = componentVal["usePhysics"].GetBool();
				component.countEmission = componentVal["countEmission"].GetInt();

				for (int i{}; i < component.countEmission; ++i)
				{
					if (!componentVal["emitters"].HasMember(std::to_string(i).c_str())) continue;

					component.emission[i].active = true;

					component.emission[i].loop = componentVal["emitters"][std::to_string(i).c_str()]["loop"].GetBool();
					component.emission[i].elapsedTime = componentVal["emitters"][std::to_string(i).c_str()]["elapsedTime"].GetFloat();
					component.emission[i].rate = componentVal["emitters"][std::to_string(i).c_str()]["rate"].GetFloat();
					component.emission[i].tLoop = componentVal["emitters"][std::to_string(i).c_str()]["tLoop"].GetFloat();
					component.emission[i].minLifeTime = componentVal["emitters"][std::to_string(i).c_str()]["minLifeTime"].GetFloat();
					component.emission[i].maxLifeTime = componentVal["emitters"][std::to_string(i).c_str()]["maxLifeTime"].GetFloat();
					component.emission[i].minRot = componentVal["emitters"][std::to_string(i).c_str()]["minRot"].GetFloat();
					component.emission[i].maxRot = componentVal["emitters"][std::to_string(i).c_str()]["maxRot"].GetFloat();
					component.emission[i].minSpinVel = componentVal["emitters"][std::to_string(i).c_str()]["minSpinVel"].GetFloat();
					component.emission[i].maxSpinVel = componentVal["emitters"][std::to_string(i).c_str()]["maxSpinVel"].GetFloat();
					component.emission[i].minSpinAccel = componentVal["emitters"][std::to_string(i).c_str()]["minSpinAccel"].GetFloat();
					component.emission[i].maxSpinAccel = componentVal["emitters"][std::to_string(i).c_str()]["maxSpinAccel"].GetFloat();
                    component.emission[i].minSpinDirAccel = componentVal["emitters"][std::to_string(i).c_str()]["minSpinDirAccel"].GetFloat();
                    component.emission[i].maxSpinDirAccel = componentVal["emitters"][std::to_string(i).c_str()]["maxSpinDirAccel"].GetFloat();

					component.emission[i].minPos = { componentVal["emitters"][std::to_string(i).c_str()]["minPos"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minPos"]["y"].GetFloat() };
					component.emission[i].maxPos = { componentVal["emitters"][std::to_string(i).c_str()]["maxPos"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxPos"]["y"].GetFloat() };
					component.emission[i].minScale = { componentVal["emitters"][std::to_string(i).c_str()]["minScale"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minScale"]["y"].GetFloat() };
					component.emission[i].maxScale = { componentVal["emitters"][std::to_string(i).c_str()]["maxScale"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxScale"]["y"].GetFloat() };
					component.emission[i].minVel = { componentVal["emitters"][std::to_string(i).c_str()]["minVel"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minVel"]["y"].GetFloat() };
					component.emission[i].maxVel = { componentVal["emitters"][std::to_string(i).c_str()]["maxVel"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxVel"]["y"].GetFloat() };
					component.emission[i].minScaleVel = { componentVal["emitters"][std::to_string(i).c_str()]["minScaleVel"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minScaleVel"]["y"].GetFloat() };
					component.emission[i].maxScaleVel = { componentVal["emitters"][std::to_string(i).c_str()]["maxScaleVel"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxScaleVel"]["y"].GetFloat() };
					component.emission[i].minAccel = { componentVal["emitters"][std::to_string(i).c_str()]["minAccel"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minAccel"]["y"].GetFloat() };
					component.emission[i].maxAccel = { componentVal["emitters"][std::to_string(i).c_str()]["maxAccel"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxAccel"]["y"].GetFloat() };
					component.emission[i].minScaleAccel = { componentVal["emitters"][std::to_string(i).c_str()]["minScaleAccel"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minScaleAccel"]["y"].GetFloat() };
					component.emission[i].maxScaleAccel = { componentVal["emitters"][std::to_string(i).c_str()]["maxScaleAccel"]["x"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxScaleAccel"]["y"].GetFloat() };
                    component.emission[i].minDirAccel = { componentVal["emitters"][std::to_string(i).c_str()]["minDirAccel"]["x"].GetFloat(),
                        componentVal["emitters"][std::to_string(i).c_str()]["minDirAccel"]["y"].GetFloat() };
                    component.emission[i].maxDirAccel = { componentVal["emitters"][std::to_string(i).c_str()]["maxDirAccel"]["x"].GetFloat(),
                        componentVal["emitters"][std::to_string(i).c_str()]["maxDirAccel"]["y"].GetFloat() };
                    component.emission[i].minScaleDirAccel = { componentVal["emitters"][std::to_string(i).c_str()]["minScaleDirAccel"]["x"].GetFloat(),
                        componentVal["emitters"][std::to_string(i).c_str()]["minScaleDirAccel"]["y"].GetFloat() };
                    component.emission[i].maxScaleDirAccel = { componentVal["emitters"][std::to_string(i).c_str()]["maxScaleDirAccel"]["x"].GetFloat(),
                        componentVal["emitters"][std::to_string(i).c_str()]["maxScaleDirAccel"]["y"].GetFloat() };

					component.emission[i].minCol = {
						componentVal["emitters"][std::to_string(i).c_str()]["minCol"]["r"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minCol"]["g"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minCol"]["b"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minCol"]["a"].GetFloat() };
					component.emission[i].maxCol = {
						componentVal["emitters"][std::to_string(i).c_str()]["maxCol"]["r"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxCol"]["g"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxCol"]["b"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxCol"]["a"].GetFloat() };
					component.emission[i].minColVel = {
						componentVal["emitters"][std::to_string(i).c_str()]["minColVel"]["r"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minColVel"]["g"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minColVel"]["b"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minColVel"]["a"].GetFloat() };
					component.emission[i].maxColVel = {
						componentVal["emitters"][std::to_string(i).c_str()]["maxColVel"]["r"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxColVel"]["g"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxColVel"]["b"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxColVel"]["a"].GetFloat() };
					component.emission[i].minColAccel = {
						componentVal["emitters"][std::to_string(i).c_str()]["minColAccel"]["r"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minColAccel"]["g"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minColAccel"]["b"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["minColAccel"]["a"].GetFloat() };
					component.emission[i].maxColAccel = {
						componentVal["emitters"][std::to_string(i).c_str()]["maxColAccel"]["r"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxColAccel"]["g"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxColAccel"]["b"].GetFloat(),
						componentVal["emitters"][std::to_string(i).c_str()]["maxColAccel"]["a"].GetFloat() };
                    component.emission[i].minColDirAccel = {
                        componentVal["emitters"][std::to_string(i).c_str()]["minColDirAccel"]["r"].GetFloat(),
                        componentVal["emitters"][std::to_string(i).c_str()]["minColDirAccel"]["g"].GetFloat(),
                        componentVal["emitters"][std::to_string(i).c_str()]["minColDirAccel"]["b"].GetFloat(),
                        componentVal["emitters"][std::to_string(i).c_str()]["minColDirAccel"]["a"].GetFloat() };
                    component.emission[i].maxColDirAccel = {
                        componentVal["emitters"][std::to_string(i).c_str()]["maxColDirAccel"]["r"].GetFloat(),
                        componentVal["emitters"][std::to_string(i).c_str()]["maxColDirAccel"]["g"].GetFloat(),
                        componentVal["emitters"][std::to_string(i).c_str()]["maxColDirAccel"]["b"].GetFloat(),
                        componentVal["emitters"][std::to_string(i).c_str()]["maxColDirAccel"]["a"].GetFloat() };
				}
			}

			return;
		}

		else Logger::Error("Unsupported component type for deserialization");
	}
}