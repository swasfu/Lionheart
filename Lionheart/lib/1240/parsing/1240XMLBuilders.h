#pragma once

#include "../../parsing/XMLParser.h"
#include "../components/NameComponent.h"
#include "../components/StringIDComponent.h"
#include "../components/FlagsComponent.h"
#include "../components/TransformComponent.h"
#include "../components/RotationComponent.h"

#include <iostream>

template <>
inline void BuildComponent<NameComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<NameComponent>(entityID);
	newComponentPtr->name_ = std::string(node.text().get());
}

template <>
inline void BuildComponent<StringIDComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<StringIDComponent>(entityID);
	newComponentPtr->stringID_ = std::string(node.text().get());
}

template <>
inline void BuildComponent<FlagsComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<FlagsComponent>(entityID);
	for (auto& flag : node)
	{
		newComponentPtr->flags_.push_back(std::string(flag.text().get()));
	}
}

template <>
inline void BuildComponent<TransformComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<TransformComponent>(entityID);
	newComponentPtr->x_ = node.child("x").text().as_double();
	newComponentPtr->y_ = node.child("y").text().as_double();
	newComponentPtr->z_ = node.child("z").text().as_double();
}

template <>
inline void BuildComponent<RotationComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<RotationComponent>(entityID);
	newComponentPtr->x_ = node.child("x").text().as_double();
	newComponentPtr->y_ = node.child("y").text().as_double();
	newComponentPtr->z_ = node.child("z").text().as_double();
}