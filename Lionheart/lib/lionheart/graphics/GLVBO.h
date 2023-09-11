#pragma once

#include "GLVertex.h"

#include <thirdparty/glad/gl.h>

#include <vector>

class GLVBO
{
public:
	GLVBO(std::vector<GLVertex>& vertices);
	void Bind();
	void Unbind();
	void Delete();
	GLuint id;
};