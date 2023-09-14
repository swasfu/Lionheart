#pragma once

#include <thirdparty/glm/glm.hpp>

#include <vector>

struct PolyVertex;

struct Polygon
{
	Polygon(std::vector<PolyVertex*>& vertices);
	glm::vec3 Centroid();
	std::vector<PolyVertex*> vertices;
};