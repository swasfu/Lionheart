#pragma once

#include "registry/Registry.h"
#include "registry/ComponentBase.h"
#include "parsing/XMLParser.h"

struct BodyPartComponent : public ComponentBase
{
	std::string name_;
};

template <>
inline void BuildComponent<BodyPartComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<BodyPartComponent>(entityID);
	newComponentPtr->name_ = std::string(node.child_value("name"));
}