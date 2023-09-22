#pragma once

#include "lionheart/ecs/Registry.h"

#include <map>

typedef void (*System)(Registry*);

struct Timer
{
	int tickCount;
	int tickRate;
};

class Scheduler
{
public:
	void ScheduleSystem(System system, int tickRate);
	void RemoveSystem(System system);
	void Tick(Registry* registry);
private:
	std::unordered_map<System, Timer> systems_;
};