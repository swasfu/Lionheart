#include "parsing/XMLParser.h"

#include <fstream>
#include <iostream>

XMLParser::XMLParser(Registry* r) : registryPtr_{ r } {}

template <>
void BuildComponent<NameComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node node)
{
	auto nameComponent = registryPtr->AddComponent<NameComponent>(entityID);
	nameComponent->name_ = std::string(node.text().get());
}

template <>
void BuildComponent<FlagsComponent>(Registry* registryPtr, EntityID entityID, pugi::xml_node node)
{
	auto flagsComponent = registryPtr->AddComponent<FlagsComponent>(entityID);
	for (auto& flag : node)
	{
		flagsComponent->flags_.push_back(std::string(node.text().get()));
	}
}

void XMLParser::RegisterComponents()
{
	componentNameToAddFunction_["name"] = &BuildComponent<NameComponent>;
	componentNameToAddFunction_["flags"] = &BuildComponent<FlagsComponent>;
}

void XMLParser::ParseFile(std::string filename)
{
	pugi::xml_document doc;

	std::ifstream stream(filename);

	if (stream.fail())
	{
		std::cout << "Failed to open file " << filename << "!" << std::endl;
		return;
	}

	pugi::xml_parse_result result = doc.load(stream);

	for (auto def : doc)
	{
		auto id = std::string(def.attribute("id").value());
		auto entityID = registryPtr_->RegisterEntity(id);

		for (auto component : def)
		{
			std::string componentName = std::string(component.name());
			componentNameToAddFunction_[componentName](registryPtr_, entityID, component);
		}
	}
}