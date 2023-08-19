#pragma once

#include "registry/ComponentBase.h"

#include <string>

struct StringIDComponent : public ComponentBase
{
	std::string stringID_;
};