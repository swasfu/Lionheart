#include "lionheart/graphics/GLModel.h"

#include <thirdparty/glm/gtc/quaternion.hpp>
#include <thirdparty/glm/gtx/quaternion.hpp>

GLModel::GLModel()
{
	position = glm::vec3(0.0f);
	rotation = glm::angleAxis(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}

GLModel::GLModel(GLMesh& mesh) : mesh(mesh)
{
	position = glm::vec3(0.0f);
	rotation = glm::angleAxis(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 GLModel::TranslationMatrix()
{
	return glm::translate(glm::mat4(1.0f), position);
}

glm::mat4 GLModel::RotationMatrix()
{
	return glm::mat4_cast(rotation);
}