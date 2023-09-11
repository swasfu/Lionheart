#pragma once

#include "GLMesh.h"

class GLModel
{
public:
	GLModel();
	GLModel(GLMesh& mesh);
	GLMesh mesh;
	glm::vec3 position;
	glm::vec3 rotation;
};