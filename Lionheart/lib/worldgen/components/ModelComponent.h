#pragma once

#include "lionheart/ecs/registry/ComponentBase.h"
#include "lionheart/graphics/GLModel.h"

struct ModelComponent : public ComponentBase
{
	GLModel model;
};