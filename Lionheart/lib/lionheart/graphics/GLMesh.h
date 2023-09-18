#pragma once

#include "GLVAO.h"
#include "GLEBO.h"
#include "GLTexture.h"

#include <vector>

class GLMesh
{
public:
	GLMesh();
	GLMesh(std::vector<GLVertex>& vertices, std::vector<GLuint>& indices, GLTexture texture);
	void Setup();
	void Reset();
	void ResetVertices();
	void ResetIndices();

	std::vector<GLVertex> vertices;
	std::vector<GLuint> indices;
	GLTexture texture;

	GLVAO vao;
	GLVBO vbo;
	GLEBO ebo;
};