#ifndef MODEL_H
#define MODEL_H
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "shaderprogram.h"

class Model
{
public:
    /*  Functions   */
    Model(const std::string & path);
    void draw(ShaderProgram & shader);

private:
    /*  Model Data  */
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<TextureExtanded> textures_loaded;

    /*  Functions   */
    void loadModel(const std::string & path);
    void processNode(aiNode * node, const aiScene * scene);
    Mesh processMesh(aiMesh * mesh, const aiScene * scene);
    std::vector<TextureExtanded> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif // MODEL_H
