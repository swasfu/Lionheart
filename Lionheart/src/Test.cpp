#include "registry/Registry.h"
#include "registry/NameComponent.h"
#include "registry/FlagsComponent.h"
#include "parsing/XMLParser.h"

#include <iostream>

int main()
{
	auto registryPtr = std::make_unique<Registry>();
	XMLParser parser(registryPtr.get());
	parser.RegisterComponent<NameComponent>("name");
	parser.RegisterComponent<FlagsComponent>("flags");

	parser.ParseFile("item/items.xml");

	std::cout << "Named entities:" << std::endl;
	for (auto entityID : registryPtr->ViewIDs<NameComponent>())
	{
		std::cout << entityID << " - " << registryPtr->GetComponent<NameComponent>(entityID)->name_ << std::endl;
		std::cout << "Flags:" << std::endl;
		if (auto flags = registryPtr->GetComponent<FlagsComponent>(entityID))
		{
			for (auto& flag : flags->flags_) std::cout << flag << std::endl;
		} else std::cout << "None" << std::endl;
		std::cout << std::endl;
	}

	return 0;
}