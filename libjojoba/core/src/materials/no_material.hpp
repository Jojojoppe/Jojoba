#pragma once
#include <jojoba/jojoba.hpp>
#include <vermilion/vermilion.hpp>
#include <string>

#include "material.hpp"

namespace Jojoba{
namespace Core{

struct NoMaterialComponent{
    unsigned int MaterialID;
};

class NoMaterial : public Material{
    public:
        static Material * init(std::shared_ptr<VmInstance> vmInstance);
};

}}