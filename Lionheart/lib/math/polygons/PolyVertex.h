#pragma once

#include <thirdparty/glm/glm.hpp>

#include <vector>

struct Polygon;

struct PolyVertex
{
	PolyVertex(glm::vec3 coords);
	void RemoveConnectedFace(Polygon* face);
	bool IsOverlapping(PolyVertex* other, float accuracy);
	glm::vec3 coords;
	std::vector<Polygon*> memberPolygons;
};