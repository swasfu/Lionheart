#version 330 core

out vec4 final;

in vec3 colour;

void main()
{
    final = vec4(colour, 1.0);
}