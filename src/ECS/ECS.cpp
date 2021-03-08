
#include "ECS.h"
#include "Logger.h"

int32_t Entity::GetId() const
{
    return(id);
}


void System::AddEnitityToSystem(Entity enitity)
{
    entities.push_back(enitity);
};



void System::RemoveEntityFromSystem(Entity entity)
{
    // this is some ridiculous syntax
    entities.erase(std::remove_if(entities.begin(), entities.end(), 
    [&entity](Entity other)
    {
        return(entity == other)
    }), 
    entities.end());
};

std::vector<Entity> System::GetSystemEntities() const
{
    return(enitites);
};


const Signature& System::GetComponentSignature() const
{
    return(componentSignature);
};

Entity Registry::CreateEntity()
{
    int32_t entityId;

    entityId = numEntities++;

    Entity entity(entityId);
    entitiesToBeAdded.insert(entity);
    Logger::Log("Created entity, ID == " + std::to_string(entityId));
    return(entity);
}

void Registry::Update()
{
    // Actually remove and add entities from sets
}

