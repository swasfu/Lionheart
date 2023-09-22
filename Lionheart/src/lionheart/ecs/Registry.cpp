#include "lionheart/ecs/Registry.h"

EntityID Registry::RegisterEntity()
{
	return entityCount_++;
}

EntityID Registry::RegisterEntity(std::string stringID)
{
	EntityID newEntityID = entityCount_++;
	stringIDsToEntityIDs_[stringID] = newEntityID;
	return newEntityID;
}

void Registry::RemoveEntity(EntityID id)
{
	for (auto componentTypeIterator = componentTypeToMap_.begin(); componentTypeIterator != componentTypeToMap_.end();)
	{
		auto& componentMap = componentTypeIterator->second;
		auto iter = componentMap.find(id);
		if (iter != componentMap.end()) componentMap.erase(iter);
		if (componentTypeIterator->second.size() == 0) componentTypeIterator = componentTypeToMap_.erase(componentTypeIterator);
		else componentTypeIterator++;
	}
}

void Registry::LodgeUnresolvedID(EntityID* entityIDPtr, std::string stringID)
{
	unresolvedEntityIDPtrsToStringIDs_[entityIDPtr] = stringID;
}

void Registry::ResolveIDs()
{
	for (const auto& pair : unresolvedEntityIDPtrsToStringIDs_)
	{
		(*pair.first) = stringIDsToEntityIDs_[pair.second];
	}
}

void Registry::FlushResolver()
{
	unresolvedEntityIDPtrsToStringIDs_.clear();
}

EntityID Registry::GetEntityCount()
{
	return entityCount_;
}
