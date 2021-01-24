#pragma once
#include <jojoba/jojoba.hpp>
#include <vermilion/vermilion.hpp>

namespace Jojoba{
namespace Core{

class Material{
    public:
        std::string name;

        virtual ~Material() = default;

    private:
};

}}