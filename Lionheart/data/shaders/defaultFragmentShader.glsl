#version 330 core

out vec4 final;

in vec3 vColour;
in vec3 vNormal;
in vec3 vPosition;

uniform vec4 lightColour;
uniform vec3 lightPos;
uniform vec3 cameraPos;

void main()
{
    // ambient lighting
    float ambient = 0.2f;

    // diffuse lighting
    vec3 normal = normalize(vNormal);
    vec3 lightDirection = normalize(lightPos - vPosition);
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    final = vec4(vColour, 1.0) * lightColour * (ambient + diffuse);
}