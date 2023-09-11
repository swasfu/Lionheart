#pragma once

#include "math/polygons/Polyhedron.h"
#include "lionheart/ecs/registry/Registry.h"

#include <vector>
#include <memory>

class World
{
public:
	static void GenerateTiles(Registry* registry, float size, int subdivisions);
};