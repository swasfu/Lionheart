#pragma once

#include "worldgen/ValueMap.h"

#include "lionheart/ecs/ComponentBase.h"

#include <thirdparty/glm/glm.hpp>
#include <thirdparty/glm/gtc/quaternion.hpp>

#include <string>

struct CelestialBodyComponent : public ComponentBase
{
	std::string name;
	EntityID bodyModelID;
	ValueMap topography;
	glm::dvec3 position;
	glm::vec3 velocity;
	glm::quat rotation;
	float mass;
	float rotationSpeed;
	int mapResolution;
};