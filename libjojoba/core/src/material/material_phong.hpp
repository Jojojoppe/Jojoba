#pragma once
#include <jojoba/jojoba.hpp>
#include <vermilion/vermilion.hpp>
#include "material.hpp"
#include <vermilion/glm/glm.hpp>

#include <memory>

namespace Jojoba{
namespace Core{

class MaterialPhong : public Material{
    public:
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 emission;
        float specular_exponent;
        float transparance;
        float optical_density;

        std::string map_ambient;
        std::string map_diffuse;
        std::string map_specular;
        std::string map_emission;
        std::string map_normal;

        static VmPipeline pipeline;
        static VmPipelineLayout pipelineLayout;
        static VmShader vShader, fShader;
        static VmShaderProgram program;
        static void createPipeline(std::shared_ptr<VmInstance> vmInstance);

    private:
};

static const std::string MaterialPhongFragmentShader = R"(
    #version 450
    #extension GL_ARB_separate_shader_objects : enable

    layout(location=0) out vec4 outColor;

    layout(location=0) in vec2 fUV;

    // layout(binding=0) uniform sampler2D map_ambient;
    layout(binding=0) uniform sampler2D map_diffuse;
    // layout(binding=1) uniform sampler2D map_diffuse;
    // layout(binding=2) uniform sampler2D map_specular;
    // layout(binding=3) uniform sampler2D map_emission;
    // layout(binding=4) uniform sampler2D map_normal;

    #ifdef VULKAN
    layout(push_constant) uniform PushConstants{
        mat4 uModel;
        mat4 uView;
        mat4 uProjection;

        // vec4 uAmbient;
        // vec4 uDiffuse;
        // vec4 uSpecular;
        // vec4 uEmission;
        // float uSpecularExponent;
        // float uTransparance;
        // float uOpticalDensity;
    };
    #else
    uniform mat4 uModel;
    uniform mat4 uView;
    uniform mat4 uProjection;

    // uniform vec4 uAmbient;
    // uniform vec4 uDiffuse;
    // uniform vec4 uSpecular;
    // uniform vec4 uEmission;
    // uniform float uSpecularExponent;
    // uniform float uTransparance;
    // uniform float uOpticalDensity;
    #endif

    void main(){
        outColor = texture(map_diffuse, fUV);
    }
)";

}}