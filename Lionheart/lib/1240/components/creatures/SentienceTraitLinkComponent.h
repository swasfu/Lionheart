#pragma once

#include "registry/Registry.h"
#include "registry/ComponentBase.h"
#include "parsing/XMLParser.h"

struct SentienceTraitLinkComponent : public ComponentBase
{
	EntityID traitID_;
};

template <>
inline void BuildComponent<SentienceTraitLinkComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<SentienceTraitLinkComponent>(entityID);
	registryPtr->LodgeUnresolvedID(&newComponentPtr->traitID_, std::string(node.child_value("trait")));
}