#include "worldgen/ValueMap.h"

#include "math/Random.h"
#include "math/Constants.h"

#include <memory>
#include <iostream>
#include <cfloat>

int QuickOverflow(int value, int max)
{
	if (value >= max) return 0;
	else if (value < 0) return max;
	else return value;
}

ValueMap::ValueMap()
{
	size = 0;
	average = 0;
	stdev = 0;
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

void ValueMap::Alloc()
{
	values.resize(size * size * sizeof(float));
}

void ValueMap::Free()
{
	values.clear();
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
	latitude += Constants::PI;
	latitude /= (Constants::PI * 2.0f);
	longitude += Constants::PI;
	longitude /= (Constants::PI * 2.0f);
	return values[size * (int)(latitude * (float)size) + (int)(longitude * (float)size)];
}

void ValueMap::operator=(ValueMap& other)
{
	values = other.values;
	size = other.size;
	average = other.average;
	stdev = other.stdev;
}

float NormalToLatitude(glm::vec3 normal)
{
	//normal = eAxesMatrix * normal;
	float equatorial = sqrtf(powf(normal.x, 2) + powf(normal.y, 2));
	float latitude = atan2f(normal.z, equatorial);
	return latitude;
}

float NormalToLongitude(glm::vec3 normal)
{
	//normal = eAxesMatrix * normal;
	float longitude = atan2f(normal.y, normal.x);
	return longitude;
}

glm::vec3 LatitudeLongitudeToNormal(float latitude, float longitude)
{
	return glm::vec3(cosf(latitude) * cosf(longitude), cosf(latitude) * sinf(longitude), sinf(latitude));
	//return eAxesMatrix * glm::vec3(sinf(latitude), cosf(latitude) * sinf(longitude), cosf(latitude) * -cosf(longitude));
}