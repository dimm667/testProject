#include "cube.h"

Cube::Cube(float dimension, glm::vec3 coordinates, Texture & diffuse, Texture & specular) :
    position{coordinates},
    diffuseMap(diffuse),
    specularMap(specular)
{
    float halfDimension = dimension / 2.0f;

    //                  |position                                           |normal                 |texture coords
    vertices.push_back({{-halfDimension, -halfDimension, -halfDimension},   {0.0f,  0.0f, -1.0f},   {0.0f,  0.0f}});
    vertices.push_back({{halfDimension,  halfDimension, -halfDimension},    {0.0f,  0.0f, -1.0f},   {1.0f,  1.0f}});
    vertices.push_back({{halfDimension, -halfDimension, -halfDimension},    {0.0f,  0.0f, -1.0f},   {1.0f,  0.0f}});
    vertices.push_back({{halfDimension,  halfDimension, -halfDimension},    {0.0f,  0.0f, -1.0f},   {1.0f,  1.0f}});
    vertices.push_back({{-halfDimension, -halfDimension, -halfDimension},   {0.0f,  0.0f, -1.0f},   {0.0f,  0.0f}});
    vertices.push_back({{-halfDimension,  halfDimension, -halfDimension},   {0.0f,  0.0f, -1.0f},   {0.0f,  1.0f}});

    vertices.push_back({{-halfDimension, -halfDimension,  halfDimension},   {0.0f,  0.0f,  1.0f},   {0.0f,  0.0f}});
    vertices.push_back({{halfDimension, -halfDimension,  halfDimension},    {0.0f,  0.0f,  1.0f},   {1.0f,  0.0f}});
    vertices.push_back({{halfDimension,  halfDimension,  halfDimension},    {0.0f,  0.0f,  1.0f},   {1.0f,  1.0f}});
    vertices.push_back({{halfDimension,  halfDimension,  halfDimension},    {0.0f,  0.0f,  1.0f},   {1.0f,  1.0f}});
    vertices.push_back({{-halfDimension,  halfDimension,  halfDimension},   {0.0f,  0.0f,  1.0f},   {0.0f,  1.0f}});
    vertices.push_back({{-halfDimension, -halfDimension,  halfDimension},   {0.0f,  0.0f,  1.0f},   {0.0f,  0.0f}});

    vertices.push_back({{-halfDimension,  halfDimension,  halfDimension},   {-1.0f,  0.0f,  0.0f},  {1.0f,  0.0f}});
    vertices.push_back({{-halfDimension,  halfDimension, -halfDimension},   {-1.0f,  0.0f,  0.0f},  {1.0f,  1.0f}});
    vertices.push_back({{-halfDimension, -halfDimension, -halfDimension},   {-1.0f,  0.0f,  0.0f},  {0.0f,  1.0f}});
    vertices.push_back({{-halfDimension, -halfDimension, -halfDimension},   {-1.0f,  0.0f,  0.0f},  {0.0f,  1.0f}});
    vertices.push_back({{-halfDimension, -halfDimension,  halfDimension},   {-1.0f,  0.0f,  0.0f},  {0.0f,  0.0f}});
    vertices.push_back({{-halfDimension,  halfDimension,  halfDimension},   {-1.0f,  0.0f,  0.0f},  {1.0f,  0.0f}});

    vertices.push_back({{halfDimension,  halfDimension,  halfDimension},    {1.0f,  0.0f,  0.0f},   {1.0f,  0.0f}});
    vertices.push_back({{halfDimension, -halfDimension, -halfDimension},    {1.0f,  0.0f,  0.0f},   {0.0f,  1.0f}});
    vertices.push_back({{halfDimension,  halfDimension, -halfDimension},    {1.0f,  0.0f,  0.0f},   {1.0f,  1.0f}});
    vertices.push_back({{halfDimension, -halfDimension, -halfDimension},    {1.0f,  0.0f,  0.0f},   {0.0f,  1.0f}});
    vertices.push_back({{halfDimension,  halfDimension,  halfDimension},    {1.0f,  0.0f,  0.0f},   {1.0f,  0.0f}});
    vertices.push_back({{halfDimension, -halfDimension,  halfDimension},    {1.0f,  0.0f,  0.0f},   {0.0f,  0.0f}});

    vertices.push_back({{-halfDimension, -halfDimension, -halfDimension},   {0.0f, -1.0f,  0.0f},   {0.0f,  1.0f}});
    vertices.push_back({{halfDimension, -halfDimension, -halfDimension},    {0.0f, -1.0f,  0.0f},   {1.0f,  1.0f}});
    vertices.push_back({{halfDimension, -halfDimension,  halfDimension},    {0.0f, -1.0f,  0.0f},   {1.0f,  0.0f}});
    vertices.push_back({{halfDimension, -halfDimension,  halfDimension},    {0.0f, -1.0f,  0.0f},   {1.0f,  0.0f}});
    vertices.push_back({{-halfDimension, -halfDimension,  halfDimension},   {0.0f, -1.0f,  0.0f},   {0.0f,  0.0f}});
    vertices.push_back({{-halfDimension, -halfDimension, -halfDimension},   {0.0f, -1.0f,  0.0f},   {0.0f,  1.0f}});

    vertices.push_back({{-halfDimension,  halfDimension, -halfDimension},   {0.0f,  1.0f,  0.0f},   {0.0f,  1.0f}});
    vertices.push_back({{halfDimension,  halfDimension,  halfDimension},    {0.0f,  1.0f,  0.0f},   {1.0f,  0.0f}});
    vertices.push_back({{halfDimension,  halfDimension, -halfDimension},    {0.0f,  1.0f,  0.0f},   {1.0f,  1.0f}});
    vertices.push_back({{halfDimension,  halfDimension,  halfDimension},    {0.0f,  1.0f,  0.0f},   {1.0f,  0.0f}});
    vertices.push_back({{-halfDimension,  halfDimension, -halfDimension},   {0.0f,  1.0f,  0.0f},   {0.0f,  1.0f}});
    vertices.push_back({{-halfDimension,  halfDimension,  halfDimension},   {0.0f,  1.0f,  0.0f},   {0.0f,  0.0f}});

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
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

void Cube::draw(ShaderProgram & shader)
{
    diffuseMap.activate(GL_TEXTURE0);
    specularMap.activate(GL_TEXTURE1);

    shader.setUniform("material.ambient",  glm::vec3(1.0f, 0.5f, 0.31f));
    shader.setUniform("material.diffuse",  diffuseMap.getBindUnit());
    shader.setUniform("material.specular", specularMap.getBindUnit());
    shader.setUniform("material.shininess", 32.0f);

    glBindVertexArray(VAO);
    glm::mat4 localModel(1.0f);
    localModel = glm::translate(localModel, position);
    shader.setUniform("model", localModel);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

SkyBox::SkyBox(TextureCubeMap & texture) :
    environment(texture)
{

    vertices.push_back({-1.0f,  1.0f, -1.0f});
    vertices.push_back({-1.0f, -1.0f, -1.0f});
    vertices.push_back({1.0f, -1.0f, -1.0f});
    vertices.push_back({1.0f, -1.0f, -1.0f});
    vertices.push_back({1.0f,  1.0f, -1.0f});
    vertices.push_back({-1.0f,  1.0f, -1.0f});

    vertices.push_back({-1.0f, -1.0f,  1.0f});
    vertices.push_back({-1.0f, -1.0f, -1.0f});
    vertices.push_back({-1.0f,  1.0f, -1.0f});
    vertices.push_back({-1.0f,  1.0f, -1.0f});
    vertices.push_back({-1.0f,  1.0f,  1.0f});
    vertices.push_back({-1.0f, -1.0f,  1.0f});

    vertices.push_back({1.0f, -1.0f, -1.0f});
    vertices.push_back({1.0f, -1.0f,  1.0f});
    vertices.push_back({1.0f,  1.0f,  1.0f});
    vertices.push_back({1.0f,  1.0f,  1.0f});
    vertices.push_back({1.0f,  1.0f, -1.0f});
    vertices.push_back({1.0f, -1.0f, -1.0f});

    vertices.push_back({-1.0f, -1.0f,  1.0f});
    vertices.push_back({-1.0f,  1.0f,  1.0f});
    vertices.push_back({1.0f,  1.0f,  1.0f});
    vertices.push_back({1.0f,  1.0f,  1.0f});
    vertices.push_back({1.0f, -1.0f,  1.0f});
    vertices.push_back({-1.0f, -1.0f,  1.0f});

    vertices.push_back({-1.0f,  1.0f, -1.0f});
    vertices.push_back({1.0f,  1.0f, -1.0f});
    vertices.push_back({1.0f,  1.0f,  1.0f});
    vertices.push_back({1.0f,  1.0f,  1.0f});
    vertices.push_back({-1.0f,  1.0f,  1.0f});
    vertices.push_back({-1.0f,  1.0f, -1.0f});

    vertices.push_back({-1.0f, -1.0f, -1.0f});
    vertices.push_back({-1.0f, -1.0f,  1.0f});
    vertices.push_back({1.0f, -1.0f, -1.0f});
    vertices.push_back({1.0f, -1.0f, -1.0f});
    vertices.push_back({-1.0f, -1.0f,  1.0f});
    vertices.push_back({1.0f, -1.0f,  1.0f});

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindVertexArray(0);
}

void SkyBox::draw(ShaderProgram & shader)
{
    environment.activate(GL_TEXTURE0);

    shader.setUniform("skyBox",  environment.getBindUnit());

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
