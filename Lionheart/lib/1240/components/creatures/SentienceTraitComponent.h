#pragma once

#include "registry/Registry.h"
#include "registry/ComponentBase.h"
#include "parsing/XMLParser.h"

#include <string>

struct SentienceTraitComponent : public ComponentBase
{
	std::string name_;
};

template <>
inline void BuildComponent<SentienceTraitComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<SentienceTraitComponent>(entityID);

}