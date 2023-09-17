#pragma once

#include "worldgen/ValueMap.h"

#include "lionheart/ecs/Registry.h"

class World
{
public:
	World();
	void GenerateTiles(Registry* registry, float size, int subdivisions);
private:
	ValueMap altitudeFractal;
	ValueMap precipitationFractal;
	ValueMap soilFractal;
};