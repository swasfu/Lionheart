#pragma once

#include "lionheart/ecs/ComponentBase.h"

struct CloudComponent : public ComponentBase
{
	float latitude;
	float longitude;
};