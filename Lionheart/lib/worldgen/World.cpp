#include "World.h"

#include "worldgen/components/TileComponent.h"
#include "worldgen/components/ModelComponent.h"

#include "math/polygons/Polyhedron.h"
#include "math/polygons/Icosahedron.h"

#include <iostream>

void World::GenerateTiles(Registry* registry, float size, int subdivisions)
{
	Polyhedron icosahedron = Icosahedron(size);
	for (auto& face : icosahedron.faces)
	{
		std::cout << face->vertices.size() << std::endl;
	}
	Polyhedron geodesic;

	std::cout << "Subdividing icosahedron into geodesic...";
	for (auto& icoFacePtr : icosahedron.faces)
	{
		std::cout << "Working on size " << icoFacePtr->vertices.size() << " icosahedron face " << icoFacePtr.get() << std::endl;
		PolyVertex* aPtr = icoFacePtr->vertices[0];
		PolyVertex* bPtr = icoFacePtr->vertices[1];
		PolyVertex* cPtr = icoFacePtr->vertices[2];
		std::cout << std::endl << "bull" << std::endl;

		glm::vec3 AB = (bPtr->coords - aPtr->coords) / (float)subdivisions;
		glm::vec3 AC = (cPtr->coords - aPtr->coords) / (float)subdivisions;
		glm::vec3 BC = (cPtr->coords - bPtr->coords) / (float)subdivisions;

		PolyVertex* newB = aPtr; PolyVertex* newC = aPtr;

		std::vector<PolyVertex*> subdividedAs = { geodesic.AddVertexIfNotExists(*aPtr) };
		//std::cout << aPtr->coords.x_ << ", " << aPtr->coords.y_ << ", " << aPtr->coords.z_ << std::endl;
		//std::cout << bPtr->coords.x_ << ", " << bPtr->coords.y_ << ", " << bPtr->coords.z_ << std::endl;
		//std::cout << subdividedAs[0]->coords.x_ << ", " << subdividedAs[0]->coords.y_ << ", " << subdividedAs[0]->coords.z_ << std::endl;
		for (int abCount = 1; abCount < subdivisions + 1; abCount++)
		{
			std::cout << "shit" << std::endl;
			newB = geodesic.AddVertexIfNotExists(PolyVertex(newB->coords + AB));
			//std::cout << newB->coords.x_ << ", " << newB->coords.y_ << ", " << newB->coords.z_ << std::endl;
			newC = geodesic.AddVertexIfNotExists(PolyVertex(newC->coords + AC));
			//std::cout << newC->coords.x_ << ", " << newC->coords.y_ << ", " << newC->coords.z_ << std::endl;
			PolyVertex* subdividedB = newB;
			std::vector<PolyVertex*> newBCs = { newB };
			for (int bcCount = 0; bcCount < abCount; bcCount++)
			{
				//std::cout << bcCount << "foo" << abCount;
				PolyVertex* subdividedC = geodesic.AddVertexIfNotExists(PolyVertex(subdividedB->coords + BC));
				newBCs.push_back(subdividedC);
				std::vector<PolyVertex*> newVertices = { subdividedAs[bcCount], subdividedB, subdividedC };
				auto newFace = geodesic.AddFaceIfNotExists(Polygon(newVertices));
				/*
				std::cout << "Creating subdivided face " << newFace << " of size " << newFace->vertices_.size() << " members: ";
				for (auto& newVertex : newFace->vertices_)
				{
					std::cout << newVertex << " ";
				}
				std::cout << std::endl;
				*/
				if (bcCount < (abCount - 1))
				{
					//std::cout << bcCount << "bar" << abCount;
					newVertices = { subdividedAs[bcCount], subdividedAs[bcCount + 1], subdividedC };
					newFace = geodesic.AddFaceIfNotExists(Polygon(newVertices));
					/*
					std::cout << "Creating subdivided face " << newFace << " of size " << newFace->vertices_.size() << " members: ";
					for (auto& newVertex : newFace->vertices_)
					{
						std::cout << newVertex << " ";
					}
					std::cout << std::endl;
					*/
				}
				subdividedB = subdividedC;
			}
			subdividedAs = newBCs;
		}
	}
	std::cout << " done" << std::endl;

	/*
	std::cout << geodesic.vertices_.size() << " vertices" << std::endl;
	for (auto& vertex : geodesic.vertices_)
	{
		std::cout << vertex->memberPolygons_.size() << std::endl;
	}

	std::cout << geodesic.faces_.size() << " faces" << std::endl;
	for (auto& face : geodesic.faces_)
	{
		std::cout << face->vertices_.size() << std::endl;
	}
	*/
	std::cout << "Projecting geodesic onto sphere...";
	for (auto& vertex : geodesic.vertices)
	{
		vertex->coords = glm::normalize(vertex->coords) * size;
	}
	std::cout << " done" << std::endl;

	std::cout << "Generating Goldberg Polyhedron...";
	Polyhedron goldberg;
	std::unordered_map<Polygon*, EntityID> tileToEntityID;
	std::unordered_map<Polygon*, std::vector<PolyVertex*>> tileToNeighbourVertexPtrs;
	std::unordered_map<PolyVertex*, Polygon*> vertexPtrToTile;
	//std::cout << "Geodesic vertices: " << geodesic.vertices_.size() << std::endl;
	//std::cout << "Geodesic faces: " << geodesic.faces_.size() << std::endl;
	for (auto& vertex : geodesic.vertices)
	{
		//std::cout << "Working on geodesic vertex " << vertex.get() << std::endl;
		std::vector<PolyVertex*> tileVertices;
		std::vector<PolyVertex*> neighbourVertices;
		//std::cout << "Member of " << vertex->memberPolygons_.size() << " polygons" << std::endl;
		for (auto& memberPolygon : vertex->memberPolygons)
		{
			//std::cout << "Storing neighbour vertices for member polygon " << memberPolygon << std::endl;
			for (auto& neighbourVertex : memberPolygon->vertices)
			{
				if (neighbourVertex != vertex.get()) neighbourVertices.push_back(neighbourVertex);
			}
			//std::cout << "Adding centre of member polygon as new vertex for goldberg" << std::endl;
			PolyVertex* centreVertex = goldberg.AddVertexIfNotExists(memberPolygon->Centre());
			tileVertices.push_back(centreVertex);
		}

		Polygon* tileFace = goldberg.AddFaceIfNotExists(Polygon(tileVertices));
		vertexPtrToTile[vertex.get()] = tileFace;
		tileToNeighbourVertexPtrs[tileFace] = neighbourVertices;
	}
	std::cout << " done" << std::endl;

	std::cout << "Generating GL data...";
	for (auto& tile : goldberg.faces)
	{
		//std::cout << "Working on tile " << tile.get() << ", size " << tile->vertices_.size() << std::endl;
		//std::cout << "Registering tile entity" << std::endl;
		EntityID tileEntityID = registry->RegisterEntity();
		tileToEntityID[tile.get()] = tileEntityID;

		//std::cout << "Creating tile components" << std::endl;
		TileComponent* tileComponentPtr = registry->AddComponent<TileComponent>(tileEntityID);
		ModelComponent* modelComponentPtr = registry->AddComponent<ModelComponent>(tileEntityID);

		//std::cout << "Winding vertices" << std::endl;
		auto orderedVertices = tile->WindOutward(true);

		//std::cout << "Creating GLvertices" << std::endl;
		std::vector<GLVertex> vertices;
		glm::vec3 tileNormal = glm::normalize(tile->Normal());
		for (auto& vertex : orderedVertices)
		{
			GLVertex newVertex;
			newVertex.position = vertex->coords;
			newVertex.normal = tileNormal;
			newVertex.colour = glm::vec3(1.0f);
			newVertex.texUV = glm::vec2(0.0f);

			vertices.push_back(newVertex);
		}

		//std::cout << "Creating indices" << std::endl;
		std::vector<GLuint> indices = std::vector<GLuint>();
		for (int i = 1; i < orderedVertices.size() - 1; i++)
		{
			indices.push_back(0);
			indices.push_back(i);
			indices.push_back(i + 1);
		}

		//std::cout << "Creating texture" << std::endl;
		GLTexture tileTexture("data/textures/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

		GLMesh tileMesh = GLMesh(vertices, indices, tileTexture);
		modelComponentPtr->model = GLModel(tileMesh);
	}
	std::cout << " done" << std::endl;

	std::cout << "Resolving tile neighbours...";
	for (auto& pair : tileToNeighbourVertexPtrs)
	{
		auto tile = pair.first;
		auto tileComponent = registry->GetComponent<TileComponent>(tileToEntityID[tile]);
		auto& neighbours = pair.second;
		for (auto& neighbour : neighbours)
		{
			auto neighbourTile = vertexPtrToTile[neighbour];
			tileComponent->neighbours.push_back(tileToEntityID[neighbourTile]);
		}
	}
	std::cout << " done" << std::endl;
}
