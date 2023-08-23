#pragma once

#include "registry/Registry.h"
#include "registry/ComponentBase.h"
#include "parsing/XMLParser.h"

#include <string>

struct StrategyPOIComponent : public ComponentBase
{
	std::string name_;
	double x_;
	double y_;
};

template <>
inline void BuildComponent<StrategyPOIComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<StrategyPOIComponent>(entityID);
	newComponentPtr->name_ = std::string(node.child_value("name"));
	newComponentPtr->x_ = node.child("x").text().as_float();
	newComponentPtr->y_ = node.child("y").text().as_float();
}