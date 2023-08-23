#pragma once

#include "registry/Registry.h"

class SystemBase
{
public:
	virtual void Tick(Registry*) = 0;
};