#include "lionheart/graphics/GLVAO.h"

GLVAO::GLVAO()
{
	glGenVertexArrays(1, &id);
}

void GLVAO::LinkAttrib(GLVBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	vbo.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	vbo.Unbind();
}

void GLVAO::Bind()
{
	glBindVertexArray(id);
}

void GLVAO::Unbind()
{
	glBindVertexArray(0);
}

void GLVAO::Delete()
{
	glDeleteVertexArrays(1, &id);
}