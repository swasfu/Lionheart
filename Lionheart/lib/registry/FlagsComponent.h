#pragma once

#include "ComponentBase.h"

#include <vector>
#include <string>

struct FlagsComponent : public ComponentBase
{
	std::vector<std::string> flags_;
};