#pragma once

#include <thirdparty/glm/glm.hpp>

#include <vector>

struct PolyVertex;

struct Polygon
{
	Polygon(std::vector<PolyVertex*>& vertices);
	glm::vec3 Centre();
	glm::vec3 Normal();
	bool HasOutwardNormal();
	std::vector<PolyVertex*> WindOutward(bool);
	std::vector<PolyVertex*> vertices;
};