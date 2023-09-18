#include "lionheart/graphics/GLEBO.h"

GLEBO::GLEBO()
{
	id = 0;
}

GLEBO::GLEBO(std::vector<GLuint>& indices)
{
	glGenBuffers(1, &id);
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void GLEBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void GLEBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLEBO::Delete()
{
	glDeleteBuffers(1, &id);
}
