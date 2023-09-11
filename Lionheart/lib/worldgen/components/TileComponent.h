#pragma once

#include "lionheart/ecs/registry/ComponentBase.h"
#include "lionheart/parsing/XMLParser.h"

struct TileComponent : public ComponentBase
{
	std::vector<EntityID> neighbours;
};