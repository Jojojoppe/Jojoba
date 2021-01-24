#include "material_phong.hpp"
#include "../object/object.hpp"

VmPipeline Jojoba::Core::MaterialPhong::pipeline;
VmPipelineLayout Jojoba::Core::MaterialPhong::pipelineLayout;
VmShader Jojoba::Core::MaterialPhong::vShader, Jojoba::Core::MaterialPhong::fShader;
VmShaderProgram Jojoba::Core::MaterialPhong::program;

void Jojoba::Core::MaterialPhong::createPipeline(std::shared_ptr<VmInstance> vmInstance){
    vmInstance->createPipelineLayout(pipelineLayout, {
        Vermilion::Core::BufferLayoutElementFloat4("aPos"),
        Vermilion::Core::BufferLayoutElementFloat3("aUV"),
        Vermilion::Core::BufferLayoutElementFloat3("aNorm"),
    },{
        // Vermilion::Core::PipelineLayoutBinding(Vermilion::Core::PipelineLayoutBindingType::PIPELINE_LAYOUT_BINDING_SAMPLER),    // map_ambient
        Vermilion::Core::PipelineLayoutBinding(Vermilion::Core::PipelineLayoutBindingType::PIPELINE_LAYOUT_BINDING_SAMPLER),    // map_diffuse
        // Vermilion::Core::PipelineLayoutBinding(Vermilion::Core::PipelineLayoutBindingType::PIPELINE_LAYOUT_BINDING_SAMPLER),    // map_specular
        // Vermilion::Core::PipelineLayoutBinding(Vermilion::Core::PipelineLayoutBindingType::PIPELINE_LAYOUT_BINDING_SAMPLER),    // map_emission
        // Vermilion::Core::PipelineLayoutBinding(Vermilion::Core::PipelineLayoutBindingType::PIPELINE_LAYOUT_BINDING_SAMPLER),    // map_normal
    },{
        Vermilion::Core::PipelineLayoutUniformMat4("uModel"),
        Vermilion::Core::PipelineLayoutUniformMat4("uView"),
        Vermilion::Core::PipelineLayoutUniformMat4("uProjection"),
        // Vermilion::Core::PipelineLayoutUniformFloat4("uAmbient"),
        // Vermilion::Core::PipelineLayoutUniformFloat4("uDiffuse"),
        // Vermilion::Core::PipelineLayoutUniformFloat4("uSecular"),
        // Vermilion::Core::PipelineLayoutUniformFloat4("uEmission"),
        // Vermilion::Core::PipelineLayoutUniformFloat1("uSpecularExponent"),
        // Vermilion::Core::PipelineLayoutUniformFloat1("uTransparence"),
        // Vermilion::Core::PipelineLayoutUniformFloat1("uOpticalDensity"),
    });

    vmInstance->createShader(fShader, MaterialPhongFragmentShader, Vermilion::Core::ShaderType::SHADER_TYPE_FRAGMENT);
    vmInstance->createShader(vShader, StandardVertexShader, Vermilion::Core::ShaderType::SHADER_TYPE_VERTEX);
    vmInstance->createShaderProgram(program, {&fShader, &vShader});

    VmRenderTarget defaultTarget;
    vmInstance->getDefaultRenderTarget(defaultTarget);
    vmInstance->createPipeline(pipeline, defaultTarget, program, pipelineLayout, {
        Vermilion::Core::PipelineSettingsDepthTest::PIPELINE_SETTINGS_DEPTH_TEST_ENABLED,
        Vermilion::Core::PipelineSettingsCullMode::PIPELINE_SETTINGS_CULL_MODE_NONE,
        Vermilion::Core::PipelineSettingsPolygonMode::PIPELINE_SETTINGS_POLYGON_MODE_TRIANGLE
    });
}