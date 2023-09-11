#version 330 core

out vec4 final;

in vec3 world;
in vec3 normal;
in vec3 colour;
in vec2 texCoords;

uniform sampler2D tex0;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    final = vec4(colour, 1.0);
}