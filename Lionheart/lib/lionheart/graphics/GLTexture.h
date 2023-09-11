#pragma once

#include "GLShaderProgram.h"

#include <thirdparty/glad/gl.h>
#include <thirdparty/stb/stb_image.h>

class GLTexture
{
public:
	GLTexture();
	GLTexture(std::string filename, GLenum textureType, GLenum slot, GLenum format, GLenum pixelType);
	GLuint id;
	GLuint unit;
	GLenum type;

	void Uniform(GLShaderProgram& shaderProgram, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();
};