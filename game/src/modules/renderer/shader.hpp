#pragma once

#include "utils/raylib_include.hpp"
#include "core/math.hpp"

#include <string>

namespace aiko
{
    class Shader
    {
    public:

        enum class ShaderUniformType
        {
            SHADER_UNIFORM_FLOAT = 0,       // Shader uniform type: float
            SHADER_UNIFORM_VEC2,            // Shader uniform type: vec2 (2 float)
            SHADER_UNIFORM_VEC3,            // Shader uniform type: vec3 (3 float)
            SHADER_UNIFORM_VEC4,            // Shader uniform type: vec4 (4 float)
            SHADER_UNIFORM_INT,             // Shader uniform type: int
            SHADER_UNIFORM_IVEC2,           // Shader uniform type: ivec2 (2 int)
            SHADER_UNIFORM_IVEC3,           // Shader uniform type: ivec3 (3 int)
            SHADER_UNIFORM_IVEC4,           // Shader uniform type: ivec4 (4 int)
            SHADER_UNIFORM_SAMPLER2D        // Shader uniform type: sampler2d
        };

        Shader(std::string vertex, std::string fragment);
        ~Shader();
        int GetLocation ( const std::string locationName );
        void SetValue(const std::string locationName, Vector4 values, ShaderUniformType type);
        void SetValueV(const int locIndex, const void* values, ShaderUniformType type);
    private:

        friend class RendererModule;
        friend class Mesh;

        raylib::Shader m_shader;



    };

}
