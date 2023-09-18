#include "worldgen/World.h"

#include "worldgen/Fractal.h"
#include "worldgen/components/TileComponent.h"
#include "worldgen/components/CloudComponent.h"
#include "worldgen/components/ModelComponent.h"

#include "math/polygons/Polyhedron.h"
#include "math/polygons/Icosahedron.h"
#include "math/polygons/Cube.h"
#include "math/Random.h"
#include "math/Constants.h"

#include <thirdparty/glm/glm.hpp>
#include <thirdparty/glm/gtx/vector_angle.hpp>

#include <iostream>
#include <chrono>
#include <map>

/*
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
*/

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

PolyVertex* AddVertexIfNotExists(Polyhedron& polyhedron, PolyVertex& vertex, std::vector<PolyVertex*>& existingVertices)
{
	for (auto& edgeVertex : existingVertices)
	{
		if (vertex.IsOverlapping(edgeVertex, 0.001f)) return edgeVertex;
	}
	auto newEdgeVertex = polyhedron.AddVertex(vertex);
	existingVertices.push_back(newEdgeVertex);
	return newEdgeVertex;
}

/*
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

float GreatCircleDistance(glm::vec3 a, glm::vec3 b, float radius)
{
	return radius * atanf(glm::length(glm::cross(a, b)) / glm::dot(a, b));
}
*/

glm::vec3 DetermineBiome(ValueMap& altitudeFractal, ValueMap& precipitationFractal, ValueMap& soilFractal, float latitude, float longitude, float seaLevel)
{
	float altitude = altitudeFractal.Value(latitude, longitude);
	altitude -= altitudeFractal.average;
	altitude /= altitudeFractal.stdev;

	float precipitation = precipitationFractal.Value(latitude, longitude);
	precipitation -= precipitationFractal.average;
	precipitation /= precipitationFractal.stdev;

	float soil = soilFractal.Value(latitude, longitude);
	soil -= soilFractal.average;
	soil /= soilFractal.stdev;

	float temperature = (-fabsf(latitude) * 1.0f) + Random::RandomFloat(-0.01f, 0.01f);
	//std::cout << temperature << std::endl;

	temperature -= (altitude > seaLevel ? (altitude - seaLevel) : 0) * 0.35f;
	precipitation -= (altitude > seaLevel ? (altitude - seaLevel) : 0) * 0.1f;

	if (temperature < -0.85f)
	{
		return glm::vec3(224, 221, 216);
	}

	if (altitude < seaLevel)
	{
		float scale = seaLevel - altitude;
		return glm::vec3(40 - scale * 40.0f, 120 - scale * 30.0f, 180 - scale * 20.0f);
	}

	if (soil < -1.0f)
	{
		if (precipitation < -1.0f)
		{
			if (temperature < -0.8f)
			{
				// Barren wasteland
				return glm::vec3(224, 221, 216);
			} else if (temperature < -0.4f)
			{
				// Temperate desert
				return glm::vec3(227, 235, 138);
			} else
			{
				// Arid desert
				return glm::vec3(247, 235, 176);
			}
		} else if (precipitation < 1.0f)
		{
			if (temperature < -0.8f)
			{
				// Tundra
				return glm::vec3(189, 196, 151);
			} else if (temperature < -0.4f)
			{
				// Steppe
				return glm::vec3(113, 140, 97);
			} else
			{
				// Arid grassland
				return glm::vec3(140, 126, 97);
			}

		} else
		{
			if (temperature < -0.8f)
			{
				// Frozen bog
				return glm::vec3(50, 54, 47);
			} else if (temperature < -0.4f)
			{
				// Temperate bog
				return glm::vec3(41, 46, 3);
			} else
			{
				// Arid shrubland
				return glm::vec3(128, 110, 75);
			}

		}
	} else if (soil < 1.0f)
	{
		if (precipitation < -1.0f)
		{
			if (temperature < -0.8f)
			{
				// Tundra
				return glm::vec3(189, 196, 151);
			} else if (temperature < -0.4f)
			{
				// Steppe
				return glm::vec3(113, 140, 97);
			} else
			{
				// Arid grassland
				return glm::vec3(140, 126, 97);
			}

		} else if (precipitation < 1.0f)
		{
			if (temperature < -0.8f)
			{
				// Boreal forest
				return glm::vec3(34, 41, 27);
			} else if (temperature < -0.4f)
			{
				// Woodlands
				return glm::vec3(77, 92, 51);
			} else
			{
				// Savannah
				return glm::vec3(102, 82, 32);
			}

		} else
		{
			if (temperature < -0.8f)
			{
				// Boreal marshland
				return glm::vec3(24, 31, 21);
			} else if (temperature < -0.4f)
			{
				// Temperate marshland
				return glm::vec3(30, 36, 22);
			} else
			{
				// Rainforest
				return glm::vec3(48, 105, 8);
			}

		}
	} else
	{
		if (precipitation < -1.0f)
		{
			if (temperature < -0.8f)
			{
				// Tundra
				return glm::vec3(189, 196, 151);
			} else if (temperature < -0.4f)
			{
				// Steppe
				return glm::vec3(113, 140, 97);
			} else
			{
				// Arid shrubland
				return glm::vec3(128, 110, 75);
			}

		} else if (precipitation < 1.0f)
		{
			if (temperature < -0.8f)
			{
				// Taiga
				return glm::vec3(21, 36, 10);
			} else if (temperature < -0.4f)
			{
				// Forest
				return glm::vec3(36, 64, 20);
			} else
			{
				// Rainforest
				return glm::vec3(36, 87, 0);
			}

		} else
		{
			if (temperature < -0.8f)
			{
				// Boreal swamp
				return glm::vec3(13, 20, 10);
			} else if (temperature < -0.4f)
			{
				// Temperate swamp
				return glm::vec3(12, 28, 6);
			} else
			{
				// Tropical swamp
				return glm::vec3(10, 38, 0);
			}

		}
	}
}

World::World()
{}

void World::GenerateTiles(Registry* registry, float size, int subdivisions)
{
	Random::Seed("test");

	CreateFractal(altitudeMap, powf(2, 10), 1.0f, 1.55f);
	CreateFractal(precipitationMap, powf(2, 10), 1.0f, 1.8f);
	CreateFractal(soilMap, powf(2, 10), 1.0f, 1.8f);
	CreateFractal(temperatureMap, powf(2, 8), 1.0f, 2.0f);

	seaLevel = 0.5f;

	Polyhedron icosahedron = Icosahedron(size);
	Polyhedron geodesic;

	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;

	std::cout << "Subdividing icosahedron into geodesic...";
	begin = std::chrono::steady_clock::now();
	std::vector<PolyVertex*> edgeVertices;
	int edgeCount = 0;
	int innerCount = 0;
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
			vertices.push_back(AddVertexIfNotExists(geodesic, nextA, edgeVertices));
			c = a;
			for (int j = 1; j < subdivisions + 1 - i; j++)
			{
				c += ac;
				PolyVertex nextC = PolyVertex(c);
				if (i == 0 || j == (subdivisions - i))
				{
					PolyVertex nextC = PolyVertex(c);
					vertices.push_back(AddVertexIfNotExists(geodesic, nextC, edgeVertices));
				} else
				{
					vertices.push_back(geodesic.AddVertex(nextC));
				}
			}
			a += ab;
		}

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
	}
	end = std::chrono::steady_clock::now();
	std::cout << " done, " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;

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
	std::map<TileComponent*, std::vector<int>> tileToVertexIndices;
	std::map<TileComponent*, std::vector<PolyVertex*>> tileToNeighbourVertices;
	std::map<PolyVertex*, TileComponent*> vertexToTile;
	for (auto& vertex : geodesic.vertices)
	{
		EntityID tileEntityID = registry->RegisterEntity();

		TileComponent* tile = registry->AddComponent<TileComponent>(tileEntityID);
		vertexToTile[vertex.get()] = tile;
		tile->worldID = worldID;

		std::vector<glm::vec3> centreVertices;

		auto& neighbourVertices = tileToNeighbourVertices[tile];

		for (auto& memberPolygon : vertex->memberPolygons)
		{
			centreVertices.push_back(memberPolygon->Centroid());
			for (auto& neighbourVertex : memberPolygon->vertices)
			{
				if (std::find(neighbourVertices.begin(), neighbourVertices.end(), neighbourVertex) == neighbourVertices.end())
				{
					neighbourVertices.push_back(neighbourVertex);
				}
			}
		}
		WindOutward(centreVertices, true);

		std::vector<GLVertex> vertices;
		glm::vec3 tileNormal = glm::normalize(Centroid(centreVertices));
		//std::cout << "(" << latitude << ", " << longitude << ")" << std::endl;
		float latitude = NormalToLatitude(tileNormal);
		float longitude = NormalToLongitude(tileNormal);

		tile->latitude = latitude;
		tile->longitude = longitude;

		glm::vec3 faceColour = DetermineBiome(altitudeMap, precipitationMap, soilMap, latitude, longitude, 0.5f) / 256.0f;

		int oldVertexCount = worldModel->model.mesh.vertices.size();
		for (auto& vertex : centreVertices)
		{
			GLVertex newVertex;
			newVertex.position = vertex;
			newVertex.normal = tileNormal;
			newVertex.colour = glm::vec4(faceColour, 1.0f);
			newVertex.texUV = glm::vec2(0.0f);

			worldModel->model.mesh.vertices.push_back(newVertex);
			tileToVertexIndices[tile].push_back(worldModel->model.mesh.vertices.size() - 1);
		}

		std::vector<GLuint> indices = std::vector<GLuint>();
		for (int i = 1; i < centreVertices.size() - 1; i++)
		{
			worldModel->model.mesh.indices.push_back(oldVertexCount);
			worldModel->model.mesh.indices.push_back(oldVertexCount + i);
			worldModel->model.mesh.indices.push_back(oldVertexCount + i + 1);
		}
	}
	end = std::chrono::steady_clock::now();
	std::cout << "done, " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;

	std::cout << "Resolving tile neighbours...";
	begin = std::chrono::steady_clock::now();
	for (auto& pair : tileToNeighbourVertices)
	{
		auto tile = pair.first;
		auto& neighbours = pair.second;
		for (auto& neighbour : neighbours)
		{
			auto neighbourTile = vertexToTile[neighbour];
			tile->neighbours.push_back(tile->id);
		}
	}
	end = std::chrono::steady_clock::now();
	std::cout << " done, " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " milliseconds" << std::endl;

	worldModel->model.mesh.Setup();
	worldModel->model.position.z = -2000.0f;
	worldModel->model.rotation = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::angleAxis(glm::radians(23.5f), glm::vec3(0.0f, 1.0f, 0.0f));

	std::cout << "Geodesic vertex count: " << geodesic.vertices.size() << std::endl;
	std::cout << "Geodesic face count: " << geodesic.faces.size() << std::endl;
	std::cout << "Average face altitude: " << altitudeMap.average << std::endl;

	GenerateClimateMap();
	//GenerateClouds(registry, size * 1.02f, subdivisions);
}

void World::GenerateClimateMap()
{
	averageTemperatureMap = temperatureMap;
	glm::quat tilt = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::angleAxis(glm::radians(23.5f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat hourlyRotations[24];
	int dayCount = 30;
	int hourCount = 24;
	for (int i = 0; i < 24; i++)
	{
		hourlyRotations[i] = glm::angleAxis(glm::radians((float)i / (float)hourCount * 360.0f - 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	for (int day = 0; day < dayCount; day++)
	{
		std::cout << "Day " << day << std::endl;
		glm::quat revolution = glm::angleAxis(glm::radians((float)day / (float)dayCount * 360.0f - 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		for (int hour = 0; hour < 24; hour++)
		{
			UpdateTemperature(glm::mat4_cast(tilt * revolution * hourlyRotations[hour]) * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 1.0f);
			for (int i = 0; i < temperatureMap.size - 1; i++)
			{
				for (int j = 0; j < temperatureMap.size - 1; j++)
				{
					averageTemperatureMap.values[i * temperatureMap.size + j] += temperatureMap.values[i * temperatureMap.size + j];
				}
			}
		}
	}

	for (int i = 0; i < temperatureMap.size - 1; i++)
	{
		for (int j = 0; j < temperatureMap.size - 1; j++)
		{
			averageTemperatureMap.values[i * temperatureMap.size + j] /= (dayCount * hourCount);
		}
	}

	temperatureMap = averageTemperatureMap;
}

void World::GenerateClouds(Registry* registry, float size, int subdivisions)
{
	Polyhedron icosahedron = Icosahedron(size);
	Polyhedron geodesic;

	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;

	std::cout << "Subdividing icosahedron into geodesic...";
	begin = std::chrono::steady_clock::now();
	std::vector<PolyVertex*> edgeVertices;
	int edgeCount = 0;
	int innerCount = 0;
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
			vertices.push_back(AddVertexIfNotExists(geodesic, nextA, edgeVertices));
			c = a;
			for (int j = 1; j < subdivisions + 1 - i; j++)
			{
				c += ac;
				PolyVertex nextC = PolyVertex(c);
				if (i == 0 || j == (subdivisions - i))
				{
					PolyVertex nextC = PolyVertex(c);
					vertices.push_back(AddVertexIfNotExists(geodesic, nextC, edgeVertices));
				} else
				{
					vertices.push_back(geodesic.AddVertex(nextC));
				}
			}
			a += ab;
		}

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
	}
	end = std::chrono::steady_clock::now();
	std::cout << " done, " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;

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
	EntityID cloudsID = registry->RegisterEntity();
	ModelComponent* cloudsModel = registry->AddComponent<ModelComponent>(cloudsID);
	for (auto& vertex : geodesic.vertices)
	{
		EntityID cloudEntityID = registry->RegisterEntity();

		CloudComponent* cloud = registry->AddComponent<CloudComponent>(cloudEntityID);

		std::vector<glm::vec3> centreVertices;

		for (auto& memberPolygon : vertex->memberPolygons)
		{
			centreVertices.push_back(memberPolygon->Centroid());
		}
		WindOutward(centreVertices, true);

		std::vector<GLVertex> vertices;
		glm::vec3 tileNormal = glm::normalize(Centroid(centreVertices));
		//std::cout << "(" << latitude << ", " << longitude << ")" << std::endl;
		float latitude = NormalToLatitude(tileNormal);
		float longitude = NormalToLongitude(tileNormal);

		int oldVertexCount = cloudsModel->model.mesh.vertices.size();
		float cloudEffect = precipitationMap.Value(latitude, longitude);
		cloudEffect -= precipitationMap.average;
		cloudEffect /= precipitationMap.stdev;
		for (auto& vertex : centreVertices)
		{
			GLVertex newVertex;
			newVertex.position = vertex;
			newVertex.normal = tileNormal;
			newVertex.colour = glm::vec4(glm::vec3(1.0f - cloudEffect / 3.0f), cloudEffect);
			newVertex.texUV = glm::vec2(0.0f);

			cloudsModel->model.mesh.vertices.push_back(newVertex);
		}

		std::vector<GLuint> indices = std::vector<GLuint>();
		for (int i = 1; i < centreVertices.size() - 1; i++)
		{
			cloudsModel->model.mesh.indices.push_back(oldVertexCount);
			cloudsModel->model.mesh.indices.push_back(oldVertexCount + i);
			cloudsModel->model.mesh.indices.push_back(oldVertexCount + i + 1);
		}
	}
	end = std::chrono::steady_clock::now();
	std::cout << "done, " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;

	cloudsModel->model.mesh.Setup();
	cloudsModel->model.position.z = -20.0f;
}

void World::UpdateTemperature(glm::vec3 sunDirection, float deltaT)
{
	float spaceTemp = -270.0f;
	float spaceLoss = 0.003f;
	float sunEffect = 2.75f;
	float sunExp = 1.1f;
	for (int i = 0; i < temperatureMap.size - 1; i++)
	{
		for (int j = 0; j < temperatureMap.size - 1; j++)
		{
			float latitude, longitude;
			temperatureMap.MapCoordsToLatitudeLongitude(i, j, latitude, longitude);
			float altitudeDeviation = (altitudeMap.Value(latitude, longitude) - altitudeMap.average) / altitudeMap.stdev - seaLevel;
			float localSpaceLoss = spaceLoss;// / altitudeDeviation >= 0 ? (1.0f + powf(altitudeDeviation, 1.05f) * 0.001f) : 1.0f;
			glm::vec3 normal = LatitudeLongitudeToNormal(latitude, longitude);
			float sunAmount = glm::dot(normal, sunDirection);
			temperatureMap.values[i * temperatureMap.size + j] *= (1.0f - localSpaceLoss);
			temperatureMap.values[i * temperatureMap.size + j] += localSpaceLoss * spaceTemp;
			if (sunAmount > 0)
			{
				temperatureMap.values[i * temperatureMap.size + j] += powf(sunAmount, sunExp) * sunEffect;
			}
		}
	}
}

void World::UpdateTemperatureModel(Registry* registry)
{
	auto worlds = registry->View<ModelComponent>();
	int bullshit = 1;
	for (auto& world : worlds)
	{
		if (bullshit)
		{
			bullshit = 0;

			for (auto& vertex : world->model.mesh.vertices)
			{
				float latitude = NormalToLatitude(vertex.normal);
				float longitude = NormalToLongitude(vertex.normal);
				float temp = temperatureMap.Value(latitude, longitude);
				temp += 60.0f;
				temp /= 90.0f;
				vertex.colour = glm::vec4(temp, 0.0f, 1.0f - temp, 1.0f);
			}

			world->model.mesh.ResetVertices();
		}
	}
}

void World::UpdatePrecipitation(Registry* registry, EntityID cloudsID)
{
	// Cloud movement
	ValueMap precipitationCopy = precipitationMap;
	for (int i = 0; i < precipitationMap.size - 1; i++)
	{
		for (int j = 0; j < precipitationMap.size - 1; j++)
		{
			int left = QuickOverflow(i - 1, precipitationMap.size);
			int right = QuickOverflow(i + 1, precipitationMap.size);
			int top = QuickOverflow(j - 1, precipitationMap.size);
			int bottom = QuickOverflow(j + 1, precipitationMap.size);

			int index = i * precipitationMap.size + j;
			int leftIndex = left * precipitationMap.size + j;
			int rightIndex = right * precipitationMap.size + j;
			int topIndex = i * precipitationMap.size + top;
			int bottomIndex = i * precipitationMap.size + bottom;

			float value = precipitationMap.values[index];
			float leftValue = precipitationMap.values[leftIndex];
			float rightValue = precipitationMap.values[rightIndex];
			float topValue = precipitationMap.values[topIndex];
			float bottomValue = precipitationMap.values[bottomIndex];

			int lowerCount = 0;
			float averageValue = 0.0f;
			bool useLeft = value > leftValue;
			if (useLeft)
			{
				lowerCount++;
				averageValue += leftValue;
			}
			bool useRight = value > rightValue;
			if (useRight)
			{
				lowerCount++;
				averageValue += rightValue;
			}
			bool useTop = value > topValue;
			if (useTop)
			{
				lowerCount++;
				averageValue += topValue;
			}
			bool useBottom = value > bottomValue;
			if (useBottom)
			{
				lowerCount++;
				averageValue += bottomValue;
			}

			averageValue /= lowerCount;

			while (true)
			{
				bool change = false;
				if (useLeft && leftValue >= averageValue)
				{
					averageValue *= lowerCount;
					averageValue -= leftValue;
					lowerCount--;
					averageValue /= lowerCount;
					useLeft = false;
					change = true;
				}
				if (useRight && rightValue >= averageValue)
				{
					averageValue *= lowerCount;
					averageValue -= rightValue;
					lowerCount--;
					averageValue /= lowerCount;
					useRight = false;
					change = true;
				}
				if (useTop && topValue >= averageValue)
				{
					averageValue *= lowerCount;
					averageValue -= rightValue;
					lowerCount--;
					averageValue /= lowerCount;
					useTop = false;
					change = true;
				}
				if (useBottom && bottomValue >= averageValue)
				{
					averageValue *= lowerCount;
					averageValue -= bottomValue;
					lowerCount--;
					averageValue /= lowerCount;
					useBottom = false;
					change = true;
				}
				if (!change) break;
			}

			if (lowerCount > 0) precipitationCopy.values[index] -= (value - averageValue);

			if (useLeft) precipitationCopy.values[leftIndex] += (averageValue - leftValue);
			if (useRight) precipitationCopy.values[rightIndex] += (averageValue - rightValue);
			if (useTop) precipitationCopy.values[topIndex] += (averageValue - topValue);
			if (useBottom) precipitationCopy.values[bottomIndex] += (averageValue - bottomValue);
		}
	}

	precipitationMap.Free();
	precipitationMap.values = precipitationCopy.values;
	precipitationCopy.Free();

	// Rain
	float precipAltDiff = (float)altitudeMap.size / (float)precipitationMap.size;
	for (int i = 0; i < precipitationMap.size - 1; i++)
	{
		for (int j = 0; j < precipitationMap.size - 1; j++)
		{
			int index = i * precipitationMap.size + j;

			if ((altitudeMap.values[(int)(i * precipAltDiff) * altitudeMap.size + (int)(j * precipAltDiff)] - altitudeMap.average) / altitudeMap.stdev < 0.5f) precipitationMap.values[index] += 0.01f;
			else precipitationMap.values[index] *= 0.5f;
		}
	}

	auto clouds = registry->GetComponent<ModelComponent>(cloudsID);
	for (auto& cloudVertex : clouds->model.mesh.vertices)
	{
		float latitude = NormalToLatitude(cloudVertex.normal);
		float longitude = NormalToLongitude(cloudVertex.normal);

		float cloudEffect = precipitationMap.Value(latitude, longitude);
		//cloudEffect -= precipitation.average;
		//cloudEffect /= precipitation.stdev;

		cloudVertex.colour = glm::vec4(glm::vec3(1.0f - cloudEffect * 0.6f), cloudEffect * 0.6f);
	}

	clouds->model.mesh.ResetVertices();
}