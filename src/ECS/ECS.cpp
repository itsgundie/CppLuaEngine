
#include "ECS.h"
#include "Logger.h"

int32_t IComponent::nextId = 0;

int32_t Entity::GetId() const
{
    return(id);
}

void Entity::Kill()
{
    registry->RemoveEntity(*this);
}

void Entity::Tag(const std::string& tag)
{
    registry->TagEntity(*this, tag);
}

bool Entity::HasTag(const std::string& tag) const
{
    return(registry->EntityHasTag(*this, tag));
};

void Entity::Group(const std::string& groupName)
{
    registry->GroupEntity(*this, groupName);
};

bool Entity::BelongsToGroup(const std::string& groupName) const
{
    return(registry->EntityBelongsToGroup(*this, groupName));
};


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

    if(freeIds.empty())
    {
        entityId = numEntities++;
        if(entityId >= entityComponentSignatures.size())
            entityComponentSignatures.resize(entityId + 1);
    }
    else
    {
        entityId = freeIds.front();
        freeIds.pop_front();
    }

    Entity entity(entityId);
    entity.registry = this;
    entitiesToBeAdded.insert(entity);

    if(entityId >= entityComponentSignatures.size())
        entityComponentSignatures.resize(entityId + 1);

    Logger::Log("Created entity, ID == " + std::to_string(entityId));
    return(entity);
}

void Registry::RemoveEntity(Entity entity)
{
    entitiesToBeRemoved.insert(entity);
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

void Registry::RemoveEntityFromSystems(Entity entity)
{
    for(auto system: systems)
        system.second->RemoveEntityFromSystem(entity);
}

void Registry::TagEntity(Entity entity, const std::string& tag)
{
    entitiesPerTag.emplace(tag, entity);
    tagPerEntity.emplace(entity.GetId(), tag);
};

bool Registry::EntityHasTag(Entity entity, const std::string& tag) const
{
    if (tagPerEntity.find(entity.GetId()) == tagPerEntity.end())
        return(false);
    return(entitiesPerTag.find(tag)->second == entity);
};

Entity Registry::GetEntityByTag(const std::string& tag) const
{
    return(entitiesPerTag.at(tag));
};

void Registry::RemoveEntityTag(Entity entity)
{
    auto taggedEntity = tagPerEntity.find(entity.GetId());
    if (taggedEntity != tagPerEntity.end())
    {
        auto tag = taggedEntity->second;
        entitiesPerTag.erase(tag);
        tagPerEntity.erase(taggedEntity);
    }
};

void Registry::GroupEntity(Entity entity, const std::string& group)
{
    entitiesPerGroup.emplace(group, std::set<Entity>());
    entitiesPerGroup[group].emplace(entity);
    groupPerEntity.emplace(entity.GetId(), group);
};

bool Registry::EntityBelongsToGroup(Entity entity, const std::string& group) const
{
    if (entitiesPerGroup.find(group) == entitiesPerGroup.end())
        return(false);
    auto groupEntities = entitiesPerGroup.at(group);
    return(groupEntities.find(entity.GetId()) != groupEntities.end());
};

std::vector<Entity> Registry::GetEntitiesByGroup(const std::string& group) const
{
    auto &setOfEntities = entitiesPerGroup.at(group);
    return(std::vector<Entity>(setOfEntities.begin(), setOfEntities.end()));
};

void Registry::RemoveEntityGroup(Entity entity)
{
    auto groupedEntity = groupPerEntity.find(entity.GetId());
    if (groupedEntity != groupPerEntity.end())
    {
        auto group = entitiesPerGroup.find(groupedEntity->second);
        if ( group != entitiesPerGroup.end())
        {
            auto entityInGroup = group->second.find(entity);
            if (entityInGroup != group->second.end())
                group->second.erase(entityInGroup);
        }
        groupPerEntity.erase(groupedEntity);
    }
};

void Registry::Update()
{
    for(auto thisEntity: entitiesToBeAdded)
    {
        AddEntityToSystems(thisEntity);
    }
    entitiesToBeAdded.clear();

    for(auto entity: entitiesToBeRemoved)
    {
        RemoveEntityFromSystems(entity);
        freeIds.push_back(entity.GetId());
        RemoveEntityTag(entity);
        RemoveEntityGroup(entity);
        entityComponentSignatures[entity.GetId()].reset();
    }
    entitiesToBeRemoved.clear();
}

