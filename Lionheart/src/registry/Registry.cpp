#include "registry/registry.h"

Registry::Registry() {}

EntityID Registry::RegisterEntity(std::string id)
{
	auto newEntityID = stringIDtoEntityID_.size();

	stringIDtoEntityID_[id] = newEntityID;

	return newEntityID;
}
