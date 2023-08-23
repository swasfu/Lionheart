#pragma once

#include "registry/Registry.h"
#include "registry/ComponentBase.h"
#include "parsing/XMLParser.h"

struct MassUnitComponent : public ComponentBase
{
	std::string singular_;
	std::string plural_;
	float mass_;
};

template <>
inline void BuildComponent<MassUnitComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<MassUnitComponent>(entityID);
	newComponentPtr->singular_ = std::string(node.child_value("singular"));
	newComponentPtr->plural_ = std::string(node.child_value("plural"));
	newComponentPtr->mass_ = node.child("mass").text().as_float();
}