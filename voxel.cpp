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

VoxelCubeBufferObject::VoxelCubeBufferObject(float voxelSize) :
    dimension(voxelSize)
{
    const float halfDimension = dimension / 2.0f;
    std::vector<voxelVertex> cube;
    //                  |position                                               |normal
    glm::mat3 normalMultiplier(glm::transpose(glm::inverse(glm::mat4(1.0))));
    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   normalMultiplier * glm::vec3{0.0f,  0.0f, -1.0f}});
    cube.push_back({{halfDimension,  halfDimension, -halfDimension},    normalMultiplier * glm::vec3{0.0f,  0.0f, -1.0f}});
    cube.push_back({{halfDimension, -halfDimension, -halfDimension},    normalMultiplier * glm::vec3{0.0f,  0.0f, -1.0f}});
    cube.push_back({{halfDimension,  halfDimension, -halfDimension},    normalMultiplier * glm::vec3{0.0f,  0.0f, -1.0f}});
    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   normalMultiplier * glm::vec3{0.0f,  0.0f, -1.0f}});
    cube.push_back({{-halfDimension,  halfDimension, -halfDimension},   normalMultiplier * glm::vec3{0.0f,  0.0f, -1.0f}});

    cube.push_back({{-halfDimension, -halfDimension,  halfDimension},   normalMultiplier * glm::vec3{0.0f,  0.0f,  1.0f}});
    cube.push_back({{halfDimension, -halfDimension,  halfDimension},    normalMultiplier * glm::vec3{0.0f,  0.0f,  1.0f}});
    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    normalMultiplier * glm::vec3{0.0f,  0.0f,  1.0f}});
    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    normalMultiplier * glm::vec3{0.0f,  0.0f,  1.0f}});
    cube.push_back({{-halfDimension,  halfDimension,  halfDimension},   normalMultiplier * glm::vec3{0.0f,  0.0f,  1.0f}});
    cube.push_back({{-halfDimension, -halfDimension,  halfDimension},   normalMultiplier * glm::vec3{0.0f,  0.0f,  1.0f}});

    cube.push_back({{-halfDimension,  halfDimension,  halfDimension},   normalMultiplier * glm::vec3{-1.0f,  0.0f,  0.0f}});
    cube.push_back({{-halfDimension,  halfDimension, -halfDimension},   normalMultiplier * glm::vec3{-1.0f,  0.0f,  0.0f}});
    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   normalMultiplier * glm::vec3{-1.0f,  0.0f,  0.0f}});
    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   normalMultiplier * glm::vec3{-1.0f,  0.0f,  0.0f}});
    cube.push_back({{-halfDimension, -halfDimension,  halfDimension},   normalMultiplier * glm::vec3{-1.0f,  0.0f,  0.0f}});
    cube.push_back({{-halfDimension,  halfDimension,  halfDimension},   normalMultiplier * glm::vec3{-1.0f,  0.0f,  0.0f}});

    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    normalMultiplier * glm::vec3{1.0f,  0.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension, -halfDimension},    normalMultiplier * glm::vec3{1.0f,  0.0f,  0.0f}});
    cube.push_back({{halfDimension,  halfDimension, -halfDimension},    normalMultiplier * glm::vec3{1.0f,  0.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension, -halfDimension},    normalMultiplier * glm::vec3{1.0f,  0.0f,  0.0f}});
    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    normalMultiplier * glm::vec3{1.0f,  0.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension,  halfDimension},    normalMultiplier * glm::vec3{1.0f,  0.0f,  0.0f}});

    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   normalMultiplier * glm::vec3{0.0f, -1.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension, -halfDimension},    normalMultiplier * glm::vec3{0.0f, -1.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension,  halfDimension},    normalMultiplier * glm::vec3{0.0f, -1.0f,  0.0f}});
    cube.push_back({{halfDimension, -halfDimension,  halfDimension},    normalMultiplier * glm::vec3{0.0f, -1.0f,  0.0f}});
    cube.push_back({{-halfDimension, -halfDimension,  halfDimension},   normalMultiplier * glm::vec3{0.0f, -1.0f,  0.0f}});
    cube.push_back({{-halfDimension, -halfDimension, -halfDimension},   normalMultiplier * glm::vec3{0.0f, -1.0f,  0.0f}});

    cube.push_back({{-halfDimension,  halfDimension, -halfDimension},   normalMultiplier * glm::vec3{0.0f,  1.0f,  0.0f}});
    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    normalMultiplier * glm::vec3{0.0f,  1.0f,  0.0f}});
    cube.push_back({{halfDimension,  halfDimension, -halfDimension},    normalMultiplier * glm::vec3{0.0f,  1.0f,  0.0f}});
    cube.push_back({{halfDimension,  halfDimension,  halfDimension},    normalMultiplier * glm::vec3{0.0f,  1.0f,  0.0f}});
    cube.push_back({{-halfDimension,  halfDimension, -halfDimension},   normalMultiplier * glm::vec3{0.0f,  1.0f,  0.0f}});
    cube.push_back({{-halfDimension,  halfDimension,  halfDimension},   normalMultiplier * glm::vec3{0.0f,  1.0f,  0.0f}});

    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, cube.size() * sizeOfVertex, &cube[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VoxelCubeBufferObject::bindBuffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

//void * VoxelCubeBufferObject::getComponentOffset(int componentIndex)
//{
//    void *result = nullptr;
//    switch (componentIndex) {
//    case 0:
//        result = static_cast<void *>(offsetof(voxelVertex, position));
//        break;
//    case 1:
//        result = static_cast<void *>(offsetof(voxelVertex, normal));
//        break;
//    }
//    return result;
//}

VoxelArray2d::VoxelArray2d(float voxelSize) :
    cubeBuffer(voxelSize)
{
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &instanceVBO);


    glBindVertexArray(cubeVAO);

    cubeBuffer.bindBuffer();
    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(voxelVertex), (void*)offsetof(voxelVertex, position));
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(voxelVertex), (void*)offsetof(voxelVertex, normal));

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Voxel) * voxel.size(), &voxel[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Voxel), (void*)offsetof(Voxel, position));
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Voxel), (void*)offsetof(Voxel, color));
    glVertexAttribDivisor(3, 1);

    glBindVertexArray(0);
}

void VoxelArray2d::construct()
{
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Voxel) * voxel.size(), &voxel[0], GL_STATIC_DRAW);
}

void VoxelArray2d::set(unsigned int x, unsigned int y, Voxel value)
{
//    voxel[x * xSize + y] = value;
    voxel.push_back(value);
//    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
//    glBufferSubData(GL_ARRAY_BUFFER, (x * xSize + y) * sizeof(Voxel), sizeof(Voxel), &value);
}

void VoxelArray2d::draw()
{
    glBindVertexArray(cubeVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, voxel.size());
    glBindVertexArray(0);
}


VoxelArray3d::VoxelArray3d(float voxelSize) :
    cubeBuffer(voxelSize)
{
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &instanceVBO);


    glBindVertexArray(cubeVAO);

    cubeBuffer.bindBuffer();
    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(voxelVertex), (void*)offsetof(voxelVertex, position));
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(voxelVertex), (void*)offsetof(voxelVertex, normal));
    // sides indexes

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Voxel) * voxel.size(), &voxel[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Voxel), (void*)offsetof(Voxel, position));
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Voxel), (void*)offsetof(Voxel, color));
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Voxel), (void*)offsetof(Voxel, dimension));
    glVertexAttribDivisor(4, 1);


    glBindVertexArray(0);
}

void VoxelArray3d::setHeigth(unsigned int x, unsigned int y, unsigned int z, glm::vec3 color, glm::vec3 dimensions)
{
    for(unsigned int zVal = z; zVal <= z; zVal--)
    {
        Voxel v{glm::vec3(x, zVal, y) * cubeBuffer.getSize(), color, glm::vec3(dimensions.x, dimensions.z, dimensions.y)};
        voxel.push_back(v);
    }
}

void VoxelArray3d::construct()
{
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Voxel) * voxel.size(), &voxel[0], GL_STATIC_DRAW);
}

void VoxelArray3d::draw()
{
    glBindVertexArray(cubeVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, voxel.size());
    glBindVertexArray(0);
}

