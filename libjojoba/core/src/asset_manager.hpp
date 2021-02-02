#pragma once
#include <jojoba/jojoba.hpp>
#include <vermilion/vermilion.hpp>
#include <memory>
#include <unordered_map>

#include "materials/material.hpp"
#include "scene.hpp"

namespace Jojoba{
namespace Core{

struct AssetMesh{
    VmRenderable renderable;
};

struct AssetObject{
    VmBuffer vBuffer, iBuffer;
    std::vector<unsigned int> meshes;
};

struct AssetTexture{
    VmTexture textue;
    VmSampler sampler;
};

struct AssetMaterialType{
    Material * material;
};

struct AssetScene{
    Scene * scene;
};

class AssetManager{
    public:

        AssetManager(std::shared_ptr<VmInstance> vmInstance);
        ~AssetManager();

        std::unordered_map<unsigned int, AssetMesh> assetMeshes;
        std::unordered_map<unsigned int, AssetObject> assetObjects;
        std::unordered_map<unsigned int, AssetTexture> assetTextures;
        std::unordered_map<unsigned int, AssetMaterialType> assetMaterialTypes;
        std::unordered_map<unsigned int, AssetScene> assetScenes;

        unsigned int loadObjectFromObjFile(const std::string& path);

        unsigned int createScene();

    private:
        std::shared_ptr<VmInstance> vmInstance;
        unsigned int IDcounter = 1;
};

}}