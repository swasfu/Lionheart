#pragma once

#include "registry/Registry.h"
#include "registry/ComponentBase.h"
#include "parsing/XMLParser.h"

struct BodyPartFunctionComponent : public ComponentBase
{
	EntityID function_;
	float weight_;
};

template <>
inline void BuildComponent<BodyPartFunctionComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<BodyPartFunctionComponent>(entityID);
	registryPtr->LodgeUnresolvedID(&newComponentPtr->function_, std::string(node.child_value("function")));
	newComponentPtr->weight_ = node.child("weight").text().as_float();
}