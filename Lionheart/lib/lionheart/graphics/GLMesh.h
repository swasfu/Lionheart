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

	std::vector<GLVertex> vertices;
	std::vector<GLuint> indices;
	GLTexture texture;

	GLVAO vao;
};