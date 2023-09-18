#include "lionheart/graphics/GLVBO.h"

GLVBO::GLVBO()
{
	id = 0;
}

GLVBO::GLVBO(std::vector<GLVertex>& vertices)
{
	glGenBuffers(1, &id);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLVertex), vertices.data(), GL_STATIC_DRAW);
}

void GLVBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void GLVBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLVBO::Delete()
{
	glDeleteBuffers(1, &id);
}
