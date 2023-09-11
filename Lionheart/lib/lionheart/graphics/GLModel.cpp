#include "GLModel.h"

GLModel::GLModel()
{}

GLModel::GLModel(GLMesh& mesh) : mesh(mesh)
{
	position = glm::vec3(0.0f);
	rotation = glm::vec3(0.0f);
}
