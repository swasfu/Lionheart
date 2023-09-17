#include "math/polygons/Cube.h"

#include "math/constants.h"

Cube::Cube(float size) : Polyhedron()
{
	vertices.reserve(8);
	vertices.push_back(std::make_unique<PolyVertex>(PolyVertex(glm::vec3(size, size, size))));
	vertices.push_back(std::make_unique<PolyVertex>(PolyVertex(glm::vec3(size, size, -size))));
	vertices.push_back(std::make_unique<PolyVertex>(PolyVertex(glm::vec3(size, -size, size))));
	vertices.push_back(std::make_unique<PolyVertex>(PolyVertex(glm::vec3(size, -size, -size))));
	vertices.push_back(std::make_unique<PolyVertex>(PolyVertex(glm::vec3(-size, size, size))));
	vertices.push_back(std::make_unique<PolyVertex>(PolyVertex(glm::vec3(-size, size, -size))));
	vertices.push_back(std::make_unique<PolyVertex>(PolyVertex(glm::vec3(-size, -size, size))));
	vertices.push_back(std::make_unique<PolyVertex>(PolyVertex(glm::vec3(-size, -size, -size))));

	faces.reserve(6);
	std::vector<PolyVertex*> face1 = { vertices[0].get(), vertices[1].get(), vertices[2].get(), vertices[3].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face1)));
	std::vector<PolyVertex*> face2 = { vertices[4].get(), vertices[5].get(), vertices[6].get(), vertices[7].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face2)));
	std::vector<PolyVertex*> face3 = { vertices[0].get(), vertices[2].get(), vertices[4].get(), vertices[6].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face3)));
	std::vector<PolyVertex*> face4 = { vertices[1].get(), vertices[3].get(), vertices[5].get(), vertices[7].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face4)));
	std::vector<PolyVertex*> face5 = { vertices[0].get(), vertices[1].get(), vertices[4].get(), vertices[5].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face5)));
	std::vector<PolyVertex*> face6 = { vertices[2].get(), vertices[3].get(), vertices[6].get(), vertices[7].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face6)));
}