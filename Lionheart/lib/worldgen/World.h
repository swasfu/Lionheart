#pragma once

#include "lionheart/ecs/Registry.h"

class World
{
public:
	static void GenerateTiles(Registry* registry, float size, int subdivisions);
};