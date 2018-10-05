#version 330 core

in vec3 position;
in vec3 normal;

out vec4 color;

uniform samplerCube skyBox;
uniform vec3 camPosition;

void main()
{
    float ratio = 1.0 / 1.1;
    vec3 I = normalize(position - camPosition);
    vec3 R = refract(I, normalize(normal), ratio);
    color = vec4(texture(skyBox, R).rgb, 1.0);
}
