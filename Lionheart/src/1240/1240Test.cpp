#include "registry/Registry.h"
#include "parsing/XMLParser.h"
#include "1240/components/FlagsComponent.h"
#include "1240/components/TransformComponent.h"
#include "1240/components/RotationComponent.h"

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

	registryPtr->GetComponentMap<FlagsComponent>();

	for (const auto& file : std::filesystem::recursive_directory_iterator("data\\"))
	{
		std::string path = file.path().string();
		if (EndsWith(path, ".xml")) parser.ParseFile(path);
	}

	registryPtr->ResolveIDs();

	return 0;
}