#pragma once

#include "GLVBO.h"

class GLVAO
{
public:
	GLuint id;
	GLVAO();

	void LinkAttrib(GLVBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void Bind();
	void Unbind();
	void Delete();
};