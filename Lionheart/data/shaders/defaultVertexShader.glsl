#version 330

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNrm;
layout (location = 2) in vec3 vCol;

out vec3 colour;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * model * vec4(vPos, 1.0);
	colour = vCol;
	normal = vNrm;
}