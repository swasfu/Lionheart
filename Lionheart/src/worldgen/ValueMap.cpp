#include "worldgen/ValueMap.h"

#include "math/Random.h"
#include "math/Constants.h"

#include <memory>
#include <iostream>

int QuickOverflow(int value, int max)
{
	if (value >= max) return 0;
	if (value < 0) return max;
	else return value;
}

ValueMap::ValueMap()
{
	values = nullptr;
	size = 0;
	average = 0;
	stdev = 0;
	alloc = 0;
}

ValueMap::~ValueMap()
{
	Free();
}

float ValueMap::Average()
{
	average = 0.0f;

	for (int i = 0; i < size - 1; i++)
	{
		float rowAvg = 0.0f;
		for (int j = 0; j < size - 1; j++)
		{
			rowAvg += values[size * i + j];
		}
		average += (rowAvg / (size - 1));
	}

	average /= (size - 1);

	return average;
}

float ValueMap::Stdev()
{
	return Stdev(Average());
}

size_t ValueMap::Alloc()
{
	alloc = (size_t)size * (size_t)size * sizeof(float);
	values = (float*)malloc(alloc);
	return alloc;
}

void ValueMap::Free()
{
	if (values) free(values);
	alloc = 0;
}

float ValueMap::Stdev(float avg)
{
	stdev = 0.0f;

	for (int i = 0; i < size - 1; i++)
	{
		for (int j = 0; j < size - 1; j++)
		{
			stdev += powf((values[size * i + j] - avg), 2);
		}
	}

	stdev /= (size * size);
	stdev = sqrtf(stdev);

	return stdev;
}

float ValueMap::Value(float latitude, float longitude)
{
	return values[size * (int)(latitude * (float)size) + (int)(longitude * (float)size)];
}

void ValueMap::operator=(ValueMap& other)
{
	size = other.size;
	Alloc();
	memcpy(values, other.values, alloc);
	average = other.average;
	stdev = other.stdev;
}

float NormalToLatitude(glm::vec3 normal)
{
	return (atan2f(normal.y, sqrtf(powf(normal.x, 2) + powf(normal.z, 2))) + Constants::PI / 2.0f) / (1.0f * Constants::PI);
}

float NormalToLongitude(glm::vec3 normal)
{
	return (atan2f(normal.z, normal.x) + Constants::PI) / (2.0f * Constants::PI);
}