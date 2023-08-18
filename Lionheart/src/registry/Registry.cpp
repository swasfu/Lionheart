#include "registry/registry.h"

Registry::Registry()
{}

unsigned int Registry::RegisterEntity(std::string id)
{
	unsigned int int_id = ids_.size();

	ids_[id] = int_id;

	return int_id;
}
