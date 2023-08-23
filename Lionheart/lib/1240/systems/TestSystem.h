#pragma once

#include "systems/SystemBase.h"

class TestSystem : public SystemBase
{
public:
	void Tick(Registry*);
};