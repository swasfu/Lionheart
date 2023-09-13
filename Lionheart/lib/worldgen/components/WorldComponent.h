#pragma once

#include "lionheart/ecs/registry/ComponentBase.h"
#include "lionheart/parsing/XMLParser.h"

struct WorldComponent : public ComponentBase
{
	std::vector<EntityID> neighbours;
};