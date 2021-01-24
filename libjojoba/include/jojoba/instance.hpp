#pragma once

#include <jojoba/jojoba.hpp>
#include <vermilion/vermilion.hpp>

namespace Jojoba{
namespace Core{

class Instance{
    public:
        Instance(unsigned int width, unsigned int height);
        ~Instance();

        bool shouldClose();
        void render();

    private:
        std::shared_ptr<VmInstance> vmInstance;

    friend class ::JJInstance;
};

}}