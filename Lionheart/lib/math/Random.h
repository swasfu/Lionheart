#pragma once

#include<random>
#include<string>

namespace Random
{
	void Seed();
	void Seed(std::string seed);

	float RandomFloat(float min, float max);
}