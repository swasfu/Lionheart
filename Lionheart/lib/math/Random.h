#pragma once

#include<random>
#include<string>

namespace Random
{
	void Seed()
	{
		srand(time(nullptr));
	}

	void Seed(std::string seed)
	{
		srand(std::hash<std::string>{}(seed));
	}

	float RandomFloat(float min, float range)
	{
		float result = (float)rand() / RAND_MAX;
		result *= range;
		result += min;
		return result;
	}
}