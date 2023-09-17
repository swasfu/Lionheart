#pragma once

#include "ecs/registry/Registry.h"

class SystemBase
{
public:
	virtual void Tick(Registry*) = 0;
};