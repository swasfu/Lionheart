#pragma once

#include <thirdparty/glad/gl.h>
#include <thirdparty/glm/glm.hpp>

class GLCamera
{
public:
	GLCamera(int width, int height);
	glm::vec3 position;
	glm::vec3 orientation;
	glm::vec3 up;

	int width;
	int height;

	float speed = 0.01f;
	float sensitivity = 1.0f;

	glm::mat4 ViewMatrix();
	glm::mat4 ProjectionMatrix(float fov, float near, float far);
};