#pragma once

#include "lionheart/ecs/registry/registry.h"

#include "thirdparty/pugixml/pugixml.hpp"

#include <string>
#include <memory>
#include <functional>

using BuildComponentFunction = std::function<void(Registry*, EntityID, pugi::xml_node&)>;

class XMLParser
{
public:
	XMLParser(Registry*);
	void ParseFile(std::string);
	template <typename T>
	void RegisterComponent(std::string);
private:
	Registry* registryPtr_;
	std::unordered_map<std::string, BuildComponentFunction> componentNameToBuildFunction_;
};

template <typename T>
void BuildComponent(Registry* registryPtr, EntityID entityID, pugi::xml_node& node)
{
	registryPtr->AddComponent<T>(entityID);
}

template <typename T>
void XMLParser::RegisterComponent(std::string tagName)
{
	componentNameToBuildFunction_[tagName] = &BuildComponent<T>;
}