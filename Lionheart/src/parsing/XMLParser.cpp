#include "parsing/XMLParser.h"

#include <fstream>
#include <iostream>

XMLParser::XMLParser(Registry* r) : registryPtr_{ r } {}

void XMLParser::ParseFile(std::string filename)
{
	pugi::xml_document doc;

	std::ifstream stream(filename);

	if (stream.fail())
	{
		std::cout << filename << ": Failed to open file" << std::endl;
		return;
	}

	pugi::xml_parse_result result = doc.load(stream);
	auto defs = doc.first_child().children();

	for (auto& def : defs)
	{
		auto entityID = registryPtr_->RegisterEntity();

		for (auto& component : def)
		{
			const std::string componentName = std::string(component.name());
			if (componentNameToBuildFunction_.find(componentName) != componentNameToBuildFunction_.end())
			{
				componentNameToBuildFunction_[componentName](registryPtr_, entityID, component);
			} else
			{
				std::cout << filename << ": Unknown component type \"" << componentName << "\" added in " << def.name() << std::endl;
			}
		}
	}
}