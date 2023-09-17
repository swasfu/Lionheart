#include "math/Random.h"

void Random::Seed()
{
	srand(time(nullptr));
}

void Random::Seed(std::string seed)
{
	srand(std::hash<std::string>{}(seed));
}

float Random::RandomFloat(float min, float max)
{
	float result = (float)rand() / RAND_MAX;
	result *= (max - min);
	result += min;
	return result;
}