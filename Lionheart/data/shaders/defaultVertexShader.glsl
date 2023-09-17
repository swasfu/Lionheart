#version 330

layout (location = 0) in vec3 dPosition;
layout (location = 1) in vec3 dNormal;
layout (location = 2) in vec4 dColour;

out vec4 vColour;
out vec3 vNormal;
out vec3 vPosition;

uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 position = translation * rotation * vec4(dPosition, 1.0f);
	vPosition = vec3(position);
	vColour = dColour;
	vNormal = vec3(rotation * vec4(dNormal, 1.0f));
	gl_Position = projection * view * position;
}