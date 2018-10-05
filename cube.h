#ifndef CUBE_H
#define CUBE_H
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderprogram.h"
#include "texture.h"

class Cube
{
public:
    Cube(float dimension, glm::vec3 coordinates, Texture & diffuse, Texture & specular);

    void draw(ShaderProgram & shader);

private:
    glm::vec3 position;

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };

    std::vector<Vertex> vertices;
    GLuint VBO, VAO, EBO;

    Texture & diffuseMap;
    Texture & specularMap;

};

class SkyBox
{
public:
    SkyBox(TextureCubeMap & environment);
    void draw(ShaderProgram & shader);

private:
    std::vector<glm::vec3> vertices;
    GLuint VBO, VAO, EBO;
    Texture & environment;
};

#endif // CUBE_H
