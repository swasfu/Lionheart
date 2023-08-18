#pragma once

#include "../registry/registry.h"

#include "../thirdparty/pugixml/pugixml.hpp"

#include <string>
#include <memory>

class XMLParser
{
public:
	XMLParser(Registry*);
	void ParseFile(std::string);
private:
	Registry* registryPtr_;
	void ParseItems(pugi::xml_node&);
};