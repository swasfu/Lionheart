#pragma once

#include "registry/ComponentBase.h"

struct RotationComponent : public ComponentBase
{
	double x_;
	double y_;
	double z_;
};