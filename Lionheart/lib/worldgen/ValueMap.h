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
	float ValueXY(int x, int y);
	float MapCoordsToLatitude(int x);
	float MapCoordsToLongitude(int y);
	void MapCoordsToLatitudeLongitude(int x, int y, float& latitude, float& longitude);
	glm::vec3 MapCoordsToNormal(int x, int y);

	void operator=(ValueMap& other);
	float& operator[](int index);

	std::vector<float> values;
	int width;
	int height;
	float average;
	float stdev;
};

float NormalToLatitude(glm::vec3 normal);
float NormalToLongitude(glm::vec3 normal);
void NormalToLatitudeLongitude(glm::vec3 normal, float& latitude, float& longitude);
glm::vec3 LatitudeLongitudeToNormal(float latitude, float longitude);