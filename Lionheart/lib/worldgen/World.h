#pragma once

#include "worldgen/ValueMap.h"

#include "lionheart/ecs/Registry.h"

void GenerateWorld(Registry* registry, float size, int subdivisions, int resolution);