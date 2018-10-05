#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aVoxelPosition;
layout (location = 3) in vec3 aColor;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
flat out vec3 fragmentPosition;
out vec3 color;

void main()
{
    gl_Position = projection * view * vec4(aPos + aVoxelPosition, 1.0);
    fragmentPosition = vec3(vec4(aPos + aVoxelPosition, 1.0));
    normal = mat3(transpose(inverse(mat4(1.0)))) * aNormal;
    color = aColor;
}
