#pragma once

#include "registry/Registry.h"
#include "registry/ComponentBase.h"
#include "parsing/XMLParser.h"

struct SentienceTraitInstanceComponent : public ComponentBase
{
	EntityID traitID_;
	float value_;
};