#include "registry/registry.h"

Registry::Registry() {}

EntityID Registry::RegisterEntity(std::string id)
{
	auto newEntityID = ids_.size();

	ids_[id] = newEntityID;

	return newEntityID;
}
