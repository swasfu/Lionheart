#pragma once

#include "registry/Registry.h"
#include "registry/ComponentBase.h"
#include "parsing/XMLParser.h"

struct BodyPartAttachedComponent : public ComponentBase
{
	EntityID attached_;
};

template <>
inline void BuildComponent<BodyPartAttachedComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<BodyPartAttachedComponent>(entityID);
	registryPtr->LodgeUnresolvedID(&newComponentPtr->attached_, std::string(node.child_value("attached")));
}