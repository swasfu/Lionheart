#include "Polyhedron.h"

PolyVertex* Polyhedron::AddVertexIfNotExists(PolyVertex newVertex)
{
	for (auto& vertex : vertices)
	{
		if (newVertex.IsOverlapping(vertex.get(), accuracy_)) return vertex.get();
	}

	vertices.push_back(std::make_unique<PolyVertex>(newVertex));
	return vertices[vertices.size() - 1].get();
}

Polygon* Polyhedron::AddFaceIfNotExists(Polygon newFace)
{
	std::sort(newFace.vertices.begin(), newFace.vertices.end());
	for (auto& face : faces)
	{
		std::sort(face->vertices.begin(), face->vertices.end());
		if (face->vertices == newFace.vertices) return face.get();
	}

	faces.push_back(std::make_unique<Polygon>(newFace));
	auto face = faces[faces.size() - 1].get();
	for (auto& vertex : face->vertices)
	{
		vertex->memberPolygons.push_back(face);
	}
	return face;
}

void Polyhedron::RemoveFace(Polygon* faceToBeRemoved)
{
	for (auto faceIterator = faces.begin(); faceIterator != faces.end();)
	{
		auto& facePtr = *faceIterator;
		if (facePtr.get() != faceToBeRemoved)
		{
			++faceIterator;
			continue;
		}

		for (auto& vertex : facePtr->vertices)
		{
			vertex->RemoveConnectedFace(facePtr.get());
		}
		faceIterator = faces.erase(faceIterator);
	}
}
