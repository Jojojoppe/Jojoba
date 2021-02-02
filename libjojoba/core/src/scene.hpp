#pragma once
#include <jojoba/jojoba.hpp>
#include <vermilion/vermilion.hpp>
#include "entt.hpp"

namespace Jojoba{
namespace Core{

class Scene{
    public:
        entt::registry registry;

        Scene();
        ~Scene();

        unsigned int createEntity();
};

}}