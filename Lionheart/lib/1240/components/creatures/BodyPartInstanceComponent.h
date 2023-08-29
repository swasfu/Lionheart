#pragma once

#include "registry/Registry.h"
#include "registry/ComponentBase.h"
#include "parsing/XMLParser.h"

struct BodyPartInstanceComponent : public ComponentBase
{
	EntityID bodyPartID_;
	float hitPoints_;
};