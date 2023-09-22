#include "worldgen/World.h"

#include "worldgen/Constants.h"
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

PolyVertex* AddEdgeVertex(Polyhedron& polyhedron, PolyVertex& vertex, std::vector<PolyVertex*>& existingVertices)
{
	for (auto& edgeVertex : existingVertices)
	{
		if (vertex.IsOverlapping(edgeVertex, 0.001f)) return edgeVertex;
	}
	auto newEdgeVertex = polyhedron.AddVertex(vertex);
	existingVertices.push_back(newEdgeVertex);
	return newEdgeVertex;
}

glm::vec3 DetermineBiome(CelestialBodyComponent* body, HabitablePlanetComponent* habitat, float latitude, float longitude)
{
	float altitude = body->topography.Value(latitude, longitude);
	altitude -= body->topography.average;
	altitude /= body->topography.stdev;

	float precipitation = habitat->humidity.Value(latitude, longitude);
	precipitation -= habitat->humidity.average;
	precipitation /= habitat->humidity.stdev;

	float soil = habitat->moisture.Value(latitude, longitude);
	soil -= habitat->moisture.average;
	soil /= habitat->moisture.stdev;

	float temperature = habitat->temperature.Value(latitude, longitude);
	temperature *= 100.0f;
	temperature -= 50.0f;

	if (temperature < -20.0f)
	{
		return glm::vec3(224, 221, 216);
	}

	if (altitude < 0.5f)
	{
		float scale = 0.5f - altitude;
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

void CreateBody(CelestialBodyComponent* body, float factor, int resolution)
{
	CreateFractal(body->topography, resolution, 1.55f);
}

void SubdivideIcosahedron(Polyhedron& geodesic, int subdivisions)
{
	Polyhedron icosahedron = Icosahedron(1.0f);

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
			vertices.push_back(AddEdgeVertex(geodesic, nextA, edgeVertices));
			c = a;
			for (int j = 1; j < subdivisions + 1 - i; j++)
			{
				c += ac;
				PolyVertex nextC = PolyVertex(c);
				if (i == 0 || j == (subdivisions - i))
				{
					PolyVertex nextC = PolyVertex(c);
					vertices.push_back(AddEdgeVertex(geodesic, nextC, edgeVertices));
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
}

void UpdateHabitats(Registry* registry)
{
	auto worldIDs = registry->ViewIDs<HabitablePlanetComponent>();

	for (auto worldID : worldIDs)
	{
		auto body = registry->GetComponent<CelestialBodyComponent>(worldID);
		auto habitat = registry->GetComponent<HabitablePlanetComponent>(worldID);

		int width = body->mapResolution;
		int height = body->mapResolution / 2;
		int size = width * height;

		ValueMap temperatureCopy = habitat->temperature;
		ValueMap humidityCopy = habitat->humidity;
		ValueMap moistureCopy = habitat->moisture;

		glm::vec3 sunDirection = glm::normalize(-body->position);

		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				int index = x * height + y;
				int topIndex = index - 1;
				if (y == 0) topIndex = index > size / 2 ? index - size / 2 : index + size / 2;
				int bottomIndex = index + 1;
				if (y == (height - 1))bottomIndex = index > size / 2 ? index - size / 2 : index + size / 2;
				int leftIndex = index - height;
				if (leftIndex < 0) leftIndex += size;
				int rightIndex = index + height;
				if (rightIndex > size) rightIndex -= size;

				// Sun heats the surface
				temperatureCopy[index] *= (1.0f - Constants::SPACE_LOSS);
				temperatureCopy[index] += (Constants::SPACE_LOSS * Constants::SPACE_TEMP);
				float sunAmount = glm::dot(temperatureCopy.MapCoordsToNormal(x, y), sunDirection);
				sunAmount = powf(sunAmount, Constants::SUN_EXP);
				if (sunAmount > 0) temperatureCopy[index] += sunAmount * Constants::SUN_EFFECT;

				// Water evaporates
				if (body->topography[index] < 0)
				{
					humidityCopy[index] += sunAmount * Constants::EVAP_RATE;
				}

				// Wind pushes cool air and water vapour to warm areas
				int coolestIndex = index;
				for (auto neighbourIndex : { topIndex, leftIndex, rightIndex, bottomIndex })
				{
					if (habitat->temperature[neighbourIndex] < habitat->temperature[coolestIndex])
					{
						coolestIndex = neighbourIndex;
					}
				}

				if (coolestIndex != index)
				{
					float tempDiff = habitat->temperature[index] - habitat->temperature[coolestIndex];
					float windChange = (tempDiff / 2.0f) * Constants::WIND_EFF;
					temperatureCopy[index] -= windChange;
					temperatureCopy[coolestIndex] += windChange;

				}
			}
		}
	}
}

void GenerateWorld(Registry* registry, float size, int subdivisions, int resolution)
{
	Random::Seed("test");

	auto worldID = registry->RegisterEntity();

	auto body = registry->AddComponent<CelestialBodyComponent>(worldID);
	CreateBody(body, 1.55f, resolution);

	auto habitat = registry->AddComponent<HabitablePlanetComponent>(worldID);

	CreateFractal(habitat->temperature, resolution, 2.0f);
	CreateFractal(habitat->humidity, resolution, 2.0f);
	CreateFractal(habitat->moisture, resolution, 1.8f);

	float seaLevel = 0.5f;

	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;

	std::cout << "Subdividing icosahedron into geodesic...";
	begin = std::chrono::steady_clock::now();
	Polyhedron geodesic;
	SubdivideIcosahedron(geodesic, subdivisions);
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
	EntityID bodyModelID = registry->RegisterEntity();
	ModelComponent* bodyModel = registry->AddComponent<ModelComponent>(bodyModelID);
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
		NormalToLatitudeLongitude(tileNormal, tile->latitude, tile->longitude);

		glm::vec3 faceColour = DetermineBiome(body, habitat, tile->latitude, tile->longitude) / 256.0f;

		int oldVertexCount = bodyModel->model.mesh.vertices.size();
		for (auto& vertex : centreVertices)
		{
			GLVertex newVertex;
			newVertex.position = vertex;
			newVertex.normal = tileNormal;
			newVertex.colour = glm::vec4(faceColour, 1.0f);
			newVertex.texUV = glm::vec2(0.0f);

			bodyModel->model.mesh.vertices.push_back(newVertex);
			tileToVertexIndices[tile].push_back(bodyModel->model.mesh.vertices.size() - 1);
		}

		std::vector<GLuint> indices = std::vector<GLuint>();
		for (int i = 1; i < centreVertices.size() - 1; i++)
		{
			bodyModel->model.mesh.indices.push_back(oldVertexCount);
			bodyModel->model.mesh.indices.push_back(oldVertexCount + i);
			bodyModel->model.mesh.indices.push_back(oldVertexCount + i + 1);
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

	bodyModel->model.mesh.Setup();
	body->bodyModelID = bodyModelID;

	body->position.z = 150160;
	body->rotation = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::angleAxis(glm::radians(23.5f), glm::vec3(0.0f, 1.0f, 0.0f));
	body->rotationSpeed = glm::radians(360.0f / (24.0f));

	std::cout << "Geodesic vertex count: " << geodesic.vertices.size() << std::endl;
	std::cout << "Geodesic face count: " << geodesic.faces.size() << std::endl;
}