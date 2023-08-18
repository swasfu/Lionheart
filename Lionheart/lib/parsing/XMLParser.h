#pragma once

#include "../registry/registry.h"
#include "registry/NameComponent.h"
#include "registry/FlagsComponent.h"

#include "../thirdparty/pugixml/pugixml.hpp"

#include <string>
#include <memory>
#include <functional>

using BuildComponentFunction = std::function<void(Registry*, EntityID, pugi::xml_node)>;

class XMLParser
{
public:
	XMLParser(Registry*);
	void ParseFile(std::string);
	void RegisterComponents();
private:
	Registry* registryPtr_;
	std::unordered_map<std::string, BuildComponentFunction> componentNameToAddFunction_;
};

template <typename T>
void BuildComponent(Registry* registryPtr, EntityID entityID, pugi::xml_node)
{
	registryPtr->AddComponent<T>(entityID);
}