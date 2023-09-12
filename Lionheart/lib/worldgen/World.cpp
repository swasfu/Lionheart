#include "World.h"

#include "worldgen/components/TileComponent.h"
#include "worldgen/components/ModelComponent.h"

#include "math/polygons/Polyhedron.h"
#include "math/polygons/Icosahedron.h"

#include <iostream>
#include <chrono>

void World::GenerateTiles(Registry* registry, float size, int subdivisions)
{
	Polyhedron icosahedron = Icosahedron(size);
	Polyhedron geodesic;

	std::cout << "Subdividing icosahedron into geodesic...";
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	for (auto& icoFacePtr : icosahedron.faces)
	{
		PolyVertex* aPtr = icoFacePtr->vertices[0];
		PolyVertex* bPtr = icoFacePtr->vertices[1];
		PolyVertex* cPtr = icoFacePtr->vertices[2];

		glm::vec3 AB = (bPtr->coords - aPtr->coords) / (float)subdivisions;
		glm::vec3 AC = (cPtr->coords - aPtr->coords) / (float)subdivisions;
		glm::vec3 BC = (cPtr->coords - bPtr->coords) / (float)subdivisions;

		PolyVertex* newB = aPtr; PolyVertex* newC = aPtr;

		std::vector<PolyVertex*> subdividedAs = { geodesic.AddVertexIfNotExists(*aPtr) };
		for (int abCount = 1; abCount < subdivisions + 1; abCount++)
		{
			newB = geodesic.AddVertexIfNotExists(PolyVertex(newB->coords + AB));
			newC = geodesic.AddVertexIfNotExists(PolyVertex(newC->coords + AC));
			PolyVertex* subdividedB = newB;
			std::vector<PolyVertex*> newBCs = { newB };
			for (int bcCount = 0; bcCount < abCount; bcCount++)
			{
				PolyVertex* subdividedC = geodesic.AddVertexIfNotExists(PolyVertex(subdividedB->coords + BC));
				newBCs.push_back(subdividedC);
				std::vector<PolyVertex*> newVertices = { subdividedAs[bcCount], subdividedB, subdividedC };
				auto newFace = geodesic.AddFaceIfNotExists(Polygon(newVertices));
				if (bcCount < (abCount - 1))
				{
					newVertices = { subdividedAs[bcCount], subdividedAs[bcCount + 1], subdividedC };
					newFace = geodesic.AddFaceIfNotExists(Polygon(newVertices));
				}
				subdividedB = subdividedC;
			}
			subdividedAs = newBCs;
		}
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << " done, " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << " nanoseconds" << std::endl;

	std::cout << "Projecting geodesic onto sphere...";
	begin = std::chrono::steady_clock::now();
	for (auto& vertex : geodesic.vertices)
	{
		vertex->coords = glm::normalize(vertex->coords) * size;
	}
	end = std::chrono::steady_clock::now();
	std::cout << " done, " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << " nanoseconds" << std::endl;

	std::cout << "Generating Goldberg Polyhedron...";
	begin = std::chrono::steady_clock::now();
	Polyhedron goldberg;
	std::unordered_map<Polygon*, EntityID> tileToEntityID;
	std::unordered_map<Polygon*, std::vector<PolyVertex*>> tileToNeighbourVertexPtrs;
	std::unordered_map<PolyVertex*, Polygon*> vertexPtrToTile;
	for (auto& vertex : geodesic.vertices)
	{
		std::vector<PolyVertex*> tileVertices;
		std::vector<PolyVertex*> neighbourVertices;
		for (auto& memberPolygon : vertex->memberPolygons)
		{
			for (auto& neighbourVertex : memberPolygon->vertices)
			{
				if (neighbourVertex != vertex.get()) neighbourVertices.push_back(neighbourVertex);
			}
			PolyVertex* centreVertex = goldberg.AddVertexIfNotExists(memberPolygon->Centre());
			tileVertices.push_back(centreVertex);
		}

		Polygon* tileFace = goldberg.AddFaceIfNotExists(Polygon(tileVertices));
		vertexPtrToTile[vertex.get()] = tileFace;
		tileToNeighbourVertexPtrs[tileFace] = neighbourVertices;
	}
	end = std::chrono::steady_clock::now();
	std::cout << " done, " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << " nanoseconds" << std::endl;

	std::cout << "Generating GL data...";
	std::chrono::nanoseconds regTime{ 0 };
	std::chrono::nanoseconds compTime{ 0 };
	std::chrono::nanoseconds windTime{ 0 };
	std::chrono::nanoseconds glVertexTime{ 0 };
	std::chrono::nanoseconds indexTime{ 0 };
	std::chrono::nanoseconds textureTime{ 0 };
	std::chrono::nanoseconds meshInitTime{ 0 };
	std::chrono::nanoseconds modelInitTime{ 0 };
	for (auto& tile : goldberg.faces)
	{
		// Register tile entity
		begin = std::chrono::steady_clock::now();
		EntityID tileEntityID = registry->RegisterEntity();
		tileToEntityID[tile.get()] = tileEntityID;
		end = std::chrono::steady_clock::now();
		regTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

		// Generate tile components
		begin = std::chrono::steady_clock::now();
		TileComponent* tileComponentPtr = registry->AddComponent<TileComponent>(tileEntityID);
		ModelComponent* modelComponentPtr = registry->AddComponent<ModelComponent>(tileEntityID);
		end = std::chrono::steady_clock::now();
		compTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

		// Wind vertices counter clockwise
		begin = std::chrono::steady_clock::now();
		auto orderedVertices = tile->WindOutward(true);
		end = std::chrono::steady_clock::now();
		windTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

		// Generate GLVertices from goldberg data
		begin = std::chrono::steady_clock::now();
		std::vector<GLVertex> vertices;
		glm::vec3 tileNormal = glm::normalize(tile->Normal());
		//glm::vec3 faceColour = glm::vec3(0.05f, ((float)rand() / RAND_MAX) * 0.1f + 0.2f, 0.125f);
		//glm::vec3 faceColour = orderedVertices.size() == 6 ? glm::vec3(1.0f) : glm::vec3(0.0f);
		glm::vec3 faceColour = orderedVertices.size() == 6 ? glm::vec3(0.05f, ((float)rand() / RAND_MAX) * 0.1f + 0.2f, 0.125f) : glm::vec3(1.0f, 0.2f, 0.2f);
		for (auto& vertex : orderedVertices)
		{
			GLVertex newVertex;
			newVertex.position = vertex->coords;
			newVertex.normal = tileNormal;
			newVertex.colour = faceColour;
			newVertex.texUV = glm::vec2(0.0f);

			vertices.push_back(newVertex);
		}
		end = std::chrono::steady_clock::now();
		glVertexTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

		// Generate triangle indices to cover tile face
		begin = std::chrono::steady_clock::now();
		std::vector<GLuint> indices = std::vector<GLuint>();
		for (int i = 1; i < orderedVertices.size() - 1; i++)
		{
			indices.push_back(0);
			indices.push_back(i);
			indices.push_back(i + 1);
		}
		end = std::chrono::steady_clock::now();
		indexTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

		// Initialise GLTexture for tile model mesh
		begin = std::chrono::steady_clock::now();
		GLTexture tileTexture;//("data/textures/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
		end = std::chrono::steady_clock::now();
		textureTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

		// Initialise GLMesh for tile model
		begin = std::chrono::steady_clock::now();
		GLMesh tileMesh = GLMesh(vertices, indices, tileTexture);
		end = std::chrono::steady_clock::now();
		meshInitTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

		// Initialise GLModel of tile
		begin = std::chrono::steady_clock::now();
		modelComponentPtr->model = GLModel(tileMesh);
		end = std::chrono::steady_clock::now();
		modelInitTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
	}
	std::cout << std::endl;
	std::cout << "     Average entity registry time: " << (regTime / goldberg.faces.size()).count() << " nanoseconds" << std::endl;
	std::cout << "  Average component creation time: " << (compTime / goldberg.faces.size()).count() << " nanoseconds" << std::endl;
	std::cout << "      Average vertex winding time: " << (windTime / goldberg.faces.size()).count() << " nanoseconds" << std::endl;
	std::cout << "   Average GLVertex creation time: " << (glVertexTime / goldberg.faces.size()).count() << " nanoseconds" << std::endl;
	std::cout << "      Average index creation time: " << (indexTime / goldberg.faces.size()).count() << " nanoseconds" << std::endl;
	std::cout << "    Average texture creation time: " << (textureTime / goldberg.faces.size()).count() << " nanoseconds" << std::endl;
	std::cout << " Average mesh initialisation time: " << (meshInitTime / goldberg.faces.size()).count() << " nanoseconds" << std::endl;
	std::cout << "Average model initialisation time: " << (modelInitTime / goldberg.faces.size()).count() << " nanoseconds" << std::endl;

	std::cout << "Resolving tile neighbours...";
	begin = std::chrono::steady_clock::now();
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
	end = std::chrono::steady_clock::now();
	std::cout << " done, " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << " nanoseconds" << std::endl;
}
