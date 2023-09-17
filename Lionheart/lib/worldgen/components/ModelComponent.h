#pragma once

#include "lionheart/ecs/ComponentBase.h"
#include "lionheart/graphics/GLModel.h"

struct ModelComponent : public ComponentBase
{
	GLModel model;
};