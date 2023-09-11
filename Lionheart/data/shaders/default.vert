#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNrm;
layout (location = 2) in vec3 vCol;
layout (location = 3) in vec2 vTex;

out vec3 world;
out vec3 normal;
out vec3 colour;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(vPos, 1.0);

    world = vec3(model * vec4(vPos, 1.0));
    normal = vNrm;
    colour = vCol;
    texCoords = vTex;
}