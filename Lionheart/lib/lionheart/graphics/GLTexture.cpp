#include "GLTexture.h"

GLTexture::GLTexture()
{}

GLTexture::GLTexture(std::string filename, GLenum textureType, GLenum unit, GLenum format, GLenum pixelType) : unit(unit)
{
	type = textureType;

	int width, height, numChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imgData = stbi_load(filename.c_str(), &width, &height, &numChannels, 0);

	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(type, id);

	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(type, 0, format, width, height, 0, format, pixelType, imgData);
	glGenerateMipmap(type);

	stbi_image_free(imgData);
	glBindTexture(type, 0);
}

void GLTexture::Uniform(GLShaderProgram& shaderProgram, const char* uniform, GLuint unit)
{
	GLuint texUniform = glGetUniformLocation(shaderProgram.id, uniform);
	shaderProgram.Use();
	glUniform1i(texUniform, unit);
}

void GLTexture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(type, id);
}

void GLTexture::Unbind()
{
	glBindTexture(type, 0);
}

void GLTexture::Delete()
{
	glDeleteTextures(1, &id);
}