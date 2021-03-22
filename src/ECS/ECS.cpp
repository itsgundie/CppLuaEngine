
#include "ECS.h"
#include "Logger.h"

int32_t IComponent::nextId = 0;

int32_t Entity::GetId() const
{
    return(id);
}


void System::AddEntityToSystem(Entity enitity)
{
    entities.push_back(enitity);
};



void System::RemoveEntityFromSystem(Entity entity)
{
    // this is some ridiculous syntax
    entities.erase(std::remove_if(entities.begin(), entities.end(), 
    [&entity](Entity other)
    {
        return(entity == other);
    }), 
    entities.end());
};

std::vector<Entity> System::GetSystemEntities() const
{
    return(entities);
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
    entity.registry = this;
    entitiesToBeAdded.insert(entity);

    if(entityId >= entityComponentSignatures.size())
        entityComponentSignatures.resize(entityId + 1);

    Logger::Log("Created entity, ID == " + std::to_string(entityId));
    return(entity);
}

void Registry::AddEntityToSystems(Entity entity)
{
    const int32_t entityId = entity.GetId();

    const auto& entComponentSignature = entityComponentSignatures[entityId];

    for (auto& system: systems)
    {
        const auto& sysComponentSignature = system.second->GetComponentSignature();
        // Bitwise AND comparison to check parity of signatures
        bool isIntrested = (entComponentSignature & sysComponentSignature) == sysComponentSignature;

        if(isIntrested)
            system.second->AddEntityToSystem(entity);
    }
}

void Registry::Update()
{
    for(auto thisEntity: entitiesToBeAdded)
    {
        AddEntityToSystems(thisEntity);
    }
    entitiesToBeAdded.clear();
}

