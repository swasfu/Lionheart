#pragma once


using EntityID = int;

struct ComponentBase
{
	ComponentBase();
	virtual ~ComponentBase();
	EntityID id_;
};