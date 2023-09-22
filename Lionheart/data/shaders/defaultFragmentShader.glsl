#version 330 core

out vec4 finalColour;

in vec4 vColour;
in vec3 vNormal;
in vec3 vPosition;

uniform vec4 lightColour;
uniform vec3 lightPos;
uniform vec3 cameraPos;

void main()
{
    // ambient lighting
    float ambient = 1.0f;

    vec3 lightDirection = normalize(lightPos - vPosition);

    // diffuse lighting
    vec3 normal = normalize(vNormal);
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    // specular lighting
    vec3 look = normalize(cameraPos - vPosition);
    vec3 reflection = reflect(-lightDirection, vNormal);
    float specular = pow(max(dot(look, reflection), 0.0f), 8) * 0.2f;


    vec4 tempColour = vColour * lightColour * (1.0f * ambient + 1.0f * diffuse + 0.0f * specular);
    tempColour.w = vColour.w;
    finalColour = tempColour;
}