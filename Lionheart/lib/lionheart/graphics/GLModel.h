#pragma once

#include "GLMesh.h"

#include <thirdparty/glm/gtc/quaternion.hpp>

class GLModel
{
public:
	GLModel();
	GLModel(GLMesh& mesh);
	glm::mat4 TranslationMatrix();
	glm::mat4 RotationMatrix();
	GLMesh mesh;
	glm::dvec3 floatingPosition;
	glm::vec3 position;
	glm::quat rotation;
};