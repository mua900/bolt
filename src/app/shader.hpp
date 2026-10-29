#ifndef SHADER_HPP
#define SHADER_HPP

#include "draw.hpp"

namespace bolt {
    using ShaderLoadResult = u32;
    #define SHADER_LOAD_SUCCESS                 0
    #define SHADER_LOAD_FAIL                    BIT(0)
    #define SHADER_LOAD_INCOMPATIBLE_FORMAT     BIT(1)

    ShaderLoadResult loadShader(RenderContext& context, Shader& shader, const char* path);
    void unloadShader(RenderContext& context, Shader& shader);

    bool create_default_shaders(SDL_GPUDevice* device, DefaultShaders* shaders);
    void destroy_default_shaders(SDL_GPUDevice* device, DefaultShaders* shaders);
} // namespace

#endif // SHADER_HPP