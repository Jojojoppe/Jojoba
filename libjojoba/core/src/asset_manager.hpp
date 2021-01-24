#pragma once
#include <jojoba/jojoba.hpp>
#include <vermilion/vermilion.hpp>
#include "material/material.hpp"
#include "object/object.hpp"

#include <unordered_map>
#include <string>

namespace Jojoba{
namespace Core{

class AssetManager{
    public:
        std::unordered_map<std::string, std::shared_ptr<VmTexture>> textures;
        std::unordered_map<std::string, std::shared_ptr<VmSampler>> samplers;

        std::unordered_map<std::string, std::shared_ptr<Material>> materials;
        std::unordered_map<std::string, std::shared_ptr<Object>> objects;

        AssetManager(std::shared_ptr<VmInstance> vmInstance);
        ~AssetManager();

        bool loadObject(const std::string& path, const std::string& name);

    private:
        std::shared_ptr<VmInstance> vmInstance;

        bool __loadObjFile(const std::vector<std::string>& path_expanded, const std::string& name);
        bool __loadMtlFile(const std::vector<std::string>& path_expanded);
        bool __loadTexture(const std::string& directory, const std::string filename);
};

}}