#include "lionheart/graphics/GLMesh.h"

GLMesh::GLMesh()
{}

GLMesh::GLMesh(std::vector<GLVertex>& vertices,
	std::vector<GLuint>& indices,
	GLTexture texture) : vertices(vertices), indices(indices), texture(texture)
{
	vao.Bind();

	GLVBO vbo(vertices);
	vbo.Bind();
	GLEBO ebo(indices);
	ebo.Bind();

	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(GLVertex), (void*)0);
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(GLVertex), (void*)(3 * sizeof(float)));
	vao.LinkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(GLVertex), (void*)(6 * sizeof(float)));
	vao.LinkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(GLVertex), (void*)(9 * sizeof(float)));
}

void GLMesh::Setup()
{
	vao.Bind();

	GLVBO vbo(vertices);
	vbo.Bind();
	GLEBO ebo(indices);
	ebo.Bind();

	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(GLVertex), (void*)0);
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(GLVertex), (void*)(3 * sizeof(float)));
	vao.LinkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(GLVertex), (void*)(6 * sizeof(float)));
	vao.LinkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(GLVertex), (void*)(9 * sizeof(float)));
}
