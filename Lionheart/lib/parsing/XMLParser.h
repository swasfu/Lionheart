#pragma once

#include "../registry/registry.h"

#include "../thirdparty/pugixml/pugixml.hpp"
#include "registry/NameComponent.h"
#include "registry/FlagsComponent.h"

#include <string>
#include <memory>
#include <functional>

using BuildComponentFunction = std::function<void(Registry*, EntityID, pugi::xml_node)>;

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
	static const std::string dataDirectory_;
};

template <typename T>
void BuildComponent(Registry* registryPtr, EntityID entityID, pugi::xml_node)
{
	registryPtr->AddComponent<T>(entityID);
}

template <>
inline void BuildComponent<NameComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node node)
{
	auto nameComponent = registryPtr->AddComponent<NameComponent>(entityID);
	nameComponent->name_ = std::string(node.text().get());
}

template <>
inline void BuildComponent<FlagsComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node node)
{
	auto flagsComponent = registryPtr->AddComponent<FlagsComponent>(entityID);
	for (auto& flag : node)
	{
		flagsComponent->flags_.push_back(std::string(flag.text().get()));
	}
}

template <typename T>
void XMLParser::RegisterComponent(std::string tagName)
{
	componentNameToBuildFunction_[tagName] = &BuildComponent<T>;
}