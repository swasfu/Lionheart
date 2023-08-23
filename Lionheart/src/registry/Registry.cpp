#include "registry/registry.h"

EntityID Registry::RegisterEntity(std::string stringID)
{
	EntityID newEntityID = entityCount_++;
	if (stringID != "") stringIDsToEntityIDs_[stringID] = newEntityID;
	return newEntityID;
}

void Registry::LodgeStringID(EntityID* entityIDPtr, std::string stringID)
{
	unresolvedEntityIDPtrsToStringIDs_[entityIDPtr] = stringID;
}

void Registry::ResolveStringIDs()
{
	for (const auto& pair : unresolvedEntityIDPtrsToStringIDs_)
	{
		(*pair.first) = stringIDsToEntityIDs_[pair.second];
	}
}

EntityID Registry::GetEntityCount()
{
	return entityCount_;
}
