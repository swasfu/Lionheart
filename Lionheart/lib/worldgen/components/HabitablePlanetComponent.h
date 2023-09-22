#pragma once

#include "worldgen/ValueMap.h"

#include "lionheart/ecs/ComponentBase.h"

#include <string>

struct HabitablePlanetComponent : public ComponentBase
{
	EntityID cloudsModelID;
	ValueMap temperature;
	ValueMap humidity;
	ValueMap moisture;
	float seaLevel;
};