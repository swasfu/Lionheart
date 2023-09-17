#include "math/polygons/PolyVertex.h"
#include "math/polygons/Polygon.h"

PolyVertex::PolyVertex(glm::vec3 coords) : coords(coords) {}

void PolyVertex::RemoveConnectedFace(Polygon* faceToBeRemoved)
{
	for (auto connectedFaceIterator = memberPolygons.begin(); connectedFaceIterator != memberPolygons.end();)
	{
		if (*connectedFaceIterator == faceToBeRemoved)
		{
			connectedFaceIterator = memberPolygons.erase(connectedFaceIterator);
		} else ++connectedFaceIterator;
	}
}

bool PolyVertex::IsOverlapping(PolyVertex* other, float accuracy)
{
	return abs(coords.x - other->coords.x) <= accuracy &&
		abs(coords.y - other->coords.y) <= accuracy &&
		abs(coords.z - other->coords.z) <= accuracy;
}