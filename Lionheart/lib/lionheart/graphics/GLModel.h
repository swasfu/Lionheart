#pragma once

#include "GLMesh.h"

class GLModel
{
public:
	GLModel();
	GLModel(GLMesh& mesh);
	glm::mat4 ModelMatrix();
	GLMesh mesh;
	glm::vec3 position;
	glm::vec3 rotation;
};