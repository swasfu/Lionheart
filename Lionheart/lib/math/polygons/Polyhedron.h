#pragma once

#include "PolyVertex.h"
#include "Polygon.h"

#include <vector>
#include <memory>
#include <algorithm>

class Polyhedron
{
public:
	PolyVertex* AddVertexIfNotExists(PolyVertex& newVertex);
	PolyVertex* AddVertex(PolyVertex& newVertex);
	//Polygon* AddFaceIfNotExists(Polygon newFace);
	Polygon* AddFace(Polygon& newFace);
	void SetScale(float scale);
	void RemoveFace(Polygon*);
	void ClearFaces();
	std::vector<std::unique_ptr<PolyVertex>> vertices;
	std::vector<std::unique_ptr<Polygon>> faces;
};