#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include "shaderprogram.h"
#include "texture.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct TextureExtanded : public TextureImage {
    TextureExtanded(const std::string & pathToFile) : TextureImage(pathToFile)
    {}
    std::string type;
    std::string path;
};

class Mesh {
public:
    /*  Mesh Data  */
    std::vector<Vertex>         vertices;
    std::vector<unsigned int>   indices;
    std::vector<TextureExtanded>        textures;

    /*  Functions  */
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureExtanded> textures);
    void draw(ShaderProgram & shader);

private:
    /*  Render data  */
    unsigned int VAO, VBO, EBO;

    /*  Functions    */
    void setupMesh();
};

#endif // MESH_H
