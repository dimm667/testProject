#ifndef VOXEL_H
#define VOXEL_H

#include <array>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderprogram.h"

struct voxelVertex {
    glm::vec3 position;
    glm::vec3 normal;
};

struct Voxel
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 dimension;
};

class VoxelArray
{
public:
    VoxelArray(unsigned int size);
    void set(unsigned int index, Voxel value);

    void draw(ShaderProgram & shader);

private:
    static constexpr float dimension = 0.1f;

    GLuint cubeVBO, cubeVAO, instanceVBO;

    std::vector<voxelVertex> cube;

    std::vector<Voxel> voxel;
    unsigned int size;
};

class VoxelCubeBufferObject
{
public:
    static const unsigned int sizeOfVertex = sizeof(voxelVertex);

//    void * getComponentOffset(int componentIndex);

    VoxelCubeBufferObject(float voxelSize);
    void bindBuffer();
    float getSize() {return dimension;}
private:
    float dimension = 0.1f;
    GLuint vbo;
};

class VoxelArray2d
{
public:
    VoxelArray2d(float voxelSize = 0.1);

    void set(unsigned int x, unsigned int y, Voxel value);
    void construct();

    void draw();

private:
//    float dimension;
    VoxelCubeBufferObject cubeBuffer;
    GLuint /*cubeVBO, */cubeVAO, instanceVBO;

//    std::vector<voxelVertex> cube;

    std::vector<Voxel> voxel;
//    unsigned int xSize, ySize;

};

class VoxelArray3d
{
public:
    VoxelArray3d(float voxelSize = 0.1);
    void setHeigth(unsigned int x, unsigned int y, unsigned int z, glm::vec3 color, glm::vec3 dimensions = glm::vec3(1.0, 1.0, 1.0));

    void construct();

    void draw();

private:
    VoxelCubeBufferObject cubeBuffer;
    GLuint cubeVAO, instanceVBO;
    std::vector<Voxel> voxel;

};

#endif // VOXEL_H
