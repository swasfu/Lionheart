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
