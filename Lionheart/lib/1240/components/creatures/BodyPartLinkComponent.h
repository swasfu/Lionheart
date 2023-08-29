#pragma once

#include "registry/Registry.h"
#include "registry/ComponentBase.h"
#include "parsing/XMLParser.h"

struct BodyPartLinkComponent : public ComponentBase
{
	EntityID bodyPartID_;
};

template <>
inline void BuildComponent<BodyPartLinkComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<BodyPartLinkComponent>(entityID);
	registryPtr->LodgeUnresolvedID(&newComponentPtr->bodyPartID_, std::string(node.child_value("bodyPart")));
}