#pragma once

#include "registry/Registry.h"
#include "registry/ComponentBase.h"
#include "parsing/XMLParser.h"

#include <string>

struct OverworldPOIComponent : public ComponentBase
{
	std::string name_;
	float x_;
	float y_;
};

template <>
inline void BuildComponent<OverworldPOIComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<OverworldPOIComponent>(entityID);
	newComponentPtr->name_ = std::string(node.child_value("name"));
	newComponentPtr->x_ = node.child("x").text().as_float();
	newComponentPtr->y_ = node.child("y").text().as_float();
}