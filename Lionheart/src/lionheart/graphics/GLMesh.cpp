#include "lionheart/graphics/GLMesh.h"

GLMesh::GLMesh()
{}

GLMesh::GLMesh(std::vector<GLVertex>& vertices,
	std::vector<GLuint>& indices,
	GLTexture texture) : vertices(vertices), indices(indices), texture(texture)
{
	Setup();
}

void GLMesh::Setup()
{
	vao.Bind();
	vbo = GLVBO(vertices);
	vbo.Bind();
	ebo = GLEBO(indices);
	ebo.Bind();

	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(GLVertex), (void*)0);
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(GLVertex), (void*)(3 * sizeof(float)));
	vao.LinkAttrib(vbo, 2, 4, GL_FLOAT, sizeof(GLVertex), (void*)(6 * sizeof(float)));
	vao.LinkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(GLVertex), (void*)(10 * sizeof(float)));
}

void GLMesh::Reset()
{
	vbo.Delete();
	vbo = GLVBO(vertices);
	ebo.Delete();
	ebo = GLEBO(indices);
	vao.Bind();
	vbo.Bind();
	ebo.Bind();

	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(GLVertex), (void*)0);
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(GLVertex), (void*)(3 * sizeof(float)));
	vao.LinkAttrib(vbo, 2, 4, GL_FLOAT, sizeof(GLVertex), (void*)(6 * sizeof(float)));
	vao.LinkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(GLVertex), (void*)(10 * sizeof(float)));
}

void GLMesh::ResetVertices()
{
	vbo.Delete();
	vbo = GLVBO(vertices);
	vao.Bind();
	vbo.Bind();
	ebo.Bind();

	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(GLVertex), (void*)0);
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(GLVertex), (void*)(3 * sizeof(float)));
	vao.LinkAttrib(vbo, 2, 4, GL_FLOAT, sizeof(GLVertex), (void*)(6 * sizeof(float)));
	vao.LinkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(GLVertex), (void*)(10 * sizeof(float)));
}

void GLMesh::ResetIndices()
{
	ebo.Delete();
	ebo = GLEBO(indices);
	vao.Bind();
	vbo.Bind();
	ebo.Bind();

	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(GLVertex), (void*)0);
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(GLVertex), (void*)(3 * sizeof(float)));
	vao.LinkAttrib(vbo, 2, 4, GL_FLOAT, sizeof(GLVertex), (void*)(6 * sizeof(float)));
	vao.LinkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(GLVertex), (void*)(10 * sizeof(float)));
}