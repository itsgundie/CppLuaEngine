#ifndef ECS_H
#define ECS_H

#include <stdint.h>
#include <set>
#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>

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
};

class System
{
    private:
    Signature componentSignature;
    std::vector<Entity> enitites;

    public:
    System() = default;
    ~System() = default;
    void AddEnitityToSystem(Entity enitity);
    void RemoveEntityFromSystem(Entity entity);
    std::vector<Entity> GetSystemEntities() const;
    const Signature& GetComponentSignature() const;

    // What component type is considered by system
    template <typename T> void RequireComponent();
};

class IPool
{
    public:
        virtual ~IPool() {};
};

template <typename T>
class Pool: public IPool
{
    private:
        std::vector<T> data;
    public:
        Pool(int32_t size = 128)
        {
            data.resize(size);
        }
        ~Pool() = default;
        bool isEmpty() const
        {
            return(data.empty());
        }
        int32_t GetSize() const
        {
            return(data.size());
        }
        void Resize(int32_t newSize)
        {
            data.resize(newSize);
        }
        void Clear()
        {
            data.clear();
        }
        void Add(T object)
        {
            data.push_back(object);
        }
        void Set(int32_t index, T object)
        {
            data[index] = object;
        }
        T& Get(int32_t index)
        {
            return((data[index]));
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
        std::vector<IPool*> componentPools;
        // Turned On Components per Entity, vector index is entity ID
        std::vector<Signature> entityComponentSignatures;
        // Don't need sorting, collection of active systems, Map Key = system type ID
        std::unordered_map<std::type_index, System*> systems;
        // Enities to add/remove in Update phase
        std::set<Entity> entitiesToBeAdded;
        std::set<Entity> entitiesToBeRemoved;

    public:
        Registry() = default;
        void Update();
        Entity CreateEntity();
        // void AddComponent<T>()
        void AddEntityToSystem(Entity entity);
        template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
        template <typename TComponent> void RemoveComponent(Entity entity);
        template <typename TComponent> bool HasComponent(Entity entity);  

        template <typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
        template <typename TSystem> void RemoveSystem();
        template <typename TSystem> bool HasSystem() const;
        template <typename TSystem> TSystem& GetSystem() const;

        // Based on entity signature adding it to systems, that interested in it 
        void AddEntityToSystems(Entity entity);
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
    TSystem* newSystem(new TSystem(std::forward<TArgs>(args)...));
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
        Pool<TComponent> *newComponentPool = new Pool<TComponent>();
        componentPools[componentId] = newComponentPool;
    }
    Pool<TComponent> *componentPool = Pool<TComponent>(componentPools[componentId]);

    if(entityId >= componentPool->GetSize())
    {
        componentPool->Resize(numEntities);
    }

    TComponent newComponent(std::forward<TArgs>(args)...);

    componentPool->Set(entityId, newComponent);
    // Activate component signature, set to 1 component position in bitset
    entityComponentSignatures[entityId].set(componentId);
}

template <typename TComponent>
void Registry::RemoveComponent(Entity entity)
{
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    if(entityComponentSignatures[entityId].test(componentId))
        entityComponentSignatures[entityId].set(componentId, false);
}

template <typename TComponent>
bool Registry::HasComponent(Entity entity)
{
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    return(entityComponentSignatures[entityId].test(componentId));
}

#endif