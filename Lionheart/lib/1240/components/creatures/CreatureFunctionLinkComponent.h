#pragma once

#include "registry/Registry.h"
#include "registry/ComponentBase.h"
#include "parsing/XMLParser.h"

struct CreatureFunctionLinkComponent : public ComponentBase
{
	EntityID otherFunctionID_;
	float weight_;
};

template <>
inline void BuildComponent<CreatureFunctionLinkComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<CreatureFunctionLinkComponent>(entityID);
	registryPtr->LodgeUnresolvedID(&newComponentPtr->otherFunctionID_, std::string(node.child_value("otherFunction")));
	newComponentPtr->weight_ = node.child("weight").text().as_float();
}