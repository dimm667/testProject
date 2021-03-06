#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aVoxelPosition;
layout (location = 3) in vec3 aColor;

void main()
{
    gl_Position = vec4(aPos + aVoxelPosition, 1.0);
}
