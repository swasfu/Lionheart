#pragma once

#include "registry/Registry.h"
#include "registry/ComponentBase.h"
#include "parsing/XMLParser.h"

#include <string>
#include <vector>

struct SentienceTemplateComponent : public ComponentBase
{
	std::string name_;
};

template <>
inline void BuildComponent<SentienceTemplateComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<SentienceTemplateComponent>(entityID);
	newComponentPtr->name_ = std::string(node.child_value("name"));
}