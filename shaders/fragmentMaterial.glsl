#version 330 core
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectinalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

    float constant;
    float linear;
    float quadratic;
};

in vec2 texCoords;
in vec3 normal;
in vec3 fragmentPosition;

uniform vec3 viewPos;
uniform Material material;

uniform DirectinalLight directionalLight;

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform SpotLight spotLight;

out vec4 FragColor;

vec3 CalcDirectionalLight(DirectinalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, texCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                             light.quadratic * (distance * distance));
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, texCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                            light.quadratic * (distance * distance));

    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 result = vec3(0.0);

    if(theta > light.outerCutOff)
    {
        // diffuse color
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));

        // specular color
        vec3 viewDir = normalize(viewPos - fragmentPosition);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

        result = (diffuse + specular) * attenuation * intensity;
    }
    return result;
}

//struct Light
//{
//    vec3 position;
//    vec3 direction;
//    float cutOff;
//    float outerCutOff;

//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//    float shininess;

//    float constant;
//    float linear;
//    float quadratic;
//};



void main()
{

//    float distance    = length(light.position - fragmentPosition);
//    float attenuation = 1.0 / (light.constant + light.linear * distance +
//                        light.quadratic * (distance * distance));
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragmentPosition);
//    vec3 lightDir = normalize(light.position - fragmentPosition);

//    float theta = dot(lightDir, normalize(-light.direction));
//    float epsilon   = light.cutOff - light.outerCutOff;
//    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // phase 1: Directional lighting
    vec3 result = CalcDirectionalLight(directionalLight, norm, viewDir);

    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, fragmentPosition, viewDir);

    // phase 3: Spot light
    result += CalcSpotLight(spotLight, norm, fragmentPosition, viewDir);

//    // ambient color
//    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

//    if(theta > light.outerCutOff)
//    {
//        // diffuse color
//        float diff = max(dot(norm, lightDir), 0.0);
//        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));

//        // specular color
//        vec3 viewDir = normalize(viewPos - fragmentPosition);
//        vec3 reflectDir = reflect(-lightDir, norm);
//        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//        vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

//        result = ambient + (diffuse + specular) * attenuation * intensity;


//    }
//    else
//    {
//        result = ambient;
//    }

    FragColor = vec4(result, 1.0);
}
