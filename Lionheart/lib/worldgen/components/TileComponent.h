#pragma once

#include "lionheart/ecs/registry/ComponentBase.h"
#include "lionheart/parsing/XMLParser.h"

struct TileComponent : public ComponentBase
{
	EntityID worldID;
	std::vector<EntityID> neighbours;
	glm::vec3 normal;
	float altitude;
};