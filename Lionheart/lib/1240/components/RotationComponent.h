#pragma once

#include "registry/Registry.h"
#include "registry/ComponentBase.h"
#include "parsing/XMLParser.h"

struct RotationComponent : public ComponentBase
{
	float x_;
	float y_;
	float z_;
};

template <>
inline void BuildComponent<RotationComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<RotationComponent>(entityID);
	newComponentPtr->x_ = node.child("x").text().as_float();
	newComponentPtr->y_ = node.child("y").text().as_float();
	newComponentPtr->z_ = node.child("z").text().as_float();
}