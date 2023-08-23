#pragma once

#include "registry/Registry.h"
#include "registry/ComponentBase.h"
#include "parsing/XMLParser.h"

struct VolumeUnitComponent : public ComponentBase
{
	std::string singular_;
	std::string plural_;
	float volume_;
};

template <>
inline void BuildComponent<VolumeUnitComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<VolumeUnitComponent>(entityID);
	newComponentPtr->singular_ = std::string(node.child_value("singular"));
	newComponentPtr->plural_ = std::string(node.child_value("plural"));
	newComponentPtr->volume_ = node.child("volume").text().as_float();
}