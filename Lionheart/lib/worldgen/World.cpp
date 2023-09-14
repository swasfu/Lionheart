#include "World.h"

#include "worldgen/components/TileComponent.h"
#include "worldgen/components/ModelComponent.h"

#include "math/polygons/Polyhedron.h"
#include "math/polygons/Icosahedron.h"
#include "math/Random.h"
#include "math/Constants.h"

#include <thirdparty/glm/glm.hpp>
#include <thirdparty/glm/gtx/vector_angle.hpp>

#include <iostream>
#include <chrono>
#include <map>

struct TerrainQuadrant
{
	TerrainQuadrant(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, float altitude) : a(a), b(b), c(c), d(d), altitude(altitude)
	{}
	glm::vec3 a, b, c, d;
	float altitude;
};

std::vector<TerrainQuadrant> QuarterQuadrant(TerrainQuadrant quadrant, float range)
{
	glm::vec3 AB = quadrant.b - quadrant.a;
	glm::vec3 AD = quadrant.d - quadrant.a;
	glm::vec3 BC = quadrant.c - quadrant.b;
	glm::vec3 DC = quadrant.d - quadrant.c;
	glm::vec3 AC = quadrant.c - quadrant.a;

	std::vector<TerrainQuadrant> quarters;
	quarters.push_back(TerrainQuadrant(quadrant.a, quadrant.a + (AB / 2.0f), quadrant.a + (AC / 2.0f), quadrant.a + (AD / 2.0f), quadrant.altitude + ((float)rand() / RAND_MAX) * range - (range / 2)));
	quarters.push_back(TerrainQuadrant(quadrant.a + (AB / 2.0f), quadrant.b, quadrant.b + (BC / 2.0f), quadrant.a + (AC / 2.0f), quadrant.altitude + ((float)rand() / RAND_MAX) * range - (range / 2)));
	quarters.push_back(TerrainQuadrant(quadrant.a + (AC / 2.0f), quadrant.b + (BC / 2.0f), quadrant.c, quadrant.d + (DC / 2.0f), quadrant.altitude + ((float)rand() / RAND_MAX) * range - (range / 2)));
	quarters.push_back(TerrainQuadrant(quadrant.a + (AD / 2.0f), quadrant.a + (AC / 2.0f), quadrant.d + (DC / 2.0f), quadrant.c, quadrant.altitude + ((float)rand() / RAND_MAX) * range - (range / 2)));

	return quarters;
}

void SphericalQuadrant(TerrainQuadrant& quadrant, float radius)
{
	quadrant.a = glm::normalize(quadrant.a) * radius;
	quadrant.b = glm::normalize(quadrant.b) * radius;
	quadrant.c = glm::normalize(quadrant.c) * radius;
	quadrant.d = glm::normalize(quadrant.d) * radius;
}

glm::vec3 QuadrantCentre(TerrainQuadrant quadrant)
{
	return (quadrant.a + quadrant.b + quadrant.c + quadrant.d) / 4.0f;
}

glm::vec3 SphericalPoint(glm::vec3 point, float radius)
{
	return glm::normalize(point) * radius;
}

glm::vec3 Centroid(std::vector<glm::vec3>& points)
{
	glm::vec3 centre(0.0f);
	for (auto& point : points) centre += point;
	return centre / (float)points.size();
}

void WindOutward(std::vector<glm::vec3>& vertices, bool ccw)
{
	std::map<float, glm::vec3> angleToVertex;


	glm::vec3 centre = Centroid(vertices);

	glm::vec3 a = vertices[0] - centre;

	for (auto& vertex : vertices)
	{
		glm::vec3 b = vertex - centre;
		float angle = atan2f(glm::dot(glm::cross(a, b), glm::normalize(centre)), glm::dot(a, b));
		angleToVertex[angle] = vertex;
	}

	vertices.clear();
	for (auto& pair : angleToVertex)
	{
		vertices.push_back(pair.second);
	}
}

PolyVertex* AddEdgeVertex(Polyhedron& polyhedron, PolyVertex& vertex, std::vector<PolyVertex*>& edgeVertices)
{
	for (auto& edgeVertex : edgeVertices)
	{
		if (vertex.IsOverlapping(edgeVertex, 0.001f)) return edgeVertex;
	}
	auto newEdgeVertex = polyhedron.AddVertex(vertex);
	edgeVertices.push_back(newEdgeVertex);
	return newEdgeVertex;
}

struct GlobalCoord
{
	float lat;
	float lon;
};

struct AltitudeSeed
{
	glm::vec3 normal;
	float altitude;
	float magnitude;
};

GlobalCoord SphereVectorToGlobalCoord(glm::vec3 sphereVector, float sphereRadius)
{
	GlobalCoord globalCoord;
	globalCoord.lat = atan2f(sphereVector.z, sqrtf(powf(sphereVector.x, 2) + powf(sphereVector.y, 2)));
	globalCoord.lon = atan2f(sphereVector.y, sphereVector.x);
	return globalCoord;
}

float GreatCircleDistance(glm::vec3 a, glm::vec3 b)
{
	return atanf(glm::length(glm::cross(a, b)) / glm::dot(a, b));
}

void World::GenerateTiles(Registry* registry, float size, int subdivisions)
{
	Random::Seed();

	Polyhedron icosahedron = Icosahedron(size);
	Polyhedron geodesic;

	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;

	std::cout << "Subdividing icosahedron into geodesic...";
	begin = std::chrono::steady_clock::now();
	std::vector<PolyVertex*> edgeVertices;
	//std::chrono::milliseconds averageEdgeVertex{ 0 };
	//std::chrono::milliseconds averageInnerVertex{ 0 };
	int edgeCount = 0;
	int innerCount = 0;
	//std::chrono::milliseconds averageFace{ 0 };
	std::unordered_map<PolyVertex*, float> vertexAltitudes;
	for (auto& face : icosahedron.faces)
	{
		glm::vec3 a = face->vertices[0]->coords;
		glm::vec3 b = face->vertices[1]->coords;
		glm::vec3 c = face->vertices[2]->coords;

		glm::vec3 ab = (b - a) / (float)subdivisions;
		glm::vec3 ac = (c - a) / (float)subdivisions;
		glm::vec3 bc = (c - b) / (float)subdivisions;

		std::vector<PolyVertex*> vertices;
		vertices.reserve(((2 + subdivisions) * (1 + subdivisions)) / 2);
		for (int i = 0; i < subdivisions + 1; i++)
		{
			PolyVertex nextA = PolyVertex(a);
			vertices.push_back(AddEdgeVertex(geodesic, nextA, edgeVertices));
			c = a;
			for (int j = 1; j < subdivisions + 1 - i; j++)
			{
				c += ac;
				PolyVertex nextC = PolyVertex(c);
				if (i == 0 || j == (subdivisions - i))
				{
					//begin = std::chrono::steady_clock::now();
					PolyVertex nextC = PolyVertex(c);
					vertices.push_back(AddEdgeVertex(geodesic, nextC, edgeVertices));
					//end = std::chrono::steady_clock::now();
					//averageEdgeVertex += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
					//edgeCount++;
				} else
				{
					//begin = std::chrono::steady_clock::now();
					vertices.push_back(geodesic.AddVertex(nextC));
					//end = std::chrono::steady_clock::now();
					//averageInnerVertex += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
					//innerCount++;
				}
			}
			a += ab;
		}

		//begin = std::chrono::steady_clock::now();
		int count = 0;
		for (int i = subdivisions; i > 0; i--)
		{
			for (int j = 0; j < i; j++)
			{
				std::vector<PolyVertex*> forwardVertices = { vertices[count], vertices[count + 1], vertices[count + i + 1] };
				Polygon forwardTriangle = Polygon(forwardVertices);
				geodesic.AddFace(forwardTriangle);
				if (j != 0)
				{
					std::vector<PolyVertex*> backwardVertices = { vertices[count], vertices[count + i], vertices[count + i + 1] };
					Polygon backwardTriangle = Polygon(backwardVertices);
					geodesic.AddFace(backwardTriangle);
				}
				count++;
			}
			count++;
		}
		//end = std::chrono::steady_clock::now();
		//averageFace += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	}
	end = std::chrono::steady_clock::now();
	std::cout << " done, " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;

	/*
	for (auto& face : icosahedron.faces)
	{
		glm::vec3 a = face->vertices[0]->coords;
		glm::vec3 b = face->vertices[1]->coords;
		glm::vec3 c = face->vertices[2]->coords;

		glm::vec3 ab = (b - a) / (float)subdivisions;
		glm::vec3 ac = (c - a) / (float)subdivisions;
		glm::vec3 bc = (c - b) / (float)subdivisions;

		PolyVertex* aVertex = geodesic.AddVertexIfNotExists(PolyVertex(a));

		for (int depth = 0; depth < subdivisions; depth++)
		{
			PolyVertex* newAVertex = nullptr;
			PolyVertex* bVertex = nullptr;
			PolyVertex* cVertex = nullptr;
			for (int width = 0; width < (depth + 1); width++)
			{
				if (width == 0)
				{
					bVertex = geodesic.AddVertexIfNotExists(PolyVertex(aVertex->coords + ab));
					newAVertex = bVertex;
				} else
				{
					bVertex = cVertex;
				}

				cVertex = geodesic.AddVertexIfNotExists(PolyVertex(aVertex->coords + ac));

				geodesic.AddFace(Polygon({ aVertex, cVertex, bVertex }));

				if (width < depth)
				{
					PolyVertex* dVertex = geodesic.AddVertexIfNotExists(aVertex->coords + bc);
					geodesic.AddFace(Polygon({ aVertex, dVertex, cVertex }));
					aVertex = dVertex;
				}
			}
			aVertex = newAVertex;
		}
	}
	*/
	/*
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
	*/

	std::cout << "Projecting geodesic onto sphere...";
	begin = std::chrono::steady_clock::now();
	for (auto& vertex : geodesic.vertices)
	{
		vertex->coords = glm::normalize(vertex->coords) * size;
	}
	end = std::chrono::steady_clock::now();
	std::cout << " done, " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;

	std::cout << "Generating GL data...";
	begin = std::chrono::steady_clock::now();
	EntityID worldID = registry->RegisterEntity();
	ModelComponent* worldModel = registry->AddComponent<ModelComponent>(worldID);
	float altitudeRange = 1.0f;
	std::map<TileComponent*, std::vector<int>> tileToVertexIndices;
	/*
	std::chrono::milliseconds regTime{ 0 };
	std::chrono::milliseconds compTime{ 0 };
	std::chrono::milliseconds windTime{ 0 };
	std::chrono::milliseconds glVertexTime{ 0 };
	std::chrono::milliseconds indexTime{ 0 };
	std::chrono::milliseconds textureTime{ 0 };
	std::chrono::milliseconds meshInitTime{ 0 };
	std::chrono::milliseconds modelInitTime{ 0 };
	*/
	for (auto& vertex : geodesic.vertices)
	{
		// Register tile entity
		//begin = std::chrono::steady_clock::now();
		EntityID tileEntityID = registry->RegisterEntity();
		//end = std::chrono::steady_clock::now();
		//regTime += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

		// Generate tile components
		//begin = std::chrono::steady_clock::now();
		TileComponent* tileComponent = registry->AddComponent<TileComponent>(tileEntityID);
		tileComponent->altitude = Random::RandomFloat(0.0f, 0.05f);
		tileComponent->worldID = worldID;
		//end = std::chrono::steady_clock::now();
		//compTime += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

		// Wind vertices counter clockwise
		//begin = std::chrono::steady_clock::now();
		std::vector<glm::vec3> centreVertices;
		for (auto& memberPolygon : vertex->memberPolygons)
		{
			centreVertices.push_back(memberPolygon->Centroid());
		}
		WindOutward(centreVertices, true);
		//end = std::chrono::steady_clock::now();
		//windTime += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

		// Generate GLVertices from goldberg data
		//begin = std::chrono::steady_clock::now();
		std::vector<GLVertex> vertices;
		glm::vec3 tileNormal = glm::normalize(Centroid(centreVertices));
		tileComponent->normal = tileNormal;
		//glm::vec3 faceColour = glm::vec3(((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX));
		//glm::vec3 faceColour = glm::vec3(0.05f, tileComponent->altitude, 0.125f);
		//glm::vec3 faceColour = orderedVertices.size() == 6 ? glm::vec3(1.0f, 0.1f, 0.1f) : glm::vec3(0.1f);
		//glm::vec3 faceColour = centreVertices.size() == 6 ? glm::vec3(0.8f, 0.9f, ((float)rand() / RAND_MAX) * 0.1f + 0.9f) : glm::vec3(1.0f, 0.2f, 0.2f);
		int oldVertexCount = worldModel->model.mesh.vertices.size();
		for (auto& vertex : centreVertices)
		{
			GLVertex newVertex;
			newVertex.position = vertex;
			newVertex.normal = tileNormal;
			//newVertex.colour = faceColour;
			newVertex.texUV = glm::vec2(0.0f);

			worldModel->model.mesh.vertices.push_back(newVertex);
			tileToVertexIndices[tileComponent].push_back(worldModel->model.mesh.vertices.size() - 1);
		}
		//end = std::chrono::steady_clock::now();
		//glVertexTime += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

		// Generate triangle indices to cover tile face
		//begin = std::chrono::steady_clock::now();
		std::vector<GLuint> indices = std::vector<GLuint>();
		for (int i = 1; i < centreVertices.size() - 1; i++)
		{
			worldModel->model.mesh.indices.push_back(oldVertexCount);
			worldModel->model.mesh.indices.push_back(oldVertexCount + i);
			worldModel->model.mesh.indices.push_back(oldVertexCount + i + 1);
		}
		//end = std::chrono::steady_clock::now();
		//indexTime += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

		// Initialise GLTexture for tile model mesh
		//begin = std::chrono::steady_clock::now();
		GLTexture tileTexture;//("data/textures/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
		//end = std::chrono::steady_clock::now();
		//textureTime += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	}
	end = std::chrono::steady_clock::now();
	std::cout << "done, " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;

	/*std::cout << std::endl;
	std::cout << "     Average entity registry time: " << (regTime / geodesic.vertices.size()).count() << "ms" << std::endl;
	std::cout << "  Average component creation time: " << (compTime / geodesic.vertices.size()).count() << "ms" << std::endl;
	std::cout << "      Average vertex winding time: " << (windTime / geodesic.vertices.size()).count() << "ms" << std::endl;
	std::cout << "   Average GLVertex creation time: " << (glVertexTime / geodesic.vertices.size()).count() << "ms" << std::endl;
	std::cout << "      Average index creation time: " << (indexTime / geodesic.vertices.size()).count() << "ms" << std::endl;
	std::cout << "    Average texture creation time: " << (textureTime / geodesic.vertices.size()).count() << "ms" << std::endl;
	std::cout << " Average mesh initialisation time: " << (meshInitTime / geodesic.vertices.size()).count() << "ms" << std::endl;
	std::cout << "Average model initialisation time: " << (modelInitTime / geodesic.vertices.size()).count() << "ms" << std::endl;*/ // old GL time testing
	/*
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
	std::cout << " done, " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " milliseconds" << std::endl;
	*/ // old tile neighbour resolution
	/*
	// Generate terrain
	std::cout << "Generating initial terrain quadrants...";
	begin = std::chrono::steady_clock::now();
	glm::vec3 XYZ(1.0f, 1.0f, 1.0f);
	glm::vec3 xYZ(-1.0f, 1.0f, 1.0f);
	glm::vec3 XyZ(1.0f, -1.0f, 1.0f);
	glm::vec3 XYz(1.0f, 1.0f, -1.0f);
	glm::vec3 xyZ(-1.0f, -1.0f, 1.0f);
	glm::vec3 xYz(-1.0f, 1.0f, -1.0f);
	glm::vec3 Xyz(1.0f, -1.0f, -1.0f);
	glm::vec3 xyz(-1.0f, -1.0f, -1.0f);

	std::vector<TerrainQuadrant> sphericalQuadrants = {
		TerrainQuadrant(xYZ, xyZ, XyZ, XYZ, 0.5f),
		TerrainQuadrant(XYZ, XyZ, Xyz, XYz, 0.5f),
		TerrainQuadrant(XYz, Xyz, xyz, xYz, 0.5f),
		TerrainQuadrant(xYz, xyz, xyZ, xYZ, 0.5f),
		TerrainQuadrant(xYz, xYZ, XYZ, XYz, 0.5f),
		TerrainQuadrant(xyz, xyZ, XyZ, Xyz, 0.5f)
	};
	end = std::chrono::steady_clock::now();
	std::cout << " done, " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " milliseconds" << std::endl;

	std::cout << "Subdividing terrain quadrants...";
	begin = std::chrono::steady_clock::now();
	std::vector<TerrainQuadrant> nextLevelQuadrants;
	int terrainGenerationLevels = 7;
	float randomisationRange = 0.1f;
	for (int i = 0; i < terrainGenerationLevels; i++)
	{
		for (auto& quadrant : sphericalQuadrants)
		{
			SphericalQuadrant(quadrant, size);
			auto quartered = QuarterQuadrant(quadrant, randomisationRange / (powf(2, i)));
			for (auto& quarter : quartered)
			{
				nextLevelQuadrants.push_back(quarter);
			}
		}
		sphericalQuadrants = nextLevelQuadrants;
		nextLevelQuadrants.clear();
	}
	end = std::chrono::steady_clock::now();
	std::cout << " done, " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " milliseconds" << std::endl;

	std::cout << "Applying terrain colours...";
	begin = std::chrono::steady_clock::now();
	float seaLevel = 0.5f;
	auto modelIDs = registry->ViewIDs<ModelComponent>();

	for (auto modelID : modelIDs)
	{
		auto modelPtr = registry->GetComponent<ModelComponent>(modelID);
		auto& model = modelPtr->model;
		auto& mesh = model.mesh;

		glm::vec3 meshCentre(0.0f);
		for (auto& vertex : mesh.vertices)
		{
			meshCentre += vertex.position;
		}
		meshCentre /= (float)mesh.vertices.size();
		meshCentre = glm::normalize(meshCentre);

		TerrainQuadrant* appliedTerrain = nullptr;
		glm::vec3 bestCentre;
		for (auto& quadrant : sphericalQuadrants)
		{
			if (appliedTerrain == nullptr)
			{
				appliedTerrain = &quadrant;
				continue;
			}
			SphericalQuadrant(quadrant, size);
			glm::vec3 quadrantCentre = glm::normalize(QuadrantCentre(quadrant));
			if (abs(glm::angle(meshCentre, quadrantCentre)) < abs(glm::angle(meshCentre, bestCentre)))
			{
				appliedTerrain = &quadrant;
				bestCentre = quadrantCentre;
			}
		}
		float tileRange = randomisationRange / (powf(2, terrainGenerationLevels));
		float tileAltitude = appliedTerrain->altitude + ((float)rand() / RAND_MAX) * tileRange - (tileRange / 2);
		for (auto& vertex : mesh.vertices)
		{
			glm::vec3 land = glm::vec3(0.02f, 0.6f - (tileAltitude - seaLevel), 0.125f);
			glm::vec3 water = glm::vec3(0.02f, 0.8f - abs(tileAltitude - seaLevel), 0.95f);
			vertex.colour = tileAltitude >= seaLevel ? land : water;
		}

		mesh.Setup();
	}
	end = std::chrono::steady_clock::now();
	std::cout << " done, " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " milliseconds" << std::endl;
	*/ // old terrain generation

	std::vector<AltitudeSeed> altitudeSeeds;
	for (int i = 0; i < 100; i++)
	{
		altitudeSeeds.push_back(AltitudeSeed{
			glm::normalize(glm::vec3{ Random::RandomFloat(-1.0f, 2.0f), Random::RandomFloat(-1.0f, 2.0f), Random::RandomFloat(-1.0f, 2.0f) }),
			Random::RandomFloat(0.4f, 0.8f),
			Random::RandomFloat(0.0f, 1.0f) });
	}

	float seaLevel = 0.5f;
	auto tiles = registry->View<TileComponent>();
	float avgAltitude = 0.0f;
	for (auto& tile : tiles)
	{
		float totalEffect = 0.0f;
		for (auto& altitudeSeed : altitudeSeeds)
		{
			float distance = GreatCircleDistance(tile->normal, altitudeSeed.normal);
			totalEffect += altitudeSeed.magnitude * altitudeSeed.altitude / distance;
		}
		totalEffect /= altitudeSeeds.size();
		tile->altitude += totalEffect;
		avgAltitude += tile->altitude;
	}
	avgAltitude /= tiles.size();

	for (auto& tile : tiles)
	{
		glm::vec3 land = glm::vec3(0.02f, 0.6f - (tile->altitude - avgAltitude), 0.125f);
		glm::vec3 water = glm::vec3(0.02f, 0.8f - abs(tile->altitude - avgAltitude), 0.95f);
		glm::vec3 colour = tile->altitude >= avgAltitude ? land : water;

		for (auto& vertexIndex : tileToVertexIndices[tile])
		{
			worldModel->model.mesh.vertices[vertexIndex].colour = colour;
		}
	}

	worldModel->model.mesh.Setup();

	std::cout << "Geodesic vertex count: " << geodesic.vertices.size() << std::endl;
	std::cout << "Geodesic face count: " << geodesic.faces.size() << std::endl;
}
