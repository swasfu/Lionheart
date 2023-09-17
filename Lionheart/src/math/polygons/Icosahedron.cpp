#include "math/polygons/Icosahedron.h"

#include "math/constants.h"

Icosahedron::Icosahedron(float size) : Polyhedron()
{
	const float tao = Constants::PI / 2;

	vertices.reserve(12);
	vertices.push_back(std::make_unique<PolyVertex>(PolyVertex(glm::vec3(size, tao * size, 0))));
	vertices.push_back(std::make_unique<PolyVertex>(PolyVertex(glm::vec3(-size, tao * size, 0))));
	vertices.push_back(std::make_unique<PolyVertex>(PolyVertex(glm::vec3(size, -tao * size, 0))));
	vertices.push_back(std::make_unique<PolyVertex>(PolyVertex(glm::vec3(-size, -tao * size, 0))));
	vertices.push_back(std::make_unique<PolyVertex>(PolyVertex(glm::vec3(0, size, tao * size))));
	vertices.push_back(std::make_unique<PolyVertex>(PolyVertex(glm::vec3(0, -size, tao * size))));
	vertices.push_back(std::make_unique<PolyVertex>(PolyVertex(glm::vec3(0, size, -tao * size))));
	vertices.push_back(std::make_unique<PolyVertex>(PolyVertex(glm::vec3(0, -size, -tao * size))));
	vertices.push_back(std::make_unique<PolyVertex>(PolyVertex(glm::vec3(tao * size, 0, size))));
	vertices.push_back(std::make_unique<PolyVertex>(PolyVertex(glm::vec3(-tao * size, 0, size))));
	vertices.push_back(std::make_unique<PolyVertex>(PolyVertex(glm::vec3(tao * size, 0, -size))));
	vertices.push_back(std::make_unique<PolyVertex>(PolyVertex(glm::vec3(-tao * size, 0, -size))));

	faces.reserve(20);
	std::vector<PolyVertex*> face1 = { vertices[0].get(), vertices[1].get(), vertices[4].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face1)));
	std::vector<PolyVertex*> face2 = { vertices[1].get(), vertices[9].get(), vertices[4].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face2)));
	std::vector<PolyVertex*> face3 = { vertices[4].get(), vertices[9].get(), vertices[5].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face3)));
	std::vector<PolyVertex*> face4 = { vertices[5].get(), vertices[9].get(), vertices[3].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face4)));
	std::vector<PolyVertex*> face5 = { vertices[2].get(), vertices[3].get(), vertices[7].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face5)));
	std::vector<PolyVertex*> face6 = { vertices[3].get(), vertices[2].get(), vertices[5].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face6)));
	std::vector<PolyVertex*> face7 = { vertices[7].get(), vertices[10].get(), vertices[2].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face7)));
	std::vector<PolyVertex*> face8 = { vertices[0].get(), vertices[8].get(), vertices[10].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face8)));
	std::vector<PolyVertex*> face9 = { vertices[0].get(), vertices[4].get(), vertices[8].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face9)));
	std::vector<PolyVertex*> face10 = { vertices[8].get(), vertices[2].get(), vertices[10].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face10)));
	std::vector<PolyVertex*> face11 = { vertices[8].get(), vertices[4].get(), vertices[5].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face11)));
	std::vector<PolyVertex*> face12 = { vertices[8].get(), vertices[5].get(), vertices[2].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face12)));
	std::vector<PolyVertex*> face13 = { vertices[1].get(), vertices[0].get(), vertices[6].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face13)));
	std::vector<PolyVertex*> face14 = { vertices[3].get(), vertices[9].get(), vertices[11].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face14)));
	std::vector<PolyVertex*> face15 = { vertices[6].get(), vertices[10].get(), vertices[7].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face15)));
	std::vector<PolyVertex*> face16 = { vertices[3].get(), vertices[11].get(), vertices[7].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face16)));
	std::vector<PolyVertex*> face17 = { vertices[11].get(), vertices[6].get(), vertices[7].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face17)));
	std::vector<PolyVertex*> face18 = { vertices[6].get(), vertices[0].get(), vertices[10].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face18)));
	std::vector<PolyVertex*> face19 = { vertices[11].get(), vertices[1].get(), vertices[6].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face19)));
	std::vector<PolyVertex*> face20 = { vertices[9].get(), vertices[1].get(), vertices[11].get() };
	faces.push_back(std::make_unique<Polygon>(Polygon(face20)));
}