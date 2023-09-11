#pragma once

#include <thirdparty/glad/gl.h>

#include <vector>

class GLEBO
{
public:
	GLEBO(std::vector<GLuint>& indices);
	void Bind();
	void Unbind();
	void Delete();
	GLuint id;
};