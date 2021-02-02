#include "no_material.hpp"

Jojoba::Core::Material * Jojoba::Core::NoMaterial::init(std::shared_ptr<VmInstance> vmInstance){
    NoMaterial * m = new NoMaterial;

    vmInstance->createShader(m->vshader, R"(
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
    )", Vermilion::Core::ShaderType::SHADER_TYPE_VERTEX);  
    vmInstance->createShader(m->fshader, R"(
        #version 450
        #extension GL_ARB_separate_shader_objects : enable

        layout(location=0) out vec4 outColor;

        layout(location=0) in vec2 fUV;

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
            outColor = vec4(0.4, 0.0, 0.4, 1.0);
        }
    )", Vermilion::Core::ShaderType::SHADER_TYPE_FRAGMENT);
    vmInstance->createShaderProgram(m->shaderProgram, {&m->vshader, &m->fshader});

    vmInstance->createPipelineLayout(m->pipelineLayout, {
        Vermilion::Core::BufferLayoutElementFloat4("aPos"),
        Vermilion::Core::BufferLayoutElementFloat3("aUV"),
        Vermilion::Core::BufferLayoutElementFloat3("aNorm"),
    },{
    },{
        Vermilion::Core::PipelineLayoutUniformMat4("uModel"),
        Vermilion::Core::PipelineLayoutUniformMat4("uView"),
        Vermilion::Core::PipelineLayoutUniformMat4("uProjection"),
    });

    VmRenderTarget defaultTarget;
    vmInstance->getDefaultRenderTarget(defaultTarget);
    vmInstance->createPipeline(m->pipeline, defaultTarget, m->shaderProgram, m->pipelineLayout, {
        Vermilion::Core::PipelineSettingsDepthTest::PIPELINE_SETTINGS_DEPTH_TEST_ENABLED,
        Vermilion::Core::PipelineSettingsCullMode::PIPELINE_SETTINGS_CULL_MODE_NONE,
        Vermilion::Core::PipelineSettingsPolygonMode::PIPELINE_SETTINGS_POLYGON_MODE_TRIANGLE
    });

    return (Material*) m;
}