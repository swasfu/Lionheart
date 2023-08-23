#pragma once

#include "registry/Registry.h"
#include "registry/ComponentBase.h"
#include "parsing/XMLParser.h"

#include <vector>
#include <string>

struct FlagsComponent : public ComponentBase
{
	std::vector<std::string> flags_;
};

template <>
inline void BuildComponent<FlagsComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<FlagsComponent>(entityID);
	for (auto& flag : node)
	{
		newComponentPtr->flags_.push_back(std::string(flag.text().get()));
	}
}