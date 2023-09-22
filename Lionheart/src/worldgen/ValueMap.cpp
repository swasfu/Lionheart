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
	width = 0;
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

	for (int i = 0; i < height - 1; i++)
	{
		float rowAvg = 0.0f;
		for (int j = 0; j < width - 1; j++)
		{
			rowAvg += values[height * i + j];
		}
		average += (rowAvg / (width - 1));
	}

	average /= (height - 1);

	return average;
}

float ValueMap::Stdev()
{
	return Stdev(Average());
}

void ValueMap::Alloc()
{
	values.resize(width * height);
}

void ValueMap::Free()
{
	values.clear();
}

float ValueMap::Stdev(float avg)
{
	stdev = 0.0f;

	for (int i = 0; i < height - 1; i++)
	{
		for (int j = 0; j < width - 1; j++)
		{
			stdev += powf((values[height * i + j] - avg), 2);
		}
	}

	stdev /= (width * height);
	stdev = sqrtf(stdev);

	return stdev;
}

float ValueMap::Value(float latitude, float longitude)
{
	latitude += Constants::HALF_PI;
	latitude /= Constants::PI;
	longitude += Constants::PI;
	longitude /= Constants::TWO_PI;
	if (latitude > 1.0f) latitude = 1.0f;
	if (latitude < 0.0f) latitude = 0.0f;
	if (longitude > 1.0f) longitude = 1.0f;
	if (longitude < 0.0f) longitude = 0.0f;
	//std::cout << "Latitude: " << latitude << std::endl;
	//std::cout << "Longitude: " << longitude << std::endl;
	return values[height * (int)(longitude * (width - 1)) + (int)(latitude * (height - 1))];
}

float ValueMap::MapCoordsToLatitude(int y)
{
	float latitude = ((float)y / (float)(height - 1)) * Constants::PI - Constants::HALF_PI;
	return latitude;
}

float ValueMap::MapCoordsToLongitude(int x)
{
	float longitude = ((float)x / (float)(width - 1)) * Constants::TWO_PI - Constants::PI;
	return longitude;
}

void ValueMap::MapCoordsToLatitudeLongitude(int x, int y, float& latitude, float& longitude)
{
	latitude = ((float)y / (float)(height - 1)) * Constants::PI - Constants::HALF_PI;
	longitude = ((float)x / (float)(width - 1)) * Constants::TWO_PI - Constants::PI;
}

void ValueMap::operator=(ValueMap& other)
{
	values = other.values;
	width = other.width;
	average = other.average;
	stdev = other.stdev;
}

float NormalToLatitude(glm::vec3 normal)
{
	float equatorial = sqrtf(powf(normal.x, 2) + powf(normal.y, 2));
	float latitude = atan2f(normal.z, equatorial);
	return latitude;
}

float NormalToLongitude(glm::vec3 normal)
{
	float longitude = atan2f(normal.y, normal.x);
	return longitude;
}

void NormalToLatitudeLongitude(glm::vec3 normal, float& latitude, float& longitude)
{
	float equatorial = sqrtf(powf(normal.x, 2) + powf(normal.y, 2));
	latitude = atan2f(normal.z, equatorial);
	longitude = atan2f(normal.y, normal.x);
}

glm::vec3 LatitudeLongitudeToNormal(float latitude, float longitude)
{
	return glm::vec3(cosf(latitude) * cosf(longitude), cosf(latitude) * sinf(longitude), sinf(latitude));
}