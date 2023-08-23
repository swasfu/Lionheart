#pragma once

#include "registry/Registry.h"
#include "registry/ComponentBase.h"
#include "parsing/XMLParser.h"

struct OverworldMapComponent : public ComponentBase
{
	double* heightMap_;
	int length_;
	int width_;
};