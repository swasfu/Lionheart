#include "GLCamera.h"

#include <thirdparty/glm/gtc/matrix_transform.hpp>

GLCamera::GLCamera(int width, int height, glm::vec3 position) : width(width), height(height), position(position)
{
	orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 GLCamera::ViewProjectionMatrix(float fov, float near, float far)
{
	glm::mat4 view(1.0f);
	view = glm::lookAt(position, position + orientation, up);

	glm::mat4 proj(1.0f);
	proj = glm::perspective(glm::radians(fov), (float)width / height, near, far);

	return proj * view;
}

glm::mat4 GLCamera::ViewMatrix()
{
	return glm::lookAt(position, position + orientation, up);
}

glm::mat4 GLCamera::ProjectionMatrix(float fov, float near, float far)
{
	return glm::perspective(glm::radians(fov), (float)width / height, near, far);
}
