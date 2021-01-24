#pragma once
#include <jojoba/jojoba.hpp>
#include <vermilion/vermilion.hpp>

#include <string>

namespace Jojoba{
namespace Core{

class Mesh{
    public:
        VmRenderable mesh;
        std::string material;
        unsigned int offset;
        unsigned int length;
        VmBinding binding;

    private:
};

}}