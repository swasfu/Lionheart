#include "lionheart/ecs/Scheduler.h"

void Scheduler::ScheduleSystem(System system, int tickRate)
{
	systems_[system] = Timer{ 0, tickRate };
}

void Scheduler::RemoveSystem(System system)
{
	auto iter = systems_.find(system);
	if (iter != systems_.end()) systems_.erase(iter);
}

void Scheduler::Tick(Registry* registry)
{
	for (auto& pair : systems_)
	{
		pair.second.tickCount++;
		if (pair.second.tickCount >= pair.second.tickRate)
		{
			pair.first(registry);
			pair.second.tickCount = 0;
		}
	}
}