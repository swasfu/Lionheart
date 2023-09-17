#pragma once

#include <thirdparty/glm/glm.hpp>

struct GLVertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec4 colour;
	glm::vec2 texUV;
};