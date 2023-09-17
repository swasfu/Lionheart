#pragma once

#include <thirdparty/glm/glm.hpp>

int QuickOverflow(int value, int max);

struct ValueMap
{
	ValueMap();
	~ValueMap();

	float Average();
	float Stdev();
	float Stdev(float avg);
	float Value(float latitude, float longitude);

	float* values;
	int size;
	float average;
	float stdev;
};

float NormalToLatitude(glm::vec3 normal);
float NormalToLongitude(glm::vec3 normal);