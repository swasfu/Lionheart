#include "lionheart/graphics/GLCamera.h"

#include <thirdparty/glm/gtc/matrix_transform.hpp>

GLCamera::GLCamera(int width, int height) : width(width), height(height)
{
	orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 GLCamera::ViewMatrix()
{
	return glm::lookAt(position, position + orientation, up);
}

glm::mat4 GLCamera::ProjectionMatrix(float fov, float near, float far)
{
	return glm::perspective(glm::radians(fov), (float)width / height, near, far);
}
