#pragma once

#include <thirdparty/glm/glm.hpp>

#include <vector>

//static glm::mat3 eAxesMatrix = glm::mat3(0.0f, 0.0f, 1.0f,
//	0.0f, 1.0f, 0.0f,
//	-1.0f, 0.0f, 0.0f);

int QuickOverflow(int value, int max);

struct ValueMap
{
	ValueMap();
	~ValueMap();

	float Average();
	float Stdev();
	void Alloc();
	void Free();
	float Stdev(float avg);
	float Value(float latitude, float longitude);

	void operator=(ValueMap& other);

	std::vector<float> values;
	int size;
	float average;
	float stdev;
};

float NormalToLatitude(glm::vec3 normal);
float NormalToLongitude(glm::vec3 normal);
glm::vec3 LatitudeLongitudeToNormal(float latitude, float longitude);