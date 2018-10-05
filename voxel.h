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

class VoxelArray2d
{
public:
    VoxelArray2d(unsigned int sizeX, unsigned int sizeY);

    void set(unsigned int x, unsigned int y, Voxel value);

    void draw(ShaderProgram & shader);

private:
    static constexpr float dimension = 0.1f;

    GLuint cubeVBO, cubeVAO, instanceVBO;

    std::vector<voxelVertex> cube;

    std::vector<Voxel> voxel;
    unsigned int xSize, ySize;

};

#endif // VOXEL_H
