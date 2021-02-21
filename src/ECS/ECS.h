#ifndef ECS_H
#define ECS_H

#include <stdint.h>
#include <bitset>
#include <vector>

const uint32_t MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;

class Component
{

};

class Entity
{
    private:
        int id;
    public:
        Entity(int32_t id): id(id) {};
        int GetId() const;
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
    Signature& GetComponentSignature() const;
};

class Registry
{

};

#endif