#ifndef ECS_H
#define ECS_H

#include <stdint.h>
#include <bitset>
#include <vector>

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

class Registry
{

};

template <typename T>
void System::RequireComponent()
{
    const auto componentId = Component<T>::GetId();
    componentSignature.set(componentId);
}


#endif