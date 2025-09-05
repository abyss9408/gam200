/******************************************************************************/
/*!
\file   ECS.h
\author Team Popplio
\author Bryan Ang Wei Ze
\co-author Ng Jun Heng Shawn
\contribution Bryan - 95% | Shawn - 5%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/10/02
\brief
        This is the header file for the Entity Component System of the engine

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>
#include <memory>
#include <deque>

#include "../EventBus/Event.h"
#include "../Logging/Logger.h"

namespace Popplio
{
	static const unsigned int MAX_COMPONENTS = 32;

	using Signature = std::bitset<MAX_COMPONENTS>;

    /**
    * @brief An interface class for components in the ECS system.
    * 
    * This class provides a base for all components in the ECS system.
    * It contains a protected static variable `nextId` which is used to assign unique ids to each component type.
    */
    struct IComponent
    {
    protected:
        static int nextId; /**< The next available id for a component type. */
    };

    /**
    * @brief A base class for components in the ECS system.
    * 
    * This class provides a static method to get the unique id of a component type.
    */
    template <typename T>
    class Component: public IComponent
    {
    public:
    /**
    * @brief Get the unique id of Component<T>.
    * 
    * @return int The unique id of Component<T>.
    */
		static int GetId()
		{
			static auto id = nextId++;
			return id;
		}
    };

    /**
    * @brief Represents an entity in the ECS system.
    * 
    * This class provides methods to manipulate and query an entity in the ECS system.
    */
    class Entity
    {
    public:
        /**
        * @brief Constructs an Entity object with the given id.
        * 
        * @param id The id of the entity.
        */
        Entity(int id) : id(id), registry(nullptr) {};

        /**
        * @brief Kills the entity.
        * 
        * This method marks the entity for removal from the registry.
        */
        void Kill();

        bool Exists() const;

        /**
        * @brief Gets the id of the entity.
        * 
        * @return int The id of the entity.
        */
        int GetId() const;

        /**
        * @brief Tags the entity with the given tag.
        * 
        * @param tag The tag to assign to the entity.
        */
        void Tag(const std::string& tag);

        /**
        * @brief Checks if the entity has the given tag.
        * 
        * @param tag The tag to check.
        * @return bool True if the entity has the tag, false otherwise.
        */
        bool HasTag(const std::string& tag) const;

        std::string GetTag() const;

        /**
        * @brief Groups the entity with the given group.
        * 
        * @param group The group to assign to the entity.
        */
        void Group(const std::string& group);

        /**
        * @brief Checks if the entity belongs to the given group.
        * 
        * @param group The group to check.
        * @return bool True if the entity belongs to the group, false otherwise.
        */
        bool BelongsToGroup(const std::string& group) const;

        /**
        * @brief Adds a component of type TComponent to the entity.
        * 
        * @tparam TComponent The type of the component to add.
        * @tparam TArgs The types of the arguments to forward to the component's constructor.
        * @param args The arguments to forward to the component's constructor.
        */
        template <typename TComponent, typename ...TArgs>
        void AddComponent(TArgs&& ...args);

        /**
        * @brief Removes the component of type TComponent from the entity.
        * 
        * @tparam TComponent The type of the component to remove.
        */
        template <typename TComponent>
        void RemoveComponent();

        /**
        * @brief Checks if the entity has a component of type TComponent.
        * 
        * @tparam TComponent The type of the component to check.
        * @return bool True if the entity has the component, false otherwise.
        */
        template <typename TComponent>
        bool HasComponent() const;

        /**
        * @brief Gets the component of type TComponent from the entity.
        * 
        * @tparam TComponent The type of the component to get.
        * @return TComponent& The reference to the component.
        */
        template <typename TComponent>
        TComponent& GetComponent() const;

        Entity Clone() const;

        //bool operator==(const Entity& other) const;

        Entity GetClonedFrom() const;
        //void SetClonedFrom(int ent);

        /**
        * @brief Holds a pointer to the entity's owner registry.
        */
        class Registry* registry;

    private:
        int id;
        //int clonedFrom; // id of entity that this entity was cloned from
    };

    class EntityRef // for use in mono
    {
        public:
        Entity& entity;
        EntityRef(Entity& e) : entity(e) {}
    };

    /**
    * @brief Represents a system in the ECS system.
    * 
    * This class provides methods to add and remove entities from the system, as well as retrieve the entities and component signature of the system.
    */
    class System
    {
    public:
        /**
        * @brief Virtual destructor for the System class.
        */
        virtual ~System() = default;

        /**
        * @brief Adds an entity to the system.
        * 
        * @param entity The entity to add.
        */
        void AddEntityToSystem(Entity entity);

        /**
        * @brief Removes an entity from the system.
        * 
        * @param entity The entity to remove.
        */
        void RemoveEntityFromSystem(Entity entity);

        /**
        * @brief Gets the entities in the system.
        * 
        * @return std::vector<Entity> The entities in the system.
        */
        std::vector<Entity> GetSystemEntities() const;

        /**
        * @brief Gets the component signature of the system.
        * 
        * @return const Signature& The component signature of the system.
        */
        const Signature& GetComponentSignature() const;

        bool HasEntity(Entity entity) const;

        /**
        * @brief Defines the component type that entities must have to be considered by the system.
        * 
        * @tparam TComponent The component type.
        */
        template <typename TComponent>
        void RequireComponent();
    private:
        Signature componentSignature; /**< The component signature for the system. */
        std::vector<Entity> entities; /**< The entities in the system. */
    };

	/**
	* @brief An interface class for component pools in the ECS system.
	*
	* This class provides a base for all component pools in the ECS system.
	* It defines a virtual method `RemoveEntityFromPool` that must be implemented by derived classes.
	*/
    class IPool
    {
    public:
        /**
        * @brief Virtual destructor for the IPool class.
        */
        virtual ~IPool() = default;

        /**
        * @brief Removes an entity from the pool.
        * 
        * @param entityId The id of the entity to remove.
        */
        virtual void RemoveEntityFromPool(int entityId) = 0;
    };

    /**
    * @brief Represents a pool of objects of type T in the ECS system.
    * 
    * This class template provides methods to manage a pool of objects of type T in the ECS system.
    * It inherits from the IPool interface.
    */
    template <typename T>
    class Pool : public IPool
    {
    public:
		/**
		* @brief Constructs a Pool object with the given capacity.
		* 
		* @param capacity The capacity of the pool.
		*/
		Pool(int capacity = 100)
		: size(0)
		{
			data.resize(capacity);
		}

		/**
		* @brief Virtual destructor for the Pool class.
		*/
		virtual ~Pool() = default;

		/**
		* @brief Checks if the pool is empty.
		* 
		* @return bool True if the pool is empty, false otherwise.
		*/
		bool IsEmpty() const
		{
			return size == 0;
		}

		/**
		* @brief Gets the size of the pool.
		* 
		* @return int The size of the pool.
		*/
		int GetSize() const
		{
			return size;
		}

        /**
        * @brief Resizes the pool to the specified size.
        * 
        * @param n The new size of the pool.
        */
        void Resize(int n)
        {
			data.resize(n);
        }

        /**
        * @brief Clears the pool.
        * 
        * This method removes all elements from the pool and resets the size to 0.
        */
        void Clear()
        {
			data.clear();
			size = 0;
        }

        /**
        * @brief Adds an object to the pool.
        * 
        * @param object The object to add to the pool.
        */
        void Add(T&& object)
        {
			data.push_back(std::move(object));
        }

        /**
        * @brief Sets the object at the specified entity id in the pool.
        * 
        * If the entity id already exists in the pool, the existing object is replaced with the new object.
        * If the entity id is new, the object is added to the pool and the entity id is tracked.
        * If the pool capacity is reached, the pool is resized by doubling its current capacity.
        * 
        * @param entityId The id of the entity.
        * @param object The object to set in the pool.
        */
        void Set(int entityId, T&& object)
        {
			if (entityIdToIndex.find(entityId) != entityIdToIndex.end())
			{
				// If the element already exists, simply replace the component object
				int index = entityIdToIndex[entityId];
				data[index] = std::move(object);
			}
			else
			{
				// When a new object, we keep track of the entity ids and their vector index
				int index = size;
				entityIdToIndex.emplace(entityId, index);
				indexToEntityId.emplace(index, entityId);
				if (index >= data.capacity())
				{
					// necessary, we resize by always doubling the current capacity
					data.resize(size * 2);
				}

				data[index] = std::move(object);
				++size;
			}
        }

        /**
        * @brief Removes the entity with the specified entityId from the pool.
        * 
        * This method removes the entity with the specified entityId from the pool and updates the index-entity maps accordingly.
        * If the entityId does not exist in the pool, no action is taken.
        *
        * @param entityId The id of the entity to remove from the pool.
        */
        void Remove(int entityId)
        {
			// Copy the last element to the deleted position to keep the array packed
			int indexOfRemoved = entityIdToIndex[entityId];
			int indexOfLast = size - 1;
			data[indexOfRemoved] = data[indexOfLast];

			// Update the index-entity maps to point to the correct elements
			int entityIdOfLastElement = indexToEntityId[indexOfLast];
			entityIdToIndex[entityIdOfLastElement] = indexOfRemoved;
			indexToEntityId[indexOfRemoved] = entityIdOfLastElement;

			entityIdToIndex.erase(entityId);
			indexToEntityId.erase(indexOfLast);

			--size;
        }

        /**
        * @brief Removes the entity with the specified entityId from the pool.
        * 
        * This method removes the entity with the specified entityId from the pool and updates the index-entity maps accordingly.
        * If the entityId does not exist in the pool, no action is taken.
        *
        * @param entityId The id of the entity to remove from the pool.
        */
        void RemoveEntityFromPool(int entityId) override
        {
			if (entityIdToIndex.find(entityId) != entityIdToIndex.end())
			{
				Remove(entityId);
			}
        }

        /**
        * @brief Gets the object at the specified entity id in the pool.
        * 
        * @param entityId The id of the entity.
        * @return T& The reference to the object.
        */
        T& Get(int entityId)
        {
            auto it = entityIdToIndex.find(entityId);
            if (it == entityIdToIndex.end())
            {
                throw std::out_of_range("Trying to get non-existent component");
            }
            return data[it->second];
        }

        /**
        * @brief Gets the object at the specified index in the pool.
        * 
        * @param index The index of the object in the pool.
        * @return T& The reference to the object.
        */
        T& operator[](unsigned int index)
        {
			return data[index];
        }

	private:
		// Keep track of the vector of component objects and their current size
		std::vector<T> data;
		int size;

		// Maps to keep track of entity ids per index so that the vector is always packed
		std::unordered_map<int, int> entityIdToIndex;
		std::unordered_map<int, int> indexToEntityId;
	};

	
	class Registry
	{
	public:
		int GetNumEntities() const;

        void ResetNumEntities();

        void UpdateNumEntities();

		void ClearFreeIds();
        /**
        * @brief Updates the ECS registry.
        * 
        * This method updates the ECS registry by processing any pending entity additions or removals.
        * It is responsible for adding new entities to their respective systems and removing killed entities from the systems.
        */
        void Update();

        void RuntimeUpdate(Entity e);

        void ForceRuntimeUpdate(Entity e);

        bool KillEntityQueueIsEmpty() const;

        const std::vector<Entity>& GetAllEntities() const;

        std::vector<Entity>& GetAllEntitiesMutable();

        /**
        * @brief Creates a new entity in the ECS registry.
        * 
        * @param original The id of the entity that this entity was cloned from if it was cloned.
        * 
        * @param idToSetTo The id to set the entity to if it is not -1 (for deserialization, etc).
        * 
        * @return Entity The newly created entity.
        */
        Entity CreateEntity(int original = -1, int idToSetTo = -1);

        void KillEntityFirstPass(Entity entity);

        std::set<Entity>& KillEntitySecondPass(Entity entity, std::set<Entity>& killedEntities);

        void KillEntityRuntime(Entity entity);

        /**
        * @brief Kills the specified entity in the ECS registry.
        * 
        * @param entity The entity to kill.
        */
        void KillEntity(Entity entity);

        /**
        * @brief Kills all entities in the ECS registry.
        */
        void KillAllEntities();

        /**
        * @brief Checks if an entity with the specified id exists in the ECS registry.
        * 
        * @param entityId The id of the entity to check.
        * @return bool True if the entity exists, false otherwise.
        */
        bool EntityExists(int entityId) const;

        /**
        * @brief Gets the entity with the specified id from the ECS registry.
        * 
        * @param entityId The id of the entity to get.
        * @return Entity The entity with the specified id.
        */
        Entity GetEntityById(int entityId);

        /**
        * @brief Gets entity ref with the specified id from the ECS registry.
        *
        * @param entityId The id of the entity to get.
        * @return Entity& The entity with the specified id.
        */
        //Entity& RefEntityById(int entityId);

        /**
        * @brief Tags the specified entity with the given tag.
        * 
        * @param entity The entity to tag.
        * @param tag The tag to assign to the entity.
        */
        void TagEntity(Entity entity, const std::string& tag);

        /**
        * @brief Checks if the specified entity has the given tag.
        * 
        * @param entity The entity to check.
        * @param tag The tag to check.
        * @return bool True if the entity has the tag, false otherwise.
        */
        bool EntityHasTag(Entity entity, const std::string& tag) const;

        /**
        * @brief Checks if the entity has any tag assigned.
        * 
        * This method checks if the entity has any tag assigned to it.
        * 
        * @param entity The entity to check.
        * @return bool True if the entity has any tag assigned, false otherwise.
        */
        bool EntityHasAnyTag(Entity entity) const;

        /**
        * @brief Gets the entity with the specified tag from the ECS registry.
        * 
        * @param tag The tag of the entity to get.
        * @return Entity The entity with the specified tag.
        */
        Entity GetEntityByTag(const std::string& tag) const;

        /**
        * @brief Gets the tag of the specified entity.
        * 
        * @param entity The entity to get the tag from.
        * @return std::string The tag of the entity.
        */
        std::string GetEntityTag(Entity entity) const;

        /**
        * @brief Removes the tag from the specified entity in the ECS registry.
        * 
        * @param entity The entity to remove the tag from.
        */
        void RemoveEntityTag(Entity entity);

        /**
        * @brief Groups the entity with the given group.
        *
        * @param entity The entity to group.
        * @param group The group to assign to the entity.
        */
        void GroupEntity(Entity entity, const std::string& group);

        /**
        * @brief Checks if the entity belongs to the given group.
        *
        * @param entity The entity to check.
        * @param group The group to check.
        * @return bool True if the entity belongs to the group, false otherwise.
        */
        bool EntityBelongsToGroup(Entity entity, const std::string& group) const;

        /**
        * @brief Checks if the entity belongs to any group.
        * 
        * This method checks if the entity belongs to any group.
        * 
        * @param entity The entity to check.
        * @return bool True if the entity belongs to any group, false otherwise.
        */
        bool EntityBelongsToAnyGroup(Entity entity) const;

        /**
        * @brief Gets the entities in the specified group.
        *
        * @param group The group to get the entities from.
        * @return std::vector<Entity> The entities in the group.
        */
        std::vector<Entity> GetEntityByGroup(const std::string& group) const;

        /**
        * @brief Gets the group of the specified entity.
        * 
        * This method retrieves the group of the specified entity.
        * 
        * @param entity The entity to get the group from.
        * @return std::string The group of the entity.
        */
        std::string GetEntityGroup(Entity entity) const;

        /**
        * @brief Removes the entity from its group.
        *
        * @param entity The entity to remove from the group.
        */
        void RemoveEntityGroup(Entity entity);

        // Component management
        /**
        * @brief Adds a component of type TComponent to the entity.
        *
        * @tparam TComponent The type of the component to add.
        * @tparam TArgs The types of the arguments to forward to the component's constructor.
        * @param entity The entity to add the component to.
        * @param args The arguments to forward to the component's constructor.
        */
        template <typename TComponent, typename ...TArgs>
        void AddComponent(Entity entity, TArgs&& ...args);

        /**
        * @brief Removes the component of type TComponent from the entity.
        *
        * @tparam TComponent The type of the component to remove.
        * @param entity The entity to remove the component from.
        */
        template <typename TComponent>
        void RemoveComponent(Entity entity);

        /**
        * @brief Checks if the entity has a component of type TComponent.
        *
        * @tparam TComponent The type of the component to check.
        * @param entity The entity to check.
        * @return bool True if the entity has the component, false otherwise.
        */
        template <typename TComponent>
        bool HasComponent(Entity entity) const;

        /**
        * @brief Gets the component of type TComponent from the entity.
        *
        * @tparam TComponent The type of the component to get.
        * @param entity The entity to get the component from.
        * @return TComponent& The reference to the component.
        */
        template <typename TComponent>
        TComponent& GetComponent(Entity entity) const;

        /**
        * @brief Adds a system of type TSystem to the ECS registry.
        *
        * This method adds a system of type TSystem to the ECS registry.
        *
        * @tparam TSystem The type of the system to add.
        * @tparam TArgs The types of the arguments to forward to the system's constructor.
        * @param args The arguments to forward to the system's constructor.
        */
        template <typename TSystem, typename ...TArgs>
        void AddSystem(TArgs&& ...args);

        /**
        * @brief Removes the system of type TSystem from the ECS registry.
        *
        * This method removes the system of type TSystem from the ECS registry.
        *
        * @tparam TSystem The type of the system to remove.
        */
        template <typename TSystem>
        void RemoveSystem();

        /**
        * @brief Checks if the system of type TSystem exists in the ECS registry.
        *
        * This method checks if the system of type TSystem exists in the ECS registry.
        *
        * @tparam TSystem The type of the system to check.
        * @return bool True if the system exists, false otherwise.
        */
        template <typename TSystem>
        bool HasSystem() const;

        /**
        * @brief Gets the system of type TSystem from the ECS registry.
        *
        * This method gets the system of type TSystem from the ECS registry.
        *
        * @tparam TSystem The type of the system to get.
        * @return TSystem& The reference to the system.
        */
        template <typename TSystem>
        TSystem& GetSystem() const;

        /**
        * @brief Adds an entity to its systems.
        *
        * This method adds an entity to its systems.
        *
        * @param entity The entity to add.
        */
        void AddEntityToSystems(Entity entity);

        /**
        * @brief Removes an entity from its systems.
        *
        * This method removes an entity from its systems.
        *
        * @param entity The entity to remove.
        */
        void RemoveEntityFromSystems(Entity entity);

        /**
        * @brief Notifies the system that the signature of an entity has changed.
        * 
        * This method is called by the Entity class when the signature of an entity has changed.
        * It takes the entity and its new signature as parameters.
        * The system uses this method to update the component signature of the entity in the system.
        * 
        * @param entity The entity whose signature has changed.
        * @param entitySignature The new signature of the entity.
        */
        void EntitySignatureChanged(Entity entity, const Signature& entitySignature);

        /**
        * @brief Clears instanced entities
        *
        * This method is completed instantly without queueing
        */
        void ClearInstancedEntities();

        // for runtime
        void CachePreRuntime(); // cache ECS info pre runtime to be reset to after runtime
        void ResetToPreRuntime(); // reset ECS info to pre runtime

        int GetFreeIdCount() const;
        std::vector<int> GetFreeIds() const;

        void ResetEntityComponentSignatures();

        Entity GetClonedFrom(Entity entity);

        void SetClonedFrom(Entity entity, Entity original);

        std::vector<Signature> StashPersistentEntities(std::set<Entity>& persistent);

        void RestorePersistentEntities(std::vector<Signature>& signatures);

	private:
		// Keep track of how many entities were added to the scene
		//int numEntities = 0;

        /**< The entities in the ECS registry. */
        std::vector<Entity> entities;

		/*
			Vector of component pools, each pool contains all the data for a certain component type
			[vector index = componentId]
			[entity id -> pool index] (mIndexToEntityId)
		*/
		std::vector<std::shared_ptr<IPool>> componentPools;
	
		/*
			Vector of component signatures per entity, saying which component is turned "on" for a given entity
			[vector index = entityId]
		*/
		std::vector<Signature> entityComponentSignatures;

		/*
			Unordered map of active systems
			index = system typeId]
		*/
		std::unordered_map<std::type_index, std::shared_ptr<System>> systems;
	
		// Set of entities that are flagged to be added or removed in the next registry Update()
		std::set<Entity> entitiesToBeAdded;
		std::set<Entity> entitiesToBeKilled;

		// Entity tags (one tag name per entity)
		std::unordered_map<std::string, Entity> entityPerTag;
		std::unordered_map<int, std::string> tagPerEntity;

		// Entity groups (a set of entities per group name and an entity belongs to only one group)
		std::unordered_map<std::string, std::set<Entity>> entitiesPerGroup;
		std::unordered_map<int, std::string> groupsPerEntity;

		// List of free entity ids that were previously removed
		std::deque<int> freeIds;

        // Entity cloned from ids
        std::unordered_map<int, int> clonedFromIds; // Key: entity id, Value: original entity id

        // runtime
        std::deque<int> cacheFreeIds{};
        int cacheNumEntities = 0;


        /**
        * @brief Behaviour on entity creation
        *
        * This method adds behaviours on entity creation.
        *
        * @param entity The entity created.
        */
        void OnEntityCreate(Entity const& entity);
        /**
        * @brief Behaviour on entity destruction
        *
        * This method adds behaviours on entity destruction.
        *
        * @param entity The entity destroyed.
        */
        void OnEntityDestroy(Entity entity);
	};
	
    /**
    * @brief Overloads the equality operator for comparing two Entity objects.
    * 
    * @param lhs The left-hand side Entity object.
    * @param rhs The right-hand side Entity object.
    * @return bool True if the two Entity objects are equal, false otherwise.
    */
    bool operator==(const Entity& lhs, const Entity& rhs);

    /**
    * @brief Overloads the inequality operator for comparing two Entity objects.
    * 
    * @param lhs The left-hand side Entity object.
    * @param rhs The right-hand side Entity object.
    * @return bool True if the two Entity objects are not equal, false otherwise.
    */
    bool operator!=(const Entity& lhs, const Entity& rhs);

    /**
    * @brief Overloads the less than operator for comparing two Entity objects.
    * 
    * @param lhs The left-hand side Entity object.
    * @param rhs The right-hand side Entity object.
    * @return bool True if lhs is less than rhs, false otherwise.
    */
    bool operator<(const Entity& lhs, const Entity& rhs);

    /**
    * @brief Overloads the greater than operator for comparing two Entity objects.
    * 
    * @param lhs The left-hand side Entity object.
    * @param rhs The right-hand side Entity object.
    * @return bool True if lhs is greater than rhs, false otherwise.
    */
    bool operator>(const Entity& lhs, const Entity& rhs);

    // =================================================================== //

    struct EntityRuntimeChangeEvent : public Event // on change at runtime
    {
        enum Change
        {
            ADDED = 0,
            REMOVED = 1,
            MODIFIED = 2
        };

        Change change;
        int id;

        EntityRuntimeChangeEvent(Change change, int id) : change(change), id(id) {}
    };

    // =================================================================== //

	template <typename TComponent>
	void System::RequireComponent()
	{
		const auto componentId = Component<TComponent>::GetId();
		componentSignature.set(componentId);
	}

	template <typename TSystem, typename ...TArgs>
	void Registry::AddSystem(TArgs&& ...args)
	{
		std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
		systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
	}

	template <typename TSystem>
	void Registry::RemoveSystem()
	{
		auto system = systems.find(std::type_index(typeid(TSystem)));
        if (system != systems.end())
        {
            systems.erase(system);
        }
	}

	template <typename TSystem>
	bool Registry::HasSystem() const
	{
		return systems.find(std::type_index(typeid(TSystem))) != systems.end();
	}

	template <typename TSystem>
	TSystem& Registry::GetSystem() const
	{
		auto system = systems.find(std::type_index(typeid(TSystem)));
        if (system == systems.end())
        {
            throw std::runtime_error("System not found");
        }
		return *(std::static_pointer_cast<TSystem>(system->second));
	}

	template <typename TComponent, typename ...TArgs>
	void Registry::AddComponent(Entity entity, TArgs&& ...args)
	{
		const auto componentId = Component<TComponent>::GetId();
		const auto entityId = entity.GetId();

		// If the component id is greater than the current size of the componentPools, then resize the vector
		if (componentId >= componentPools.size())
		{
			componentPools.resize(componentId + 1, nullptr);
		}

		// If we still don't have a Pool for that component type
		if (!componentPools[componentId])
		{
			std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
			componentPools[componentId] = newComponentPool;
		}

		// Get the pool of component values for that component type
		std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

		// Create a new Component object of the type T, and forward the various parameters to the constructors
		TComponent newComponent(std::forward<TArgs>(args)...);

		// Add the component to the component pool list, using the entity Id as index
		componentPool->Set(entityId, std::move(newComponent));

		// Finally, change the component signature of the entity and set the component id on the bitset to 1
		entityComponentSignatures[entityId].set(componentId);

        // Notify systems that an entity's components have changed
        EntitySignatureChanged(entity, entityComponentSignatures[entityId]);

		Logger::Debug("Component added to Entity ID: " + std::to_string(entity.GetId()) +
			", Component Type: " + typeid(TComponent).name());
	}

	template <typename TComponent>
	void Registry::RemoveComponent(Entity entity)
	{
		const auto componentId = Component<TComponent>::GetId();
		const auto entityId = entity.GetId();

        if (!entityComponentSignatures[entityId].test(componentId))
        {
            Logger::Warning("Tried to remove non-existent component from entity " + std::to_string(entityId));
            return;
        }

		// Remove the component from the component pool for that entity
		std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
		componentPool->Remove(entityId);

		// Set the component signature for that entity to false
		entityComponentSignatures[entityId].set(componentId, false);

        EntitySignatureChanged(entity, entityComponentSignatures[entityId]);

        Logger::Debug("Component removed from Entity ID: " + std::to_string(entity.GetId()) +
            ", Component Type: " + typeid(TComponent).name());
	}

	template <typename TComponent>
	bool Registry::HasComponent(Entity entity) const
	{
		const auto componentId = Component<TComponent>::GetId();
		const auto entityId = entity.GetId();
		return entityId < entityComponentSignatures.size() && entityComponentSignatures[entityId].test(componentId);
	}

	template <typename TComponent>
	TComponent& Registry::GetComponent(Entity entity) const
	{
		const auto componentId = Component<TComponent>::GetId();
		const auto entityId = entity.GetId();
        if (componentId >= componentPools.size() || !componentPools[componentId])
        {
            Logger::Error("Attempt to get non-existent component from Entity ID: " + std::to_string(entityId));
            throw std::runtime_error("Component not found");
        }
		auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
		return componentPool->Get(entityId);
	}

	template <typename TComponent, typename ...TArgs>
	void Entity::AddComponent(TArgs&& ...args)
	{
		registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
	}

	template <typename TComponent>
	void Entity::RemoveComponent()
	{
		registry->RemoveComponent<TComponent>(*this);
	}

	template <typename TComponent>
	bool Entity::HasComponent() const
	{
		return registry->HasComponent<TComponent>(*this);
	}

	template <typename TComponent>
	TComponent& Entity::GetComponent() const
	{
		return registry->GetComponent<TComponent>(*this);
	}
}