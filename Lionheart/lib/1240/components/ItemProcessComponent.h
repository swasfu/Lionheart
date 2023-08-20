#pragma once

#include "registry/ComponentBase.h"

#include <string>
#include <vector>

struct ItemProcessComponent : ComponentBase
{
	std::vector<std::string> inputStringIDs_;
	std::vector<int> inputQuantities_;
	std::vector<std::string> outputStringIDs_;
	std::vector<int> outputQuantities_;
	std::string processStringID_;
	std::string workplaceStringID_;
};