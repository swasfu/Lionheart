#pragma once

#include "registry/ComponentBase.h"

struct TransformComponent : public ComponentBase
{
	double x_;
	double y_;
	double z_;
};