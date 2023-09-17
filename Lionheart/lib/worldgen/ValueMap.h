#pragma once

#include <thirdparty/glm/glm.hpp>

int QuickOverflow(int value, int max);

struct ValueMap
{
	ValueMap();
	~ValueMap();

	float Average();
	float Stdev();
	size_t Alloc();
	void Free();
	float Stdev(float avg);
	float Value(float latitude, float longitude);

	void operator=(ValueMap& other);

	float* values;
	int size;
	float average;
	float stdev;
	size_t alloc;
};

float NormalToLatitude(glm::vec3 normal);
float NormalToLongitude(glm::vec3 normal);