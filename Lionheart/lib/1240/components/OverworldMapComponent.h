#pragma once

#include "registry/ComponentBase.h"

struct OverworldMapComponent : public ComponentBase
{
	double* heightMap_;
	int length_;
	int width_;
};