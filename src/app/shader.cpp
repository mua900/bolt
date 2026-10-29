#include "shader.hpp"
#include "util/file_util.hpp"

#include <bundle/bundle_shaders.h>

namespace bolt {

    void unloadShader(RenderContext& context, Shader& shader)
    {
        SDL_ReleaseGPUShader(context.device, shader.shader);
    }

    ShaderLoadResult loadShader(RenderContext& context, Shader& shader, const char* path)
    {
        SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_SPIRV;
        SDL_GPUShaderStage shaderStage = SDL_GPUShaderStage(shader.stage);

        BinaryData code = {};
        if (!load_file(path, code)) {
            log_error("Could not load shader %s", path);
            return SHADER_LOAD_FAIL;
        }

        String extension = string_get_extension(String(path));
        if (string_compare(extension, String("dxil")))
        {
            format = SDL_GPU_SHADERFORMAT_DXIL;
        }
        else if (string_compare(extension, String("spv")))
        {
            format = SDL_GPU_SHADERFORMAT_SPIRV;
        }
        else if (string_compare(extension, String("msl")))
        {
            format = SDL_GPU_SHADERFORMAT_MSL;
        }
        else
        {
            return SHADER_LOAD_FAIL;
        }

        SDL_GPUShaderFormat expectedFormat = SDL_GetGPUShaderFormats(context.device);
        if (!(expectedFormat & format))
        {
            return SHADER_LOAD_INCOMPATIBLE_FORMAT;
        }

        SDL_GPUShaderCreateInfo info = {};
        info.code_size = code.size;
        info.code = code.data;
        info.entrypoint = "main";
        info.format = format;
        info.stage = shaderStage;
        info.num_samplers = shader.numSamplers;
        info.num_storage_textures = shader.numStorageTextures;
        info.num_storage_buffers = shader.numStorageBuffers;
        info.num_uniform_buffers = shader.numUniformBuffers;

        SDL_GPUShader* shaderObj = SDL_CreateGPUShader(context.device, &info);
        if (!shaderObj) {
            log_error("%s", SDL_GetError());
            return SHADER_LOAD_FAIL;
        }

        shader.shader = shaderObj;

        return SHADER_LOAD_SUCCESS;
    }

    bool create_default_shaders(SDL_GPUDevice* device, DefaultShaders* shaders)
    {
        SDL_GPUShaderCreateInfo vertexInfo = {};
        SDL_GPUShaderCreateInfo vertexInstanceInfo = {};
        SDL_GPUShaderCreateInfo vertexLightInfo = {};
        SDL_GPUShaderCreateInfo vertexCompositionInfo = {};
        SDL_GPUShaderCreateInfo fragmentInfo = {};
        SDL_GPUShaderCreateInfo fragmentTextureInfo = {};
        SDL_GPUShaderCreateInfo fragmentLightInfo = {};
        SDL_GPUShaderCreateInfo fragmentCompositionInfo = {};

        vertexInfo.stage = SDL_GPU_SHADERSTAGE_VERTEX;
        vertexInstanceInfo.stage = SDL_GPU_SHADERSTAGE_VERTEX;
        vertexLightInfo.stage = SDL_GPU_SHADERSTAGE_VERTEX;
        vertexCompositionInfo.stage = SDL_GPU_SHADERSTAGE_VERTEX;
        fragmentInfo.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
        fragmentTextureInfo.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
        fragmentLightInfo.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
        fragmentCompositionInfo.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;

        vertexInfo.entrypoint = "main";
        vertexInstanceInfo.entrypoint = "main";
        vertexLightInfo.entrypoint = "main";
        vertexCompositionInfo.entrypoint = "main";
        fragmentInfo.entrypoint = "main";
        fragmentTextureInfo.entrypoint = "main";
        fragmentLightInfo.entrypoint = "main";
        fragmentCompositionInfo.entrypoint = "main";

        // mvp
        vertexInfo.num_uniform_buffers = 1;
        vertexInstanceInfo.num_uniform_buffers = 1;
        vertexLightInfo.num_uniform_buffers = 1;

        fragmentTextureInfo.num_storage_textures = 1;
        fragmentTextureInfo.num_samplers = 1;

        fragmentCompositionInfo.num_uniform_buffers = 1;
        fragmentCompositionInfo.num_storage_textures = 2;
        fragmentCompositionInfo.num_samplers = 2;

        SDL_GPUShaderFormat shaderFormat = SDL_GetGPUShaderFormats(device);
        if (shaderFormat & SDL_GPU_SHADERFORMAT_DXIL)
        {
            vertexInfo.format = SDL_GPU_SHADERFORMAT_DXIL;
            vertexInstanceInfo.format = SDL_GPU_SHADERFORMAT_DXIL;
            vertexLightInfo.format = SDL_GPU_SHADERFORMAT_DXIL;
            vertexCompositionInfo.format = SDL_GPU_SHADERFORMAT_DXIL;
            fragmentInfo.format = SDL_GPU_SHADERFORMAT_DXIL;
            fragmentTextureInfo.format = SDL_GPU_SHADERFORMAT_DXIL;
            fragmentLightInfo.format = SDL_GPU_SHADERFORMAT_DXIL;
            fragmentCompositionInfo.format = SDL_GPU_SHADERFORMAT_DXIL;

            vertexInfo.code_size = vertex_dxil_len;
            vertexInfo.code = vertex_dxil;
            vertexInstanceInfo.code_size = vertex_instance_dxil_len;
            vertexInstanceInfo.code = vertex_instance_dxil;
            vertexLightInfo.code_size = vertex_light_dxil_len;
            vertexLightInfo.code = vertex_light_dxil;
            vertexCompositionInfo.code = vertex_composition_dxil;
            vertexCompositionInfo.code_size = vertex_composition_dxil_len;
            fragmentInfo.code_size = fragment_dxil_len;
            fragmentInfo.code = fragment_dxil;
            fragmentTextureInfo.code_size = fragment_texture_dxil_len;
            fragmentTextureInfo.code = fragment_texture_dxil;
            fragmentLightInfo.code_size = fragment_light_dxil_len;
            fragmentLightInfo.code = fragment_light_dxil;
            fragmentCompositionInfo.code = fragment_composition_dxil;
            fragmentCompositionInfo.code_size = fragment_composition_dxil_len;
        }
        else if (shaderFormat & SDL_GPU_SHADERFORMAT_SPIRV)
        {
            vertexInfo.format = SDL_GPU_SHADERFORMAT_SPIRV;
            vertexInstanceInfo.format = SDL_GPU_SHADERFORMAT_SPIRV;
            vertexLightInfo.format = SDL_GPU_SHADERFORMAT_SPIRV;
            vertexCompositionInfo.format = SDL_GPU_SHADERFORMAT_SPIRV;
            fragmentInfo.format = SDL_GPU_SHADERFORMAT_SPIRV;
            fragmentTextureInfo.format = SDL_GPU_SHADERFORMAT_SPIRV;
            fragmentLightInfo.format = SDL_GPU_SHADERFORMAT_SPIRV;
            fragmentCompositionInfo.format = SDL_GPU_SHADERFORMAT_SPIRV;

            vertexInfo.code_size = vertex_spv_len;
            vertexInfo.code = vertex_spv;
            vertexInstanceInfo.code_size = vertex_instance_spv_len;
            vertexInstanceInfo.code = vertex_instance_spv;
            vertexLightInfo.code_size = vertex_light_spv_len;
            vertexLightInfo.code = vertex_light_spv;
            vertexCompositionInfo.code = vertex_composition_spv;
            vertexCompositionInfo.code_size = vertex_composition_spv_len;
            fragmentInfo.code_size = fragment_spv_len;
            fragmentInfo.code = fragment_spv;
            fragmentTextureInfo.code_size = fragment_texture_spv_len;
            fragmentTextureInfo.code = fragment_texture_spv;
            fragmentLightInfo.code_size = fragment_light_spv_len;
            fragmentLightInfo.code = fragment_light_spv;
            fragmentCompositionInfo.code = fragment_composition_spv;
            fragmentCompositionInfo.code_size = fragment_composition_spv_len;
        }
        else if (shaderFormat & SDL_GPU_SHADERFORMAT_MSL)
        {
            vertexInfo.format = SDL_GPU_SHADERFORMAT_MSL;
            vertexInstanceInfo.format = SDL_GPU_SHADERFORMAT_MSL;
            vertexLightInfo.format = SDL_GPU_SHADERFORMAT_MSL;
            vertexCompositionInfo.format = SDL_GPU_SHADERFORMAT_MSL;
            fragmentInfo.format = SDL_GPU_SHADERFORMAT_MSL;
            fragmentTextureInfo.format = SDL_GPU_SHADERFORMAT_MSL;
            fragmentLightInfo.format = SDL_GPU_SHADERFORMAT_MSL;
            fragmentCompositionInfo.format = SDL_GPU_SHADERFORMAT_MSL;

            vertexInfo.code_size = vertex_msl_len;
            vertexInfo.code = vertex_msl;
            vertexInstanceInfo.code_size = vertex_instance_msl_len;
            vertexInstanceInfo.code = vertex_instance_msl;
            vertexLightInfo.code_size = vertex_light_msl_len;
            vertexLightInfo.code = vertex_light_msl;
            vertexCompositionInfo.code = vertex_composition_msl;
            vertexCompositionInfo.code_size = vertex_composition_msl_len;
            fragmentInfo.code_size = fragment_msl_len;
            fragmentInfo.code = fragment_msl;
            fragmentTextureInfo.code_size = fragment_texture_msl_len;
            fragmentTextureInfo.code = fragment_texture_msl;
            fragmentLightInfo.code_size = fragment_light_msl_len;
            fragmentLightInfo.code = fragment_light_msl;
            fragmentCompositionInfo.code = fragment_composition_msl;
            fragmentCompositionInfo.code_size = fragment_composition_msl_len;
        }
        else
        {
            log_info("No supported shader formats");
            return false;
        }

        SDL_GPUShader *vertex = SDL_CreateGPUShader(device, &vertexInfo);
        SDL_GPUShader *vertex_instance = SDL_CreateGPUShader(device, &vertexInstanceInfo);
        SDL_GPUShader *vertex_light = SDL_CreateGPUShader(device, &vertexLightInfo);
        SDL_GPUShader* vertex_composition = SDL_CreateGPUShader(device, &vertexCompositionInfo);
        SDL_GPUShader *fragment = SDL_CreateGPUShader(device, &fragmentInfo);
        SDL_GPUShader *fragment_texture = SDL_CreateGPUShader(device, &fragmentTextureInfo);
        SDL_GPUShader *fragment_light = SDL_CreateGPUShader(device, &fragmentLightInfo);
        SDL_GPUShader* fragment_composition = SDL_CreateGPUShader(device, &fragmentCompositionInfo);

        if (!(vertex && vertex_instance && fragment && fragment_texture && vertex_light && fragment_light && vertex_composition && fragment_composition))
        {
            log_error("%s", SDL_GetError());
            return false;
        }

        shaders->vertex = vertex;
        shaders->vertexInstance = vertex_instance;
        shaders->fragment = fragment;
        shaders->fragmentTexture = fragment_texture;
        shaders->vertexLight = vertex_light;
        shaders->fragmentLight = fragment_light;
        shaders->vertexComposition = vertex_composition;
        shaders->fragmentComposition = fragment_composition;

        return true;
    }

    void destroy_default_shaders(SDL_GPUDevice* device, DefaultShaders* shaders)
    {
        SDL_ReleaseGPUShader(device, shaders->vertex);
        SDL_ReleaseGPUShader(device, shaders->vertexInstance);
        SDL_ReleaseGPUShader(device, shaders->vertexLight);
        SDL_ReleaseGPUShader(device, shaders->vertexComposition);
        SDL_ReleaseGPUShader(device, shaders->fragment);
        SDL_ReleaseGPUShader(device, shaders->fragmentTexture);
        SDL_ReleaseGPUShader(device, shaders->fragmentLight);
        SDL_ReleaseGPUShader(device, shaders->fragmentComposition);
    }
} // namespace