#pragma once

#include "worldgen/ValueMap.h"

#include "lionheart/ecs/ComponentBase.h"

#include <thirdparty/glm/glm.hpp>

#include <string>

struct CelestialBodyComponent : public ComponentBase
{
	std::string name;
	EntityID bodyModel;
	ValueMap topography;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 rotationAxis;
	float mass;
	float rotationSpeed;
	int mapResolution;
};