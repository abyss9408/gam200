/******************************************************************************/
/*!
\file   ECS.cpp
\author Team Popplio
\author Bryan Ang Wei Ze
\co-author Ng Jun Heng Shawn
\contribution Bryan - 95% | Shawn - 5%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/10/02
\brief
        This is the source file for the Entity Component System of the engine

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>

#ifndef IMGUI_DISABLE
#include "../Editor/Editor.h"
#endif

#include "../Cloning/CloneEntityEvent.h"
#include "../Cloning/CloneSystem.h"

namespace Popplio
{
    namespace
    {
        // l-value required
        static CloneEntityEvent cloneEvent = CloneEntityEvent(-1);
    }

    int IComponent::nextId = 0;

    bool Entity::Exists() const
    {
        return registry->EntityExists(id);
    }

    int Entity::GetId() const
    {
        return id;
    }

    void Entity::Kill()
    {
        registry->KillEntity(*this);
    }

    void Entity::Tag(const std::string& tag)
    {
        registry->TagEntity(*this, tag);
    }

    bool Entity::HasTag(const std::string& tag) const
    {
        return registry->EntityHasTag(*this, tag);
    }

    std::string Entity::GetTag() const
    {
        return registry->GetEntityTag(*this);
    }

    void Entity::Group(const std::string& group)
    {
        registry->GroupEntity(*this, group);
    }

    bool Entity::BelongsToGroup(const std::string& group) const
    {
        return registry->EntityBelongsToGroup(*this, group);
    }

    Entity Entity::Clone() const
    {
        return registry->GetEntityById(registry->GetSystem<CloneSystem>().CloneEntity(cloneEvent = CloneEntityEvent(id)));
    }

    //bool Entity::operator==(const Entity& other) const
    //{
    //    return this->id == other.id;
    //}

    Entity Entity::GetClonedFrom() const { return registry->GetClonedFrom(*this); }
    //void Entity::SetClonedFrom(int ent) { clonedFrom = ent; }

    void System::AddEntityToSystem(Entity entity)
    {
        if (!HasEntity(entity))
        {
            entities.push_back(entity);
        }
    }

    void System::RemoveEntityFromSystem(Entity entity)
    {
        entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other)
        {
            return entity == other;
        }), entities.end());
    }

    bool System::HasEntity(Entity entity) const
	{
		return std::find(entities.begin(), entities.end(), entity) != entities.end();
	}

    std::vector<Entity> System::GetSystemEntities() const
    {
        return entities;
    }

    const Signature& System::GetComponentSignature() const
    {
        return componentSignature;
    }

    const std::vector<Entity>& Registry::GetAllEntities() const
	{
		return entities;
	}

    std::vector<Entity>& Registry::GetAllEntitiesMutable()
    {
        return entities;
    }

    void Registry::ClearFreeIds()
    {
		freeIds.clear();
    }

    bool Registry::KillEntityQueueIsEmpty() const
    {
        return entitiesToBeKilled.empty();
    }

    void Registry::ResetNumEntities()
    {
		//numEntities = 0;
    }

    void Registry::UpdateNumEntities()
    {
        //numEntities = static_cast<int>(entities.size());
    }

	int Registry::GetNumEntities() const
	{
		//return numEntities;
        return static_cast<int>(entities.size());
	}

    Entity Registry::CreateEntity(int original, int idToSetTo)
    {
        int entityId = -1;

        // If a specific ID is requested (for deserialization or cloning)
        if (idToSetTo >= 0)
        {
            size_t initialSize = entityComponentSignatures.size();
            if (idToSetTo >= initialSize)
            {
                entityComponentSignatures.resize(idToSetTo + 1);

                for (size_t i{ initialSize }; i < entityComponentSignatures.size() - 1; ++i)
                {
                    freeIds.push_back(static_cast<int>(i));
                }
                entityId = idToSetTo;
            }
            else if (!freeIds.empty())
            {
                auto locationToErase = std::find(freeIds.begin(), freeIds.end(), idToSetTo);
                if (locationToErase != freeIds.end())
                {
                    freeIds.erase(locationToErase);
                    entityId = idToSetTo;
                    if (freeIds.empty()) Logger::Info("No more free IDs");
                }
            }

            if (std::find(entities.begin(), entities.end(), Entity(idToSetTo)) != entities.end())
            {
                Logger::Error("Entity with ID " + std::to_string(idToSetTo) + " already exists");
                Logger::Critical("Scene file is corrupted. Please check the scene file for errors and duplicates.");
            }
        }
        else // Generate a new unique ID (for instantiating prefabs)
        {
            // If there are no free ids waiting to be reused
            if (freeIds.empty())
            {
                entityId = static_cast<int>(entities.size());

                // Make sure the entityComponentSignatures vector can accommodate the new entity
                if (entityId >= entityComponentSignatures.size())
                {
                    entityComponentSignatures.resize(entityId + 1);
                }
                Logger::Debug("Created entity with ID: " + std::to_string(entityId));
            }
            else
            {
                // Reuse an id from the list of previously removed entities
                entityId = freeIds.front();
                freeIds.pop_front();
            }
        }

        if (entityId < 0)
        {
            std::vector<int> ids;
            for (int i : freeIds)
            {
                ids.push_back(i);
            }

            Logger::Error("Entity ID " + std::to_string(idToSetTo) + " is invalid.");
            if (std::find(entities.begin(), entities.end(), Entity(idToSetTo)) != entities.end())
            {
                Logger::Error("Entity with ID " + std::to_string(idToSetTo) + " already exists");
            }
            return Entity(-1);
        }

        Entity entity(entityId);
        clonedFromIds[entityId] = original;
        entity.registry = this;
        entitiesToBeAdded.insert(entity);

        // Add ActiveComponent by default
        entity.AddComponent<ActiveComponent>();

        // Add TransformComponent by default
        entity.AddComponent<TransformComponent>();

        entities.push_back(entity);

        return entity;
    }

    void Registry::KillEntityFirstPass(Entity entity)
    {
        OnEntityDestroy(entity);

        // Remove from parent's children list if this entity is a child
        if (entity.HasComponent<ParentComponent>())
        {
            auto& parentComp = entity.GetComponent<ParentComponent>();

            // Then handle destroying children
            //std::set<Entity>& childrenToDestroy = parentComp.children;
            //for (auto child : childrenToDestroy)
            for (auto& child : parentComp.children)
            {
                KillEntityFirstPass(child);
            }
        }
    }

    std::set<Entity>& Registry::KillEntitySecondPass(Entity entity, std::set<Entity>& killedEntities)
    {
        // Remove from parent's children list if this entity is a child
        if (entity.HasComponent<ParentComponent>())
        {
            auto& parentComp = entity.GetComponent<ParentComponent>();
            if (parentComp.parent.GetId() != -1 && EntityExists(parentComp.parent.GetId()))
            {
                Entity parent = GetEntityById(parentComp.parent.GetId());
                if (parent.HasComponent<ParentComponent>())
                {
                    parent.GetComponent<ParentComponent>().RemoveChild(entity);
                }
            }

            // Then handle destroying children
            //std::vector<Entity>& childrenToDestroy = parentComp.children;
            //for (auto& child : childrenToDestroy)
            std::set<Entity> temp{};
            for (auto child : parentComp.children)
            {
                //std::set<Entity> temp = killedEntities;
                temp = KillEntitySecondPass(child, killedEntities);

                //killedEntities.insert(temp.begin(), temp.end());
            }

            for (Entity e : temp)
            {
                killedEntities.insert(e);
            }
        }
        killedEntities.insert(entity);
        return killedEntities;
    }

    void Registry::KillEntityRuntime(Entity entity)
    {
        KillEntityFirstPass(entity);

        std::set<Entity> killedEntities{};

        KillEntitySecondPass(entity, killedEntities);

        //killedEntities.insert(entity);

        for (Entity e : killedEntities)
        {
            Logger::Debug("Entity queued for deletion. ID: " + std::to_string(entity.GetId()));
            entitiesToBeKilled.insert(e);
        }
    }

    void Registry::KillEntity(Entity entity)
    {
        //std::set<Entity> temp = KillEntityRuntime(entity);

        Logger::Debug("Entity queued for deletion. ID: " + std::to_string(entity.GetId()));
        entitiesToBeKilled.insert(entity);
    }

    void Registry::KillAllEntities()
    {
        for (Entity e : GetAllEntities())
        {
            KillEntity(e);
        }
    }

    bool Registry::EntityExists(int entityId) const
    {
        return entityId >= 0 && entityId < entityComponentSignatures.size() && entityComponentSignatures[entityId].any();
    }

    Entity Registry::GetEntityById(int entityId)
    {
        if (!EntityExists(entityId))
        {
            Logger::Error("Attempted to get non-existent entity.");
            return Entity(-1);
            //throw std::runtime_error("Attempted to get non-existent entity.");
        }
        Entity entity(entityId);
        entity.registry = this;
        return entity;
    }

    void Registry::AddEntityToSystems(Entity entity)
    {
        const auto entityId = entity.GetId();
        // Match entityComponentSignature <---> systemConponentSignature
        const auto& entityComponentSignature = entityComponentSignatures[entityId];

        for (auto& system : systems)
        {
            const auto& systemComponentSignature = system.second->GetComponentSignature();

            bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

            // Add entity to system if entityComponentSignature matches systemConponentSignature
            if (isInterested)
            {
                system.second->AddEntityToSystem(entity);
            }
        }
    }

    void Registry::RemoveEntityFromSystems(Entity entity)
    {
        for (auto& system : systems)
        {
            system.second->RemoveEntityFromSystem(entity);
        }
    }

    void Registry::TagEntity(Entity entity, const std::string& tag)
    {
        entityPerTag.emplace(tag, entity);
        tagPerEntity.emplace(entity.GetId(), tag);
    }

    bool Registry::EntityHasTag(Entity entity, const std::string& tag) const
    {
        if (tagPerEntity.find(entity.GetId()) == tagPerEntity.end())
        {
            return false;
        }
        return entityPerTag.find(tag)->second.GetId() == entity.GetId();
    }

    bool Registry::EntityHasAnyTag(Entity entity) const
	{
		return tagPerEntity.find(entity.GetId()) != tagPerEntity.end();
	}

    Entity Registry::GetEntityByTag(const std::string& tag) const
    {
        auto it = entityPerTag.find(tag);
        if (it == entityPerTag.end())
        {
            throw std::runtime_error("No entity found with tag: " + tag);
        }
        return it->second;
    }

    std::string Registry::GetEntityTag(Entity entity) const
    {
        auto it = tagPerEntity.find(entity.GetId());
        if (it != tagPerEntity.end())
        {
            return it->second;
        }
        return ""; // Return empty string if entity has no tag
    }

    void Registry::RemoveEntityTag(Entity entity)
    {
        auto taggedEntity = tagPerEntity.find(entity.GetId());
        if (taggedEntity != tagPerEntity.end())
        {
            const auto& tag = taggedEntity->second;
            entityPerTag.erase(tag);
            tagPerEntity.erase(taggedEntity);
        }
    }

    void Registry::GroupEntity(Entity entity, const std::string& group)
    {
        entitiesPerGroup[group].emplace(entity);
        groupsPerEntity.emplace(entity.GetId(), group);
    }

    bool Registry::EntityBelongsToGroup(Entity entity, const std::string& group) const
    {
        auto groupEntities = entitiesPerGroup.find(group);
        // entity does not belong to group specified
        if (groupEntities == entitiesPerGroup.end())
        {
            return false;
        }
        return groupEntities->second.find(entity.GetId()) != groupEntities->second.end();
    }

    bool Registry::EntityBelongsToAnyGroup(Entity entity) const
	{
		return groupsPerEntity.find(entity.GetId()) != groupsPerEntity.end();
	}

    std::vector<Entity> Registry::GetEntityByGroup(const std::string& group) const
    {
        auto groupOfEntities = entitiesPerGroup.find(group);
        if (groupOfEntities == entitiesPerGroup.end())
        {
            return {};
        }
        return std::vector<Entity>(groupOfEntities->second.begin(), groupOfEntities->second.end());
    }

    std::string Registry::GetEntityGroup(Entity entity) const
    {
        auto it = groupsPerEntity.find(entity.GetId());
        if (it != groupsPerEntity.end())
		{
			return it->second;
		}

        return ""; // Return empty string if entity has no tag
    }

    void Registry::RemoveEntityGroup(Entity entity)
    {
        auto groupedEntity = groupsPerEntity.find(entity.GetId());
        if (groupedEntity != groupsPerEntity.end())
        {
            auto group = entitiesPerGroup.find(groupedEntity->second);
            if (group != entitiesPerGroup.end())
            {
                group->second.erase(entity);
            }
            groupsPerEntity.erase(groupedEntity);
        }
    }

    void Registry::Update()
    {
        // Process the entities that are waiting to be created to the active Systems
        for (const auto& entity : entitiesToBeAdded)
        {
            AddEntityToSystems(entity);
            OnEntityCreate(entity);
        }
        entitiesToBeAdded.clear();

        // Process the entities that are waiting to be killed to the active Systems
        for (const auto& entity : entitiesToBeKilled)
        {
            if (!entity.Exists()) continue;
            //OnEntityDestroy(entity);
            RemoveEntityFromSystems(entity);
            entityComponentSignatures[entity.GetId()].reset();

            // Remove the entity from component pools
            for (auto pool : componentPools)
            {
                if (pool)
                {
                    pool->RemoveEntityFromPool(entity.GetId());
                }
            }

            // Make the entity id available to be reused
            freeIds.push_back(entity.GetId());

            // Remove any traces of that entity from the tag/group maps
            RemoveEntityTag(entity);
            RemoveEntityGroup(entity);

            // Remove the entity from the entities vector
            entities.erase(std::remove_if(entities.begin(), entities.end(),
                [&entity](const Entity& e) { return e.GetId() == entity.GetId(); }),
                entities.end());
        }
        entitiesToBeKilled.clear();
    }

    void Registry::RuntimeUpdate(Entity e)
    {
        //if (e.Exists()) return;

#ifndef IMGUI_DISABLE
        if (GetSystem<Editor>().editorState == Editor::EDITING) return;
#endif

        std::set<Entity>::iterator i = std::find(entitiesToBeAdded.begin(), entitiesToBeAdded.end(), e);
        if (i != entitiesToBeAdded.end())
        {
            AddEntityToSystems(e);
            OnEntityCreate(e);
            entitiesToBeAdded.erase(i);
        }
    }

    void Registry::ForceRuntimeUpdate(Entity e) // FIX
    {
        //if (e.Exists()) return;

#ifndef IMGUI_DISABLE
        if (GetSystem<Editor>().editorState == Editor::EDITING) return;
#endif
        AddEntityToSystems(e);
        OnEntityCreate(e);
        entitiesToBeAdded.erase(e);
    }

    void Registry::EntitySignatureChanged(Entity entity, const Signature& entitySignature)
	{
		// Notify each system that an entity's signature has changed
		for (auto& system : systems)
		{
			const auto& systemComponentSignature = system.second->GetComponentSignature();

            // Check if system is interested in this entity
			bool isInterested = (entitySignature & systemComponentSignature) == systemComponentSignature;
			
            bool alreadyTracking = system.second->HasEntity(entity);

			// Add entity to system if entityComponentSignature matches systemConponentSignature
			if (isInterested && !alreadyTracking)
			{
				system.second->AddEntityToSystem(entity);
			}
			// Remove entity from system if entityComponentSignature does not match systemConponentSignature
			else if (!isInterested && alreadyTracking)
			{
				system.second->RemoveEntityFromSystem(entity);
			}
		}

        Update();
	}

    void Registry::ClearInstancedEntities()
    {
        std::vector<Entity> entitiesToRemove;

        for (const auto& entity : GetAllEntities())
        {
            if (entity.BelongsToGroup("instanced_entity"))
            {
                entitiesToRemove.push_back(entity);
            }
        }

        for (const auto& entity : entitiesToRemove)
        {
            KillEntity(entity);
        }

        Logger::Info("Cleared " + std::to_string(entitiesToRemove.size()) + " instanced entities");
    }

    void Registry::CachePreRuntime()
    {
        cacheFreeIds.clear();
        for (auto ids : freeIds)
            cacheFreeIds.push_back(ids);

        //cacheNumEntities = numEntities;
    }
    void Registry::ResetToPreRuntime()
    {
        freeIds.clear();
        for (auto ids : cacheFreeIds)
            freeIds.push_back(ids);

        //numEntities = cacheNumEntities;
    }

    int Registry::GetFreeIdCount() const
    {
        return static_cast<int>(freeIds.size());
    }

    std::vector<int> Registry::GetFreeIds() const
    {
        std::vector<int> container{};
        for (auto ids : freeIds)
            container.push_back(ids);
        return container;
    }

    void Registry::ResetEntityComponentSignatures()
    {
        entityComponentSignatures.clear();
    }

    Entity Registry::GetClonedFrom(Entity entity) 
    {
        return GetEntityById(clonedFromIds[entity.GetId()]);
    }

    void Registry::SetClonedFrom(Entity entity, Entity original)
    {
        clonedFromIds[entity.GetId()] = original.GetId();
    }

    std::vector<Signature> Registry::StashPersistentEntities(std::set<Entity>& persistent)
    {
        std::vector<Signature> signatures{};
        for (Entity const e : persistent)
        {
            signatures.push_back(entityComponentSignatures[e.GetId()]);
            RemoveEntityFromSystems(e);
        }
        return signatures;
    }

    void Registry::RestorePersistentEntities(std::vector<Signature>& signatures)
    {
        std::vector<Entity> persistentEntities{};
        for (Signature sig : signatures)
        {
            persistentEntities.push_back(CreateEntity());
        }
        Update();
        for (size_t i{}; i < persistentEntities.size(); ++i)
        {
            entityComponentSignatures[persistentEntities[i].GetId()] = signatures[i];
            AddEntityToSystems(persistentEntities[i]);
        }
        Update();
    }

    void Registry::OnEntityCreate(Entity const& entity)
    {
#ifndef IMGUI_DISABLE
        if (GetSystem<Editor>().IsEditorRunning())
        {
#endif
            if (entity.HasComponent<LogicComponent>())
            {
                GetSystem<LogicSystem>().CSOnEntityCreate(entity.GetId(), entity.GetComponent<LogicComponent>().scriptObject);
            }

            if (entity.HasComponent<AudioComponent>())
            {
                entity.GetComponent<AudioComponent>().Update(GetSystem<AudioSystem>());
            }
#ifndef IMGUI_DISABLE
        }
#endif
    }

    void Registry::OnEntityDestroy(Entity entity)
    {
#ifndef IMGUI_DISABLE
        if (GetSystem<Editor>().IsEditorRunning())
        {
#endif
            if (entity.HasComponent<LogicComponent>())
            {
                switch (entity.GetComponent<LogicComponent>().scriptLang)
                {
                case ScriptLang::CPP:
                    GetSystem<LogicSystem>().CPPOnEntityDestroy(entity.GetId());
                    break;
                case ScriptLang::CS:
                    GetSystem<LogicSystem>().
                        CSOnEntityDestroy(entity.GetId(), entity.GetComponent<LogicComponent>().scriptObject);
                    break;
                case ScriptLang::UNASSIGNED:
                    break;
                }
            }

            if (entity.HasComponent<AudioComponent>())
            {
                //Logger::Critical(std::to_string(entity.GetId()));
                GetSystem<AudioSystem>().GetAudioInst(entity.GetComponent<AudioComponent>().data).Kill();
            }

            //if ()

#ifndef IMGUI_DISABLE
        }
        GetSystem<Popplio::Editor>().ResetSelectedEntity(entity);
#endif
    }

    bool operator==(const Entity& lhs, const Entity& rhs) { return lhs.GetId() == rhs.GetId(); }

    bool operator!=(const Entity& lhs, const Entity& rhs) { return lhs.GetId() != rhs.GetId(); }

    bool operator<(const Entity& lhs, const Entity& rhs) { return lhs.GetId() < rhs.GetId(); }

    bool operator>(const Entity& lhs, const Entity& rhs) { return lhs.GetId() > rhs.GetId(); }
}