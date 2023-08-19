#include "registry/Registry.h"
#include "1240/components/NameComponent.h"
#include "1240/components/StringIDComponent.h"
#include "1240/components/FlagsComponent.h"
#include "1240/components/TransformComponent.h"
#include "1240/components/RotationComponent.h"
#include "1240/parsing/1240XMLBuilders.h"

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
	parser.RegisterComponent<NameComponent>("name");
	parser.RegisterComponent<StringIDComponent>("id");
	parser.RegisterComponent<FlagsComponent>("flags");
	parser.RegisterComponent<TransformComponent>("transform");
	parser.RegisterComponent<RotationComponent>("rotation");

	for (const auto& file : std::filesystem::recursive_directory_iterator("data\\"))
	{
		std::string path = file.path().string();
		//std::cout << path << std::endl;
		if (EndsWith(path, ".xml"))
		{
			parser.ParseFile(path);
		}
	}

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