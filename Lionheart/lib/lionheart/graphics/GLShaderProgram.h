#pragma once

#include <thirdparty/glad/gl.h>
#include <string>

class GLShaderProgram
{
public:
	GLShaderProgram(std::string, std::string);
	GLuint id;
	void Use();
	void Delete();
};