#pragma once
#include <jojoba/jojoba.hpp>
#include <vermilion/vermilion.hpp>

namespace Jojoba{
namespace Core{

class Material{
    public:
        VmPipelineLayout pipelineLayout;
        VmPipeline pipeline;
        VmShader vshader, fshader;
        VmShaderProgram shaderProgram;
};

}}