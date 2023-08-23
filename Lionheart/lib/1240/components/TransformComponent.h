#pragma once

#include "registry/Registry.h"
#include "registry/ComponentBase.h"
#include "parsing/XMLParser.h"

struct TransformComponent : public ComponentBase
{
	float x_;
	float y_;
	float z_;
};

template <>
inline void BuildComponent<TransformComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<TransformComponent>(entityID);
	newComponentPtr->x_ = node.child("x").text().as_float();
	newComponentPtr->y_ = node.child("y").text().as_float();
	newComponentPtr->z_ = node.child("z").text().as_float();
}