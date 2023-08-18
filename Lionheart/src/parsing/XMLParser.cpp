#include "XMLParser.h"

#include "registry/NameComponent.h"

#include <fstream>
#include <iostream>

XMLParser::XMLParser(Registry* r) : registryPtr_{ r } {}

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

	for (auto& child : doc)
	{
		//std::cout << "Reading child " << child.name() << std::endl;
		if (std::string(child.name()) == "items")
		{
			//std::cout << "Found items!" << std::endl;
			ParseItems(child);
		}
	}
}

void XMLParser::ParseItems(pugi::xml_node& items)
{
	for (const auto& item : items)
	{
		//std::cout << "Reading " << item.name() << " " << item.attribute("id").value() << std::endl;

		auto id = std::string(item.attribute("id").value());
		auto intID = registryPtr_->RegisterEntity(id);

		for (const auto& child : item)
		{
			//std::cout << "Examining " << child.name() << " attribute" << std::endl;
			for (const auto& attribute : child.attributes())
			{
				//std::cout << attribute.name() << ": " << attribute.value() << std::endl;
			}

			if (std::string(child.name()) == "name")
			{
				auto itemNameComponent = registryPtr_->AddComponent<NameComponent>(intID);
				itemNameComponent->name_ = std::string(item.child_value("name"));
			}
		}
	}
}
