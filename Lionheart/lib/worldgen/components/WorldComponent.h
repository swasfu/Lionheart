#pragma once

#include "lionheart/ecs/ComponentBase.h"

#include <string>

struct WorldComponent : public ComponentBase
{
	std::string name;
};