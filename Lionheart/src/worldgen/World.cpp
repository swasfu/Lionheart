#include "worldgen/World.h"

#include "worldgen/Fractal.h"
#include "worldgen/components/TileComponent.h"
#include "worldgen/components/CloudComponent.h"
#include "worldgen/components/ModelComponent.h"
#include "worldgen/components/HabitablePlanetComponent.h"
#include "worldgen/components/CelestialBodyComponent.h"

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

glm::vec3 World::DetermineBiome(float latitude, float longitude)
{
	float altitude = altitudeMap.Value(latitude, longitude);
	altitude -= altitudeMap.average;
	altitude /= altitudeMap.stdev;

	float precipitation = precipitationMap.Value(latitude, longitude);
	precipitation -= precipitationMap.average;
	precipitation /= precipitationMap.stdev;

	float soil = soilMap.Value(latitude, longitude);
	soil -= soilMap.average;
	soil /= soilMap.stdev;

	float temperature = temperatureMap.Value(latitude, longitude);
	temperature *= 100.0f;
	temperature -= 50.0f;
	precipitation -= (altitude > seaLevel ? (altitude - seaLevel) : 0) * 0.1f;

	if (temperature < -20.0f)
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
			if (temperature < 0.0f)
			{
				// Barren wasteland
				return glm::vec3(224, 221, 216);
			} else if (temperature < 20.0f)
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
			if (temperature < 0.0f)
			{
				// Tundra
				return glm::vec3(189, 196, 151);
			} else if (temperature < 20.0f)
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
			if (temperature < 0.0f)
			{
				// Frozen bog
				return glm::vec3(50, 54, 47);
			} else if (temperature < 20.0f)
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
			if (temperature < 0.0f)
			{
				// Tundra
				return glm::vec3(189, 196, 151);
			} else if (temperature < 20.0f)
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
			if (temperature < 0.0f)
			{
				// Boreal forest
				return glm::vec3(34, 41, 27);
			} else if (temperature < 20.0f)
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
			if (temperature < 0.0f)
			{
				// Boreal marshland
				return glm::vec3(24, 31, 21);
			} else if (temperature < 20.0f)
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
			if (temperature < 0.0f)
			{
				// Tundra
				return glm::vec3(189, 196, 151);
			} else if (temperature < 20.0f)
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
			if (temperature < 0.0f)
			{
				// Taiga
				return glm::vec3(21, 36, 10);
			} else if (temperature < 20.0f)
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
			if (temperature < 0.0f)
			{
				// Boreal swamp
				return glm::vec3(13, 20, 10);
			} else if (temperature < 20.0f)
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

	CreateFractal(altitudeMap, powf(2, 9), 1.0f, 1.55f);
	CreateFractal(precipitationMap, powf(2, 9), 1.0f, 1.8f);
	CreateFractal(soilMap, powf(2, 9), 1.0f, 1.8f);
	CreateFractal(temperatureMap, powf(2, 9), 1.0f, 2.0f);

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
	float maxLatitude = 0.0f;
	float minLatitude = 10.0f;
	float maxLongitude = 0.0f;
	float minLongitude = 10.0f;
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
		NormalToLatitudeLongitude(tileNormal, tile->latitude, tile->longitude);

		if (tile->latitude < minLatitude) minLatitude = tile->latitude;
		if (tile->latitude > maxLatitude) maxLatitude = tile->latitude;
		if (tile->longitude < minLongitude) minLongitude = tile->longitude;
		if (tile->longitude > maxLongitude) maxLongitude = tile->longitude;

		glm::vec3 faceColour = DetermineBiome(tile->latitude, tile->longitude) / 256.0f;

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

	std::cout << "Maximum latitude: " << maxLatitude << std::endl;
	std::cout << "Minimum latitude: " << minLatitude << std::endl;
	std::cout << "Maximum longitude: " << maxLongitude << std::endl;
	std::cout << "Minimum longitude: " << minLongitude << std::endl;

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
}

/*

void UpdateTemperature(HabitablePlanetComponent* planet, CelestialBodyComponent* body)
{
	glm::vec3 sunDirection = glm::normalize(-body->position);
	auto& altitudeMap = body->topography;
	auto& temperatureMap = planet->temperature;
	float spaceTemp = -270.0f;
	float spaceLoss = 0.00042f;
	float sunEffect = 0.4f;
	float sunExp = 0.9f;
	for (int i = 0; i < temperatureMap.width - 1; i++)
	{
		for (int j = 0; j < temperatureMap.width - 1; j++)
		{
			auto& temperature = temperatureMap.values[i * temperatureMap.width + j];
			float latitude, longitude;
			temperatureMap.MapCoordsToLatitudeLongitude(i, j, latitude, longitude);
			float altitudeDeviation = (altitudeMap.Value(latitude, longitude) - planet->seaLevel) / altitudeMap.stdev;
			float localSpaceLoss = spaceLoss * ((altitudeDeviation >= 0) ? (1.0f + (powf(altitudeDeviation, 2.0f) * 0.02f)) : 1.0f);
			glm::vec3 normal = LatitudeLongitudeToNormal(latitude, longitude);
			float sunAmount = glm::dot(normal, sunDirection);
			temperatureMap.values[i * temperatureMap.width + j] *= (1.0f - localSpaceLoss);
			temperatureMap.values[i * temperatureMap.width + j] += localSpaceLoss * spaceTemp;
			if (sunAmount > 0)
			{
				temperatureMap.values[i * temperatureMap.width + j] += powf(sunAmount, sunExp) * sunEffect;
			}
		}
	}
	temperatureMap.Average();
	temperatureMap.Stdev(temperatureMap.average);
}

void UpdateHabitat(HabitablePlanetComponent* habitat, CelestialBodyComponent* body)
{
	glm::vec3 sun = glm::normalize(-body->position);
	auto& topography = body->topography;
	auto& temperature = habitat->temperature;
	auto& humidity = habitat->humidity;
	auto& moisture = habitat->moisture;
	int resolution = body->mapResolution;
	for (int i = 0; i < resolution - 1; i++)
	{
		for (int j = 0; j < resolution - 1; j++)
		{
			int index = i * resolution + j;
			float& localTemperature = temperature.values[index];
			float& localAltitude = topography.values[index];
			float& localHumidity = humidity.values[index];
			float& localMoisture = moisture.values[index];



			float localSpaceLoss = spaceLoss * ((altitudeDeviation >= 0) ? (1.0f + (powf(altitudeDeviation, 2.0f) * 0.02f)) : 1.0f);
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

void GenerateTiles(Registry* registry, EntityID worldID, int subdivisions)
{
	Polyhedron icosahedron = Icosahedron(1.0f);
	Polyhedron geodesic;

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

	for (auto& vertex : geodesic.vertices) vertex->coords = glm::normalize(vertex->coords);

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

		float latitude = NormalToLatitude(vertex->coords);
		float longitude = NormalToLongitude(vertex->coords);

		tile->latitude = latitude;
		tile->longitude = longitude;

		glm::vec3 faceColour = glm::vec3(1.0f);

		int oldVertexCount = worldModel->model.mesh.vertices.size();
		for (auto& centreVertex : centreVertices)
		{
			GLVertex newVertex;
			newVertex.position = centreVertex;
			newVertex.normal = vertex->coords;
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
}

void GenerateWorld(Registry* registry, int subdivisions)
{
	EntityID worldID = registry->RegisterEntity();
	auto planet = registry->AddComponent<HabitablePlanetComponent>(worldID);
	auto body = registry->AddComponent<CelestialBodyComponent>(worldID);

	int mapResolution = powf(2, 9);

	CreateFractal(body->topography, mapResolution, 1.0f, 1.55f);



	GenerateTiles(registry, worldID, subdivisions);
}
*/