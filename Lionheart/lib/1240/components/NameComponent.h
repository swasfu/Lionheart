#pragma once

#include "registry/ComponentBase.h"

#include <string>

struct NameComponent : public ComponentBase
{
	std::string name_;
};