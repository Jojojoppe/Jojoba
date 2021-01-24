#pragma once

#include <memory>

// ------------------------------------------------------

// General constants

#define JOJOBA_VERSION_MAJOR 0
#define JOJOBA_VERSION_MINOR 1

#define JOJOBA_AUTHOR_NAME "Joppe Blondel"
#define JOJOBA_AUTHOR_EMAIL "jojoba@joppeb.nl"

#define JOJOBA_LICENCE "BSD-2"

// ------------------------------------------------------

// General constant, enum and struct definitions

namespace Jojoba{
namespace Core{

    class Instance;

}}

// ------------------------------------------------------

// Wrapper class definitions
// Implementation in core/src/jojoba.cpp

class JJInstance{
    public:
        JJInstance(unsigned int width=600, unsigned int height=600);
        ~JJInstance();

        bool shouldClose();
        void render();

    private:
        std::unique_ptr<Jojoba::Core::Instance> instance;
};