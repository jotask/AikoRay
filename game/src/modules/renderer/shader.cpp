#include "shader.hpp"

#include "core/platform_constants.hpp"

namespace aiko
{
    Shader::Shader(std::string vertex, std::string fragment)
    {
        m_shader = raylib::LoadShader(vertex.c_str(), fragment.c_str());
    }

    Shader::~Shader()
    {
        raylib::UnloadShader(m_shader);
    }

    int Shader::GetLocation( const std::string locationName )
    {
        return raylib::GetShaderLocation(m_shader, locationName.c_str() );
    }


    void Shader::SetValue(const std::string locationName, int value)
    {
        const auto location = GetLocation(locationName);
        SetValue(location, value);
    }

    void Shader::SetValue(const int location, int value)
    {
        raylib::SetShaderValue(m_shader, location, &value, static_cast<raylib::ShaderUniformDataType>(ShaderUniformType::SHADER_UNIFORM_INT));
    }

    void Shader::SetValue(const std::string locationName, Vector3 values)
    {
        const auto location = GetLocation(locationName);
        SetValue(location, values);
    }

    void Shader::SetValue(const int locIndex, Vector3 values)
    {
        raylib::SetShaderValue(m_shader, locIndex, &values, static_cast<raylib::ShaderUniformDataType>(ShaderUniformType::SHADER_UNIFORM_VEC3));
    }

    void Shader::SetValue(const std::string locationName, Vector4 values)
    {
        const auto location = GetLocation(locationName);
        SetValue(location, values);
    }

    void Shader::SetValue(const int locIndex, Vector4 values)
    {
        raylib::SetShaderValue(m_shader, locIndex, &values, static_cast<raylib::ShaderUniformDataType>(ShaderUniformType::SHADER_UNIFORM_VEC4));
    }

}