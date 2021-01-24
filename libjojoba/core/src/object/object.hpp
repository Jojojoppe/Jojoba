#pragma once
#include <jojoba/jojoba.hpp>
#include <vermilion/vermilion.hpp>
#include "mesh.hpp"

#include <vector>
#include <string>

namespace Jojoba{
namespace Core{

class Object{
    public:
        std::string name;
        std::vector<std::shared_ptr<Mesh>> meshes;
        VmBuffer vertex_buffer, index_buffer;

    private:
};

static const std::string StandardVertexShader = R"(
    #version 450
    #extension GL_ARB_separate_shader_objects : enable

    layout(location=0) in vec4 aPos;
    layout(location=1) in vec3 aUV;
    layout(location=2) in vec3 aNorm;

    layout(location=0) out vec2 fUV;

    #ifdef VULKAN
    layout(push_constant) uniform PushConstants{
        mat4 uModel;
        mat4 uView;
        mat4 uProjection;
    };
    #else
    uniform mat4 uModel;
    uniform mat4 uView;
    uniform mat4 uProjection;
    #endif

    void main(){
        gl_Position = uProjection * uView * uModel * aPos;
        fUV = aUV.xy;
    }
)";

}}