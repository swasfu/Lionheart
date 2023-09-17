#include "math/polygons/Polygon.h"
#include "math/polygons/PolyVertex.h"

#include <map>

Polygon::Polygon(std::vector<PolyVertex*>& givenVertices)
{
	vertices.reserve(givenVertices.size());
	for (auto& vertex : givenVertices) vertices.push_back(vertex);
}

glm::vec3 Polygon::Centroid()
{
	glm::vec3 centre(0.0f);
	for (auto& vertex : vertices)
	{
		centre += vertex->coords;
	}

	return centre / (float)vertices.size();
}