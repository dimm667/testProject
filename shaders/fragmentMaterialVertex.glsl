#version 330 core
//struct Material
//{
//    sampler2D diffuse;
//    sampler2D specular;
//    float shininess;
//};

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

in vec3 color;
in vec3 normal;
in vec3 fragmentPosition;
in vec4 fragmentPositionDirectionalLightSpace;

uniform vec3 viewPos;
//uniform Material material;
uniform sampler2D shadowMap;
uniform samplerCube depthMap;

uniform DirectinalLight directionalLight;

#define NR_POINT_LIGHTS 2
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform SpotLight spotLight;
uniform float far_plane;

out vec4 FragColor;

float getPhongSpecular(vec3 lightDir, vec3 viewDir, vec3 normal, float shininess)
{
    vec3 reflectDir = reflect(-lightDir, normal);
    return pow(max(dot(viewDir, reflectDir), 0.0), shininess);
}

float getBlinnPhongSpecular(vec3 lightDir, vec3 viewDir, vec3 normal, float shininess)
{
    vec3 halfwayDir = normalize(lightDir + viewDir);
    return pow(max(dot(normal, halfwayDir), 0.0), shininess);
}

float directionalLightShadow(vec4 fragPosLightSpace, vec3 lightDir, vec3 normal)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // check whether current frag pos is in shadow
    float bias = max(0.01 * (1.0 - dot(normal, lightDir)), 0.001);
//    float shadow = (currentDepth - bias) > closestDepth  ? 1.0 : 0.0;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    return shadow;
}

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

float pointLightShadow(vec3 fragPos, vec3 lightPos)
{
    vec3 fragToLight = fragPos - lightPos;
    float closestDepth = texture(depthMap, fragToLight).r;
    closestDepth *= far_plane;
    float currentDepth = length(fragToLight);

    float bias = 0.01;
//    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
    float shadow = 0.0;
    int samples  = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(depthMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= far_plane;   // Undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;
}

vec3 CalcDirectionalLight(DirectinalLight light, vec3 normal, vec3 viewDir, vec4 posLightSpace)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1);
    float spec = getBlinnPhongSpecular(lightDir, viewDir, normal, 32.0);

    // combine results
    vec3 ambient  = light.ambient;
    vec3 diffuse  = light.diffuse  * diff;
    vec3 specular = light.specular * spec;

    // shadows
    float shadow = directionalLightShadow(posLightSpace, lightDir, normal);

    return (ambient + (diffuse + specular) * (1.0 - shadow)) * color;

}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1);
    float spec = getBlinnPhongSpecular(lightDir, viewDir, normal, 32.0);

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                             light.quadratic * (distance * distance));
    // combine results
    vec3 ambient  = light.ambient;
    vec3 diffuse  = light.diffuse  * diff;
    vec3 specular = light.specular * spec;


    float shadow = pointLightShadow(fragPos, light.position);
    return (ambient + (diffuse + specular) * (1.0 - shadow)) * color * attenuation;
//    return (ambient + (diffuse + specular)) * color;// * attenuation;

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
        vec3 diffuse = light.diffuse * diff * color;

        // specular color
        vec3 viewDir = normalize(viewPos - fragmentPosition);
//        vec3 reflectDir = reflect(-lightDir, normal);
//        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1);
        float spec = getBlinnPhongSpecular(lightDir, viewDir, normal, 32.0);
        vec3 specular = light.specular * spec * color;

        result = (diffuse + specular) * attenuation * intensity;
    }
    return result;
}

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
    vec3 result = CalcDirectionalLight(directionalLight, norm, viewDir, fragmentPositionDirectionalLightSpace);

    // phase 2: Point lights
//    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[0], norm, fragmentPosition, viewDir);

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
