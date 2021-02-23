#ifndef ECS_H
#define ECS_H

#include <stdint.h>
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
        // Don't need sorting, collection of systems
        std::unordered_map<std::type_index, System*> systems;
    public:
        Registry() = default;
        // TODO: create/destroy entity, add/remove component from entity
        //      has component, get component
        //      add/remove system, has/get system
};

template <typename T>
void System::RequireComponent()
{
    const auto componentId = Component<T>::GetId();
    componentSignature.set(componentId);
}


#endif