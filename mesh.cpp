#include "mesh.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureExtanded> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                     &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

void Mesh::draw(ShaderProgram & shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    for(auto && texture : textures)
    {
        // activate proper texture unit before binding
        texture.activate(GL_TEXTURE0 + (diffuseNr - 1) + (specularNr - 1));

        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = texture.type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);

        shader.setUniform((/*"material." + */name + number).c_str(), texture.getBindUnit());
    }

//    for(int i = 0; i < textures.size(); i++)
//    {
////        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
//        textures[i].activate();
//        // retrieve texture number (the N in diffuse_textureN)
//        std::string number;
//        std::string name = textures[i].type;
//        if(name == "texture_diffuse")
//            number = std::to_string(diffuseNr++);
//        else if(name == "texture_specular")
//            number = std::to_string(specularNr++);

//        shader.setUniform(("material." + name + number).c_str(), i);
////        glBindTexture(GL_TEXTURE_2D, textures[i].id);
//    }

    // TODO: for what?
    //glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
