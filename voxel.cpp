#include "voxel.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include <ctime>

VoxelArray::VoxelArray(unsigned int arraySize) :
    size(arraySize)
{
    const float halfDimension = dimension / 2.0f;

    //                  |position                                               |normal                 |texture coords
    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   {0.0f,  0.0f, -1.0f}});
    cube.push_back({{halfDimension,  halfDimension, -halfDimension},    {0.0f,  0.0f, -1.0f}});
    cube.push_back({{halfDimension, -halfDimension, -halfDimension},    {0.0f,  0.0f, -1.0f}});
    cube.push_back({{halfDimension,  halfDimension, -halfDimension},    {0.0f,  0.0f, -1.0f}});
    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   {0.0f,  0.0f, -1.0f}});
    cube.push_back({{-halfDimension,  halfDimension, -halfDimension},   {0.0f,  0.0f, -1.0f}});

    cube.push_back({{-halfDimension, -halfDimension,  halfDimension},   {0.0f,  0.0f,  1.0f}});
    cube.push_back({{halfDimension, -halfDimension,  halfDimension},    {0.0f,  0.0f,  1.0f}});
    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    {0.0f,  0.0f,  1.0f}});
    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    {0.0f,  0.0f,  1.0f}});
    cube.push_back({{-halfDimension,  halfDimension,  halfDimension},   {0.0f,  0.0f,  1.0f}});
    cube.push_back({{-halfDimension, -halfDimension,  halfDimension},   {0.0f,  0.0f,  1.0f}});

    cube.push_back({{-halfDimension,  halfDimension,  halfDimension},   {-1.0f,  0.0f,  0.0f}});
    cube.push_back({{-halfDimension,  halfDimension, -halfDimension},   {-1.0f,  0.0f,  0.0f}});
    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   {-1.0f,  0.0f,  0.0f}});
    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   {-1.0f,  0.0f,  0.0f}});
    cube.push_back({{-halfDimension, -halfDimension,  halfDimension},   {-1.0f,  0.0f,  0.0f}});
    cube.push_back({{-halfDimension,  halfDimension,  halfDimension},   {-1.0f,  0.0f,  0.0f}});

    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    {1.0f,  0.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension, -halfDimension},    {1.0f,  0.0f,  0.0f}});
    cube.push_back({{halfDimension,  halfDimension, -halfDimension},    {1.0f,  0.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension, -halfDimension},    {1.0f,  0.0f,  0.0f}});
    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    {1.0f,  0.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension,  halfDimension},    {1.0f,  0.0f,  0.0f}});

    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   {0.0f, -1.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension, -halfDimension},    {0.0f, -1.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension,  halfDimension},    {0.0f, -1.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension,  halfDimension},    {0.0f, -1.0f,  0.0f}});
    cube.push_back({{-halfDimension, -halfDimension,  halfDimension},   {0.0f, -1.0f,  0.0f}});
    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   {0.0f, -1.0f,  0.0f}});

    cube.push_back({{-halfDimension,  halfDimension, -halfDimension},   {0.0f,  1.0f,  0.0f}});
    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    {0.0f,  1.0f,  0.0f}});
    cube.push_back({{halfDimension,  halfDimension, -halfDimension},    {0.0f,  1.0f,  0.0f}});
    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    {0.0f,  1.0f,  0.0f}});
    cube.push_back({{-halfDimension,  halfDimension, -halfDimension},   {0.0f,  1.0f,  0.0f}});
    cube.push_back({{-halfDimension,  halfDimension,  halfDimension},   {0.0f,  1.0f,  0.0f}});

    glGenBuffers(1, &cubeVBO);
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &instanceVBO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, cube.size() * sizeof(voxelVertex), &cube[0], GL_STATIC_DRAW);



    glBindVertexArray(cubeVAO);
    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(voxelVertex), (void*)offsetof(voxelVertex, position));
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(voxelVertex), (void*)offsetof(voxelVertex, normal));


    // voxel position
    // initialize voxel array
    std::srand(std::time(nullptr));
    for(unsigned int idx = 0; idx < size; ++idx)
        voxel.push_back({glm::vec3(0.0f),
                         glm::vec3(0.0f)});

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Voxel) * size, &voxel[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Voxel), (void*)offsetof(Voxel, position));
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Voxel), (void*)offsetof(Voxel, color));
    glVertexAttribDivisor(3, 1);

    glBindVertexArray(0);
}

void VoxelArray::set(unsigned int index, Voxel value)
{
    voxel[index] = value;
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, (index) * sizeof(Voxel), sizeof(Voxel), &value);
}

void VoxelArray::draw(ShaderProgram & shader)
{
    glBindVertexArray(cubeVAO);
//    glm::mat4 localModel(1.0f);
//    shader.setUniform("model", localModel);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, voxel.size());
    glBindVertexArray(0);
}


VoxelArray2d::VoxelArray2d(unsigned int sizeX, unsigned int sizeY) :
    xSize(sizeX), ySize(sizeY)
{
    const float halfDimension = dimension / 2.0f;

    //                  |position                                               |normal                 |texture coords
    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   {0.0f,  0.0f, -1.0f}});
    cube.push_back({{halfDimension,  halfDimension, -halfDimension},    {0.0f,  0.0f, -1.0f}});
    cube.push_back({{halfDimension, -halfDimension, -halfDimension},    {0.0f,  0.0f, -1.0f}});
    cube.push_back({{halfDimension,  halfDimension, -halfDimension},    {0.0f,  0.0f, -1.0f}});
    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   {0.0f,  0.0f, -1.0f}});
    cube.push_back({{-halfDimension,  halfDimension, -halfDimension},   {0.0f,  0.0f, -1.0f}});

    cube.push_back({{-halfDimension, -halfDimension,  halfDimension},   {0.0f,  0.0f,  1.0f}});
    cube.push_back({{halfDimension, -halfDimension,  halfDimension},    {0.0f,  0.0f,  1.0f}});
    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    {0.0f,  0.0f,  1.0f}});
    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    {0.0f,  0.0f,  1.0f}});
    cube.push_back({{-halfDimension,  halfDimension,  halfDimension},   {0.0f,  0.0f,  1.0f}});
    cube.push_back({{-halfDimension, -halfDimension,  halfDimension},   {0.0f,  0.0f,  1.0f}});

    cube.push_back({{-halfDimension,  halfDimension,  halfDimension},   {-1.0f,  0.0f,  0.0f}});
    cube.push_back({{-halfDimension,  halfDimension, -halfDimension},   {-1.0f,  0.0f,  0.0f}});
    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   {-1.0f,  0.0f,  0.0f}});
    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   {-1.0f,  0.0f,  0.0f}});
    cube.push_back({{-halfDimension, -halfDimension,  halfDimension},   {-1.0f,  0.0f,  0.0f}});
    cube.push_back({{-halfDimension,  halfDimension,  halfDimension},   {-1.0f,  0.0f,  0.0f}});

    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    {1.0f,  0.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension, -halfDimension},    {1.0f,  0.0f,  0.0f}});
    cube.push_back({{halfDimension,  halfDimension, -halfDimension},    {1.0f,  0.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension, -halfDimension},    {1.0f,  0.0f,  0.0f}});
    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    {1.0f,  0.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension,  halfDimension},    {1.0f,  0.0f,  0.0f}});

    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   {0.0f, -1.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension, -halfDimension},    {0.0f, -1.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension,  halfDimension},    {0.0f, -1.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension,  halfDimension},    {0.0f, -1.0f,  0.0f}});
    cube.push_back({{-halfDimension, -halfDimension,  halfDimension},   {0.0f, -1.0f,  0.0f}});
    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   {0.0f, -1.0f,  0.0f}});

    cube.push_back({{-halfDimension,  halfDimension, -halfDimension},   {0.0f,  1.0f,  0.0f}});
    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    {0.0f,  1.0f,  0.0f}});
    cube.push_back({{halfDimension,  halfDimension, -halfDimension},    {0.0f,  1.0f,  0.0f}});
    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    {0.0f,  1.0f,  0.0f}});
    cube.push_back({{-halfDimension,  halfDimension, -halfDimension},   {0.0f,  1.0f,  0.0f}});
    cube.push_back({{-halfDimension,  halfDimension,  halfDimension},   {0.0f,  1.0f,  0.0f}});

    glGenBuffers(1, &cubeVBO);
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &instanceVBO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, cube.size() * sizeof(voxelVertex), &cube[0], GL_STATIC_DRAW);



    glBindVertexArray(cubeVAO);
    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(voxelVertex), (void*)offsetof(voxelVertex, position));
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(voxelVertex), (void*)offsetof(voxelVertex, normal));


    // voxel position
    // initialize voxel array
    std::srand(std::time(nullptr));
    for(unsigned int yIdx = 0; yIdx < sizeY; ++yIdx)
        for(unsigned int xIdx = 0; xIdx < sizeX; ++xIdx)
            voxel.push_back({glm::vec3(0.0f),
                             glm::vec3(0.0f)});

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Voxel) * sizeX * sizeY, &voxel[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Voxel), (void*)offsetof(Voxel, position));
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Voxel), (void*)offsetof(Voxel, color));
    glVertexAttribDivisor(3, 1);

    glBindVertexArray(0);
}

void VoxelArray2d::set(unsigned int x, unsigned int y, Voxel value)
{
    voxel[x * xSize + y] = value;
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, (x * xSize + y) * sizeof(Voxel), sizeof(Voxel), &value);
}

void VoxelArray2d::draw(ShaderProgram & shader)
{
    glBindVertexArray(cubeVAO);
//    glm::mat4 localModel(1.0f);
//    shader.setUniform("model", localModel);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, voxel.size());
    glBindVertexArray(0);
}


