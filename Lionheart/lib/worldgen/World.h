#pragma once

#include "worldgen/ValueMap.h"

#include "lionheart/ecs/Registry.h"

class World
{
public:
	World();
	void GenerateTiles(Registry* registry, float size, int subdivisions);
	void GenerateClouds(Registry* registry, float size, int subdivisions);
	void UpdateTemperature(glm::vec3 sunDirection);
	void UpdatePrecipitation();
private:
	ValueMap altitudeFractal;
	ValueMap precipitation;
	ValueMap soilFractal;
};