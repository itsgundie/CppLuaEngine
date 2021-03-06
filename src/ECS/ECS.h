#ifndef ECS_H
#define ECS_H

#include <stdint.h>
#include <set>
#include <deque>
#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <iostream>

#include "Logger.h"

const uint32_t MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent
{
    protected:
    static int32_t nextId;
};

template <typename T>
class Component: public IComponent
{
    public:
        static int32_t GetId()
        {
            static auto id = nextId++;
            return(id);
        }
};

class Entity
{
    private:
        int32_t id;
    public:
        Entity(int32_t id): id(id) {};
        int32_t GetId() const;
        void Kill();

        Entity(const Entity& entity) = default;

        bool operator ==(const Entity& other) const
        {
            return id == other.id;
        };
        bool operator !=(const Entity& other) const
        {
            return id != other.id;
        };
        bool operator >(const Entity& other) const
        {
            return id > other.id;
        };
        bool operator <(const Entity& other) const
        {
            return id < other.id;
        };
        Entity& operator =(const Entity& other) = default;

        class Registry* registry;

        template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
        template <typename TComponent> void RemoveComponent();
        template <typename TComponent> bool HasComponent() const;
        template <typename TComponent> TComponent& GetComponent() const;

        void Tag(const std::string& tag);
        bool HasTag(const std::string& tag) const;
        void Group(const std::string& groupName);
        bool BelongsToGroup(const std::string& groupName) const;

};

class System
{
    private:
    Signature componentSignature;
    std::vector<Entity> entities;

    public:
    System() = default;
    ~System() = default;
    void AddEntityToSystem(Entity enitity);
    void RemoveEntityFromSystem(Entity entity);
    std::vector<Entity> GetSystemEntities() const;
    const Signature& GetComponentSignature() const;

    // What component type is considered by system
    template <typename T> void RequireComponent();
};

class IPool
{
    public:
        virtual ~IPool() = default;
        virtual void RemoveEntityFromPool(int32_t entityId) = 0;
};

template <typename T>
class Pool: public IPool
{
    private:
        std::vector<T> data;
        int32_t size;

        std::unordered_map<int32_t, int32_t> entityToIndex;
        std::unordered_map<int32_t, int32_t> indexToEntity;

    public:
        Pool(int32_t capacity = 128)
        {
            size = 0;
            data.resize(capacity);
        }

        virtual ~Pool() = default;

        bool IsEmpty() const
        {
            return(size == 0);
        }

        int32_t GetSize() const
        {
            return(size);
        }

        void Resize(int32_t newSize)
        {
            data.resize(newSize);
        }

        void Clear()
        {
            data.clear();
            size = 0;
        }

        void Add(T object)
        {
            data.push_back(object);
        }

        void Set(int32_t entityId, T object)
        {   
            if (entityToIndex.find(entityId) != entityToIndex.end())
            {
                int32_t index = entityToIndex[entityId];
                data[index] = object;
            }
            else
            {
                int32_t index = size;
                entityToIndex.emplace(entityId, index);
                indexToEntity.emplace(index, entityId);
                if (index >= data.capacity())
                    data.resize(size * 2);
                data[index] = object;
                size++;
            }
        }

        void Remove(int32_t entityId)
        {
            int32_t indexOfRemoved = entityToIndex[entityId];
            int32_t indexOfLast = size - 1;
            data[indexOfRemoved] = data[indexOfLast];

            int32_t entityIdOfLastElement = indexToEntity[indexOfLast];
            entityToIndex[entityIdOfLastElement] = indexOfRemoved;
            indexToEntity[indexOfRemoved] = entityIdOfLastElement;

            entityToIndex.erase(entityId);
            indexToEntity.erase(indexOfLast);

            size--;
        }

        void RemoveEntityFromPool(int32_t entityId) override
        {
            if (entityToIndex.find(entityId) != entityToIndex.end())
            {
                Remove(entityId);
            }
        }

        T& Get(int32_t entityId)
        {
            int32_t index = entityToIndex[entityId];
            return(static_cast<T&>(data[index]));
        }

        T& operator [](uint32_t index)
        {
            return(data[index]);
        }
};

class Registry
{
    private:
        int32_t numEntities = 0;
        // Vector index is Component type, Pool index is entity ID
        std::vector<std::shared_ptr<IPool>> componentPools;
        // Turned On Components per Entity, vector index is entity ID
        std::vector<Signature> entityComponentSignatures;
        // Don't need sorting, collection of active systems, Map Key = system type ID
        std::unordered_map<std::type_index, std::shared_ptr<System>> systems;
        // Enities to add/remove in Update phase
        std::set<Entity> entitiesToBeAdded;
        std::set<Entity> entitiesToBeRemoved;

        // ids will be added when entity being removed
        std::deque<int32_t> freeIds;

        // Data structures for tags and groups
        // For fast access we duplicate
        std::unordered_map<std::string, Entity> entitiesPerTag;
        std::unordered_map<int32_t, std::string> tagPerEntity;
        std::unordered_map<std::string, std::set<Entity>> entitiesPerGroup;
        std::unordered_map<int32_t, std::string> groupPerEntity;


    public:
        // Registry() = default;
        Registry() 
        {
            Logger::Log("Created Registry With Smart Pointer");
        }
        ~Registry() 
        {
            Logger::Log("Registry deleted automatically");
        }
        void Update();
        Entity CreateEntity();
        void RemoveEntity(Entity entity);
        // void AddComponent<T>()
        void AddEntityToSystem(Entity entity);
        template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
        template <typename TComponent> void RemoveComponent(Entity entity);
        template <typename TComponent> bool HasComponent(Entity entity);  
        template <typename TComponent> TComponent& GetComponent(Entity entity) const;

        template <typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
        template <typename TSystem> void RemoveSystem();
        template <typename TSystem> bool HasSystem() const;
        template <typename TSystem> TSystem& GetSystem() const;

        // Based on entity signature adding it to systems, that interested in it 
        void AddEntityToSystems(Entity entity);
        void RemoveEntityFromSystems(Entity entity);

        void TagEntity(Entity entity, const std::string& tag);
        bool EntityHasTag(Entity entity, const std::string& tag) const;
        Entity GetEntityByTag(const std::string& tag) const;
        void RemoveEntityTag(Entity entity);

        void GroupEntity(Entity entity, const std::string& group);
        bool EntityBelongsToGroup(Entity entity, const std::string& group) const;
        std::vector<Entity> GetEntitiesByGroup(const std::string& group) const;
        void RemoveEntityGroup(Entity entity);
};

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
    auto systemIterator = systems.find(std::type_index(typeid(TSystem)));
    systems.erase(systemIterator);
}

template <typename TSystem>
bool Registry::HasSystem() const
{
    auto systemExist = systems.find(std::type_index(typeid(TSystem)));
    // If pointer not at terminator, we finded system
    return(systemExist != systems.end());
}

template <typename TSystem>
TSystem& Registry::GetSystem() const
{
    auto systemFinded = systems.find(std::type_index(typeid(TSystem)));
    // Get pointer to secobd element of collection -> System itself
    return *(std::static_pointer_cast<TSystem>(systemFinded->second));
}

// Generic method implementation needs to be in the header
template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs&& ...args)
{
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    // If there is no space, increase length of vector of pools with components
    // and set pointer to null
    if(componentId >= componentPools.size())
    {
        componentPools.resize(componentId + 1, nullptr);
    }
    // Create new pool with given type of component, if it's nonexistent/null
    if(!componentPools[componentId])
    {
        std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
        componentPools[componentId] = newComponentPool;
    }
    std::shared_ptr<Pool<TComponent>> componentPool = 
        std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);


    TComponent newComponent(std::forward<TArgs>(args)...);

    componentPool->Set(entityId, newComponent);
    // Activate component signature, set to 1 component position in bitset
    entityComponentSignatures[entityId].set(componentId);


    Logger::Log("Component ID = " + std::to_string(componentId) + " was ADDED to entity ID " + std::to_string(entityId));

    // std::cout << "COMPONENT ID = " << componentId << " --> POOL SIZE: " << componentPool->GetSize() << std::endl;
}

template <typename TComponent>
void Registry::RemoveComponent(Entity entity)
{
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();        

    std::shared_ptr<Pool<TComponent>> componentPool = 
        std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

    componentPool->Remove(entityId);
    entityComponentSignatures[entityId].set(componentId, false);

    Logger::Log("Component ID = " + std::to_string(componentId) + " was REMOVED from entity ID " + std::to_string(entityId));
}

template <typename TComponent>
bool Registry::HasComponent(Entity entity)
{
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    return(entityComponentSignatures[entityId].test(componentId));
}

template <typename TComponent>
TComponent& Registry::GetComponent(Entity entity) const
{
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();
    auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

    return(componentPool->Get(entityId));
} 



//

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
    return(registry->HasComponent<TComponent>(*this));
}

template <typename TComponent>
TComponent& Entity::GetComponent() const
{
    return(registry->GetComponent<TComponent>(*this));
}

#endif