#pragma once

#include "registry/Registry.h"
#include "registry/ComponentBase.h"
#include "parsing/XMLParser.h"

struct CreatureFunctionComponent : public ComponentBase
{
	std::string name_;
};

template <>
inline void BuildComponent<CreatureFunctionComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<CreatureFunctionComponent>(entityID);
	newComponentPtr->name_ = std::string(node.child_value("name"));
}