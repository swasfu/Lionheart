#include "registry/Registry.h"
#include "registry/NameComponent.h"
#include "registry/FlagsComponent.h"
#include "parsing/XMLParser.h"

#include <iostream>

int main()
{
	auto registryPtr = std::make_unique<Registry>();
	XMLParser parser(registryPtr.get());
	parser.RegisterComponents();

	parser.ParseFile("data/item/items.xml");

	auto testComponentPtr = registryPtr->GetComponent<NameComponent>(0);
	std::cout << "Test: " << testComponentPtr->name_ << std::endl;
	registryPtr->View<NameComponent>();
	registryPtr->ViewIDs<NameComponent>();
	std::cout << "Items with both name and flags component:" << std::endl;
	for (auto id : registryPtr->ViewIDs<NameComponent, FlagsComponent>()) std::cout << id << std::endl;
	registryPtr->GetComponentMap<NameComponent>();

	return 0;
}