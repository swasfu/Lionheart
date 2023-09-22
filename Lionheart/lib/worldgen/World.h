#pragma once

#include "worldgen/ValueMap.h"

#include "lionheart/ecs/Registry.h"

class World
{
public:
	World();
	glm::vec3 DetermineBiome(float latitude, float longitude);
	void GenerateTiles(Registry* registry, float size, int subdivisions);
	void GenerateClouds(Registry* registry, float size, int subdivisions);
	void GenerateClimateMap();
	void UpdateTemperature(glm::vec3 sunDirection, float deltaT);
	void UpdateTemperatureModel(Registry* registry);
	void UpdatePrecipitation(Registry* registry, EntityID cloudsID);
private:
	ValueMap altitudeMap;
	ValueMap averageTemperatureMap;
	ValueMap averagePrecipitationMap;
	ValueMap soilMap;
	ValueMap precipitationMap;
	ValueMap temperatureMap;
	ValueMap cloudMap;
	float seaLevel;
};