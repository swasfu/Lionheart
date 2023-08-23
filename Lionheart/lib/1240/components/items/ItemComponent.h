#pragma once

#include "registry/Registry.h"
#include "registry/ComponentBase.h"
#include "parsing/XMLParser.h"

#include <string>

struct ItemComponent : public ComponentBase
{
	std::string name_;
};

template <>
inline void BuildComponent<ItemComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<ItemComponent>(entityID);
	newComponentPtr->name_ = std::string(node.child_value("name"));
}