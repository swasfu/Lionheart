#include "GLShaderProgram.h"

#include "lionheart/util/File.h"

#include <thirdparty/glad/gl.h>

GLShaderProgram::GLShaderProgram(std::string vertexShaderFilename, std::string fragmentShaderFilename)
{
	id = glCreateProgram();

	std::string vertexShaderSource = File::ReadFile(vertexShaderFilename);
	const char* vertexShaderCStr = vertexShaderSource.c_str();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCStr, NULL);
	glCompileShader(vertexShader);
	glAttachShader(id, vertexShader);

	std::string fragmentShaderSource = File::ReadFile(fragmentShaderFilename);
	const char* fragmentShaderCStr = fragmentShaderSource.c_str();
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCStr, NULL);
	glCompileShader(fragmentShader);
	glAttachShader(id, fragmentShader);

	glLinkProgram(id);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void GLShaderProgram::Use()
{
	glUseProgram(id);
}

void GLShaderProgram::Delete()
{
	glDeleteProgram(id);
}
