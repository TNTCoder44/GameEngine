#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stb/stb_image.h>

#include "Mesh.h"

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model
{
public:
    std::vector<Mesh_Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;

    Model(const std::string& path, bool gamma = false);

    void Draw(Shader& shader);

private:
    void loadModel(const std::string& path);
    Material loadMaterial(aiMaterial* mat);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Mesh_Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
