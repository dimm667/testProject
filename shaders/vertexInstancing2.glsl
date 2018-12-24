#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aVoxelPosition;
layout (location = 3) in vec3 aColor;
layout (location = 4) in vec3 aDim;

//uniform mat4 model;
layout (std140) uniform TViewProjMatrices
{
    mat4 view;
    mat4 projection;
    mat4 proj_view;
};

uniform mat4 lightSpaceMatrix;

out vec3 color;
out vec3 normal;
out vec3 fragmentPosition;
out vec4 fragmentPositionDirectionalLightSpace;

void main()
{
    vec3 mult = vec3(0.0, 0.0, 0.0);

    if(aPos[0] > 0.0 && aDim[0] > 1.0)
        mult[0] = (aDim[0] - 1) * 2;

    if(aPos[1] > 0.0 && aDim[1] > 1.0)
        mult[1] = (aDim[1] - 1) * 2;

    if(aPos[2] > 0.0 && aDim[2] > 1.0)
        mult[2] = (aDim[2] - 1) * 2;

    vec4 position = vec4(aPos + aPos * mult + aVoxelPosition, 1.0);

    gl_Position = proj_view * position;
    fragmentPosition = vec3(position);
    normal = aNormal;
    fragmentPositionDirectionalLightSpace = lightSpaceMatrix * position;//vec4(fragmentPosition, 1.0);
    color = aColor;
}
