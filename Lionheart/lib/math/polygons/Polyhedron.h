#pragma once

#include "PolyVertex.h"
#include "Polygon.h"

#include <vector>
#include <memory>
#include <algorithm>

class Polyhedron
{
public:
	PolyVertex* AddVertexIfNotExists(PolyVertex newVertex);
	Polygon* AddFaceIfNotExists(Polygon newFace);
	void RemoveFace(Polygon*);
	std::vector<std::unique_ptr<PolyVertex>> vertices;
	std::vector<std::unique_ptr<Polygon>> faces;
private:
	const float accuracy_ = 0.0001f;
};