#pragma once

#include "lionheart/ecs/ComponentBase.h"

struct TileComponent : public ComponentBase
{
	EntityID worldID;
	std::vector<EntityID> neighbours;
	float latitude;
	float longitude;
};