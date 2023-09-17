#include "lionheart/graphics/GLModel.h"

#include <thirdparty/glm/gtc/quaternion.hpp>
#include <thirdparty/glm/gtx/quaternion.hpp>

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

glm::mat4 GLModel::ModelMatrix()
{
	glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotationMatrix = glm::toMat4(glm::quat(rotation));
	return positionMatrix * rotationMatrix;
}
