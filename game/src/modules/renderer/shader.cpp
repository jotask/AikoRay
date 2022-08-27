#include "shader.hpp"

#include "core/platform_constants.hpp"

namespace aiko
{
    Shader::Shader(std::string vertex, std::string fragment)
    {
        m_shader = raylib::LoadShader(vertex.c_str(), fragment.c_str());
    }

    int Shader::GetLocation( const std::string locationName )
    {
        return raylib::GetShaderLocation(m_shader, locationName.c_str() );
    }

    void Shader::SetValue(const std::string locationName, Vector4 values, ShaderUniformType type)
    {
        const auto location = GetLocation( locationName );
        raylib::SetShaderValue(m_shader, location, &values, static_cast<raylib::ShaderUniformDataType>(type));
    }

    void Shader::SetValueV(const int locIndex, const void* values, ShaderUniformType type)
    {
        raylib::SetShaderValue(m_shader, locIndex, &values, static_cast<raylib::ShaderUniformDataType>(type));
    }

}