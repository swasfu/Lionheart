#version 330

layout (location = 0) in vec3 dPosition;
layout (location = 1) in vec3 dNormal;
layout (location = 2) in vec3 dColour;

out vec3 vColour;
out vec3 vNormal;
out vec3 vPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	vPosition = vec3(model * vec4(dPosition, 1.0f));
	vColour = dColour;
	vNormal = vec3(model * vec4(dNormal, 1.0f));
	gl_Position = proj * view * vec4(vPosition, 1.0);
}