#pragma once
#include <jojoba/jojoba.hpp>
#include <vermilion/vermilion.hpp>
#include <memory>
#include <unordered_map>

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

class AssetManager{
    public:

        AssetManager(std::shared_ptr<VmInstance> vmInstance);
        ~AssetManager();

        std::unordered_map<unsigned int, AssetMesh> assetMeshes;
        std::unordered_map<unsigned int, AssetObject> assetObjects;
        std::unordered_map<unsigned int, AssetTexture> assetTextures;

        unsigned int loadObjectFromObjFile(const std::string& path);

    private:
        std::shared_ptr<VmInstance> vmInstance;
        unsigned int IDcounter = 1;
};

}}