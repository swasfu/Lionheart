#include "Polygon.h"
#include "PolyVertex.h"

Polygon::Polygon(std::vector<PolyVertex*>& vertices) : vertices(vertices) {}

glm::vec3 Polygon::Centre()
{
	glm::vec3 centre(0.0f);
	for (auto& vertex : vertices)
	{
		centre += vertex->coords;
	}

	return centre / (float)vertices.size();
}

glm::vec3 Polygon::Normal()
{
	glm::vec3 AB = vertices[1]->coords - vertices[0]->coords;
	glm::vec3 AC = vertices[2]->coords - vertices[0]->coords;

	glm::vec3 cross = glm::cross(AB, AC);

	return cross / glm::length(cross);
}

bool Polygon::HasOutwardNormal()
{
	glm::vec3 normal = Normal();
	glm::vec3 centre = Centre();
	return (glm::length(centre - normal) < glm::length(centre + normal));
}

std::vector<PolyVertex*> Polygon::WindOutward(bool ccw)
{
	bool gate = !(ccw != HasOutwardNormal());
	std::vector<PolyVertex*> ordered = { vertices[0],
		gate ? vertices[1] : vertices[2],
		gate ? vertices[2] : vertices[1] };
	return ordered;
}
