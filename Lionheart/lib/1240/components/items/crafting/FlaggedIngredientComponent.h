#pragma once

#include "registry/Registry.h"
#include "registry/ComponentBase.h"
#include "parsing/XMLParser.h"

#include <string>

struct SpecificIngredientComponent : public ComponentBase
{
	std::string ingredientFlag_;
	int quantity_;
};

template <>
inline void BuildComponent<SpecificIngredientComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	auto newComponentPtr = registryPtr->AddComponent<SpecificIngredientComponent>(entityID);
	newComponentPtr->ingredientFlag_ = std::string(node.child_value("flag"));
	newComponentPtr->quantity_ = node.child("quantity").text().as_int();
}