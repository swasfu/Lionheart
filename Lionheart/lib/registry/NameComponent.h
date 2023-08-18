#pragma once

#include "ComponentBase.h"

#include <string>

struct NameComponent : public ComponentBase
{
	NameComponent();
	~NameComponent();
	std::string name_;
};