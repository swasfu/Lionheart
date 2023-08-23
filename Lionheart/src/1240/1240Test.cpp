#include "registry/Registry.h"
#include "parsing/XMLParser.h"
#include "1240/components/FlagsComponent.h"
#include "1240/components/TransformComponent.h"
#include "1240/components/RotationComponent.h"
#include "1240/components/StrategyPOIComponent.h"

#include <iostream>
#include <filesystem>

inline bool EndsWith(std::string const& string, std::string const& ending)
{
	if (ending.size() > string.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), string.rbegin());
}

int main()
{
	auto registryPtr = std::make_unique<Registry>();
	XMLParser parser(registryPtr.get());
	parser.RegisterComponent<FlagsComponent>("flags");
	parser.RegisterComponent<TransformComponent>("transform");
	parser.RegisterComponent<RotationComponent>("rotation");
	parser.RegisterComponent<StrategyPOIComponent>("strategyPOI");

	for (const auto& file : std::filesystem::recursive_directory_iterator("data\\"))
	{
		std::string path = file.path().string();
		if (EndsWith(path, ".xml")) parser.ParseFile(path);
	}

	registryPtr->ResolveIDs();

	for (EntityID entityID = 0; entityID < registryPtr->GetEntityCount(); entityID++)
	{
		if (auto ref = registryPtr->GetComponent<StrategyPOIComponent>(entityID))
		{
			std::cout << "Test: " << ref->name_ << std::endl;
		}
	}

	return 0;
}