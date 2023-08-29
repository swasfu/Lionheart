#pragma once

#include "registry/Registry.h"
#include "registry/ComponentBase.h"
#include "parsing/XMLParser.h"

struct OrganComponent : public ComponentBase
{
	EntityID within_;
};

template <>
inline void BuildComponent<OrganComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<OrganComponent>(entityID);
	registryPtr->LodgeUnresolvedID(&newComponentPtr->within_, std::string(node.child_value("within")));
}