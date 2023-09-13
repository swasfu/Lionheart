#include "GLModel.h"

GLModel::GLModel()
{
	position = glm::vec3(0.0f);
	rotation = glm::vec3(0.0f);
}

GLModel::GLModel(GLMesh& mesh) : mesh(mesh)
{
	position = glm::vec3(0.0f);
	rotation = glm::vec3(0.0f);
}
