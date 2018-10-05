#version 330 core

in vec3 normal;
in vec3 fragmentPosition;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    // ambient color
    float ambientValue = 0.1;
    vec3 ambient = ambientValue * lightColor;


    // diffuse color
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragmentPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular color
    float specularValue = 0.5;
    vec3 viewDir = normalize(viewPos - fragmentPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularValue * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, 1.0);
}
